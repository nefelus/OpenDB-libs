## plugin.tcl - plugin methods
## System module for Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::papi 0.1


itcl::class PAPI_CELTIC {

    inherit PAPI_BASE;

    ##### ACTUAL METHODS
    public method constructor { } {
        set init_list { }
        set plugin_proc_list { celtic_init celtic_analyze }
        set server_proc_list { Server exit_remote open_debug debug close_debug }
        set plugin_api_list { }
    }

    public proc celtic_init { dprefix top spef sdc libs noiseLib vddSignals vddValue gndSignals gndValue settingsFile { myprocess 130nm } } {

        global dpref

        set dpref $dprefix

        #if { $::dlevel & 1 } {
        #    open_debug
        #}

        ## set the run mode
        set_run_mode -process $myprocess

        ## To turn on the timer
        set_parm timing 1

        ## enable the message handler
        message_handler -set_msg_level ALL

        foreach lib $libs {
            read_dotlib $lib
        }
        
        ## Set supply voltage
        set_supply -vdd $vddValue -gnd $gndValue

        # load_netlist
        load_netlist -spef $spef -cdb $noiseLib -top $top \
                     -vdd "$vddSignals" -gnd "$gndSignals"

        process_netlist
        read_dc_script $sdc
        calculate_delay

        if { [file exists $settingsFile] } {
            source $settingsFile
        }

    }

    public proc celtic_analyze { sdfFile si { reportFile "" } } {

        if { $si > 0 } {
            set_tw_convergence -iterations $si -first_infinite_windows
        }
        ## get noise data
        analyze_noise -delay

        if { $reportFile != "" } {
            if { [file exists $reportFile] } {
                source $reportFile
            } else {
                error "Unable to find report file '$reportFile' - exiting"
            }
        }

        ## write sdf file
        generate_sdf -file $sdfFile
    }

    public proc exit_remote { } {
        exit
    }

}
