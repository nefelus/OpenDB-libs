package provide ade::ui 1.0

#       Class: CPanel
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: ATHENA DESIGN SYSTEMS, 2004
#
itcl::class CPanel {
    inherit LFrame LBinder
    # -------------------------------------------------- Public Variables --
    public variable bbox ""
    public variable colors ""
    public variable rpc ""
    # -------------------------------------------------- Public Methods ----
    # -- show the filters defined
    public method ShowFilters {expr} {
        filter_set $expr
        selection_init
    }
    public method Selection {} {
        array get SELECTION
    }
    public method LoadBV {tiles} {
        $bview LoadTiles $tiles
    }
    public method SelectionSet {args} {
        eval $bview SelectionSet $args 
    }
    public method Monitor {} {
        $MV Monitor
    }

    # -- set a filter to a value, no events will be produced
    public method Set! {filters} {
        foreach {filter fv} $filters {
            foreach {a v} $fv {
                # skip the root node
                if { $a == "." } { continue }
                $FILTER($filter) Select! $a $v
            }
        }
    }

    public method SelectAll {} {
        $bview SelectAll
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable bview
    protected variable wfilt
    protected variable FILTER
    protected variable MV                  ;# multiprocessing viewer
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief sunken
        pack [mk_filters $This.filters] -fill x -side bottom
        pack [label $This.tl -text Filters: ] -side bottom -fill x

        set bview [BView $This.bview -width 180 -height 140]
        pack $This.bview -side top 
        $bview Redirect <<select-region>> $this 
        LBridge bbox "[namespace current]::$bview configure -bbox"

        set MV [MSHViewer $This.bv]
        pack $MV -side top

        # get all events
        $MV Redirect <<task-info>> $this

        LBridge rpc "[namespace current]::$MV configure -rpc"

        selection_init
    }
    destructor {
        $bview delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mk_filters {w} {
        frame $w -class Filter  -border 1 -relief sunken
        set wfilt $w.f
        pack [frame $wfilt] -side top -fill both 
        set w
    }
    # ----------------------------------------------- Interface Implementation
    protected method filter_set {filters} {
        foreach flt [array names FILTER] {
            $FILTER($flt) delete
        }
        destroy $wfilt
        pack [frame $wfilt -padx 5 -pady 5 ] -side top -fill both 
        set N 0
        foreach {filter flname fdef} $filters {
            set w $wfilt.f$N
            label $w-l -text $flname

            set FILTER($filter) [LSTree $w -terminals 1 -timeout 800]
            if { $filter == "layer" } {
                $FILTER($filter) configure -colors $colors
            }
            grid $w-l -padx 1 -pady 2 -sticky ew -column $N -row 0
            grid $w -padx 1 -pady 2 -sticky nsew -column $N -row 1

            foreach {f fname val} $fdef {
                $w Add $f $fname
                $w Select $f $val
            }
            $w Bind <<selected>> $this eFilterSet $filter
            incr N
        }
    }
    protected  method selection_init {} {
        array set SELECTION {x0 0 y0 0}
        set SELECTION(x1) [$bview cget -width]
        set SELECTION(y1) [$bview cget -height]
        set SELECTION(filters) [array get FILTER]
    }
    # ----------------------------------------------- events
    public method eFilterSet {filter value} {
        set res ""
        foreach f [array names FILTER] {
            lappend res $f [$FILTER($f) Get]
        }
        Event <<select-filter>> $res
    }
    # -------------------------------------------------- Common Initialization
}
