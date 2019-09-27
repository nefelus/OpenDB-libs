## rmt.tcl
## Remote module/proxy used by Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl
package require ade::milos::mproxy
package require ade::milos::rmt_base

package provide ade::milos::rmt 0.1


itcl::class RMT_pt {
    inherit RMT_psta

    public method set_plugin_status { } {
        set plugins_local { }
        set plugins_remote { TE CELTIC }
        set plugins_ignore { }
        set plugins_invalid { }
    
        init_plugin_object TE PLUGIN_pt
    }   


    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        if { $name eq "" } {
            set r [RMT_pt #auto $async $rmt_app]
        } else {
            set r [RMT_pt $name $async $rmt_app]
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

        send_command acmd "\$::apps::milos::mycf plugin_cmd TE set_obj_value sobj -spef_file [$sobj cget -spef_file]"
        send_command acmd "\$::apps::milos::mycf plugin_cmd TE set_obj_value sobj -verilog_file [$sobj cget -verilog_file]"

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
