package provide ade::main 1.0

#
#       Class: GUI
# Description: Support for remote GUI's on zrouter
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2005
#
# One instance of the GUI object is created when a remote gui connects
# to ADE (The remote GUI will run GUI::Check)
# 
# 
itcl::class GUI {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -- check if
    public proc Check {} {
	if { [itcl::find object ::gui] == "" } {
	    namespace eval :: GUI gui
	}
    }
    public method List {} {
	set out ""
	foreach t [lsort [array names RGUI]] {
	    append out [format "%10s %s\n" $t $RGUI($t)] 
	}
	set out
    }
    # -- the remote gui has registered
    public method Register {attrs} {
	upvar peer peer
	set n [array size RGUI] ; incr n
	set tag gui$n
	if [info exists peer] {
	    lappend attrs peer $peer
	}
	set RGUI($tag) $attrs
	set LAST $tag
	return $tag
    }
    # -- the remote gui has unregistered
    public method UnRegister {tag} {
	if { $LAST == $tag }  {
	    set LAST ""
	}
	unset RGUI($tag)
    }

    # -- tell the gui of the available crawlers
    public method Crawlers {} {
        set out ""
        foreach cr [itcl::find objects -class Dcr ::*] {
            set crn [namespace tail $cr]
            lappend out $cr [list title $crn tm "" cr ""]
        }
        set out
    }
    # -- send an event to the last gui connected
    public method Send {args} {
	if { $LAST == "" } {
	    error "no gui connected"
	}
	array set S $RGUI($LAST)
	eval $::zroute::SERVER SendEvent $S(peer) $args
    }
    # -- plot a data set
    public method plot {data {tag ""}} {
	Send <<plot>> $data $tag
    }
    # -- send a note over
    public method note {data {tag ""}} {
	Send <<note>> $data $tag
    }

    # -- wait for a gui to connect
    public method WaitAny {} {
	while { [array size RGUI] == 0 } {
	    notice gui waiting for remote GUIs to connect
	    $sema Wait 1000
	}
    }

    # -- start an external zui
    public proc Start {} {
       set port $zroute::PORT
       set hostname [info hostname]
       set ZUI zui
       if { [info exists ::env(MPT_HOME)] } {
           set ZUI "$::env(MPT_HOME)/zui"
       }
       #exec zui -s $port@$hostname  2> ui.log >ui.log &
       exec $ZUI -s $port@$hostname  2> ui.log >ui.log &
    }

    # -- wait for at least one GUI to connect
    public proc Wait {} {
	set s [LSemaphore gui_sema]
	# -- is there at least one gui connected?
	set n 0
	while { [info command gui] == "" } {
	    if !$n {
		notice gui waiting for a remote gui to connect
	    }
	    $s Wait 1000
	    incr n
	}
	$s delete
	gui WaitAny
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable RGUI          ;# remote GUI
    protected variable DCR  dcr0     ;# global crawler
    protected variable LAST ""       ;# last gui connected
    protected variable sema ""       ;#
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set sema [LSemaphore $this.sema]
	# -- initialize the ade environment to accept the remote GUI
	ade_init
    }
    destructor {
	$sema delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method ade_init {} {
	# -- if there are any crawlers, do not bother to create one
	set crs [itcl::find objects -class Dcr]
	if { $crs != "" }  { 
	    set DCR ""
	    return
	}
	# -- otherwise, you need to create a default crawler

	# -- initialize the default database
	db make_area_search -top -nets -insts
	# -- create & connect the global crawler
	Dcr ::$DCR
	$DCR init -dcr= cr
	db attach_gui -dcr $cr -top
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
