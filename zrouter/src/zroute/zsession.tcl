package require Itcl

package provide ade::main 1.0
#
#       Class: ZSession
# Description: Session Controller for zroute
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2004
#
# The session holds all necessary state of a zroute session 
#
itcl::class ZSession {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable profile "default"
    public variable ns      "::"        ;# which namespace to run
    public variable evmgr   ""          ;# event manager
    # -------------------------------------------------- Public Methods ----
    public method ProfileSet {prof} {
        set cmd $PROFILE($prof)

        # expand the profile
        set cmd [subst -nocomm $cmd]

        # evaluate it
        namespace eval $ns $cmd
    }
    public method Register {var value} {
        set CONF($var) $value
    }
    public method Resolve {var} {
        if [info exists CONF($var)] {
            return $CONF($var)
        } else {
            return ""
        }
    }
    # -- make a note of a new tm object
    public method NewTM {tm} {
        # provide tm with its event manager
        $tm configure -evmgr $evmgr

        # keep a note for it
        lappend ZTMS $tm
    }

    # -- info
    public method Info {} {
        puts "Configuration Variables"
        foreach var [array names CONF] {
            puts "$var $CONF($var)"
        }
        puts "Technology Modules"
        foreach tm $ZTMS {
            puts "$tm"
        }
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable PROFILE
    protected variable CONF
    protected variable ZTMS ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        # initialize profiles
        profile_init

        # initalize technology modules
        zroute::ztm::init

        # configure the zsession static variable on all TM classes
        # just change the super class:
        set ZTM::zsession $this
        if { [itcl::find class ::ZTMSession] == "" } {
            warning ztm "ZTMSession: class not defined"
            warning ztm "there will be no tmodule access"
        } else {
            ProfileSet $profile
        }
    }
    destructor {
        foreach tm $ZTMS {
            $tm delete
        }
    }
    # -------------------------------------------------- Private Methods ---
    # load in the profiles
    protected method profile_init {} {
        array set PROFILE {
            default {
                ZTMSession zs
                DB         db
                LTimer     timer
            }
        }
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
