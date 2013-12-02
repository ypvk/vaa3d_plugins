#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <v3d_interface.h>

class Dijkstra
{
  public:
    Dijkstra();
    ~Dikstra();
    void search(V3DLONG start);
    void search(V3DLONG start, V3DLONG end);
  private:
    void init();
    void addEdge();
    void sift(V3DLONG r);
    int extractMin();
    void decreaseKey(V3DLONG v, float dist); 
  private:
    V3DLONG* parent_list;
    V3DLONG* dist;
    V3DLONG n_nodes;
    V3DLONG n_edges;
};
#endif /*DIJKSTRA*/
