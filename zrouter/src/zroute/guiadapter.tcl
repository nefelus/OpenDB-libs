package provide ade::main 1.0

#
#       Class: UIAdapter
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2006
#
itcl::class UIAdapter {
    inherit LObject LBinder
    # -------------------------------------------------- Public Variables --
    public variable name "Baseline ADE UI"
    public variable stimeout 1                    ;# every 1 second
    public variable application app               ;# name of ade application
    # -------------------------------------------------- Public Methods ----
    # -- register the new state
    public method NewState {tag state} {
	$this uia_new_state $tag $state
    }
    # -- send a new stylesheet over
    public method StyleSheet {stylesheet} {
	$this uia_stylesheet $stylesheet
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private   variable TASK
    private   variable CHANGED              ;# which state has changed?
    private   variable STATE                ;# STATE value
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set TASK(sampling) ""
	array set CHANGED {}
    }
    destructor {
	foreach t [array names TASK] {
	    catch {after cancel $t}
	}
    }
    # -------------------------------------------------- Private Methods ---
    # -- send a uiadapter event over to the gui
    private method uia_send {event args} {
	eval gui Send <<uiadapter-post>> $application $event $args
    }
    # ----------------------------------------------- Interface Implementation
    protected method uia_new_state {tag state} {
	set STATE($tag) $state
	if { $TASK(sampling) == "" } { 
	    set TASK(sampling) \
		[after [expr int($stimeout * 200)] [list $this eSendState]]
	}
	set CHANGED($tag) 1
    }
    protected method uia_stylesheet {stylesheet} {
	uia_send <<stylesheet>> $stylesheet
    }
    # ----------------------------------------------- CallBacks
    public method eSendState {} {
	set TASK(sampling) ""
	set res ""
	foreach tag [array names CHANGED] {
	    unset CHANGED($tag)
	    lappend res $tag $STATE($tag)
	}
	if { $res == "" } { return }
	uia_send <<newstates>> $res
    }
    # -------------------------------------------------- Common Initialization
}


#
#       Class: UIAdapter_milos
# Description: UIAdapter for milos
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2006
#
itcl::class UIAdapter_milos {
    inherit UIAdapter
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
	configure -application milos
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    public method demo {} {
	NewState main.status run
	NewState main.info "initializing"
	pause

	foreach i {1 2 3 4} {
	    NewState slave1.status idle
	    NewState slave1.info "idling"
	    NewState slave2.status idle
	    NewState slave2.info "idling"

	    NewState main.status run
	    NewState main.info "optimizing"

	    pause
	    NewState slave1.status run
	    NewState slave1.count $i
	    NewState slave1.info "analysing"

	    NewState slave2.status run
	    NewState slave2.count $i
	    NewState slave2.info "analysing"

	    NewState main.status idle
	    NewState main.info "waiting for timing"
	    NewState main.count [expr $i*2]
	    pause
	}
	NewState slave1.status idle
	NewState slave2.status idle
	NewState main.status   run
	NewState main.info "assembling"
	NewState slave1.info "done"
	NewState slave2.info "done"
	pause
	NewState main.status   idle
	NewState main.info "done"
    }
    protected method pause {{timeout 1000}} {
	set s [LSemaphore #auto]
	$s Wait $timeout
	$s delete
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
