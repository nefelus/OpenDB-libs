#define NET_PATH_ID 1
#define SNET_PATH_ID 2
#define NET_ID 3
#define ANY_ID 4

extern int deflex_kid;
extern bool deflex_casesens;
extern int deflex_lineno;
extern int defdebug;
extern char * deftext;

class definIBlockage;
class definIComponent;
class definIFill;
class definIGCell;
class definINet;
class definIPin;
class definIReader;
class definIRow;
class definISNet;
class definITracks;
class definIVia;
class definIRegion;
class definINonDefaultRule;
class definIPropDefs;
class definIPinProps;

void defparse_init(FILE * f);
int defparse();
void defparse_done();
void defparse_linecnt();

// Set reader callback interface. These callbacks overide the default
// readers which do nothing.
void defin_set_IBlockage( definIBlockage * blockage );
void defin_set_IComponent( definIComponent * component );
void defin_set_IFill( definIFill * fill );
void defin_set_IGCell( definIGCell * gcell );
void defin_set_INet( definINet * net );
void defin_set_IPin( definIPin * pin );
void defin_set_IReader( definIReader * reader );
void defin_set_IRow( definIRow * row );
void defin_set_ISNet( definISNet * snet );
void defin_set_ITracks( definITracks * tracks );
void defin_set_IVia( definIVia * via );
void defin_set_IRegion( definIRegion * region );
void defin_set_INonDefaultRule( definINonDefaultRule * rule );
void defin_set_IPropDefs( definIPropDefs * defs );
void defin_set_IPinProps( definIPinProps * props );

int deflex();
void deflex_init();
void deflex_done();
void deflex_history();
void deflex_extension();
