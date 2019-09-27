## rmt.tcl
## Remote module/proxy used by Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl
package require ade::milos::mproxy
package require ade::milos::rmt_base

package provide ade::milos::rmt 0.1


itcl::class RMT_milos {
    inherit RMT RMT_LOCAL_milos
    ### PUBLIC PROCS - NEW
    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        #puts "NRM!! $name $async $rmt_app"
        if { $name eq "" } {
        #    puts "NRM 1 $async $rmt_app"
            set r [RMT_milos #auto $async $rmt_app]
        } else {
        #    puts "NRM 2 $name $async $rmt_app"
            set r [RMT_milos $name $async $rmt_app]
        }
        return $r
    }

    constructor { async rmt_app } {
        RMT::constructor $async $rmt_app 
    } {
        #puts "Derived const!"
    }

    public method update_node_slack { req_slacks } {
        set newret $SLACK_UPDATE
        send_command acmd "\$apps::milos::mycf get_slacks $req_slacks"
    }

    public method update_eco { str } {
        send_command acmd "Subscribe $str"
        send_command tmg update_timing -force 0
    }

    public method reinit_db { dbs } {
        get_db $dbs
    }

    public method get_db { dbs } {
        send_command acmd "Subscribe $dbs"
        send_command acmd { set ::ns [::apps::milos::cur_ns] }
        send_command acmd "set \${::ns}::STATE(db_loaded) 1"
        send_command acmd "set \${::ns}::STATE(db_parasitics) 1"
        send_command acmd "set \${::ns}::STATE(tmg_rcmodels) 1"
    }

    public method start_db { } {
    }

    public method update_timing { {scenario 0} {num_scenarios 1} } {
        send_command acmd {[$::apps::milos::mycf get_tm Tmg] reset_timing_cache -scenario 0 -num_scenarios 1}
    }

    public method publish { name } {
        send_command acmd "Publish $name"
    }

    public method unpublish { name } {
        send_command acmd "Unpublish $name"
    }

    public method subscribe { name } {
        send_command acmd "Subscribe $name"
    }

    public method set_update_method { method } {
        send_command acmd "\[\$::apps::milos::mycf get_tm Tmg] set_update_method -method $method"
    }

    public method send_activeterms { string } {
        send_command acmd "Subscribe $string"
    }

    public method set_si { num } {
        send_command tmg "set_si -si $num"
    }

    public method set_sobj { obj } {
        set sobj $obj
        set corner [find_obj corner_list corner [$sobj cget -corner]]
        ## initialize remote CF/plugin object from here..
    }
}
