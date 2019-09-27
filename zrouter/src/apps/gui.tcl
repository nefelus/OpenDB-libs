package require Tk

namespace eval history {
    proc add? {w} {
    variable $w
    variable n$w
    upvar 0 $w hist
    set s [set ::[$w cget -textvariable]]
    if {$s == ""} return
    if [string compare $s [lindex $hist end]] {
        lappend hist $s
        set n$w [llength $hist]
    }
    }
    proc move {w where} {
    variable $w
    variable n$w
    upvar 0 $w hist
    incr n$w $where
    if {[set n$w]<0} {set n$w 0}
    if {[set n$w]>=[llength $hist]+1} {
        set n$w [llength $hist]
    }
    set ::[$w cget -textvar] [lindex $hist [set n$w]]
    }
    proc for {type name args} {
    switch -- $type {
        entry {
        uplevel $type $name $args
        bind $name <Up> {history::move %W -1}
        bind $name <Down> {history::move %W 1}
        bind $name <Next> {history::move %W 99999}
        bind $name <Return> {history::add? %W}
        variable $name {}
        variable n$name 0
        }
        default {error "usage: history::for entry <w> <args>"}
    }
    }
 }



text .zgt -wrap word -relief flat -state disabled
set zgtprompt "ade>"

proc zgtinsert { w text } {
    $w configure -state normal
    $w insert end $text
    $w configure -state disabled
    $w see end
}

proc zgtprocess { w cmd} {
    global zgtprompt
    global cmpcmd
    #zgtinsert $w "$zgtprompt $cmd\n"
    if { $cmpcmd == "" } {
        if { [info complete $cmd] } {
            ::zr.cli runcmd $cmd
        } else {
            zgtinsert $w $cmd
            append cmpcmd $cmd
        }
    } else {
        append cmpcmd $cmd
        if { [info complete $cmpcmd] } {
            ::zr.cli runcmd $cmpcmd
            set cmpcmd ""
        } else {
            zgtinsert $w $cmd
        }
    }

}

set cmpcmd ""
frame .zgf 
label .zgf.l -text $zgtprompt
history::for entry .zgf.e -textvar zgtcmd -relief flat
bind .zgf.e <Return> {+ zgtprocess .zgt $zgtcmd; set zgtcmd ""}
pack .zgf.l -side left
pack .zgf.e -side left -fill x -expand 1

eval pack [winfo children .] -fill both -expand 1
focus .zgf.e

rename exec ::tcl_exec

proc exec { cmd } {
    set rc [ catch { ::tcl_exec $cmd } out ]
    puts $out
    return $rc
}

::zr.zlog configure -guiwidget .zgt -guiinsert "zgtinsert"
