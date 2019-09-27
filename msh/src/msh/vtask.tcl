package provide MSH 1.0

#       Class: VTask
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
# Represent and manage a running task
#
itcl::class VTask {
    inherit LObject LBinder
    # Event <<deleted>>
    # -------------------------------------------------- Public Variables --
    public variable class default          ;# task class
    public variable timeout 0              ;# do timeout
    public variable cmd ""                 ;# the command to run
    public variable env ""                 ;# environment to pass
    public variable tid 0                  ;# task ID
    public variable output ""              ;# current output
    public variable stdin ""               ;# stdin given
    public variable host ""                ;# where it runs
    public variable serial 1               ;# task serial number (for resub)
    # -------------------------------------------------- Public Methods ----
    public method Info {} {
	set cmd
    }
    public method NewState {state} {
        task_record $state
    }
    public method Stats {ss} {
        upvar $ss S
        array set S [task_stats]
    }
    public method PrintStats {} {
        format "%5s %4s sec queue %4s sec disp" $tid $STAT(QUEUE) $STAT(DISP)
    }
    public method Status {} {
        list state $LASTSTATE stats [task_stats] output $output \
            host $host tid $tid start $STARTTIME class $class
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable STARTTIME
    protected variable STAT
    protected variable LASTSTATE  ""
    protected variable LASTTIME ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set STARTTIME [clock seconds]
        array set STAT "DEF 0 QUEUE 0 DISP 0"
        NewState DEF
    }
    destructor {
        Event <<deleted>> $tid
    }
    # -------------------------------------------------- Private Methods ---
    protected method task_record {state} {
        set now [clock seconds]
        # -- charge time to the last state
        if { $LASTSTATE != "" } {
            set STAT($LASTSTATE) [expr $now - $LASTTIME]
        }
        set LASTSTATE $state
        set LASTTIME $now
    }
    # give us the time (in sec) on each state
    protected method task_stats {} {
        # update the time on the current state
        switch $LASTSTATE {
            DONE - "" { }
            default {
                set now [clock seconds]
                set STAT($LASTSTATE) [expr $now - $LASTTIME]
            }
        }
        array get STAT
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
