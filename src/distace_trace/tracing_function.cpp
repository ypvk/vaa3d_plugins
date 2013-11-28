#include "tracing_function.h"
#include "imageutils.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

//initial the edget tables
EdgeTable  edge_table[13] = 
{
  {0,0,0, 1,0,0, 1},       
	{0,0,0, 0,1,0, 1},
	{0,0,0, 0,0,1, 1},
	{0,0,0, 1,1,0, sqrt(2.0)}, 
	{0,0,0, 0,1,1, sqrt(2.0)},
	{0,0,0, 1,0,1, sqrt(2.0)},
	{1,0,0, 0,1,0, sqrt(2.0)},
	{0,1,0, 0,0,1, sqrt(2.0)},
	{0,0,1, 1,0,0, sqrt(2.0)},
	{0,0,0, 1,1,1, sqrt(3.0)},
	{1,0,0, 0,1,1, sqrt(3.0)},
	{0,1,0, 1,0,1, sqrt(3.0)},
	{0,0,1, 1,1,0, sqrt(3.0)},
};

inline double metric_function(double value, double max_v)
{
  double tmp = 1 - value/max_v;
  return exp((tmp*tmp)*10);
}
inline double edge_weight(double dist, double va, double vb, double max_v)
{
  double value = 0.5*(metric_function(va, max_v) + metric_function(vb, max_v));

  const double step = 1e-5;
  return dist * value * step;
}
//NeuronTracing class member function
NeuronTracing::NeuronTracing(unsigned char* data, 
    V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, float zthickness, 
    V3DLONG bx0, V3DLONG by0, V3DLONG bz0, 
    V3DLONG bx1, V3DLONG by1, V3DLONG bz1, 
    float x0, float y0, float z0, 
    int n_end_nodes, float* x1, float* y1, float* z1, 
    const Parameters& parameters)
{
  this->data = data;
  this->dimx = dimx;
  this->dimy = dimy;
  this->dimz = dimz;
  this->zthickness = zthickness;
  this->bx0 = bx0;
  this->by0 = by0;
  this->bz0 = bz0;
  this->bx1 = bx1;
  this->by1 = by1;
  this->bz1 = bz1;
  this->x0 = x0;
  this->y0 = y0;
  this->z0 = z0;
  this->n_end_nodes = n_end_nodes;
  this->parameters = &parameters;
  this->x1 = x1;
  this->y1 = y1;
  this->z1 = z1;
  this->print_basic_info();
}
void NeuronTracing::print_basic_info()
{
  printf("================================\n");
  printf("Image size : %ld %ld %ld\n", dimx, dimy, dimz);
  printf("zthickness: %f\n", zthickness);
  printf("region: (%ld,%ld,%ld)->(%ld,%ld,%ld)", bx0, by0,bz0, bx1,by1,bz1);
  printf("================================\n");
}
NeuronTracing::~NeuronTracing(){}

//inline function

inline bool NeuronTracing::validate_region()
{
  if (bx0 < 0 || bx0 >= dimx || by0 < 0 || by0 >= dimy || bz0 < 0 || bz0 >= dimz
     || bx1 < 0 || bx1 >= dimx || by1 < 0 || by1 >= dimy || bz1 < 0 || bz1 >= dimz)
    return false;
  else return true;
}
inline bool NeuronTracing::validate_coordinate(float x0, float y0, float z0)
{
  const float error = 0.5;
  if ( x0 < bx0 - error || x0 > bx1 + error || y0 < by0 - error  || y0 > by1 + error || z0 < bz0 - error || \
      z0 > bz1 + error) 
    return false;
  else return true; 
}

inline bool NeuronTracing::validate_index(V3DLONG index, V3DLONG n_nodes)
{
  if (index < 0 || index >= n_nodes) return false;
  else return true;
}

