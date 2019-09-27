package provide ade::gdb 1.0

#
# gdb debug utilities for ade
#

namespace eval ade::gdb {
    variable dbpid { 0 0 }
    variable zr                                 ;# name of the executable
    namespace export dbkill gdb xxgdb ddd

proc init {} {
    variable zr
    set zr [info nameofexecutable]
}

# -- kill debugger
proc dbkill { } {
    variable dbpid
    foreach { sig pid } $dbpid { break; }
    if { $pid != 0 } {
        switch { $sig } {
            -INT { exec kill -INT $pid }
            -HUP { exec kill -HUP $pid }
            default { exec kill -INT $pid }
        }
        set dbpid { 0 0 }
    }
}

# -- run gdb on this ade session
proc gdb { {display ""} args } {
    variable zr
    variable dbpid
    set pid [pid]
    set dbpid -INT
    if { $display == "" } {
        set cmd "exec xterm -e gdb $zr $pid $args &"
    } else {
        if { $display == 0 } {
            set cmd "exec xterm -e gdb $zr $pid $args &"
        } else {
            set cmd "exec xterm -display $display -e gdb $zr $pid $args &"
        }
    }
    lappend dpid [eval $cmd]
}

# -- run xxgdb on this session
proc xxgdb { {display ""} } {
    variable dbpid
    variable zr 

    set pid [pid]
    set dbpid -HUP

    if { $display == "" } {
        lappend dbpid [exec xxgdb $zr $pid &]
    } else {
        lappend dbpid [exec xxgdb -display $display $zr $pid &]
    }
}

# -- run ddd (?)
proc ddd { {display ""} args } {
    variable zr
    variable dbpid

    set dbpid -HUP
    set pid [pid]

    if { $display == "" } {
        lappend dbpid [exec ddd $zr $pid $args &]"
    } else {
        lappend dbpid [exec ddd -display $display $zr $pid $args &]"
    }
}

}
