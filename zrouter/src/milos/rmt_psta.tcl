## rmt.tcl
## Remote module/proxy used by Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl
package require ade::milos::mproxy
package require ade::milos::rmt_base

package provide ade::milos::rmt 0.1


itcl::class RMT_psta {
    inherit RMT RMT_LOCAL_psta

    public method sta { args } {
    }

    public method set_plugin_status { } {
        set plugins_local { }
        set plugins_remote { CELTIC TE }
        set plugins_ignore { }
        set plugins_invalid { }

        init_plugin_object TE PLUGIN_psta
    }

    protected method send_remote_plugin_api { type cmd {args ""}} {

        if { $args eq "" } {
            send_command acmd [escape "::eval \$::apps::milos::mycf plugin_cmd $type $cmd "]
        } else {
            send_command acmd [escape "::eval \$::apps::milos::mycf plugin_cmd $type $cmd \"[join $args]\""]
        }
    }

    protected method init_remote_plugin_obj { type datatype } {
        send_command acmd "\$::apps::milos::mycf init_plugin_obj $type $datatype"
    }

    protected method copy_data_to_plugin { } {
        ## TODO
        set lst [$sobj get_conf]
        #puts "CDTP: $lst"
        foreach { option value } $lst {
            #puts "CDTP: $option, $value"
            send_command acmd "\$::apps::milos::mycf plugin_cmd TE set_obj_value sobj $option {$value}"
        }

        set lst [$corner get_conf]
        foreach { option value } $lst {
            send_command acmd "\$::apps::milos::mycf plugin_cmd TE set_obj_value corner $option {$value}"
        }

        if { [state thresh_obj] ne "" } {
            foreach { option value } [ [state thresh_obj] get_conf] {
                send_command acmd "\$::apps::milos::mycf plugin_cmd TE set_obj_value thresh $option $value"
            }
        }

        plugin_api TE set_sidx [$sobj get_sidx]
        #plugin_api TE set_si [state rc_models_si]
    }

    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        debug PLUGIN:E "06192012:rmt_psta.tcl name=$name async=$async rmt_app=$rmt_app"
        if { $name eq "" } {
            set r [RMT_psta #auto $async $rmt_app]
        } else {
            set r [RMT_psta $name $async $rmt_app]
        }
        debug PLUGIN:E "r=$r  --- 06192012:rmt_psta.tcl name=$name async=$async rmt_app=$rmt_app"
        return $r
    }


    protected method get_eco_string { } {
        ### Remote version

        debug PLUGIN:E "GET ECO string:"
        set eco_string [UPOBJ get_eco_string]
        debug PLUGIN:E "GET ECO String 2: $eco_string"
        set eco_string [escape_line $BS_DLR $eco_string "\${::MILOS_DOLLAR_SIGN}"]
        debug PLUGIN:E "GET ECO String 3: $eco_string"
        if { [$sobj cget -translate] == 1 } {
            set eco_string [escape_line $BS_P $eco_string ""]
        }
        plugin_api TE send_eco_string [list $eco_string]
    }

    public method set_update_method { method } {
        send_command acmd "\[\$::apps::milos::mycf get_tm Tmg] set_update_method $method"
    }


    public method update_node_slack { req_slacks } {
        debug PLUGIN:E "                                           update_node_slack -- rmt_psta\n"
        foreach type $req_slacks {
            set newret $SLACK_UPDATE
            plugin_api TE update_node_slack $type
        }
        debug PLUGIN:E "                                     END   update_node_slack\n"
    }


    public method send_activeterms { str_string { BBD_OPT "" } } {
        debug PLUGIN:E "send_activeterms $str_string $BBD_OPT"
	if {$BBD_OPT ne "ON"} {
        	subscribe $str_string
	}
        plugin_api TE send_activeterms
    }
}
