package provide ade::ui 1.0

#       Class: ZScope
# Description: Zui microscope
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2004
#
itcl::class ZScope {
    inherit LTopLevelEditor LBinder
    # Event <<select>> id
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method ShowObjects {objs} {
        $nb Raise view
        $TOOL(view) Clear
        $TOOL(view) Show $objs
    }
    public method Show {text} {
        $nb Raise info
        $TOOL(info) Clear
        $TOOL(info) Show $text
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable TOOL
    protected variable nb
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        pack [mktabs $This.tabs] -side top -expand 1 -fill both
        wm title $This "ZUI: INFO"
    }
    destructor {
        foreach t [array names TOOL] {
            $TOOL($t) delete
        }
        $nb delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mktabs {w} {
        set nb [LSNotebook $w]
        set tools "view TreeViewer View info TextViewer Info"
        foreach {tag class tl} $tools {
            set wt [eval $class [$nb Add $tag  $tl].$tag]
            pack $wt  -side top -expand 1 -fill both
            set TOOL($tag) $wt
            $wt Redirect <<select>> $this
        }
        set w
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#
#       Class: TreeViewer
# Description: Dependency viewer
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2004
#
itcl::class TreeViewer {
    inherit LFrame LBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {objs} {
        foreach o $objs {
            array set P "type ? hierarchy ?"
            array set P $o
            $tree Add /$P(id) -title "$P(id) ($P(hierarchy)/$P(type))"
            unset P
        }
        $tree Open /
    }
    public method Clear {} {
        $tree Clear
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable tree
    protected variable linfo
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set tree [LDirTree $This.tree -width 200 -height 200]
        pack $tree -side top -expand 1 -fill both
        $tree Bind <<select>> $this eSelect
    }
    destructor {
        $tree delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eSelect {path} {
        if { $path == "" } { return }
        Event <<select>> [file tail $path]
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: TextViewer
# Description: 
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2004
#
itcl::class TextViewer {
    inherit LFrame LBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {val} {
        $text delete 1.0 end
        $text insert 1.0 $val
    }
    public method Clear {} {
        $text delete 1.0 end
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable text
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set text [LScrollableWidget $This text -width 40 -height 10 -bg white]
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eSelect {path} {
    }
    # -------------------------------------------------- Common Initialization
}