inline V3DLONG NeuronTracing::node_from_xyz(V3DLONG x, V3DLONG y, V3DLONG z, V3DLONG nx,\
    V3DLONG ny, V3DLONG nz, int step)
{
  return V3DLONG((z-bz0)/step * nx * ny + (y-by0)/step *nx + (x-bx0)/step);
}

inline V3DLONG NeuronTracing::node_from_xyz(float x, float y, float z, V3DLONG nx,\
    V3DLONG ny, V3DLONG nz, int step)
{
  return node_from_xyz((V3DLONG)(x+0.5), (V3DLONG)(y+0.5), (V3DLONG)(z+0.5), nx, ny, nz, step);
}

inline void NeuronTracing::node_to_xyz(V3DLONG node, float& x, float& y, float& z, \
    V3DLONG nx, V3DLONG ny, V3DLONG nz, int step)
{
  z = node/(ny*nz);
  y = (node - (V3DLONG)z*ny*nx)/nx;
  x = node - (V3DLONG)z*ny*nx - (V3DLONG)y*nx;

  x = bx0 + x*step;
  y = by0 + y*step;
  z = bz0 + z*step;
}

const char* NeuronTracing::find_shortest_path()
{
  const char* error = NULL;
  //validate
  if (!data || dimx <= 0 || dimy <= 0 || dimz <= 0)
  {
    error = "Image data is NULL, Return";
    cerr << error << endl;
    return error; 
  }
  if(!validate_region())
  {
    error = "region out of the image";
    cerr << error << endl;
    return error;
  }
  //basic parameters;
  int min_step = parameters->node_step;
  int edge_select = parameters->edge_select;  
  int background_select = parameters->background_select;
  if (min_step < 1) min_step = 1; 

  // define the node index
  V3DLONG nx = (bx1-bx0)/min_step + 1;
  V3DLONG ny = (by1-by0)/min_step + 1;
  V3DLONg nz = (bz1-bz0)/min_step + 1;
  
  int n_edge_table = edge_select == 0 ? 3:13;
   
  printf("%ld x %ld x %ld nodes %d steps, connects = %d ", nx, ny, nz, min_step, n_edge_table*2);

  V3DLONG n_nodes = nx*ny*nz;
  
  V3DLONG i, j, k;
  
  V3DLONG start_node;
  V3DLONG* end_nodes = NULL;

  if (n_end_nodes > 0)
  {
    end_nodes = new V3DLONG[n_end_nodes];
  }
  //get the markeer and validate the marker 
  if (!validate_coordinate(x0, y0, z0))
  {
    error = "node out of bounding";
    cerr << error << endl;
    if(end_nodes) {
      delete[] end_nodes;
      end_nodes = NULL;
    }
    return error; 
  }
  start_node = node_from_xyz(x0, y0, z0, nx, ny, nz, step);
  if(!validate_index(start_node, n_nodes))
  {
    error = "start node out of index";
    cerr << error << endl;
    if(end_nodes) {
      delete[] end_nodes;
      return error;
    }
  }
  int n_end_nodes_outbound = 0;
  for(i = 0; i < n_end_nodes; ++i)
  {
    if(!validate_coordinate(x1[i], y1[i], z1[i]))
    {
      n_end_nodes_outbound++;
      end_nodes[i] = -1;
      cout << "node[" << i << "]outof bounding" << endl;
      continue;
    }
    end_nodes[i] = node_from_xyz(x1[i], y1[i], z1[i], nx, ny, nz, step);
    if (!validate_index(end_nodes[i], n_nodes))
    {
      n_end_nodes_outbound++;
      end_nodes[i] = -1;
      cout << "node[" << i << "]outof bounding" << endl;
    }
  }
  if(n_end_nodes > 0 && n_end_nodes_outbound >= n_end_nodes)
  {
    error = "all end nodes is out of bounding";
    if(end_nodes) {
      delete[] end_nodes;
      end_nodes = NULL;
    }
    return error;
  }
  //set threshold
  double i_max = ImageUtils::getImageMaxValue(data, dimx, dimy, dimz);
  double i_ave = ImageUtils::getImageAveValue(data, dimx, dimy, dimz);
  double i_std = ImageUtils::getImageStdValue(data, dimz, dimy, dimz);
  
  double i_threshold = 0;
  if(background_select) i_threshold = (i_ave < i_std) ? i_ave : (i_ave + i_std) * 0.5;
  //define node and egde list
  vector<Node> par_list<n_nodes>;
  vector<Edge> edge_list;
  vector<Weight> weight_list;
  
  for(i = 0; i < n_nodes; ++i)
    par_list[i] = i;
  edge_list.clear();
  weight_list.clear();  
  
  //initial the dist of the edge table;
  for(i = 0; i < n_edge_table; ++i)  
  {
    double di = edge_table[i].i0 - edge_table[i].i1;
    double dj = edge_table[i].j0 - edge_table[i].j1; 
    double dk = edge_table[i].k0 - eget_table[i].k1;
    edge_table[i].dist = sqrt(di*di + dj*dj + dk*dk);
  }

  double value_a;
  double value_b;
  double max_value = 0;
  double min_value = 1e+6;
  V3DLONG num_edges = 0;
  V3DLONG all_count = 0;
  V3DLONG it;
  for(k = 0; k < nz; ++k)
    for(j = 0; j < ny; ++j)
      for(i = 0; i < nx; ++i)
        for(it = 0; it < n_edge_table; ++it)
        {
          V3DLONG i0 = i + edge_table[it].i0;
          V3DLONG j0 = j + edge_table[it].j0;
          V3DLONG k0 = k + edge_table[it].k0;
         
          V3DLONG i1 = i + edge_table[it].i1;
          V3DLONG j1 = j + edge_table[it].j1;
          V3DLONG k1 = k + edge_table[it].k1;
         
          if(i0 >= nx || j0 >= ny || k0 >= nz || i1 >= nx || j1 >= ny || k1 >= nz) continue;
          
          V3DLONG node_a = i0 + j0 * nx + k0*nx*ny;
          V3DLONG node_b = i1 + j1*nx + k1*nx*ny;   
          all_count++;
          
          value_a = ImageUtils::getBlockAveValue(data, dimx, dimy, dimz, bx0 + i0 * min_step, by0+j0*min_step,\
              (V3DLONG)(bz0+k0*min_step)/zthickness);
          value_b = ImageUtils::getBlockAveValue(data, dimx, dimy, dimz, bx0 + i1 * min_step, by0+j1*min_step, \
              (V3DLONG)(bz0+k1*min_step)/zthickness);

          if (value_a < i_threshold || value_b < i_threshold)
            continue; //skip the background value
          
          Weight weight = edge_weight(edge_table[it].dist, value_a, value_b, 255);
          weigth_list.push_back(weight);

          Edge edge = Edge(node_a, node_b);
          edge_list.push_back(edge);

          num_edges++;

          if(max_value < weight) max_value = weight;
          if(min_value > weight) min_value = weight;
        }
  // print some info about the graph
  printf("max edge weigt: %g min edge weght: %g\n", max_value, min_value);
  if (num_edges != edge_list.size())
  {
    error = "number of edges doesn't match";
    cerr << error << endl;
    if(n_end_nodes)
    {
      delete[] n_end_nodes; 
      n_end_nodes = NULL;
    }
    return error;
  }
  // print the image value 
  printf("image ave=%g, std=%g, max=%g. chose %ld out of %ld links \n", i_ave, i_std, i_max, num_edges, all_count);
  printf("Node num=%ld, Edge num =%ld\n", n_nodes, num_edges);
  printf("start form node=%ld\n", start_node);
  for(i = 0; i < n_end_nodes, ++i)printf("%d: to node=%ld\n", i, end_nodes[i]);
  printf("=================================================\n");
  
  //now use shottest path algorithm to get the result  

}
