## plugin.tcl - plugin methods
## System module for Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::papi 0.1

itcl::class PAPI_UTILS {

    public method this { } {
        return $this
    }
    public method issocket { fp } {
        set rc [ catch { fconfigure $fp -sockname } ]
        #puts "ISS: $rc"
        return $rc
    }

    public method write_utils { fp proc_list } {
        foreach proc $proc_list { 
            #puts "Writing proc $proc"
            set pargs [$this info args $proc]
            set pdef [$this info body $proc]
            set cmd [list proc $proc $pargs $pdef]
            if { [issocket $fp] == 0 } {
                #puts "SOCKET!"
                puts $fp $cmd
                #puts $fp [list $cmd]
                puts $fp "."
                get_plugin_results $fp
            } else {
                #puts "not SOCKET!"
                puts $fp $cmd
            }
        }
    }

    public method write_code { fp proc_list } {
        foreach proc $proc_list { 
            puts "Writing proc $proc"
            set pdef [$this info body $proc]
            puts $fp $pdef
        }

    }

    protected method get_plugin_results { fp } {
        set result ""
        while { true } {
            gets $fp line
            if { $line eq "." } {
                break;
            } elseif { $line eq ".E." } {
                puts "ERROR in GPR: $result"
            } else {
                append result $line
            }
        }
        #puts "GPR results: $result"
    }
            

}
