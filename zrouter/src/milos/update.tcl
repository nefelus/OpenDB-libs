## update.tcl
## Update module for Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::update 0.1

## Proxy object to receive messages for UPDATE
itcl::class UPDATE_PROXY { 
    inherit VOB LBinder

    protected method constructor { } {
        Bind <<ST>> $this test
    }

    public method test { args } {
        puts "Thread test: [join [join $args]]"
    }
}



## UPDATE is an abstract class - should not instantiate instances of it
itcl::class UPDATE {

    inherit VOB LBinder

    private variable db;
    private variable block;
    private variable tmg;
    private variable ext;

    ## if eco has been updated, we set this
    private variable eco_updated 0;

    ## list of active plugin engines

    ## different debug levels:
    ## > 0, print debugging messages
    ## 

    private variable eco_subscribe_string;
    private variable scratch_prefix "scratch";


    protected variable buffering 0;
    protected variable force_buffering 0;
    protected variable incr_spef_cnt 0;
    protected variable int_spef_cnt 0;
    protected variable dual_incr_spef 0;
    protected variable orig_incr_spef_name;
    protected variable new_incr_spef_name;
    protected variable si 0;

    protected common ECO_CNT 0;
    protected common DB_CNT 0;
    protected common PAR_CNT 0;
    protected common FULL_CNT 0;
    protected common INCR_CNT 0;
    protected common ACTIVE_CNT 0;

    protected variable idlist_init 0;
    protected variable timing_init 0;

    protected variable required_slacks "min max";

    protected variable eco_string "";
    protected variable eco_init 0;

    protected variable eco_status 0;
    protected variable eco_log_cnt 0;
    protected variable ext_corners {};
    protected variable ext_corner_cnt 0;

    protected variable _rmt_local "";
    protected variable rmt_list {};
    protected variable rmt_list_len 0;
    ## local sobj to use, some tmp directory available for everyone..
    protected variable sobj "";
    ## Protected Vars
    ##

    protected variable merged_slacks

    protected variable gui "";

    ## End Protected Vars

    protected method constructor { parent } { 
        VOB::constructor $parent 
        set NAME "Update"
        set SELFNAME "UPDATE$this"
        set_state slacks_to_update "min max"
        set_state node_slack_status "invalid"
        set_state slackslew_requirements "0 1 0 1"
        set_state slack_requirements "0 1"
        set_state slew_requirements "0 1"
        set merged_slacks [ [NODE_SLACK #auto] this]
    }

    destructor {
        puts "UPDATE: END GUI!"
        end_gui
    }

    private variable mdebug 0;
    protected method mydebug { msg } {
        if { $mdebug > 0 } {
            puts "UO: $msg"
        }
    }

    protected variable my_debug {0 0 0 0}
    public method set_debug { {ndebug {0 0 0 0}} } {
        set my_debug $ndebug
    }
    protected variable BBD_OPT "";
    public method enable_bbd_remotes { {bbd "ON"} } {
	set BBD_OPT $bbd
        notice "BBD_OPT is $BBD_OPT"
	
    }


    protected method plugins_enabled { } {
        if { $eco_log_cnt == 0 } {
            return -code return
        }
    }

    protected method set_ext_corners { } {
        set ext_corners [get_extraction_corners]
        set ext_corner_cnt [llength $ext_corners]
    }

    public method set_vars { } {
        set db [get_tm DB]
        $db get -top -object= ::mmblock
        set block $::mmblock
        set tmg [get_tm Tmg]
        set ext [get_tm Ext]
    }

    public method sync_nodes { {delay 0.1} {msg ""} } {
        return [parent sync_nodes $delay $msg]
    }

    public method set_si { val } {
        set si $val
        foreach rmt [full_list] {
            $rmt set_si $si
        }
    }

    ##### GUI-related methods ############################################
    public method end_gui { } {
        if { $gui ne "" } {
            $gui end_gui
        }
    }

    public method start_gui { } {
        #set gui [[NGUI_PROXY #auto] this]
        set gui [NGUI_PROXY #auto]
        Bind * $gui gui_event
        $gui start_gui
    }

    public method send_gui_event { event args } {
        debug GUI:E "UPDATE GUI EVENT: $event $args"
        if { $gui ne "" } {
            set cmd "$gui gui_event $event $args"
            ::eval $cmd
        } else {
            notice "ignoring gui command: $event $args"
        }
    }

    ##### ECO-related methods ############################################
    public method update_eco { {undo 0} } {
        if { $::msh::OPT(dryrun) == 1 } {
            return
        }


        mydebug "BEGIN UPDATE ECO!!"

        set se_rc [store_eco_string]
        set eco_subscribe_string ""
        ## is there actually something to update on a remote node??
        set ec [ecocnt]
        mydebug "ECOCNT: $ec ($eco_status) ($se_rc)"
        if { ($eco_status > 0) && ($ec > 0) }  {
            set eco_subscribe_string "ECO.$ECO_CNT.eco"

            disable_eco
            save_eco

            Publish $eco_subscribe_string

            foreach rmt $rmt_list {
                $rmt update_eco $eco_subscribe_string
            }
            ## slowing down updates - finishes before main can run..
            #sync_nodes 0.1
            #check_multi_error

            enable_eco

            set_state node_slack_status "invalid"
        } else {
            if { ($se_rc == 0) && ($force_buffering == 0) } {
                ## NOTHING to update
                mydebug "NO ECOs to update!"
                return 0
            }
            set_state node_slack_status "invalid"
        }

        rmt_local update_eco ""

        reset_eco_strings
        $tmg reset_eco_string

        ## Now account for updated incremental spef
        if { $buffering == 1 } {
            
            set n2pref [rename_incr_spef $undo]

            if { $dual_incr_spef == 1 } {
                if { $n2pref ne "" } {
                    foreach rmt $rmt_list {
                        $rmt read_ni_incr_spef $n2pref.1
                        $rmt read_i_incr_spef $n2pref.2
                    }
                    rmt_local read_ni_incr_spef $n2pref.1
                    rmt_local read_i_incr_spef $n2pref.2
                }
            } else {
        mydebug "update_eco: dual_incr_spef=$dual_incr_spef -- n2pref= $n2pref"
                if { $n2pref ne "" } {
                    foreach rmt $rmt_list {
                        $rmt read_incr_spef $n2pref
                    }
                    rmt_local read_incr_spef $n2pref
                }
            }
        }


        sync_nodes 0.1 "Waiting for ECO update to finish"
        check_multi_error
        if { $eco_subscribe_string ne "" } {
            Unpublish $eco_subscribe_string
        }
        set eco_updated 1
        incr ECO_CNT
        mydebug "END UPDATE ECO!!"
        return 1
    }

    public method enable_eco { } {
        $db begin_eco -block $block
        incr eco_status;
    }

    public method disable_eco { } {
        $db end_eco -block $block
        incr eco_status -1
    }

    public method enable_eco_log {engine} {
        if { $eco_init == 0 } {
            ## TODO
            #$tmg enable_eco_log -file eco.out
            incr eco_init
        }
        incr eco_log_cnt
        set engine_list($engine,spef) 1
        set_ext_corners
    }

    public method get_eco_string { } {
        return $eco_string
        #return $engine_list($engine,eco)
    }

    private method store_eco_string { } {
        set ::myeco ""
        if { $eco_log_cnt > 0 } {
            $tmg get_eco_string -eco= ::myeco
            set eco_string $::myeco
            if { $eco_string ne "" } {
                return 1
            } else {
                return 0
            }
        } else {
            set eco_string ""
            return 0
        }
    }
    private method reset_eco_strings { } {
        set eco_string ""
    }


    public method eco_status { } {
        return "$eco_status"
    }

    public method ecocnt { } {
        $db check_eco -block $block -result= ::ecocnt
        return $::ecocnt
    }

    public method save_eco { {file ""} } {
        if { $debug >= 1 } {
            if { $file eq "" } {
                set file "ECO_$ECO_CNT.eco"
            }

	    file mkdir $scratch_prefix
            $db write_eco -block $block -file [file join $scratch_prefix $file]
        }
    }

    #### REMOTE-related methods #############################################
    
    protected method rmt_local { cmd args } {
        if { $_rmt_local ne "" } {
            if { [llength $args] > 0 } {
                set cmd "$_rmt_local $cmd $args"
                return [::eval $cmd] 
            } else {
                return [$_rmt_local $cmd]
            }
        }
    }

    public method set_rmt_local { rmt } {
        set _rmt_local $rmt
        set sobj [$rmt get_sobj]
    }

    public method get_rmt_local { } {
        return $_rmt_local
    }

    public method add_rmt { rmt } {
            notice "add_rmt: $rmt --"
        if { $sobj eq "" } {
            set sobj [$rmt get_sobj]
        }
        lappend rmt_list $rmt
        incr rmt_list_len
    }

    public method reset_rmt { } {
        set rmt_list {}
    }

    public method get_rmt { } {
        return $rmt_list
    }

    ## 1 = old, merged slack
    ## 2 = new, fully stored slack
    protected variable merge_method 1
    public method set_merge_method { type } {
        set merge_method $type
    }

    protected method full_list { } {
        return [concat $rmt_list $_rmt_local]
    }

    #### SPEF-related methods ######################################

    private method rename_spef_file { old new } {
        if { [file exists $old] == 0} {
            #warning "INCR Spef file $old not found!!"
            return -1
        } else {
            mydebug "RENAME: $old to $new"
            file rename $old $new
            return 0
        }
    }

    private method get_spef_prefix { } {
        return "[$sobj cget -tmp_dir]/[pid].$int_spef_cnt"
    }

    private variable cdebug 0;
    #012710D for SPEF file debugging
    private variable eco_debug 1;
    private method clean_files { args } {
        foreach arg $args {
            if { [file exists $arg] == 0 } {
                if { $cdebug > 0 } {
                    warning "clean_file $arg does not exist!!"
                }
            } else {
                if { $cdebug > 0 } {
                    mydebug "clean_file $arg"
                } else {
                    file delete $arg
                }
            }
        }
    }
    private method cleanup_spef { } {
        incr int_spef_cnt -1
        set npref [get_spef_prefix]
        incr int_spef_cnt 1
        set opref [get_spef_prefix]

        set ecidx 0
        foreach ec $ext_corners {
            if { $dual_incr_spef == 0 } {
                set nf "${npref}.new.${ecidx}.spef"
                set of "${opref}.old.${ecidx}.spef"
		if { $eco_debug==0 } {
                	clean_files $nf $of
		} elseif { $eco_debug==1 } {
                	clean_files $of
		}
            } else {
                foreach idx {1 2} {
                    set nf "${npref}.new.${idx}.${ecidx}.spef"
                    set of "${opref}.old.${idx}.${ecidx}.spef"
		    if { $eco_debug==0 } {
                	clean_files $nf $of
		    } elseif { $eco_debug==1 } {
                	clean_files $of
		    }
                }
            }
            incr ecidx
        }
    }
    private method rename_incr_spef { undo } {
        if { $undo == 0 } {
            cleanup_spef

            set npref "[get_spef_prefix].new"
            incr int_spef_cnt
            set opref "[get_spef_prefix].old"
            mydebug "npref: $npref ($undo) ($ext_corners)"
            set ecidx 0
            foreach ec $ext_corners {

                mydebug "ISPEF: Processing corner $ec"
                if { $dual_incr_spef == 0 } {
                    set nn "${new_incr_spef_name}.${ec}.${ecidx}.spef"
                    set n2 "${npref}.${ecidx}.spef"
                    if { [rename_spef_file $nn $n2] == -1 } {
                        ## no files written - no need for more updates
                        mydebug "NEW SPEFFILE $nn missing - skipping update cycle!"
                        return ""
                    }
    
                    set nn "${orig_incr_spef_name}.${ec}.${ecidx}.spef"
                    set n2 "${opref}.${ecidx}.spef"
                    rename_spef_file $nn $n2
                    incr ecidx
                    continue;
                }
                # cleanup spef
                set nn "${new_incr_spef_name}.${ec}.1.${ecidx}.spef"
                set n2 "${npref}.1.${ecidx}.spef"
                if { [rename_spef_file $nn $n2] == -1 } {
                    ## no files written - no need for more updates
                    mydebug "NEW SPEFFILE $nn missing - skipping update cycle!"
                    return ""
                }

                set nn "${orig_incr_spef_name}.${ec}.1.${ecidx}.spef"
                set n2 "${opref}.1.${ecidx}.spef"
                rename_spef_file $nn $n2

                # incr spef
                set nn "${new_incr_spef_name}.${ec}.2.${ecidx}.spef"
                set n2 "${npref}.2.${ecidx}.spef"
                if { [rename_spef_file $nn $n2] == -1 } {
                    ## no files written - no need for more updates
                    mydebug "NEW SPEFFILE $nn missing - skipping update cycle!"
                    return ""
                }

                set nn "${orig_incr_spef_name}.${ec}.2.${ecidx}.spef"
                set n2 "${opref}.2.${ecidx}.spef"
                rename_spef_file $nn $n2
                incr ecidx
            }
            return $npref
        } else {
            ## if undo == 1, read the original file from one turn ago..
            set opref "[get_spef_prefix].old"
            set topref $opref
            if { $dual_incr_spef == 1 } {
                set topref $opref.1
            }
            if { [file exists "${topref}.0.spef"] == 0  } {
                mydebug "OLD SPEFFILE ${topref}.0.spef missing - skipping update cycle!"
                return ""
            } else {
                return $opref
            }
        }
    }

    public method set_scratch_prefix { pref } {
        set scratch_prefix $pref
    }

    #### BUFFERING-related methods ##########################################

    public method enable_buffering { {forced 0} } {
        ## incr spef for buffering only used if plugins exist
        plugins_enabled

        set_state need_timing_for_undo 1

        set buffering 1
        set force_buffering $forced
        incr incr_spef_cnt
        set orig_incr_spef_name "[$sobj cget -tmp_dir]/orig_spef.$incr_spef_cnt.[pid]"
        set new_incr_spef_name "[$sobj cget -tmp_dir]/new_spef.$incr_spef_cnt.[pid]"
        set reader "prime_time"
        set idx 0
        set translate 0
        foreach sobj [state scenario_list] {
            debug PLUGIN:S "INCR Test: [$sobj cget -engine]"
            switch [$sobj cget -engine] {
                pt { break; }
                psta { set reader "psta"; break; }
            }
        }
        set translate [$sobj cget -translate]

        debug PLUGIN:S "INCR Final: $reader ($translate)"
        if { $translate == 1 } {
            $ext init_incremental_spef -origp $orig_incr_spef_name -newp $new_incr_spef_name -reader $reader -no_backslash
        } else {
            $ext init_incremental_spef -origp $orig_incr_spef_name -newp $new_incr_spef_name -reader $reader
        }
    }

    public method set_dual_incr_spef { } {
        set dual_incr_spef 1
    }
    public method disable_buffering { } {
        ## incr spef for buffering only used if plugins exist
        plugins_enabled

        set orig_incr_spef_name ""
        set new_incr_spef_name ""
        $ext init_incremental_spef -origp {} -newp {} 
        set buffering 0
        set force_buffering 0
        set_state need_timing_for_undo 0
    }


    #### DB/INIT-related methods ############################################

    ## send DB should transfer all the required data to a remote
    ## We rely on each RMT object to know how to transfer the appropriate data

    ## Meaning of Debug (by position - all default to 0):
    ## 0: print update debug messages (0 off, 1 on)
    ## 1: save spef files (0 off, 1 on)
    ## 2: F_DEBUG - write files instead of streaming data (0 off, 1 on)
    ## 3: P_DEBUG - write procs instead of streaming (0 off, 1 on)
    public method init_db { {pub_db 0}  {pub_blocks 0 } } {
        notice "UPDATE::init_db pub_db=$pub_db pub_blocks=$pub_blocks"
        set r 0
        set r [_init_db $pub_db $pub_blocks]
        return $r
    }
    private method _publish_db_blocks { suffix } {
        notice "Publish blocks"
        foreach rmt $rmt_list {
		set attr_val "-block"
		#set db_block [get_value $rmt $attr_val]
		set db_block [node_name $rmt]
		Publish $db_block.$suffix
        	mydebug "Publish $db_block.$suffix"
        }
	#Publish $db_block.block
        #mydebug "Publish $db_block.block"
    }
    private method _un_publish_db_blocks { suffix } {
        notice "Unpublish chip.$suffix"
        foreach rmt $rmt_list {
		#set db_block [get_value $rmt -block]
		set db_block [node_name $rmt]
		Unpublish $db_block.$suffix
        	mydebug "Unpublish $db_block.$suffix"
        }
	#Unpublish $db_block.block
        #mydebug "Unpublish $db_block.block"
    }
    private method _subscribe_db_top { suffix } {
        notice "Subscribe chip.$suffix"
        foreach rmt $rmt_list {
		$rmt send_command acmd "puts HERE"
		$rmt send_command acmd "Subscribe chip.tech"
		$rmt send_command acmd "Subscribe chip.libs"
		$rmt send_command acmd "Subscribe chip.$suffix"
		$rmt send_command acmd "::db reset_inst_hier -top"
	}
        sync_nodes
        check_multi_error
    }
    private method _subscribe_db_blocks { suffix } {
        notice "Subscribe blocks"
        foreach rmt $rmt_list {
			#set db_block [get_value $rmt -block]
		set db_block [node_name $rmt]
		$rmt send_command acmd "Subscribe $db_block.$suffix"
        	mydebug "request to Subscribe $db_block.$suffix"
        }
	#rmt_local subscribe chip.tech
	#rmt_local send_command acmd "Subscribe chip.libs"
	#rmt_local send_command acmd "Subscribe chip.mblock"
	
	#set db_block [get_value rmt_local -block]
	set db_block [node_name $rmt]
	rmt_local send_command acmd "Subscribe $db_block.$suffix"
        mydebug "request to Subscribe $db_block.$suffix"
        sync_nodes 0.1 "sync db_blocks"
        check_multi_error
    }
    private method _init_db { {pub_db 0}  {pub_blocks 0 }} {
        if { $::msh::OPT(dryrun) == 1 } {
            return
        }

        clear_verilog_names

        foreach {mdebug cdebug fdebug pdebug} $my_debug { break; }
        set debug [list $fdebug $pdebug]

        notice "INITIALIZING REMOTES"
        mydebug "INIT_DB ($my_debug)!"
        incr DB_CNT
        set db_subscribe_string "DB_${DB_CNT}.db"
        # Publish $db_subscribe_string
        # Publish chip.tech
        # Publish chip.libs

	if { $pub_db>0 || $pub_blocks>0 } {
        	notice "Publish chip.tech chip.libs chip.mblock"

        	Publish chip.tech
        	Publish chip.libs

        	set suffix "mblock"
        	Publish -dbargs "-skip_hier_refs" chip.$suffix

		_subscribe_db_top $suffix

        	Unpublish chip.tech
        	Unpublish chip.libs
        	Unpublish chip.$suffix

		if { $pub_blocks>0 } {
        		notice "Sending db blocks to Remotes"
        		set suffix "subblock"

    			_publish_db_blocks $suffix
			_subscribe_db_blocks $suffix
    			_un_publish_db_blocks $suffix
		}
		#_un_publish_db_blocks $pub_blocks
	}
	mydebug "CORNER_LIST= [state corner_list]"
	foreach iii [state corner_list] {
		mydebug "[$iii cget -corner] :: [$iii get_cmd]"
	}
        foreach rmt $rmt_list {
            $rmt pdebug $debug
            $rmt set_si $si
	    if { $BBD_OPT ne "ON" } {
	    	$rmt send_command acmd "::db save_def -file $rmt.def"
            	$rmt get_db $db_subscribe_string
		continue
	    }
            set inst [get_value $rmt -block]
            set blk [node_name $rmt]
            mydebug "get_db: RMT -- [node_name $rmt] $blk]"
	    $rmt send_command acmd "::zr.zlog enable_debug_log -module CLOCK"
	    $rmt send_command acmd "::db save_def -block_inst $blk -file $blk.def"
	    $rmt send_command acmd "::db get_children -top -block_name_list= NONE"
	    $rmt send_command acmd "::db bind_block -force -block_name $blk -instance $inst"
    	    set corner_cmd [get_corner_object $rmt]
	mydebug "before write_rmt_tmg_files:  $rmt has corner cmd: $corner_cmd"
            set rmt_script [$rmt write_rmt_tmg_files "ext.debug" $corner_cmd]
	    Publish $rmt_script.file

	    $rmt send_command acmd "Subscribe $rmt_script.file"
	    $rmt send_command acmd "source $rmt_script"
	#mydebug "INSTEAD of source:\n $CMDS"
	    #set NEW_CMD "set cmds \"$CMDS\"\nexec mkdir -p ETHERNET\nset tmpfp [open $rmt_script.new w]\nputs \$tmpfp \"\$::cmds\" \n close \$tmpfp\n source $rmt_script.new"
	#mydebug "NEW_CMD= $NEW_CMD"
	    #$rmt send_command acmd "$CMDS"

		# 033110 : get_db does NOT send DB, but writting Verilog/SPEF
            #$rmt get_db $db_subscribe_string
        }
        rmt_local set_si $si
        rmt_local pdebug $debug
        #mydebug "get_db: rmt_local -- [node_name rmt_local] [get_value rmt_local -block]"
        rmt_local get_db $db_subscribe_string

        notice "Data sent, waiting for Remotes to finish"
        sync_nodes
        check_multi_error
        notice "Remotes finished initializing"

        ### NOW start plugins
        notice "06192012 Starting plugins (if necessary)"
        foreach rmt $rmt_list {
            set inst [get_value $rmt -block]
	    #06192012 if { $inst ne "" } {
		#06192012 $rmt reset_default_tmg_tm "tmg"
	    #06192012 }
notice "06192012 -- rmt: $rmt  --- inst: $inst"
		# 033110D : actually start 3rd party plugins, make 3rd party read verilog, SPEF, initializing routines, loading tcl code; not really start ade db????
            $rmt start_db
        }
        rmt_local start_db

        sync_nodes
        check_multi_error
        Unpublish $db_subscribe_string

        ## just in case
        clear_verilog_names
    }

    public method get_corner_object { rmt } {
	set corner [get_value $rmt -corner]
	set cobj ""
	foreach iii [state corner_list] {
		mydebug "[$iii cget -corner] :: [$iii get_cmd]"
		if { [$iii cget -corner] eq $corner } {
			return [$iii get_cmd]
			break
		}
	}
        if { $cobj == "" } {
        error "No corner named $corner has been defined!" "" -1
        }
	mydebug "$rmt has corner cmd: [$cobj get_cmd]"
	return cobj
    }



    public method annotate_timing_structure { } {
        set r 0
        set r [_annotate_timing_structure]
        return $r
    }
    private method _annotate_timing_structure { } {
        if { $::msh::OPT(dryrun) == 1 } {
            return
        }
        notice "Getting Timing Structure info from Remotes"
        foreach rmt $rmt_list {
        notice "$rmt: Getting Timing Structure info from Remotes BEGIN"
            $rmt annotate_timing_structure 0 0
        notice "$rmt: Getting Timing Structure info from Remotes END"
        }
        notice "rmt_local: Getting Timing Structure info from Remotes BEGIN"
        rmt_local annotate_timing_structure 0 0
        notice "rmt_local: Getting Timing Structure info from Remotes END"

    }
    public method update_parasitics { } {
        if { $::msh::OPT(dryrun) == 1 } {
            return
        }

        ### This is called when extraction is done during multimode..
        ### Need to update parasitics..

        notice "UPDATE PARASITICS.."
        
        #incr PAR_CNT
        #set par_subscribe_string "PAR_$PAR_CNT.db"
        #Publish -dbargs -parasitics $par_subscribe_string
        #foreach rmt $rmt_list {
            #$rmt get_parasitics $par_subscribe_string
        #}
        #Unpublish $par_subscribe_string
    }

    public method restart_remotes { {replay 1} } {
        ## all ECOs so far will be captured by this update
        clear_verilog_names
        disable_eco
        incr DB_CNT
        set db_subscribe_string "DB_${DB_CNT}.db"
        Publish $db_subscribe_string
        foreach rmt [full_list] {
            puts "Reinitializing remote $rmt"
            $rmt reinit_db $db_subscribe_string
            if { $replay == 1 } {
                $rmt replay_plugin_cmds
            }
        }
        notice "Data sent, waiting for Remotes to finish"
        sync_nodes
        check_multi_error
        notice "Remotes finished re-initializing"
        enable_eco
        clear_verilog_names
        set ss [state slackslew_requirements]
        set_state node_slack_status "invalid"
        set_state slackslew_requirements "0 0 0 0"
        set_state slack_requirements "0 0"
        set_requirements $ss

        ## reset eco/spef values
        reset_eco_strings
        $tmg reset_eco_string
        disable_eco
        enable_eco
    }

    #### SLACK-related methods #############################################

    public method update_node_slacks { required_slacks } {
        if { $::msh::OPT(dryrun) == 1 } {
            return
        }

        mydebug "BEGIN UPDATE NODE SLACKS!!"


        if { [state node_slack_status] ne "invalid" } {
            ## nothing needed to update
            mydebug "NO SLACK UPDATE NEEDED: [state node_slack_status]!"
            return
        }
        mydebug "SLACK UPDATE NEEDED: $required_slacks!"

        send_node_status action "Slacks ==> Main"
        gui_status main "" "Receive Slacks from Remotes"

        ## update ECO if needed

        ## Now, always force an update when we got here, because slacks are 
        ## definitely out of date..
        mydebug "before get_one_end_term"
        # DONOTFORGET ---------------------------------------- $tmg get_one_end_term
        #$tmg get_one_end_term
	if { $BBD_OPT ne "ON" } {
        	$tmg get_one_end_term
	} else {
        mydebug "BBD_OPT -- FORCED to do -- update_timing" 
		update_timing "aterms" "atfl"
        mydebug "END                     BBD_OPT -- FORCED to do -- update_timing" 
	}
        mydebug "AFTER get_one_end_term"

        #this will update remote node slacks
        sync_nodes
        check_multi_error
        foreach rmt $rmt_list {
            $rmt update_node_slack $required_slacks
        }
        rmt_local update_node_slack $required_slacks
        sync_nodes
        check_multi_error
        set_state node_slack_status "valid"
        mydebug "END UPDATE NODE SLACKS!!"

        send_node_status action "Slacks Sent"
        gui_status main "" "Slacks Updated"

    }

    private method get_value { rmt attr_name } {
        return [ [$rmt get_sobj] cget $attr_name]
    }
    private method node_name { rmt } {
        #set rl [split $rmt ":"]
        #return [lindex $rl [expr [llength $rl] - 1] ]
        return [ [$rmt get_sobj] cget -name]
    }

    public method get_node_slack { node } {
        if { [string toupper $node] eq "MAIN" } {
            return [ [rmt_local get_node_slack "min max"] get_conf]
        } else {
            foreach rmt $rmt_list { 
                puts "Trying node [node_name $rmt]"
                if { $node eq [node_name $rmt] } {
                    return [ [$rmt get_node_slack "min max"] get_conf]
                }
            }
            puts "Node $node not found!"
            return ""
        }
    }

    protected variable wwns
    protected variable wtns
    protected method reset_worst { } {
        set wwns 99999
        set wtns 0
    }
    protected method set_worst { type nobj } {
        set nwns [$nobj cget -worst_${type}_slack]
        set ntns [$nobj cget -total_${type}_slack]
        if { $nwns < $wwns } {
            set wwns $nwns
        }
        if { $ntns < $wtns} {
            set wtns $ntns
        }
    }
    protected method get_worst { } {
        return [list $wtns $ntns]
    }
    protected method compare_worst { type nobj } {
        set nwns [$nobj cget -worst_${type}_slack]
        set ntns [$nobj cget -total_${type}_slack]

        if { ($nwns == 0) } {
            if { $wwns != 0 } {
                set wdiff $wwns
            } else {
                set wdiff 0
            }
        } else {
            set wdiff [expr { abs( ($wwns - $nwns) / $nwns) } ]
        }
        #puts "wdiff: $wdiff"
        if { $wdiff > 0.001 } {
            error "Main's WNS ($nwns) for $type NOT EQUAL to worst: $wwns"
        } 
        if { ($ntns < 0) } {
            if { $ntns > $wtns } {
                if { $ntns == 0 } {
                    set ntns 0.0000001
                }
                set tdiff [expr { abs( ($wtns - $ntns) / $ntns) } ]
                if { $tdiff > 0.001 } {
                    warning "Main's TNS ($ntns) for $type greater than worst TNS: $wtns"
                }
            }
        } 
    }

    protected common SLACK_FMT_MILOS  "%-15s WNS: %-10g TNS: %-10g VIO: %d"
    protected common SLACK_FMT_MERGE  "%-15s WNS: %-10g TNS: %-10g VIO: %d"
    protected common SLACK_FMT_PLUGIN "%-15s WNS: %-10g TNS: %-10g VIO: %d %6s: %-10g"
    public method get_slack_fmt { nobj rmt type } {
        set sobj [$rmt get_sobj]
        set e [$sobj cget -engine]
        if { $e eq "quartz" } {
            set fmt [format $SLACK_FMT_PLUGIN "[node_name $rmt] ($e):" [$nobj cget -worst_${type}_slack] [$nobj cget -total_${type}_slack] [$nobj cget -num_${type}_violations] $e [$nobj cget -native_worst_${type}_slack]]
        } else {
            set fmt [format $SLACK_FMT_MILOS "[node_name $rmt] ($e):" [$nobj cget -worst_${type}_slack] [$nobj cget -total_${type}_slack] [$nobj cget -num_${type}_violations]]
        }
        return $fmt
    }
    protected common SLACK_XML_PLUGIN "<NODE>%s</NODE><ENGINE>%s</ENGINE><WNS>%-10g</WNS><TNS>%-10g</TNS><VIO>%d</VIO><SIZED>%d</SIZED><NEWSIZED>%d</NEWSIZED><REMOVEDCELLS>%d</REMOVEDCELLS><OPTMETHOD>%s</OPTMETHOD><ELAPSED>%d</ELAPSED>"
    public method get_slack_xml { nobj rmt type } {
        set sobj [$rmt get_sobj]
        set e [$sobj cget -engine]
        set fmt [format $SLACK_XML_PLUGIN "[node_name $rmt]" $e [$nobj cget -worst_${type}_slack] [$nobj cget -total_${type}_slack] [$nobj cget -num_${type}_violations] [$nobj cget -sized_cells_${type}] [$nobj cget -new_cells_${type}] [$nobj cget -cells_removed_${type}] [$nobj cget -opt_method_${type}] [$nobj cget -time_elaps_${type}] ]
        return $fmt
    }

    protected method find_req_settings { type sreq } {
        set req_slacks ""
        set nreq ""
        foreach {smin smax} $sreq { break; }
        #puts "REQUIREMENTS 0: $type, $smin, $smax"
        switch $type {
            current { 
                ## current means don't change what's there
                set nreq [state slackslew_requirements]
                set req_slacks ""
                if { $smax == 1 } {
                    append req_slacks "max "
                }
                if { $smin == 1 } {
                    append req_slacks "min "
                }
            }
            both - minmax - min_max { 
                ## forced to be both
                if { ($smin == 0) || ($smax == 0) } {
                    set nreq "1 1 0 0"
                }
                set req_slacks "max min"
            }
            min - hold {
                if { $smin == 0 } {
                    set nreq "1 0 0 0"
                }
                set req_slacks "min"
            }
            max - setup { 
                if { $smax == 0 } {
                    set nreq "0 1 0 0"
                }
                set req_slacks "max"
            }
            default { error "Invalid type $type for show_slacks!" }
        }
        #puts "REQUIREMENTS 1: $nreq"
        #puts "REQUIREMENTS 2: $req_slacks"
        return [list $nreq $req_slacks]
    }

    protected method show_one_slack { merged type xml {verbose 0} } {
        set name ""
        switch $type { 
            min { set name "Hold" }
            max { set name "Setup" }
        }

        if { $merged  } {
            reset_worst
        }
	set xml_out ""

	if {$xml} { 
		append xml_out "\n<$name>\n"
	} else {
        notice "\nCurrent $name Slacks:"
	}
        foreach rmt "$rmt_list $_rmt_local" {
            set sobj [$rmt get_sobj]
            if { [$sobj cget -engine] == "null" } { continue }
            set nobj [$rmt get_node_slack $type]
            Event <<NODE_SLACK>> [$sobj cget -gname] [$nobj get_conf]
            if { $merged } { 
                set_worst $type $nobj 
            }
	    if {$xml} { 
            	set fmt [get_slack_xml $nobj $rmt $type]
            	if { $verbose == 1 } {
                	append fmt "<WT>[$nobj cget -worst_${type}_term]</WT>"
            	}
            	append xml_out "<$type>$fmt</$type>"
	    } else {
            set fmt [get_slack_fmt $nobj $rmt $type]
            if { $verbose == 1 } {
                append fmt " WT: [$nobj cget -worst_${type}_term]"
            }
            notice "$type: $fmt"
	    }
        }
        if { $merged } {
            set fmt [format $SLACK_FMT_MERGE "Merged:" [$merged_slacks cget -worst_${type}_slack] [$merged_slacks cget -total_${type}_slack] [$merged_slacks cget -num_${type}_violations]]
            if { $verbose == 1 } {
                append fmt " WT: [$merged_slacks cget -worst_${type}_term]"
            }
            notice "$fmt :$type"
            # DIMITRIS 1/14/10 compare_worst $type $merged_slacks
        }
	if {$xml} { 
		append xml_out "\n</$name>\n"
		return $xml_out
	} else {
        notice " "
	}
    }

    public method show_worst { {merged 0} {type "current"} {xml 0} {verbose 0}} {
        if { $::msh::OPT(dryrun) == 1 } {
            return
        }

        mydebug "BEGIN SHOW WORST!!!"

        set sreq [state slack_requirements]
        set oreq [state slackslew_requirements]

        ## find nreq and req_slack values
        foreach {nreq req_slacks} [find_req_settings $type $sreq] {break; }
        mydebug "REQ ($oreq): $nreq, $req_slacks, $type, $sreq"

        ## activate required slacks for show_slacks
        if { ($nreq ne "") && ($nreq ne $oreq) } {
            mydebug "NEW SET REQUIREMENTS: $nreq"
            set_requirements $nreq
            set_state node_slack_status "invalid"
        } else {
            mydebug "NO NEW SET REQUIREMENTS: $nreq, $oreq"
        }

        update_node_slacks $req_slacks

        if { $merged } {
            get_merged_slack $merged_slacks $req_slacks
        }

	set xml_out "<SLACKS_SUMMARY>"
        foreach { type } $req_slacks {
            append xml_out [ show_one_slack $merged $type $xml $verbose ]
        }
	if {$xml} {
		append xml_out "</SLACKS_SUMMARY>\n"
	}

        ## do not restore previous setting for now - new optimization will reset
        #if { $nreq ne $oreq } {
            #set_requirements $oreq
        #}
        mydebug "END SHOW WORST!!!"
	return $xml_out
    }

    public method find_node_slack { node } {
        if { $::msh::OPT(dryrun) == 1 } {
            return {}
        }

        update_node_slacks "max min"
        set retval {}
        foreach rmt "$rmt_list $_rmt_local" {
            if { [[$rmt get_sobj] cget -engine] == "null" } { continue }
            set nobj [$rmt get_node_slack "min max"]
            set nname [node_name $rmt]
            if { ($node eq "ALL") || ($node eq $nname) } {
                lappend retval [get_slack_ret $nname $nobj max]
                lappend retval [get_slack_ret $nname $nobj min]
            }
        }
        return $retval
    }

    public method get_slack_ret { nname nobj type } {
        return [list $nname [$nobj cget -worst_${type}_slack] [$nobj cget -total_${type}_slack] [$nobj cget -num_${type}_violations]]
    }

    #### General utility methods ######################################

    ## 
    public method set_requirements { reqs } {
        foreach {min_slack max_slack min_slew max_slew} $reqs { break; }
        mydebug "SQ: $reqs, $min_slack, $max_slack, $min_slew, $max_slew"
        foreach rmt "$rmt_list" {
            #012710D $rmt set_requirements $min_slack $max_slack $min_slew $max_slew
            $rmt set_requirements $reqs
        }
        #012710D rmt_local set_requirements $min_slack $max_slack $min_slew $max_slew
        rmt_local set_requirements $reqs

        set_state slackslew_requirements $reqs
        set_state slack_requirements "$min_slack $max_slack"
        set_state slew_requirements "$min_slew $max_slew"

        sync_nodes
        check_multi_error
    }
    public method execute_remotes { node cmd } {

        mydebug "execute_remotes ---- node: $node -- cmd: $cmd"
        mydebug "rmt_list: $rmt_list"

        foreach rmt "$rmt_list" {
            $rmt run_any_sta_cmd $cmd
        }

        sync_nodes
        check_multi_error

        foreach rmt "$rmt_list" {
            set v [$rmt get_any_sta_result nnnnnnnnn]
		mydebug "execute_remotes ---- get_any_sta_result== $v"
        }
    }
    protected method call_remotes1 { cmd arg1 } {

        foreach rmt "$rmt_list" {
            $rmt $cmd $arg1
        }
        rmt_local $cmd $arg1

        sync_nodes
        check_multi_error
    }

    protected method call_remotes2 { cmd arg1 arg2 } {

        foreach rmt "$rmt_list" {
            $rmt $cmd $arg1 $arg2
        }
        rmt_local $cmd $arg1 $arg2

        sync_nodes
        check_multi_error
    }

    public method set_plugin_control { type } {
        call_remotes1 set_plugin_control $type
    }
    public method set_plugin_mode { mode } {
        call_remotes1 set_plugin_mode $mode
    }

    public method get_wire_spread_paths { prefix } {
        set if "${prefix}.in"
        if { [file exists $if] == 0 } {
            error "GWSP: file $if not found!"
        }

        ## 
        call_remotes1 get_wire_spread_paths $prefix

        set rptfn "${prefix}.rpt.lst"
        set fp [open $rptfn w]

        foreach rmt [full_list] {
            set idx [[$rmt get_sobj] get_sidx]
            set fn "${prefix}.${idx}.rpt.lst"
            puts "Reading list from $fn"
            set data [read_file $fn]
            #puts "Read: $data"
            puts $fp "Scenario $idx"
            puts $fp $data
        }
        close $fp
    }

    protected variable path_report_offset 4
    protected method get_rpt_lines { pcnt scnt i } {
        if { $scnt == 1 } {
            return [list $path_report_offset end]
        }

        set divi [expr $pcnt / $scnt]
        set rem [expr $pcnt % $scnt ]

        set sl [expr $divi * $i + $rem]
        set el [expr $sl + $divi - 1]

        if { $i == 0 } {
            set sl [expr $sl - $rem]
        }

        incr sl $path_report_offset
        incr el $path_report_offset

        puts "GPL: $pcnt / $scnt : $i, $sl $el"
        return [list $sl $el]
    }

    protected method find_groups { } {
        set gl {}
        foreach corner [state corner_list] {
            set sl {}
            foreach sobj [$corner cget -sobj_list] {
                if { [$sobj cget -engine] eq "pt" } {
                    lappend sl $sobj
                }
            }
            lappend gl $sl
        }
        puts "SCEN list: $gl"
        return $gl
    }

    public method get_path_report { prefix } {
        #DKF 01/22/11 
	#set if "${prefix}.in"
        #if { [file exists $if] == 0 } {
            #error "GPR: file $if not found!"
        #}

        ## 
        set sobj [rmt_local get_sobj]
        set fp [open "$prefix.lst" w]
        [get_tm Tmg] write_all_terms -file $fp
        close $fp

	# DKF 01/22/11
        #if { [$sobj cget -translate] == 1 } {
            #[get_tm Tmg] write_all_terms -file $fp -no_backslash
        #} else {
            #[get_tm Tmg] write_all_terms -file $fp
        #}
        #close $fp

        set eplst {}
        if { ([state split_reports] == 0) || ( [llength [full_list]] == 1) } {
                puts "call_remotes2 get_path_report $prefix 1"
            call_remotes2 get_path_report $prefix 1
        } else {
            ## obsolete..
            #set data [read_file ${prefix}.in]
            #set pl [split $data "\n"]
            #set pcnt [expr [llength $pl] - $path_report_offset ]
            #puts "$pcnt groups in reports!"

            set gl [find_groups]
            if { [llength $gl] == [llength [full_list]] } {
                puts "No scenario overlap found - no report splitting!"
                call_remotes2 get_path_report $prefix 1
            } else {
                set local_sl 0
                set local_el 0
                set local_ep 0
                foreach group $gl {
                    set scnt [llength $group]
                    puts "$scnt scenarios in group!"
                    for { set i 0 } { $i < $scnt } { incr i } {
                        set sl [expr $path_report_offset + $i]
                        set el $scnt
                        set ep 0
                        set sobj [lindex $group $i]
                        set rmt [$sobj get_remote]
                        if { $i == 0 } {
                            lappend eplst [$rmt get_sobj]
                            debug EP:A "EP: NEW scenario for eplst: $eplst"
                            set ep 1
                        }
                        if { $rmt == $_rmt_local } {
                            set local_sl $sl
                            set local_el $el
                            set local_ep $ep
                            debug EP:A "EP set: RL: $ep"
                        } else {
                            debug EP:A "EP: cmd: $rmt get_path_report $prefix $ep $sl $el"
                            $rmt get_path_report $prefix $ep $sl $el
                        }
                    }
                }
                debug EP:A "EP: RL: cmd: rmt_local get_path_report $prefix $local_ep $local_sl $local_el"
                rmt_local get_path_report $prefix $local_ep $local_sl $local_el

                sync_nodes
                check_multi_error
            }
        }

        set rptfn "${prefix}.rpt.lst"
        set fp [open $rptfn w]

        foreach rmt [full_list] {
            set idx [[$rmt get_sobj] get_sidx]
            set fn "${prefix}.${idx}.rpt.lst"
            puts "Reading list from $fn"
            set data [read_file $fn]
            #puts "Read: $data"
            puts $fp "Scenario $idx"
            puts $fp $data
        }
        close $fp

        set eptfn "${prefix}.ep.lst"
        set clkfn "${prefix}.clk.lst"
        set fp [open $eptfn w]
        set fp2 [open $clkfn w]

        if { [llength $eplst] == 0 } {
            debug EP:A "EP: branch 1: $eplst"
            foreach rmt [full_list] {
                set idx [[$rmt get_sobj] get_sidx]
                set fn "${prefix}.${idx}.ep.lst"
                puts $fp $fn
                set fn "${prefix}.${idx}.clk.lst"
                puts $fp2 $fn
            }
        } else {
            debug EP:A "EP: branch 2: $eplst"
            foreach sobj $eplst {
                set idx [$sobj get_sidx]
                set fn "${prefix}.${idx}.ep.lst"
                puts $fp $fn
                set fn "${prefix}.${idx}.clk.lst"
                puts $fp2 $fn
            }
        }
        close $fp
        close $fp2
    }

    public method add_clock_delay { file } {
        if { [file exists $file] == 0 } {
            error "ACD: file $file not found!"
        }

        call_remotes1 add_clock_delay $file

    }

    public method get_clock_period { prefix } {
        call_remotes1 get_clock_period $prefix

        set pfn "${prefix}.periods.lst"
        set fp [open $pfn w]

        foreach rmt [full_list] {
            set idx [[$rmt get_sobj] get_sidx]
            set fn "${prefix}.${idx}.periods.out"
            puts $fp $fn
        }
        close $fp
    }

    #### ACTIVETERM-related methods #########################################

    protected method activeterm_cnt { } {
        $tmg size_activeterms -size= ::acsize
        return $::acsize
    }

    protected method show_activeterms { } {
        puts "ACTIVE TERMS Count: [activeterm_cnt]"
        $tmg print_activeterms -limit 100
    }
    protected method update_activeterms { } {
        mydebug "update_activeterms-----------------------------------------BEGIN UA!"
        incr ACTIVE_CNT

        mydebug "Active Term timing update #$ACTIVE_CNT at time [runtime_incr]"

        set flag 0
        if { $idlist_init == 0 } {
            mydebug "IDLIST INIT!"
            Publish ID.id
            Publish EP.ep

            foreach rmt $rmt_list {
                $rmt lists_to_plugin $BBD_OPT
            }
            rmt_local lists_to_plugin
            set flag 1
        }
        #show_activeterms
        [get_tm Tmg] are_activeterms_modified -modified= ::mod
        debug PLUGIN:A "Active term status: $::mod, $idlist_init, $eco_log_cnt, $rmt_list"
        #puts "Active term status: $::mod, $idlist_init, $eco_log_cnt, $rmt_list"
        if { ($::mod == 1 || $idlist_init == 0)  } {
            
            ## resend active terms to each node
            mydebug "Active Terms update - sending active term list at time [runtime_incr]"
            mydebug "UA ACTIVE ($eco_log_cnt, $rmt_list)!"

            set fname ""

            set icnt ""
            if { $rmt_list ne "" } {
                set icnt "ACTIVE.$ACTIVE_CNT.atil"

                mydebug "UA REMOTES! -- $icnt"

		if { $BBD_OPT ne "ON" } {
                	Publish $icnt
		}

                foreach rmt $rmt_list {
                    $rmt send_activeterms $icnt $BBD_OPT
                }
                mydebug "END UA REMOTES!"
            }
            if { $eco_log_cnt > 0 } {
                mydebug "UA PLUGINS 2!"
                set rc [ catch {rmt_local send_activeterms $icnt $BBD_OPT} output]
            }

            set flag 1
            mydebug "UA BEGIN SYNC!"
            sync_nodes
            check_multi_error
            mydebug "UA END SYNC!"
	    if { $BBD_OPT ne "ON" } {
            	Unpublish $icnt
	    }
        }
        if { $idlist_init == 0 } {
            set idlist_init 1
            Unpublish EP.ep
            Unpublish ID.id
        }
        mydebug "update_activeterms-----------------------------------------END UA!"
        return $flag
    }


    #### Timing/update-related methods ###################################

    ## the big update method
    ## Use the .atsl suffix to send data..

    protected method update_incr_timing { } {

        send_node_status action "Update Timing!"
        gui_status main "" "Wait for Remotes"
        mydebug "BEGIN UIT"

        set local_update 0
        foreach rmt $rmt_list {
            $rmt update_timing 0 0
            $rmt reset_unpublish
        }
        ## do it all here, after sending commands to remotes..
        rmt_local update_timing 0 0
        mydebug "UIT 2"

        set acnt ""
        set active_nodes $rmt_list
        set round 0

        while { [llength $active_nodes] >= 3 } {
            mydebug "Split merge: Round $round, an: $active_nodes"
            set new_an {}
            foreach { rmt1 rmt2 } $active_nodes {
                mydebug "N1: $rmt1, N2: $rmt2"

                ## odd case - nothing to merge with only one remote
                if { $rmt2 eq "" } { break }

                set dstring "[$rmt1 getAlias]$round.$INCR_CNT.atsl"
                mydebug "DSTRING: $dstring"
                $rmt1 publish $dstring
                $rmt2 subscribe $dstring 

                ## store subscribe/publish info so we can undo it later
                $rmt1 add_unpublish $dstring

                lappend new_an $rmt2
            }
            ## handle odd case
            if { $rmt2 eq "" } {
                set dstring "MAIN$round.$INCR_CNT.atsl"
                $rmt1 publish $dstring
                ## store subscribe/publish info so we can undo it later
                $rmt1 add_unpublish $dstring
                Subscribe $dstring
            }
            mydebug "Round $round, new_an: $new_an"
            set active_nodes $new_an
            incr round
        }
        mydebug "UIT 3"
        mydebug "Out of merge loop: $active_nodes!"
        foreach rmt $active_nodes {
            set dstring "[$rmt getAlias].$INCR_CNT.atsl"
            $rmt publish $dstring
            mydebug "Subscribe $dstring!"
            Subscribe $dstring
            mydebug "Done Subscribe $dstring!"
            $rmt add_unpublish $dstring
        }
        mydebug "UIT 4"

        mydebug "Out of final merge!"

        sync_nodes
        check_multi_error
        foreach rmt $rmt_list {
            $rmt do_unpublish
        }

        send_node_status action "Timing Sent"
        gui_status main "" "Timing Data Received"
        mydebug "Update local node!"
        mydebug "END update_incr_timing!"
        incr INCR_CNT
        mydebug "END UIT"
        return INCR_CNT
    }

    ## send everything to main..
    ## use the .atfl data suffix
    protected method update_incr_full_timing { rmt_tmg_info suffix } {

	#rmt_tmg_info= { aterms cterms } 
	#suffix= {atfl ctfl}

	set num_scenarios 0
	if { $BBD_OPT eq "ON" } {
		set num_scenarios 1
	}
        mydebug "update_incr_full_timing ..........................  BEGIN UIT"
        set local_update 0
        foreach rmt $rmt_list {
        	mydebug "\n\t$rmt update_timing $rmt_tmg_info 0 $num_scenarios   BEGIN update_incr_full_timing "
            $rmt update_timing $rmt_tmg_info 0 $num_scenarios
        	mydebug "\t$rmt update_timing 0 0 $num_scenarios   END    update_incr_full_timing \n"
        }
	if { $BBD_OPT ne "ON" } {
        foreach rmt $rmt_list {
            #020810D set dstring "[$rmt getAlias].$INCR_CNT.atfl"
            set dstring "[$rmt getAlias].$INCR_CNT.$suffix"
        	mydebug "\tPublish $dstring"
            $rmt publish $dstring
            	mydebug "\tSubscribe $dstring!"
            Subscribe $dstring
            mydebug "\tDone Subscribe $dstring!"
            $rmt unpublish $dstring
        }
	} elseif { $rmt_tmg_info eq "aterms" } {
        	foreach rmt $rmt_list {
            		$rmt store_timing_cache $num_scenarios
		}
	}
        check_multi_error
        mydebug "update_incr_full_timing ..........................  BEGIN UIT"

        incr INCR_CNT
        return INCR_CNT
    }
    protected method update_incr_full_timing_bak_06192012 { rmt_tmg_info suffix } {

	#rmt_tmg_info= { aterms cterms } 
	#suffix= {atfl ctfl}

	set num_scenarios 0
	if { $BBD_OPT eq "ON" } {
		set num_scenarios 1
	}
        mydebug "update_incr_full_timing ..........................  BEGIN UIT"
        set local_update 0
        foreach rmt $rmt_list {
        	mydebug "\n\t$rmt update_timing $rmt_tmg_info 0 $num_scenarios   BEGIN update_incr_full_timing "
            $rmt update_timing $rmt_tmg_info 0 $num_scenarios
        	mydebug "\t$rmt update_timing 0 0 $num_scenarios   END    update_incr_full_timing \n"
        }
        ## do it all here, after sending commands to remotes..
        	mydebug "\n\n\trmt_local update_timing $rmt_tmg_info 0 0   BEGIN update_incr_full_timing "
        rmt_local update_timing $rmt_tmg_info 1 [state num_scenarios]
        	mydebug "\trmt_local update_timing 0 0 $rmt_tmg_info   END  update_incr_full_timing "

	if { $BBD_OPT ne "ON" } {
        foreach rmt $rmt_list {
            #020810D set dstring "[$rmt getAlias].$INCR_CNT.atfl"
            set dstring "[$rmt getAlias].$INCR_CNT.$suffix"
        	mydebug "\tPublish $dstring"
            $rmt publish $dstring
            	mydebug "\tSubscribe $dstring!"
            Subscribe $dstring
            mydebug "\tDone Subscribe $dstring!"
            $rmt unpublish $dstring
        }
	} elseif { $rmt_tmg_info eq "aterms" } {
        	foreach rmt $rmt_list {
            		$rmt store_timing_cache $num_scenarios
		}
	}
        check_multi_error
        mydebug "update_incr_full_timing ..........................  BEGIN UIT"

        incr INCR_CNT
        return INCR_CNT
    }
    ## 020310D send clock structural info to main..
    ## use the .ctfl data suffix
    protected method read_clock_pin_info { } {

        mydebug "\n\nread_clock_pin_info ..........................  BEGIN"
        set local_update 0
        
        foreach rmt $rmt_list {
        	mydebug "\n\n\tannotate_timing_structure $rmt BEGIN"
            #$rmt annotate_timing_structure 0 0
            #$rmt write_cterms_file
            $rmt write_clock_pins 0 0
        	mydebug "\n\tannotate_timing_structure $rmt END\n"
        }

        ## do it all here, after sending commands to remotes..
        	mydebug "\n\n\tannotate_timing_structure RMT_LOCAL BEGIN"
        rmt_local annotate_timing_structure 1 [state num_scenarios]
        	mydebug "\n\n\tannotate_timing_structure RMT_LOCAL END"

        foreach rmt $rmt_list {
            set dstring "[$rmt getAlias].$INCR_CNT.ctfl"
            mydebug "Publish $dstring!"
            $rmt publish $dstring
            mydebug "Subscribe $dstring!"
            Subscribe $dstring
            mydebug "Done Subscribe $dstring!"
            $rmt unpublish $dstring
        }
        check_multi_error
        sync_nodes
        mydebug "\nread_clock_pin_info ..........................  END\n\n"

        incr INCR_CNT
        return INCR_CNT
    }
    public method update_clock_structure { {undo 0} } {
	mydebug "\t\trmt_local: update_clock_structure ----------------- BEGIN"
        if { $_rmt_local eq "" } {
		if { [llength $rmt_list] == 0 } {
            ## nothing to do
            return
		}
        }
        _update_timing "cterms" "ctfl" $undo
        #read_clock_pin_info
	    #mydebug "\t\t\tmerge_method=$merge_method scenarios [state num_scenarios]"
            #$tmg store_timing_cache -num_scenarios [state num_scenarios]
	    #mydebug "\t\t\tafter store_timing_cache"
	mydebug "\t\trmt_local: update_clock_structure ----------------- END"
    }








public method run_tmg_remote_script { cmd_name { args ""} } {
	mydebug "--------------------------> run_tmg_remote_script: BEGIN\n"
	if { [llength $rmt_list] == 0 } {
		return
	}


#cterms ctlf

	Event <<TIMING_UPDATE_START>> $INCR_CNT

	#update_incr_full_timing $rmt_tmg_info $suffix

	set num_scenarios 0
	if { $BBD_OPT eq "ON" } {
		set num_scenarios 1
	}
	foreach rmt $rmt_list {
		mydebug "\t<<<$rmt run_remote_tmg_cmd $cmd_name args=$args "
		#$rmt run_remote_tmg_cmd $rmt_tmg_info 0 $num_scenarios
		#$rmt run_remote_tmg_cmd "example" {a b c d fgh 2}
		$rmt run_remote_tmg_cmd "[$rmt getAlias].A.OK" $cmd_name $args
		mydebug "\t>>>$rmt run_remote_tmg_cmd "
	}
        #if { $BBD_OPT ne "ON" } {
        foreach rmt $rmt_list {
            #set dstring "[$rmt getAlias].3.OK.$INCR_CNT.file"
            set dstring "[$rmt getAlias].A.OK.file"
                mydebug "\tPublish $dstring"
            $rmt publish $dstring
                mydebug "\tSubscribe $dstring!"
            Subscribe $dstring
            mydebug "\tDone Subscribe $dstring!"
            $rmt unpublish $dstring
        }
        #} elseif { $rmt_tmg_info eq "aterms" } {
                #foreach rmt $rmt_list {
                        #$rmt store_timing_cache $num_scenarios
                #}
        #}

            sync_nodes
	check_multi_error
	incr INCR_CNT

	
	Event <<TIMING_UPDATE_END>> $INCR_CNT

	mydebug "--------------------------> run_tmg_remote_script: END\n"
}
























    public method update_timing { rmt_tmg_info suffix {undo 0} } {
	#slacks for active terms: "aterms atfl"
	mydebug "\t\trmt_local: update_timing ----------------- BEGIN"
        if { $_rmt_local eq "" } {
		if { [llength $rmt_list] == 0 } {
            ## nothing to do
            return
		}
        }
        _update_timing $rmt_tmg_info $suffix $undo
        #020810D _update_timing $undo
        if { $merge_method == 1 } {
	    mydebug "___merge_method__: $merge_method"
            $tmg store_timing_cache
        } else {
	    mydebug "\t\t\tmerge_method=$merge_method scenarios [state num_scenarios]"
	    if { $BBD_OPT ne "ON" } {
            $tmg store_timing_cache -num_scenarios [state num_scenarios]
	    mydebug "\t\t\tafter store_timing_cache"
	    } else {
	    	mydebug "\t\t\tskip====== $BBD_OPT -------===> store_timing_cache"
	    }
            #for {set i 0} {$i <= [state num_scenarios] } { incr i } {
                #$tmg dump_active_slack -scenario $i
            #}
        }
	mydebug "\t\trmt_local: update_timing ----------------- END"
    }

    public method update_int_timing { {undo 0} } {
	mydebug "\t\trmt_local: update_int_timing ----------------- BEGIN"
        _update_timing "aterms" "atfl" $undo
        #020810D _update_timing $undo
	mydebug "\t\trmt_local: update_int_timing ----------------- END"
        if { $merge_method == 1 } {
            return 0
        } else {
            #for {set i 0} {$i <= [state num_scenarios] } { incr i } {
                #$tmg dump_active_slack -scenario $i
            #}
            return [state num_scenarios]
        }
    }

    public method exit_plugins { } {
        foreach rmt $rmt_list {
            $rmt exit_plugin
        }
        rmt_local exit_plugin
    }

    public method enable_timing { status } {
        set timing_enabled $status
    }

    protected variable timer -1
    protected variable timing_enabled 0
    protected method start_update_timer { } {
        mydebug "Begin Timing Update"
        if { $timing_enabled == 1 } {
            set timer [start_timer]
        }
    }

    protected method show_update_timer { } {
        if { $timing_enabled == 1 } {
            set elapsed [get_timer $timer]
            set msg [format "Timing Update ran in %.3f seconds." $elapsed]
            notice $msg
            reset_timer $timer
            set timer -1
        }
        mydebug "End Timing Update"
    }

    ## ALL timing has to be done this way
    ## Remote nodes with RTE have no db - NEED active terms
    private method _update_timing { { rmt_tmg_info "aterms" } { suffix "atfl" } {undo 0} } {

	mydebug "::_update_timing $rmt_tmg_info $suffix -- dimitris"

        Event <<TIMING_UPDATE_START>> $INCR_CNT
        start_update_timer
        debug HTIMER:U "UPDATE: got timer $timer"
        if { ($_rmt_local == "" ) && ( $rmt_list_len == 0 ) } {
            mydebug "Nothing to update!"
            show_update_timer
            return
        }
        send_node_status action "ECO from Main"
        gui_status main "" "Send ECOs to Remotes"

        mydebug "BEGIN UT! $undo"

	if { $rmt_tmg_info eq "aterms" } {
        set ec_rc [update_eco $undo]
        mydebug "UO UT! 2"

        if { $undo == 0 } {
            set cc_rc [check_celtic]
        }
        mydebug "UO UT! 3"

        set ac_rc [update_activeterms]
        mydebug "UO UT! 4"
        if { ($ec_rc == 1) || ($ac_rc == 1) } {
            mydebug "UO UT! 5"
            if { $merge_method == 1 } {
                update_incr_timing
            } elseif { $merge_method == 2 } {
                update_incr_full_timing $rmt_tmg_info $suffix
            }

            if { [expr {$INCR_CNT % [state mme_update_freq]}] == 0 } {
                if { $gui ne "" } {
                    set_state node_slack_status "invalid"
                    show_worst
                }
            }
        }
	} elseif { $rmt_tmg_info eq "cterms" } {
        	mydebug "update_clock_info $rmt_tmg_info $suffix"
                update_incr_full_timing $rmt_tmg_info $suffix
		mydebug "update_clock_info END"
	}
        
        send_node_status action "Updated!"
        gui_status main "" "Timing Update complete!"

        mydebug "END UT!"
        show_update_timer
        Event <<TIMING_UPDATE_END>> $INCR_CNT
    }

    #### CELTIC-related methods ##########################################

    protected variable celtic_init 0;
    protected variable celtic_update_freq 0;
    protected variable celtic_force 0;
    protected variable celtic_obj "";
    protected variable celtic_update_cnt 0;
    protected variable celtic_rmt_lst {};
    protected variable celtic_cnt 0;

    public method set_celtic_update_freq { freq } {
        set celtic_update_freq $freq
    }

    public method set_celtic_force { } {
        set celtic_force 1
    }
    protected method check_celtic { } {
        mydebug "CCEL $celtic_force, $celtic_update_cnt, $celtic_update_freq: 1"
        incr celtic_update_cnt
        if { ($celtic_force == 0) && ( ($celtic_update_freq<=0) || ($celtic_update_cnt < $celtic_update_freq) ) } {
            ## not time to update yet
            return
        }
        mydebug "CCEL: 2"
        set celtic_update_cnt 0
        set celtic_force 0

        if { $celtic_init == 0 } {

            mydebug "CCEL: 3"
            set celtic_rmt_cnt 0
            ## initialize each local celtic object (for those using celtic)
            foreach rmt [full_list] {
                if { [[$rmt get_sobj] cget -engine] == "null" } { continue }
                mydebug "CCEL: 3a: $rmt"
                set sobj [$rmt get_sobj]
                set corner [$rmt get_corner]
                set co [$sobj cget -celtic_on]
                if { $co == 1 } {
                    mydebug "CCEL: 3b: $rmt ([$sobj get_sidx])"
                    $rmt init_plugin_object CELTIC PLUGIN_celtic
                    $rmt create_celtic_tmp_dir
                    $rmt plugin_api CELTIC create_tmp_files
                    $rmt plugin_api CELTIC write_plugin_file $si
                    lappend celtic_rmt_lst $rmt
                }
            }
            set celtic_init 1
            ## so in next step, Celtic is done in parallel..
            sync_nodes
            check_multi_error
        }

        if { $celtic_rmt_lst eq "" } {
            return
        }


        mydebug "CCEL: 4: $celtic_rmt_lst"

        get_tm MSH

        set tasks ""
        foreach rmt $celtic_rmt_lst {
            mydebug "CCEL: 4a: $rmt"
            $rmt plugin_api CELTIC write_plugin_file $si
            $rmt write_celtic_data_files 
            lappend tasks [$rmt launch_celtic $celtic_cnt]
        }
        monitor $tasks {
            done {
                if { [string trim $task(error)]  eq "" } {
                    notice "Celtic finished: $task(output)"
                } else {
                    error "Error running Celtic: $task(error)"
                }
            }

        }
        foreach rmt $celtic_rmt_lst {
            $rmt read_sdf $celtic_cnt
        }
        incr celtic_cnt
        sync_nodes
        check_multi_error
        mydebug "CCEL: 5"

        return 0
    }
}

