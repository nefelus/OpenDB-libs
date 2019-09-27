## plugin.tcl - plugin methods
## System module for Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::plugin 0.1

itcl::class PLUGIN_pt {

    inherit PLUGIN_psta

    public method constructor { } {
        set rc [ catch {set api_obj [ [PAPI_PT #auto] this]} output ]
    }

    public method read_incr_spef { file } {
        plugin_cmd TE read_spef $file
    }

    public method read_i_incr_spef { file } {
        plugin_cmd TE read_i_spef $file
    }

    public method read_ni_incr_spef { file } {
        plugin_cmd TE read_ni_spef $file
    }

}
