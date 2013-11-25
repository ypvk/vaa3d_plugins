#include "tracing_function.h"
#include <iostream>
#include <cstdio>

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
static inline bool validate_coordinate(V3DLONG x0, V3DLONG y0, V3DLONG z0, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz)
{
  const float error = 0.5;
  if ( x0 < 0 || x0 >= dimx-error || y0 < 0 || y0 >= dimy-error || z0 < 0 || z0 >= dimz -error) return false;
  else return true; 
}

const char* NeuronTracing::find_shortest_path()
{
 //TODO do it 
  const char* error = NULL;
  //validate
  if (!data || dimx <= 0 || dimy <= 0 || dimz <= 0)
  {
    error = "Image data is NULL, Return";
    cerr << error << endl;
    return error; 
  }
  if (!validate_coordinate(bx0, by0, bz0, dimx, dimy, dimz) || \
      !validate_coordinate(bx1, by1, bz1, dimx, dimy, dimz))
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


}
