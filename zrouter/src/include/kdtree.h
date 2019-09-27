/*-------------------------------------------------------------
////	AUTHOR: SANJEEV MAHAJAN 
---------------------------------------------------------------*/
#ifndef _KDTREE_H
#define _KDTREE_H
#include "darr.h"
enum Cuttype { X, Y, C };
typedef struct _Point {
  int x;
  int y;
} Point;
enum Itype { TOUCH, OVLAP, BOUNDARY };
class Rect {
   public:
	int left[2];
	int right[2]; 
};
int rects_intersect(Rect *r1, Rect *r2);
int rects_touch(Rect *r1, Rect *r2);
int rects_intersect_boundary(Rect *r1, Rect *r2);
typedef struct _Idata {
	struct _Node *left;
	struct _Node *right;
	int cut;
} Idata;
union Data {
	Idata *data;
	Darr<Rect*> *rectar;
};
typedef struct _Node {
	Data data;	
	Cuttype typ;
	Rect bbx;
} Node;
class Kdtree {
	public: Kdtree();
		~Kdtree();
		Kdtree(Darr<Rect*> *sites);
                void init(Darr<Rect*> *sites);
                void init(Rect *sites,int n);
		//int	insert(Rect *r);
		//int	insert_rect_array(Darr<Rect*> *ar);
		//int	remove(Rect *r);
		void	get_cut_rects(Rect *r, Darr<Rect*> *ar, Itype itype=OVLAP);
		int	is_rect_cut(Rect *r, Itype itype);
                Node* find_parent_nodes(Rect *r,Darr<Node *>*ar);
                void testTreeDbg();
                void insertRect(Rect *r);
                void removeRect(Rect *r);
                Node *getNode() {return _node;}
                void printTree(Node *nd);
	private: 
		Node *_node;
		int _n;
		int find_cutline(Cuttype typ, Darr<Rect*> *ar);
		void destroy_rec(Node *nd);
		void create_tree_rec(Node *nd, Darr<Rect*> *rectar, int num);
		void get_cut_rects_rec(Rect *r, Node *nd, Darr<Rect*> *ar, Hash<Rect*, int> *table, Itype itype=OVLAP);
                Node *find_parent_node_rec(Rect *r, Node *nd,Darr<Node *> *ar);
		void get_cut_rects_bottom(Rect *r, Darr<Rect*> *bar, Darr<Rect*> *ar, Hash<Rect*, int> *table, Itype itype=OVLAP);
		int is_rect_cut_rec(Rect *r, Node *nd, Itype itype);
		int is_rect_cut_bottom(Rect *r, Darr<Rect*> *bar, Itype itype);
                void printNodeRec(Node *nd);
		
};

#endif
