## plugin.tcl - plugin methods
## System module for Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::plugin 0.1

itcl::class PLUGIN_celtic {

    inherit PLUGIN_base

    public method constructor { } {
        set rc [ catch {set api_obj [ [PAPI_CELTIC #auto] this]} output ]
    }

    protected variable tmp_dir
    public method create_tmp_files { } {
        set tmp_dir [file join [$sobj cget -tmp_dir] celtic_[$sobj get_sidx]]
        puts "TMP: $tmp_dir ([$sobj get_sidx])"

        if { [$sobj cget -celtic_copy_lib] eq 1 } {
            foreach l [$corner cget -lib] {
                exec cp -f $l $tmp_dir
            }

            foreach l [$sobj cget -celtic_noiselib] {
                exec cp -f $l $tmp_dir
            }
            exec cp -f [join [$sobj cget -celtic_settings_file]] $tmp_dir
            exec cp -f [join [$sobj cget -sdc_file]] $tmp_dir

            set rf [join [$sobj cget -celtic_report_file]]
            if { [file exists $rf] == 1 } {
                exec cp -f $rf $tmp_dir
            }
        }
        ## need msh.conf in rundir..
        exec cp -f $::env(MSHCONFIG) $tmp_dir
    }

    public method write_plugin_file { si } {

        set startdir [pwd]
        cd $tmp_dir

        set fname "celtic.tcl"
        set fp [open $fname w]

        ## save for later
        set plugin_tcl_file $fname

        $api_obj write_timing_functions $fp
        myinit $fp
        myanalyze $fp $si
        close $fp

        cd $startdir
    }

    public method set_si { si } {
        plugin_cmd CELTIC set_si $si
    }

    protected method myinit { fp } {

        ## first, create proper verilog
       
        set deb_pref [plugin_fname $sobj "celtic_debug" "log"]
        set cmd "celtic_init "
        append cmd "$deb_pref "
        append cmd "[$sobj cget -verilog_module] "
        append cmd "[$sobj cget -celtic_spef_file] "
        append cmd "[$sobj cget -sdc_file]  "
        append cmd "{"
        if { [$sobj cget -celtic_copy_lib] eq 1 } {
            foreach l [$corner cget -lib] {
                append cmd "[file tail $l] "
            }
        } else {
            foreach l [$corner cget -lib] {
                append cmd "$l "
            }
        }
        append cmd "} "
        append cmd "{[$sobj cget -celtic_noiselib]} "
        append cmd "{[$sobj cget -celtic_vdd_signals]} "
        append cmd "[$sobj cget -celtic_vdd_value] "
        append cmd "{[$sobj cget -celtic_gnd_signals]} "
        append cmd "[$sobj cget -celtic_gnd_value] "
        append cmd "[$sobj cget -celtic_settings_file] "
        append cmd "[$sobj cget -celtic_process] "

        debug PLUGIN:C "Celtic init cmd: $cmd"

        #set plog [plugin_fname $sobj "plog_celtic" "log"]
        #open plog CELTIC $plog

        puts $fp "file delete celtic.log"
        puts $fp $cmd

        #plugin_cmd CELTIC $cmd
    }

    protected method myanalyze { fp si } {

        puts $fp "celtic_analyze celtic.sdf $si [$sobj cget -celtic_report_file]"
    }

}
