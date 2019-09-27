#
# provide applications
# 
package provide ade::main 1.0

#
# User Application Handling
#
# This file provides all functionality to support the reduced user
# access to ade internals, and only expose a particular application
#

namespace eval apps  {
    # Initialize application, if any
    # Create a slave interpeter, and initialize the application in there
    variable VERSION "";# the version of the app loaded
    variable APPNAME "";

    proc init {argv0 args} {
        variable VERSION
        variable APPNAME

        ## initialize to ADE settings
        set APPNAME [set ::zroute::APPNAME]
        set VERSION [set ::zroute::VERSION]

        set argv0 [file tail $argv0]
        if { $argv0 == "ade" } { return "" }
        if { $argv0 == "zroute" } { return "" }
        if { [info command ${argv0}::init] == "" } { 
            error "no application $argv0"
        }

        ## reset to app settings
        set APPNAME [set ${argv0}::APPNAME]
        set VERSION [set ${argv0}::VERSION]

        set ai [interp create -safe]
        set m [${argv0}::init $ai $args]
        if { $m == "" } {
            set VERSION [set ${argv0}::VERSION]
            return $ai
        }
        interp delete $ai
        error "$argv0: failed to initialize application:\n$m"
    }

    # -- return a banner to print
    proc banner {} {
        variable APPNAME
        variable VERSION 
        if { [info procs ${APPNAME}::banner] != "" } {
            ${APPNAME}::banner
        } elseif { $VERSION != "" } {
            return "\n$VERSION"
        }
    }

    proc rx { args } {
        package require ade::milos::rte 0.1
        set cmd "RTE::rnode $args"
        eval $cmd
    }

    proc rexec { dir name script log log2 sdf2 } {
        cd $dir
        puts "REXEC! [pwd] [exec hostname] ($name $script $log)"
        set ::env(MSHCONFIG) "./msh.conf"
        set cmd "exec $name $script > $log"
        eval $cmd
        exec cp $log $log2
        if { [file exists celtic.sdf] == 0 } {
            error "Celtic.sdf not found! Celtic did not run!"
        } else {
            exec cp celtic.sdf $sdf2
        }
    }
}
