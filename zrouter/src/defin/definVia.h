#ifndef ADS_DEFIN_VIA_H
#define ADS_DEFIN_VIA_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_IVIA_H
#include "definIVia.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

#ifndef ADS_DB_H
#include "db.h"
#endif

BEGIN_NAMESPACE_ADS

class dbVia;
 
class definVia : public definIVia, public definBase
{
    dbVia *  _cur_via;
    dbViaParams * _params;

    // Via interface methods
    virtual void viaBegin( const char * name );
    virtual void viaRule( const char * rule );
    virtual void viaCutSize( int xSize, int ySize );
    virtual void viaLayers(  const char * bottom, const char * cut, const char * top );
    virtual void viaCutSpacing( int xSpacing, int ySpacing );
    virtual void viaEnclosure( int xBot, int yBot, int xTop, int yTop );
    virtual void viaRowCol( int numCutRows, int numCutCols );
    virtual void viaOrigin( int xOffset, int yOffset );
    virtual void viaOffset( int xBot, int yBot, int xTop, int yTop );

    virtual void viaPattern( const char * pattern );
    virtual void viaRect( const char * layer, int x1, int y1, int x2, int y2 );
    virtual void viaEnd();

  public:
    definVia();
    virtual ~definVia();
    void init();
};
    
END_NAMESPACE_ADS

#endif
