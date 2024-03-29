#ifndef SVER_H
#define SVER_H

#ifndef ADS_H
#include "ads.h"
#endif

// A very simple verilog parser
// for the sole purpose of interpreting
// hierarchical pin names in SDC timing
// constraints files.

#ifdef _WIN32
	#define N_MODH 5000
	#define N_HI_RING 1024
	#define NW2 32768
#endif


BEGIN_NAMESPACE_ADS

class sver_names;
class sver_module;
struct sver_inst;
struct sver_net;
struct sver_term;
struct sver_term_net;
struct sver_term_inst;

class sver_module {
 public:
  char *_name;
  sver_term *_terms;
  sver_module *_next;
  int _instN;
  int _netN;
  int _ref;
 private:
  int instH_n;
  sver_inst **instH;
  sver_inst *instL;
  int netH_n;
  sver_net **netH;
  sver_net *netL;

 public:
  sver_module();
  ~sver_module();
  sver_inst *find_inst(const char *name);
  sver_net *find_net(const char *name);
  void add_inst(sver_inst *inst);
  void add_net(sver_net *net);
  sver_term *find_or_create_term(const char *name,bool,sver_names *);
  sver_term *find_term(const char *name);
  sver_net *find_or_create_net(const char *name,sver_names *);
};

struct sver_inst {
  char *name;
  sver_module *module; // down
  sver_term_net *term_nets;
  sver_inst *next;
};

struct sver_net {
  char *name;
  sver_module *module; // up
  sver_term_inst *term_insts;
  sver_net *next;
};

struct sver_term_net {
  sver_term *term;
  sver_net *net;
  sver_term_net *next;
};

struct sver_term_inst {
  sver_term *term;
  sver_inst *inst; // may be nil, for mterm
  sver_term_inst *next;
};

struct sver_term {
  char *name;
  sver_module *module;
  sver_net *net; // may be nil
  sver_term *next;
};
  
struct sver_hier_inst {
  sver_inst *inst;
  sver_hier_inst *next;
};

struct sver_hier_net {
  sver_hier_inst *hier_inst;
  sver_net *net;
};

struct sver_hier_pin {
  sver_hier_inst *hier_inst;
  sver_term *term;
};

struct sver_name_list {
  char *name;
  sver_name_list *next;
};

class sver
{
 public:
  sver_module *_top;
 private:
  sver_names *_names;

#ifndef _WIN32
  static const int N_MODH = 5000;
  static const int N_HI_RING = 1024;
  static const int NW2 = 32768;
#endif
  sver_module *_modH[N_MODH];

  sver_hier_inst _hi_ring[N_HI_RING];
  int _hi_ring_next;

  sver_name_list _name_ring[N_HI_RING];
  char _name_ring_str[N_HI_RING*1024];
  int _name_ring_next;

  sver_name_list _name_ring2[NW2];
  int _name_ring2_next;

  sver_hier_net _tmp_hier_net;
  sver_hier_pin _tmp_hier_pin;
  sver_hier_pin _tmp_hier_pin2;
  char _tmp_str[4096];

  sver_module *_cur_module;
  bool _old_module;
  sver_module *_cur_inst_mod;
  bool _old_inst_mod;
  sver_inst *_cur_inst;

 private:
  void set_default_top();
  sver_hier_inst *new_hier_inst();  
  sver_module *create_module(const char *name);
  sver_module *find_or_create_module(const char *name);

 public:
  sver();
  ~sver();
  void read(const char *file);
  void start_module(const char *name);
  void add_module_pin(const char *name);
  void start_inst(const char *name,const char *module_name);
  void add_inst_pin(const char *name,const char *net_name);
  void stop_inst();
  void stop_module();

  sver_module *find_module(const char *name);
  sver_hier_inst *find_hier_inst(sver_name_list *names,sver_module *top = NULL);
  sver_hier_net *find_hier_net(sver_name_list *names,sver_module *top = NULL);
  sver_hier_pin *find_hier_pin(sver_name_list *names,sver_module *top = NULL);
  sver_hier_pin *get_any_leaf_pin(sver_hier_pin *hier_pin);

  sver_net *find_upper_net(sver_hier_inst *hi,sver_net *net,sver_hier_inst **at);
  sver_term * get_mterm(sver_net *net);
  sver_net * get_net(sver_inst *inst,sver_term *t);
  sver_module *get_top() { return _top; }

  sver_name_list *new_name_list();  // with name allocated
  sver_name_list *new_name_list_no_str();  // without name allocated
  sver_name_list *name_list(const char *name,const char delim = '/');
  const char *name_list_str(sver_name_list *nm,const char delim = '/',const char last_delim = '/');
  void name_list_print(char *str,sver_name_list *nm,const char delim = '/',const char last_delim = '/');

  sver_name_list *name(sver_hier_inst *hier_inst);
  sver_name_list *name(sver_hier_net *hier_net);
  sver_name_list *name(sver_hier_pin *hier_pin);
};

void sver_read(char *file);

END_NAMESPACE_ADS

#endif
