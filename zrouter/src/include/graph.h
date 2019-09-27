/*-------------------------------------------------------------
////	AUTHOR: SANJEEV MAHAJAN 
---------------------------------------------------------------*/
#ifndef _GRAPH_H
#define _GRAPH_H
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#define MAXIT 1000
#define LARGE 2000000000
enum Matchkind {OPT = 0, NOOPT};
typedef struct _Edge {  
  int left; 
  int right; 
  long wt;
} Edge; 
class Queue {
  public: Queue(int size = 0);
  void insert(int el);
  int  remove(void);
  void set_size(int size);
  void reset();
  int  is_empty();
  ~Queue(void);
  
  private:
    int *_qu;
    int _size;
    int _front;
    int _back;
};

class Graph { 
  public: Graph(int n=0); 
  void add_edge(int left, int right, long weight); 
  void set_num_left(int); 
  int  get_num_left(void);
  Darr<Edge *> *get_neighbors(int vertex); 
  int get_degree(int vertex); 
  Darr<Edge *> get_all_edges(); 
  int get_num_vertices(); 
  void get_neighbor(int vertex, int i, int &neigh, long &wt); 
  void add_right_vertex(int thresh=1);
  ~Graph(void); 
  void sort_neighbor_weights(void);
  int  get_thresh(int vertex);
  int find_matching(Matchkind kind = OPT);
  int get_matched_vertex(int i);
  void print();
  long matchwt();
  void find_connected_components(Darr<Darr<Edge*>*> &ed);
  void dfs(int i, int *vis, int k, Darr<Edge*> *edg, Hash<Edge*, int> &edge_tab);
  int find_hall_set(Darr<Darr<int>*> &hall);
  private: Darr<Darr<Edge *>*> _neighbors; 
           int _num_left; 
           int _num_vert;
           Darr<int> _degree; 
           Darr<Edge *> _edges; 
           HashP<int, Edge *> _edge_table;
	   Darr<int> _thresh;
           int *_match;
           int  _negative_cycle(long *dist, int *pred, int *vis, int *rightd, int &k); 
           int _bellman_ford(Darr<Edge*> *edges, long *dist, int *pred, int *vis, int *rightd, int factor, int &k);
	   void _dfs_hall(int i, int *vis, int *par, int k);
}; 
#endif
