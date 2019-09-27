## rmt.tcl
### Remote module/proxy used by Milos
### (c) 2006 Athena Design Systems
### Author: Mattias Hembruch

package require Itcl
package require ade::milos::plugin 0.1

package provide ade::milos::rmt 0.1

## RMT LOCAL is a handle for the LOCAL RMT object - its main function is plugin
## handling - everything else is pretty much a pass-through

itcl::class RMT_LOCAL_pt {

    inherit RMT_LOCAL_psta

    public method set_plugin_status { } {
        set plugins_local { TE CELTIC }
        set plugins_remote { }
        set plugins_ignore { }
        set plugins_invalid { }
        
        init_plugin_object TE PLUGIN_pt 

    }

    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        if { $name eq "" } {
            set r [RMT_LOCAL_pt #auto]
        } else {
            set r [RMT_LOCAL_pt $name]
        }
        return $r
    }

    public method reinit_db { name } {

        set ec [$corner cget -ext_corner]
        set sname [plugin_fname $sobj "pt${reinit_cnt}" "spef"]
        $sobj configure -spef_file "${sname}[get_spef_ext]"
        if { [$sobj cget -translate] == 1 } {
            call_milos "pdb write_spef -no_backslash 1 -prime_time 1 -outfile $sname -ext_corner_name $ec"
        } else {
            call_milos "pdb write_spef -prime_time 1 -outfile $sname -ext_corner_name $ec"
        }
        incr reinit_cnt

        if { [$sobj cget -translate] == 1 } {
            write_verilog $sobj / 1
        } else {
            write_verilog $sobj
        }

        ## End write files

        check_files [$sobj cget -spef_file] [$sobj cget -verilog_file]

        plugin_api TE set_obj_value sobj -spef_file [$sobj cget -spef_file]
        plugin_api TE set_obj_value sobj -verilog_file [$sobj cget -verilog_file]
        plugin_api TE myreinit
        lists_to_plugin
    }

    public method get_db { name } {

        ## Write files (can only be done on main - not sure of having DB)
        file mkdir [$sobj cget -tmp_dir]

        UPOBJ enable_eco_log pt

        if { [$sobj cget -spef_file] eq "" } {
            set ec [$corner cget -ext_corner]
            set sname [plugin_fname $sobj "pt" "spef"]
            $sobj configure -spef_file "${sname}[get_spef_ext]"
            if { [$sobj cget -translate] == 1 } {
                call_milos "pdb write_spef -no_backslash 1 -prime_time 1 -outfile $sname -ext_corner_name $ec"
            } else {
                call_milos "pdb write_spef -prime_time 1 -outfile $sname -ext_corner_name $ec"
            }
        }

        if { [$sobj cget -verilog_file] eq "" } {
            write_verilog $sobj
            if { [$sobj cget -translate] == 1 } {
                $sobj configure -verilog_original_file [$sobj cget -verilog_file]
                write_verilog $sobj / 1
            }
        }
        ## End write files

        check_files [$sobj cget -spef_file] [$sobj cget -verilog_file]
        plugin_api TE set_timing_engine 
        copy_data_to_plugin
    }

    public method start_db { } {
        ## Set up plugin
        incr startsocket 100
        plugin_api TE start_plugin $startsocket
    }

    public method save_session { dir } {
        plugin_api TE save_session $dir
    }
}

