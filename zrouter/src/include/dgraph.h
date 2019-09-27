/*----------------------------------------------------------------
/
/////   AUTHOR: SANJEEV MAHAJAN
_________________________________________________________________*/

#ifndef _DGRAPH_H
#define _DGRAPH_H
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#define MAXIT 1000
class Dedge {
  public:
         int s;
         int d;
         double wt;
	 double cwt;
         bool is_hold;
         bool preserve;
}; 
class Dgraph {
  public:
         Dgraph(int n = 0);
         ~Dgraph();
         Dedge* add_edge(int source, int dest, double weight, bool is_hold, bool preserve);
         void remove_edge(Dedge *edge); 
         int neighbors_begin(int v);
         bool next_neighbor(Dedge* &e);
         void init();
         bool relax(Dedge *e, bool *restr = NULL);
         void modify(int i, double delta); 
         bool has_cycle();
         bool bellmanford(bool *rest = NULL);
         int dfs(int i); 
         bool Dfs(int i);
         int n();
         bool topo_sort();
         bool find_longest_path();
         void remove_negative_edges();
         double *dist;
         void set_upper_bound(int i, double j); 
         bool uppers_met();
         double get_upper_bound(int i);
         Darr<int> _topo_sorted;
         void get_slacks();
         Darr<Darr<Dedge*>*> cycles;
         Darr<double> residues;
  private:
         int _n;   
         int *_vis;
         int *_cycle;
         int *_pred;
         double *_upper;
         Dedge** _prede;
         Darr<Dedge*> _cycle_edges;
         Hash<int, Hash<Dedge*, int>*> _neighbors;
         Hash<Dedge*, int> *_curnei;
         Darr<int> sources;
         
};
#endif
