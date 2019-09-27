package provide MSH 1.0

#
#       Class: VRJE
# Description: Run task on remote slaves
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
# This object simulates an RJE and owns all objects representing remote tasks
#
itcl::class VRJE {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable queue "" ;# task queue
    # -------------------------------------------------- Public Methods ----
    # -- run cmd using stdin and env. Configure vtask object with opt args
    public method Run {tid cmd stdin env {argv ""}} {
        # pass the TASKID
        lappend env MSHTID $tid
        set j [namespace eval :: VTask $tid -tid $tid]
        $j configure -cmd $cmd -env $env -stdin $stdin
	# -- configure the object, if needed
	if { $argv != "" } { 
	    eval $j configure $argv
	}
        $j Bind <<deleted>> $this eVTaskDeleted $j
        $queue Enq $j
        $j NewState QUEUE
        set TID($j) $tid
        set TASK($tid) $j
        set j
    }

    public method TaskStatus {tid} {
        $TASK($tid) Status
    }

    public method Kill {tid} {
        # ---
    }
    public method Tasks {} {
        array names TASK
    }
    # -- display the overall status
    public method SystemStatus {} {
        return "*** RJE OK ([array size TASK] tasks)\n"
    }

    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable TASK
    protected variable TID
    protected variable NTASKS 1000
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        array set TASK {}
        array set TID {}
    }
    destructor {
        # foreach t [array names TID] { $t delete } 
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- Callbacks
    # -- unregister a task
    public method eVTaskDeleted {task tid} {
        unset TID($task)
        unset TASK($tid)
    }
    # -------------------------------------------------- Common Initialization
}
