#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <v3d_interface.h>

class map;
class vector;

class Dijkstra
{
  public:
    Dijkstra(map<V3DLONG, vector<V3DLONG> >& edge_index, vector<std::pair<V3DLONG. V3DLONG> >& edge_list, 
        vector<double>& weight_list, vector<V3DLONG>& parent_list, V3DLONG n_nodes);
    ~Dikstra();
    void search(V3DLONG start);
    void search(V3DLONG start, V3DLONG end);
  private:
    void init();
    void sift(V3DLONG r);//sift from the index
    V3DLONG extractMin();
    void decreaseKey(V3DLONG v, double _dist); //update the the heap index and the dist value for the ndoe v
    void swap(V3DLONG& a, V3DLONG& b);
  private:
    //V3DLONG* parent_list;
    double* dist; // cout all the dist to the source node
    bool* is_visited; // cout of the node is visited
    V3DLONG* heap; // heap for all the node
    V3DLONG heap_size; //heap size start from 1
    V3DLONG* heap_index;  //heap index node_value -> index in the heap
    //insert into the class
    map<V3DLONG, vector<V3DLONG> >* edge_index; // map for the value start_node_value -> edge_indexs_vector  
    vector<std::pair<V3DLONG. V3DLONG> >* edge_list; // edge list each value is start_node:end_node
    vector<double>* weight_list; // edge_weight index are the same as the edge_list
    vector<V3DLONG>* parent_list; // cout the parent_list
    V3DLONG n_nodes; // cout all the node number
};
#endif /*DIJKSTRA*/
