package provide ade::ui 1.0

#
#       Class: ZContext
# Description: Keep the GUI context
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2004
#
# This object keeps and manages the GUI context
# It accepts changes to the context slots
# It generates accummulated update events (after timeout)
#
itcl::class ZContext {
    inherit LObject LBinder
    # Event <<update>> last current force
    # last, current: previous and current values of context
    #         force: true if event was due to Update call 
    # -------------------------------------------------- Public Variables --
    public variable timeout idle  ;# when to generate update event (in ms)
    # -------------------------------------------------- Public Methods ----
    # Set an attribute
    public method Set {a v} {
        zc_set $a $v
    }
    # Set an attribute -- do not produce events
    public method Set! {a v} {
        zc_set! $a $v
    }
    # Get Total Context
    public method Get {{a ""}} {
        if { $a == "" } { return [array get VAL] }
        set VAL($a)
    }
    # add a variable to the context
    public method Add {var} {
        set v [string tolower [namespace tail $var]]
        set VAL($v) [uplevel set $var]
        uplevel [list trace add variable $var write "$this eChange $v"]
    }
    # flush any pending updates, pass the force flag back with the event
    public method Update {} {
        if { $task != "" } { after cancel $task }
        $this eUpdate 1
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable VAL
    protected variable LAST              ;# last value of a changed attribute
    protected variable task ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
        if { $task != "" } { after cancel $task }
    }
    # -------------------------------------------------- Private Methods ---
    protected method ev_schedule {timeout} {
        if { $task != "" } { after cancel $task }
        if { $timeout == 0 } { return }
        set task [after $timeout "$this eUpdate"]
    }
    # ----------------------------------------------- Interface Implementation
    # set the value, schdule update notification
    protected method zc_set {a v} {
        zc_set_val $a $v
        ev_schedule $timeout
    }
    # set the value, DO NOT schdule update notification
    protected method zc_set! {a v} {
        zc_set_val $a $v
    }
    protected method zc_set_val {a v} {
        if ![info exists VAL($a)] { set VAL($a) "" }
        # do not overwrite the last state
        if ![info exists LAST($a)] { set LAST($a) $VAL($a) }
        set VAL($a) $v
    }
    # ----------------------------------------------- CallBacks
    # geterate an update event, if there has been an update
    # pass along the force flag
    public method eUpdate {{force 0}} {
        set task ""
        set al [array get LAST]
        if { (!$force) && ($al == "") } { return }
        catch {unset LAST}
        Event <<update>> $al [array get VAL] $force
    }
    # called when a monitored variable changes
    public method eChange {var realvar args} {
        set val [uplevel set $realvar]
        zc_set $var $val
    }   
    # -------------------------------------------------- Common Initialization
}
