
package provide ade::apps 1.0

namespace eval apps { 
}

namespace eval apps::turborc {

variable VERSION "TurboRC 1.0"
variable AE_PW "MYAEPW"

#
# TURBORC application
#

# -- Initialize the turborc application
# --    Context: This call is made on the master interpeter
# --     Inputs: interp (the slave interpeter to be initialized)
# --    Outputs: "" if ok, "message" on error
# -- Exceptions: raise errors

proc init {slave} {
    # -- expose the read command
    package require ade::turborc 1.0
    TurboRC ::apps::turborc::trc
    interp alias $slave trc "" apps::turborc::trc
    interp alias $slave rx "" apps::turborc::rnode
    #interp alias $slave puts "" apps::turborc::puts
    interp alias $slave puts "" puts
    interp alias $slave exit "" exit


    $slave expose source

    $slave expose pwd
    $slave expose cd
    set cwd [file normalize .]
    set twd [file join $::top "../lib/apps"]
    set cmd " cd $twd; source history.tcl; cd $cwd"

    $slave eval $cmd
    #$slave hide cd

    set cmd "
            proc ::parray {a {pattern *}} {
                upvar 1 \$a array
                if {!\[array exists array]} {
                    puts \"\\\"\$a\\\" isn't an array\"
                }
                set maxl 0
                foreach name \[lsort \[array names array \$pattern]] {
                if {\[string length \$name] > \$maxl} {
                    set maxl \[string length \$name]
                }
                }
                set maxl \[expr {\$maxl + \[string length \$a] + 2}]
                foreach name \[lsort \[array names array \$pattern]] {
                set nameString \[format %s(%s) \$a \$name]
                puts \[format \"%-*s = %s\" \$maxl \$nameString \$array(\$name)]
                }
            }"

    $slave eval $cmd

    #set cmd "proc ::!! { } {
        #set nid \[expr \[history nextid] - 2]
        #history redo \$nid
    #}"
    #$slave eval $cmd

    return ""
}

proc banner { } {
    trc banner
    return 0
}


proc putx { str args } {
    set len [llength $args]
    if { $len == 0 } {
        ::zr.zlog putl $str
    } elseif  { $len == 1 } {
        ::zr.zlog putl $str [lindex $args 0]
    } elseif  { $len == 2 } {
        ::zr.zlog putl $str [lindex $args 0] [lindex $args 1]
    }
}

proc rx { b args } {
    package require ade::turborc 1.0
    set cmd "TurboRC::rnode $b $args"
    eval $cmd
}


# -- end of namespace apps
}
