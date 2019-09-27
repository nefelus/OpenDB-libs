## rmt.tcl
### Remote module/proxy used by Milos
### (c) 2006 Athena Design Systems
### Author: Mattias Hembruch

package require Itcl
package require ade::milos::plugin 0.1

package provide ade::milos::rmt 0.1

## RMT LOCAL is a handle for the LOCAL RMT object - its main function is plugin
## handling - everything else is pretty much a pass-through

itcl::class RMT_LOCAL_quartz {

    inherit RMT_LOCAL_psta

    public method set_plugin_status { } {
        set plugins_local { TE CELTIC }
        set plugins_remote { }
        set plugins_ignore { }
        set plugins_invalid { }
        
        init_plugin_object TE PLUGIN_quartz 

    }

    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        if { $name eq "" } {
            set r [RMT_LOCAL_quartz #auto]
        } else {
            set r [RMT_LOCAL_quartz $name]
        }
        return $r
    }

    public method get_db { name } {

        ## Write files (can only be done on main - not sure of having DB)
        file mkdir [$sobj cget -tmp_dir]

        UPOBJ enable_eco_log quartz

        if { [$sobj cget -spef_file] eq "" } {
            set ec [$corner cget -ext_corner]
            set sname [plugin_fname $sobj "quartz" "spef"]
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

