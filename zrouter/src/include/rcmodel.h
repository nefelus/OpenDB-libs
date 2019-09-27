#ifndef RCMODEL_H
#define RCMODEL_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ARNOLDI1_H
#include "arnoldi1.h"
#endif

BEGIN_NAMESPACE_ADS

//
// This is the rcmodel, without Rd.
// n is the number of terms
// The vectors U[j] are of size n
//
class rcmodel : public arnoldi1 {
 public:
  void **itermV; // [n]
  void **btermV; // [n]
  rcmodel *rise_cap_model;
  rcmodel *next;

 public:
  rcmodel() { itermV = NULL; btermV = NULL; rise_cap_model = NULL; next=NULL; }
  ~rcmodel() {}
};


//
// rcmodel for a simple line
//
rcmodel *get_line_rcmodel(int n,double *r,double *c,int max_order,int jout);

//
// use this with get_line_rcmodel
//
void rcmodelDestroy(rcmodel *);

//
// get total cap from rcmodel
//
double rcmodel_ctot(rcmodel *mod);

//
// get max elmore from rcmodel
// this is not the whole elmore delay,
// does not include Rdrive*ctot
//
double rcmodel_max_elmore(rcmodel *mod);

END_NAMESPACE_ADS
#endif
