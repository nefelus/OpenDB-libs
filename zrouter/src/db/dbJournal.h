#ifndef ADS_DB_JOURNAL_H
#define ADS_DB_JOURNAL_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_JOURNAL_LOG_H
#include "dbJournalLog.h"
#endif

BEGIN_NAMESPACE_ADS

class dbIStream;
class dbOStream;
class dbBlock;
class dbMaster;
class dbNet;
class dbInst;
class dbITerm;

class dbJournal
{
    dbJournalLog  _log;
    dbBlock *     _block;
    bool          _start_action;
    uint          _action_idx;
    unsigned char _cur_action;

    void redo_createObject();
    void redo_deleteObject();
    void redo_connectObject();
    void redo_disconnectObject();
    void redo_swapObject();
    void redo_updateField();
    void redo_updateBlockField();
    void redo_updateNetField();
    void redo_updateInstField();
    void redo_updateITermField();
    void redo_updateRSegField();
    void redo_updateCapNodeField();
    void redo_updateCCSegField();
    
    void undo_createObject();
    void undo_deleteObject();
    void undo_connectObject();
    void undo_disconnectObject();
    void undo_swapObject();
    void undo_updateField();
    void undo_updateNetField();
    void undo_updateInstField();
    void undo_updateITermField();
    void undo_updateRSegField();
    void undo_updateCapNodeField();
    void undo_updateCCSegField();

  public:
    enum Action
    {
        CREATE_OBJECT,
        DELETE_OBJECT,
        CONNECT_OBJECT,
        DISCONNECT_OBJECT,
        SWAP_OBJECT,
        UPDATE_FIELD,
        END_ACTION
    };

    dbJournal( dbBlock * block );
    ~dbJournal();
    void clear();

    int size()
    {
        return _log.size();
    }

    //
    // Methods to push entries into the transaction log.
    //
    // The entries in the log take the form:
    //
    //    <ACTION>
    //    <PARAM-1>
    //     ...
    //    <PARAM-N>
    //    <ACTION-OFFSET>
    //
    void beginAction( Action type );
    void pushParam( bool value );
    void pushParam( char value );
    void pushParam( unsigned char value );
    void pushParam( int value );
    void pushParam( unsigned int value );
    void pushParam( float value );
    void pushParam( double value );
    void pushParam( const char * value );
    void endAction();

    //
    // updateField : helper methods to update fields in objects.
    //
    // The update field entries in the log take the form:
    //
    //    <ACTION>
    //    <OBJECT-TYPE>
    //    <OBJECT-ID>
    //    <FIELD-ID>
    //    <PREV_VALUE>
    //    <NEW_VALUE>
    //    <ACTION-OFFSET>
    //
    void updateField( dbObject * obj, int field_id, bool prev_value, bool new_value );
    void updateField( dbObject * obj, int field_id, char prev_value, char new_value );
    void updateField( dbObject * obj, int field_id, unsigned char prev_value, unsigned char new_value );
    void updateField( dbObject * obj, int field_id, int prev_value, int new_value );
    void updateField( dbObject * obj, int field_id, unsigned int prev_value, unsigned int new_value );
    void updateField( dbObject * obj, int field_id, float prev_value, float new_value );
    void updateField( dbObject * obj, int field_id, double prev_value, double new_value );
    void updateField( dbObject * obj, int field_id, const char * prev_value, const char * new_value );

    // redo the transaction log
    void redo();

    // undo the transaction log
    void undo();

    bool empty() { return _log.empty(); }

    friend dbIStream & operator>>( dbIStream & stream, dbJournal & jrnl );
    friend dbOStream & operator<<( dbOStream & stream, const dbJournal & jrnl );
    friend class dbDatabase;
};

dbIStream & operator>>( dbIStream & stream, dbJournal & jrnl );
dbOStream & operator<<( dbOStream & stream, const dbJournal & jrnl );

END_NAMESPACE_ADS    


#endif
