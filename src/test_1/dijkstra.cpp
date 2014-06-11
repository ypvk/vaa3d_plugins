#include "dijkstra.h"
#include <map>
#include <vector>
#include <memory.h>
#include <iostream>

using namespace std;

const static double INF = 1.79769e+308; //max_value

Dijkstra::Dijkstra(map<V3DLONG, vector<V3DLONG> >& edge_index, vector<std::pair<V3DLONG, V3DLONG> >& edge_list, 
    vector<double>& weight_list, vector<V3DLONG>& parent_list, V3DLONG n_nodes)
{
  this->edge_index = &edge_index;
  this->edge_list = &edge_list;
  this->weight_list = &weight_list;
  this->parent_list = &parent_list;
  this->n_nodes = n_nodes;
  this->init();
}
Dijkstra::~Dijkstra()
{
  delete[] this->dist;
  delete[] this->is_visited;
  delete[] this->heap;
  delete[] this->heap_index;

  dist = NULL;
  is_visited = NULL;
  heap = NULL;
  heap_index = NULL;
}
void Dijkstra::init()
{
  this->dist = new double[n_nodes];
  this->is_visited = new bool[n_nodes];
  this->heap = new V3DLONG[n_nodes + 1];//index of heap top is 1 
  this->heap_index = new V3DLONG[n_nodes]; //index for the node that in the heap
  this->heap_size = 0;
  //init value  
  int i;
  for(i = 0; i < n_nodes; ++i)
  {
    this->dist[i] = INF;
    this->is_visited[i] = false;
  } 
  //set all the heap_index to -1
  memset(heap_index, -1, n_nodes*sizeof(V3DLONG));
}

inline void Dijkstra::swap(V3DLONG& a, V3DLONG& b)
{
  V3DLONG tmp = a;
  a = b;
  b = tmp;
}
V3DLONG Dijkstra::extractMin()
{
  V3DLONG res = heap[1];
  heap_index[heap[heap_size]] = 1;
  heap[1] = heap[heap_size--];
  sift(1);
  return res;
}

void Dijkstra::sift(V3DLONG r)
{
  V3DLONG s = r; 
  V3DLONG fa;
  while((fa = s << 1) <= heap_size)
  {
    V3DLONG p = fa, q = fa+1;
    if(q <= heap_size && dist[heap[q]] < dist[heap[p]])
      p = q;
    if(dist[heap[p]] < dist[heap[s]])
    {
      heap_index[heap[p]] = s;
      heap_index[heap[s]] = p;
      this->swap(heap[p], heap[s]);   
    }
    else break;

    s = p;
  }
}

void Dijkstra::decreaseKey(V3DLONG v, double _dist)
{
  dist[v] =  _dist;
  V3DLONG p = heap_index[v];
  //if the target v is not in the heap add it
  if (p < 0) 
  {
    heap[++heap_size] = v;
    heap_index[v] = heap_size;
    p = heap_size;
  }
  int fa;
  while(p > 1 && dist[heap[p]] < dist[heap[fa = p >> 1]])
  {
    heap_index[heap[fa]] = p;
    heap_index[heap[p]] = fa;
    this->swap(heap[fa], heap[p]); 
    p = fa;
  }
}
void Dijkstra::search(V3DLONG start)
{
  this->dist[start] = 0;
  this->heap[1] = start;
  (this->heap_size)++;
  this->heap_index[start] = 1;
  int i;
  while(heap_size > 0)
  {
    V3DLONG node = extractMin();
    is_visited[node] = true;
    vector<V3DLONG> edge_for_node = (*edge_index)[node];
    for(i = 0; i < edge_for_node.size(); ++i)
    {
      V3DLONG index = edge_for_node[i];
      V3DLONG to_node;
      if (node == (*edge_list)[index].first)
        to_node = (*edge_list)[index].second;
      else
        to_node = (*edge_list)[index].first;
      if(!is_visited[to_node] && dist[to_node] > dist[node] + (*weight_list)[index])
      {
        (*parent_list)[to_node] = node;
        decreaseKey(to_node, dist[node] + (*weight_list)[index]);
      }
    }
  } 
}

void Dijkstra::search(V3DLONG start, V3DLONG end)
{
  this->dist[start] = 0;
  this->heap[1] = start;
  (this->heap_size)++;
  this->heap_index[start] = 1;
  int i;
  while(heap_size > 0)
  {
    V3DLONG node = extractMin();
    is_visited[node] = true;
    if(node == end) return;
    vector<V3DLONG> edge_for_node = (*edge_index)[node];
    for(i = 0; i < edge_for_node.size(); ++i)
    {
      V3DLONG index = edge_for_node[i];
      V3DLONG to_node;
      if (node == (*edge_list)[index].first)
        to_node = (*edge_list)[index].second;
      else
        to_node = (*edge_list)[index].first;
      if(!is_visited[to_node] && dist[to_node] > dist[node] + (*weight_list)[index])
      {
        (*parent_list)[to_node] = node;
        decreaseKey(to_node, dist[node] + (*weight_list)[index]);
      }
    }
  }
}
