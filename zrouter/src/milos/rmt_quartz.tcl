## rmt.tcl
## Remote module/proxy used by Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl
package require ade::milos::mproxy
package require ade::milos::rmt_base

package provide ade::milos::rmt 0.1


itcl::class RMT_quartz {
    inherit RMT_psta 

    public method set_plugin_status { } {
        set plugins_local { }
        set plugins_remote { CELTIC TE }
        set plugins_ignore { }
        set plugins_invalid { }

        init_plugin_object TE PLUGIN_quartz
    }

    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        if { $name eq "" } {
            set r [RMT_quartz #auto $async $rmt_app]
        } else {
            set r [RMT_quartz $name $async $rmt_app]
        }
        return $r
    }


    public method get_db { name } {

        ## Write files (can only be done on main - not sure of having DB)
        file mkdir [$sobj cget -tmp_dir]

        set sname [plugin_fname $sobj "quartz" "spef"]
        UPOBJ enable_eco_log quartz

        if { [$sobj cget -spef_file] eq "" } {
            set ec [$corner cget -ext_corner]
            call_milos "pdb write_spef -prime_time 1 -outfile $sname -ext_corner_name $ec"
            $sobj configure -spef_file "${sname}[get_spef_ext]"
        }
        
        if { [$sobj cget -verilog_file] eq "" } {
            write_verilog $sobj
        }

        check_files [$sobj cget -spef_file] [$sobj cget -verilog_file]
        plugin_api TE set_timing_engine
        copy_data_to_plugin
    }


    public method start_db { } {
        ## Set up plugin
        incr startsocket 100
        plugin_api TE start_plugin $startsocket
    }

}
