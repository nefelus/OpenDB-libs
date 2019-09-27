package provide ade::help 1.0

namespace eval apps { 
    proc mod_init { } {
        variable MOD_DESC
        set MOD_DESC(ext) "Extraction Commands"
        set MOD_DESC(pdb) "Physical/DB Commands: load, save, export"
        set MOD_DESC(opt) "Optimization Commands"
        set MOD_DESC(tmg) "Timing Commands"
        set MOD_DESC(clk) "Clockworks Commands"
        set MOD_DESC(rte) "Routing Commands"
        set MOD_DESC(rpt) "Reporting Commands"
        set MOD_DESC(mme) "Multimode Commands"
        set MOD_DESC(sys) "System/Utility Commands"
    }

    proc mod_help { {args ""} } {
        variable cur_app
        variable ${cur_app}::VERSION
        variable ${cur_app}::APPNAME
        variable ${cur_app}::MODS
        variable MOD_DESC

        if { $args ne "" } {
            array set ml {}
            set cnt 0
            foreach mod $MODS {
                set ml($mod) 0
                foreach arg $args {
                    if { [ string match -nocase $arg $mod] == 1 } {
                        incr ml($mod)
                        incr cnt
                        break;
                    }
                }
            }
            if { $cnt > 0 } {
                puts "$APPNAME $VERSION help"
                foreach mod [array names ml] {
                    if { $ml($mod) > 0 } {
                        puts "    $mod: $MOD_DESC($mod)"
                    }
                }
                return
            }
        }
        puts "$APPNAME $VERSION help"
        puts "    Available command modules: "
        foreach mod $MODS {
            puts "        $mod: $MOD_DESC($mod)"
        }
        puts ""
        puts "    For additional help:"
        puts "    Command list: <module> help"
        puts "    Command detail: <module> help <cmd>"
        puts "    Full Command detail: <module> help <cmd> -long"
        puts "    Note: <cmd> may contain wildcards ? * \[] \\"
        puts "    Examples: ext help"
        puts "              pdb help load_design"
        puts "              pdb help write_spef -long"
        puts "              pdb help write* "

    }
    mod_init
}


