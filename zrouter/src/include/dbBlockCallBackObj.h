#include "ads.h"
#include <list>

BEGIN_NAMESPACE_ADS

class dbBlock;
class dbInst;
class dbMaster;
class dbNet;
class dbITerm;
class dbRegion;
class dbWire;

///////////////////////////////////////////////////////////////////////////////
///
/// dbBlockCallBackObj - An object comprising a list of stub routines
/// invoked by dbBlock.
/// Derived classes may implement these routines, causing external code 
/// to be invoked from inside dbBlock methods.
///
///////////////////////////////////////////////////////////////////////////////

class dbBlockCallBackObj
{
 public:
  virtual void inDbInstCreate(dbInst *brand_new) {}
  virtual void inDbInstCreate(dbInst *brand_new, dbRegion * region) {}
  virtual void inDbInstDestroy(dbInst *gonna_die) {}
  virtual void inDbInstSwapMasterBefore(dbInst *shell, dbMaster *brand_new) {}
  virtual void inDbInstSwapMasterAfter(dbInst *inst) {}
  virtual void inDbNetCreate(dbNet *brand_new) {}
  virtual void inDbNetDestroy(dbNet *gonna_die) {}
  virtual void inDbITermCreate(dbITerm *brand_new) {}
  virtual void inDbITermDestroy(dbITerm *gonna_die) {} // Bugzilla #7 - payam
  virtual void inDbITermDisconnect(dbITerm *soon_detached) {}
  virtual void inDbITermConnect(dbITerm *just_attached) {}
  virtual void inDbBlockStreamOutBefore(dbBlock *block) {}
  virtual void inDbBlockStreamOutAfter(dbBlock *block) {}
  virtual void inDbBlockReadNetsBefore(dbBlock *block) {}
  virtual void inDbMoveInst(dbInst *inst) {}
  virtual void inDbWireUpdate( dbWire * wire ) { }

  // allow ECO client initialization - payam
  virtual dbBlockCallBackObj &operator()() { return *this; }

  // Manipulate _callback list of owner -- in journal.cpp
  void addOwner(dbBlock *new_owner);
  bool hasOwner() const { return (_owner != NULL); }
  void removeOwner();

  dbBlockCallBackObj() { _owner = NULL; }
  virtual ~dbBlockCallBackObj() { removeOwner(); }

 private:
  dbBlock * _owner;
};

END_NAMESPACE_ADS
