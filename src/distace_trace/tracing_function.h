#ifndef TRACING_FUNCTION_H
#define TRACING_FUNCTION_H
#include <v3d_interface.h>
#include <vector>
#include <pair>
#include <math.h>

using std::vector;

//base graph parameters
typedef double Weight;
typedef V3DLONG Node;
typedef std::pair<Node, Node> Edge;

//EdgeTable
typedef struct EdgeTableItem
{
  //source posi
  V3DLONG i0; 
  V3DLONG j0;
  V3DLONG k0;
  //dist func
  V3DLONG i1;
  V3DLONG j1;
  V3DLONG k1;
  //dist
  double dist;
} EdgeTable;

struct Parameters
{
  int node_step;
  int outsample_step;
  int smooth_winsize;
  int edge_select; //chose edge type
  int background_select;
  Parameters()
  {
    node_step = 3;
    outsample_step = 2;
    smoth_windsize = 5;
    edge_select = 0;
    background_select = 1;   
  }
};
class NeuronTracing
{
  public:
    NeuronTracing(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, 
        float zthickness, V3DLONG bx0, V3DLONG by0, V3DLONG bz0, V3DLONG bx1,
        V3DLONG by1, V3DLONG bz1, float x0, float y0, float, z0, int n_end_nodes, 
        float* x1, float* y1, float* z1, const Parameters& parameters);
    ~NeuronTracing();
    const char* find_shortest_path(); 
    vector< vector<V_NeuronSWC_unit> > & get_cordinate() const;
  private:
    void print_basic_info();
    bool validate_region();
    bool validate_coordinate(float x0, float y0, float z0);
    bool validate_index(V3DLONG index, V3DLONG n_nodes);
    V3DLONG node_from_xyz(V3DLONG x, V3DLONG y, V3DLONG z, V3DLONG nx, V3DLONG ny, V3DLONG nz, int step);
    V3DLONG node_from_xyz(float x, float y, float z, V3DLONG nx, V3DLONG ny, V3DLONG nz, int step); 
    void node_to_xyz(V3DLONG node, float& x, float& y, float& z, V3DLONG nx, V3DLONG ny, V3DLONG nz, int step);
  private:
    //image base info
    unsigned char* data;
    V3DLONG dimx;
    V3DLONG dimy;
    V3DLONG dimz;
    float zthickness;
    //region
    V3DLONG bx0;
    V3DLONG by0;
    V3DLONG bz0;
    V3DLONG bx1;
    V3DLONG by1;
    V3DLONG bz1;
    //start node
    float x0;
    float y0;
    float z0;
    //end nodes;
    int n_end_nodes;
    float* x1;
    float* y1;
    float* z1;    
    vector< vector<V_NeuronSWC_unit> > mmUnit;
    Parameters* parameters;
};
 
inline double metric_function(double value, double max_v=255.0);
inline double edge_weight(double dist, double va, double vb, double max_v=255.0);
#endif /*trancing_function.h*/
