## plugin.tcl - plugin methods
## System module for Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package require ade::milos::papi 0.1
package provide ade::milos::plugin 0.1



itcl::class PLUGIN_psta {

    inherit PLUGIN_base

    public method constructor { } {
        set rc [ catch {set api_obj [ [PAPI_PSTA #auto] this]} output ]

    }

    public method read_incr_spef { file } {
        plugin_cmd TE read_spef $file

    }
    public method read_spef_after_eco { file min_max } {
        plugin_cmd TE read_spef $file $min_max
    }

    public method read_sdf { cnt } {
        set tmp_dir [file join [$sobj cget -tmp_dir] celtic_[$sobj get_sidx]]
        set csn "$tmp_dir/celtic_${cnt}.sdf"
        #puts "READ SDF: $csn"
        if { [file exists $csn] == 0 } {
            error "File $csn does not exist. Celtic did not run!"
        } else {
            plugin_cmd TE "remove_annotated_delay -all"
            plugin_cmd TE read_sdf $csn
        }
    }


}
