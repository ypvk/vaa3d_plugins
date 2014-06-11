#ifndef TRACING_FUNCTION_H
#define TRACING_FUNCTION_H
#include <v3d_interface.h>
#include <vector>
#include <math.h>
#include <map>

using std::vector;
using std::map;

class Point;

//base graph parameters
typedef double Weight;
typedef V3DLONG Node;
typedef std::pair<Node, Node> Edge;
typedef std::pair<Point, Point> EdgePoint;

//EdgeTable
class Point
{
  public:
    V3DLONG x;
    V3DLONG y;
    V3DLONG z;
    Point(V3DLONG x, V3DLONG y, V3DLONG z): x(x), y(y), z(z) {}
    ~Point(){};
};
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
    node_step = 2;
    outsample_step = 2;
    smooth_winsize = 5;
    edge_select = 0;
    background_select = 1;   
  }
};
class NeuronTracing
{
  public:
    NeuronTracing(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, 
        float zthickness, V3DLONG bx0, V3DLONG by0, V3DLONG bz0, V3DLONG bx1,
        V3DLONG by1, V3DLONG bz1, float x0, float y0, float z0, int n_end_nodes, 
        float* x1, float* y1, float* z1, const Parameters& parameters);
    ~NeuronTracing();
    const char* find_shortest_path(); 
    const char* merge_traced_path(); // merge the same node in diffrednt segment and rebuild the segemnt
    void rearrange_curve_index();     
    vector< vector<NeuronSWC> > & get_cordinate() const;
    vector<EdgePoint>& get_edgePoints();
    void downsample_curve();
    void refit_pos_r_and_smooth_r(bool move_position, bool in_xy_pannel_only, bool media_filter=false);
    void fetch_data_for_neurontree(QList<NeuronTree>& traced_neuron_trees);
  private:
    void print_basic_info();
    bool validate_region();
    bool validate_coordinate(float x0, float y0, float z0);
    bool validate_index(V3DLONG index, V3DLONG n_nodes);
    V3DLONG node_from_xyz(V3DLONG x, V3DLONG y, V3DLONG z, V3DLONG nx, V3DLONG ny, V3DLONG nz, int step);
    V3DLONG node_from_xyz(float x, float y, float z, V3DLONG nx, V3DLONG ny, V3DLONG nz, int step); 
    void node_to_xyz(V3DLONG node, float& x, float& y, float& z, V3DLONG nx, V3DLONG ny, V3DLONG nz, int step);
    void refit_position_and_radius(vector<NeuronSWC>& coord, bool move_position,
        bool in_xy_pannel_only);
    void smooth_radius(vector<NeuronSWC>& coord, int win_size, bool media_filter=false);
    double refit_radius(float x, float y, float z, double image_thresh, double bound_r,
        bool in_xy_pannel_only);
    void refit_position(float & x, float& y, float& z, double r, double* diff, double image_thresh);
    V3DLONG get_max_n_num(vector< vector<NeuronSWC> >& mm_swc);
    V3DLONG get_max_n_num(vector<NeuronSWC>& m_swcs);
    void set_swc_unit(NeuronSWC& v, V3DLONG num, vector<NeuronSWC>& swc_units, V3DLONG index, bool order, double r=1);
    vector<NeuronSWC> downsample(const vector<NeuronSWC>& coord, int step); 
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
    vector< vector<NeuronSWC> > mm_swc_unit;
    const Parameters* parameters;
    vector<EdgePoint> m_EdgePoints;
};
 
static inline double metric_function(double value, double max_v=255.0);
static inline double edge_weight(double dist, double va, double vb, double max_v=255.0);
static inline double metric_function_v1(double value_a, double value_b, double max_v=255.0);
static inline double edge_weight_v1(double dist, double value_a, double value_b, double max_v=255.0);
void plot(unsigned char* data, int dimx, int dimy, int dimz, const Point& point_a, const Point& point_b, int value=255);
#endif /*trancing_function.h*/
