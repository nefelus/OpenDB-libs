package provide ade::ui 1.0

#
#       Class: ZHelp
# Description: Zui Popup help
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2004
#
itcl::class ZHelp {
    inherit LWBrowser
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {page} {
        zh_show $page
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    common HELP
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        Show /
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    private method zh_show {url} {
        if ![info exists HELP($url)] {
            set url /error
        }
        LWBrowser::ShowPage $HELP($url)
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization

array set HELP {

/ { }

/gcmds {
<h2>ZUI commands</h2>

The following keyboard/mouse shortcuts are recognized:<br>

<PRE>
<b>        KEY         Description</b>
           SPACE: zoom in
  CONTROL  SPACE: zoom out
      Arrow Keys: pan left, right, up and down
LEFT MOUSE
          BUTTON: Select an object
SHIFT+LEFT
    MOUSE BUTTON: select an area to zoom in
RIGHT
    MOUSE BUTTON: Inspect an object
             ESC: Interrupt the drawing of large number of objects
</PRE>
}

/error {
    This page does not exist
}

/about {
<center>
<b>
<i>
ZUI: ZROUTE(tm) graphical user interface<br>
(c) Athena Design Systems, 2004-2005<br>
</i>
</b>
</center>
}

/zdb {
<h2>ZDB: ZROUTE debugger</h2>

<PRE>
It can be invoked from the zroute command prompt, and will
accept additional arguments corresponding to the tcl script to run:

% zdb db ztest

This particular command will invoke the ztest routine will simulate
the effect of debugging c++ code.

At the zdb prompt, you can use help to see the available commands.

ZDB will allow stepping through any zevent posted by the C++ code,
with additional logic to stop when a condition is true.

}


/ucmds {
<h2>ZUI user commands</h2>
<P>
Those commands are entered from the command line window, at the
bottom of the screen:
<PRE>
show net id ID  : to show a net by id
show id         : try to guess what is id, and then show the corresponding obj
gfactor  INT    : grid size in pixels to sample/aggregate
bbox x0 y0 x1 y1: shoe the bounding box specified in the current mode
</PRE>
}


}



}
