## pdb.tcl
## Physical Data Base module for Milos
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::pdb 0.1

itcl::class PDB {

    inherit VOB

    ## protected vars
    ## variables with string values

    protected method constructor { parent } {
        VOB::constructor $parent
        set NAME "PDB"
        set SELFNAME "PDB$this"
        ## ONLY for public vars
        set_state db_loaded 0
        set_state db_saved 0
        set_state db_modified 0

        if { [::info exists ::env(NTILES)] } {
            set_state num_tiles  $::env(NTILES)
        } else {
            set_state num_tiles -1
        }
        set_state assembled 0
        set_state tile_lefs ""
    }

    ## used to "close the hole"
    protected method info { } 

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help PDB $cmd $param $long
        } else {
            mod_help PDB $cmd
        }
    }

    public method set_rebuffer_altered_nets { } {
        set db [get_tm DB]
        $db set_rebuffer_altered_nets
    }
    
    public method get_extraction_corners { } {
        return [parent get_extraction_corners]
    }

    public method needs_extraction { } {
        if { [state db_loaded] != 1 } {
            error "pdb needs_extraction is only valid after pdb load_design!" "" -1
        }
        get_ext_status
        if { [state db_parasitics] == 0 } {
            return 1
        }

        get_db_change_status
        if { [state db_altered_cnt] > 0 } {
            return 1
        }

        return 0
    }

    public method needs_routing { } {
        get_db_change_status
        if { [state db_disconnected_cnt] > 0 } {
            return 1
        }

        return 0
    }

    public method check_lef_sites { } {
        echo [cmdline]
        [get_tm DB] check_lef_sites
    }

    VOB::add_args PDB_DEFAULT_CLASS_SITE {
        {class_name nstr 0 "CORE"}
        {site_name nstr 0 "CORE"}
    }
    public method default_class_site { args } {
        parse_args
        echo [cmdline]

        set cmd [build_cmd [get_tm DB] "default_class_site"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in default_class_site" 
        }
    }

    VOB::add_args PDB_LOAD_LEF { 
        { lefs flist } 
    }
    public method load_lef { args } {
        parse_args
        echo [cmdline]
        set lefl [get_arg lefs]
        set db [get_tm DB]
        set cmd "$db load_lef -list_file [list $lefl]"
        set rc [::eval $cmd]
        if { $rc != 0 } {
            warning "Error $rc reading LEF files $lefl."
            return $rc
        }
        set_state db_loaded 1
    }


    ## PARAMETER setup
    VOB::add_args PDB_LOAD_DESIGN { 
        { def ifile OR_dbname }
        { hier_def_list nslist 0 ""}
        { lefs flist }
        { leffiles flist }
        { dbname ifile NOT_def } 
        { dbu_per_micron num NOT_dbname }
        { no_order_wires hbool 0 0 }
        { check_layer nstr 0 "-type -width -spacing -pitch" }
        { ignore_non_routing_layers bool 0 0}
        { skip_special_wires bool 0 0}
        { skip_signal_wires bool 0 0}
        { skip_signal_conns bool 0 0}
    }

    VOB::add_help PDB_LOAD_DESIGN { 
        { lefs "list of LEF files to be read" }
        { hier_def_list "list of pairs of hierarchical instance name and corresponding def file" }
        { leffiles "list of files containing names of LEF files to be read" }
        { def "name of DEF file to read" }
        { dbname "filename to read instead of LEF/DEF" }
        { dbu_per_micron "valid values: 100, 200, 1000, 2000, 10000, 20000" }
    }


    public method load_design { args } {
        parse_args

        if { [state db_loaded] == 1 } {
            error "A design has already been loaded!" "" -1
        }
        echo [cmdline]
        #echo [settings]

        if { [check_arg def] } {
            ## Lef/Def branch

            set lefl ""
            if { [check_arg lefs] } {
                set lefl [get_arg lefs]
            } elseif { [check_arg leffiles] } {
                foreach arg [get_arg leffiles] {
                    set flc [read_file $arg]
                    foreach lf $flc {
                        append lefl $arg
                    }
                }
            }
	    set top_def [get_arg def] 
	    set def_list ""
	    append def_list $top_def " "
            set hier_def_list [get_arg hier_def_list]
	    puts "hier_def_list= $hier_def_list"
            if { $hier_def_list ne "" } {
		foreach {inst def} $hier_def_list {
			append def_list $def " "
			puts "inst=$inst def= $def"
		}
	    }
	    puts "def_list= $def_list"
            
            if { $lefl eq "" } {
                warning "Please specify one or more LEF files with -lefs or -leffiles"
                return 1
            }

            set dbu ""
            if { [check_arg dbu_per_micron] == 1 } {
                set dbu "-dbu_per_micron [get_arg dbu_per_micron]"
            }

            set inrl ""
            if { [get_arg ignore_non_routing_layers] != 0 } {
                set inrl "-ignore_non_routing_layers"
            }

            set db [get_tm DB]
            notice "Reading input files."
            #set cmd "$db load_lef $dbu $inrl -list_file [list $lefl] -target_def_list_file [get_arg def]"
            set cmd "$db load_lef $dbu $inrl -list_file [list $lefl] -target_def_list_file $def_list"
            set rc [::eval $cmd]
            if { $rc != 0 } {
                warning "Error $rc reading LEF files $lefl."
                return $rc
            }
            set_state lef_loaded [list $lefl]

            set check_layer_opts [get_arg check_layer]
            if { $check_layer_opts ne "" } {
                set cl "$db check_layer $check_layer_opts" 
                set rc [::eval $cl]
                if { $rc != 0 } {
                    warning "Error $rc during check_layer $check_layer_opts"
                    return $rc
                }
            }

            set nospcwires ""
            if { [get_arg skip_special_wires] != 0 } {
                set nospcwires "-nospcwires"
            }
            set nowires " "
            if { [get_arg skip_signal_wires] != 0 } {
                set nowires "-nowires"
            }
            set nosignalconns " "
            if { [get_arg skip_signal_conns] != 0 } {
                set nosignalconns "-noconns"
            }

            notice "."
            set cmd "$db load_def -file [get_arg def] $nospcwires $nowires $nosignalconns"
            notice "$cmd"
            set rc [::eval $cmd]
            if { $rc != 0 } {
                warning "Error $rc reading DEF file $def."
                return $rc
            }
            set_state def_loaded [get_arg def]
            notice "."
            if { [get_arg no_order_wires] == 0 } {
                $db order_wires 
            }
            if { $hier_def_list ne "" } {
		$db get -top -object= ::top_obj
		foreach {inst deff} $hier_def_list {
			notice "Reading DEF $deff for instance set $inst"
        		$db load_def -block $::top_obj -file $deff -instance $inst $nosignalconns $nospcwires $nowires		
        		#$db load_def -block $::top_obj -file $deff $nosignalconns $nospcwires $nowires		
		}
		#$db get_children -chip -block_list= ::block_list -block_name_list= ::tile_names
		$db get_children -chip -block_name_list= ::tile_names
		notice "HIER_BLOCKS: $::tile_names"

		#foreach tile $::tile_names {
        		#$db find -block $tile -object= ::blk
        		#$db save_def -file $tile.def
		#}
			#notice "Finished writting DEF files"
		if { [get_arg no_order_wires] == 0 } {
		foreach tile $::tile_names {
        		$db find -block $tile -object= ::blk
        		$db order_wires -block $::blk
		}
			notice "Finished order_wires for block $::blk $tile"
		}
	    }

            notice "Done reading input files."

        } else {
            ## readdb branch
            set db [get_tm DB]
            if { ([file exists [get_arg dbname]]) } {
                notice "Reading database: [get_arg dbname]"
                set rc [$db read -file [get_arg dbname]]
                if { $rc != 0 } {
                    error "Error $rc reading datbase [get_arg dbname]" "" -1
                }
                set ec [get_extraction_corners]
                #puts "EC: $ec, [llength $ec]"
                if { [llength $ec] >= 1 } {
                    set_state db_has_extraction_corners 1
                }
            } else {
                warning "Database [get_arg dbname] does not exist!"
                return 1
            }
        }
        set_state db_loaded 1
        set_state db_read_spef 0
        UPOBJ set_vars

        $db get -top -object= ::block

        cmd_rlog
        return 0
    }

    private method milos { args } {
        set_state db_loaded 1
        set dcr [get_tm Dcr]
        $dcr test_milos -cmd [join $args]
    }

    private method ade { args } {
        set_state db_loaded 1
        set dcr [get_tm Dcr]
        $dcr test_ade -cmd [join $args]
    }

    ## PARAMETER setup
    VOB::add_args PDB_SAVE_DESIGN { 
        { def_out ofile 0 }
        { lef_out ofile 0 }
        { dbname ofile 0 }
        { use_ids bool 0 0}
    }

    VOB::add_help PDB_SAVE_DESIGN { 
        { def_out "filename for saving DEF information" }
        { lef_out "filename for saving LEF information" }
        { dbname "filename for saving DB information" }
    }

    public method save_design { args } {
        if { [state db_loaded] != 1 } { 
            warning "Please load a design before saving!"
            return 1
        }

        parse_args

        set rc [check_arg def_out]
        incr rc [check_arg lef_out]
        incr rc [check_arg dbname]
        if { $rc == 0 } {
            error "One of: -def_out, -lef_out or -dbname must be specified!"  "" -1
        }

        echo [cmdline]
        #echo [settings]

        set db [get_tm DB]
        if { [check_arg dbname] } {
            notice "Saving DB file [get_arg dbname]."
            set rc [$db write -file [get_arg dbname]]
            if { $rc != 0 } {
                error "Error $rc while saving database [get_arg dbname]." "" -1
            }
        }
        if { [check_arg def_out] } {
            notice "Saving DEF file [get_arg def_out]."
            if { [get_arg use_ids] } {
                $db save_def -use_ids -file [get_arg def_out]
            } else {
                $db save_def -file [get_arg def_out]
            }
            set flag 1
        }
        if { [check_arg lef_out] } {
            notice "Saving LEF file [get_arg lef_out]."
            $db save_lef -file [get_arg lef_out]
            set flag 1
        }
        cmd_rlog
    }

    VOB::add_args PDB_SAVE_DEF { 
        { def_out ofile 1 }
        { nets nstr 1 }
    }
    VOB::add_help PDB_SAVE_DEF { 
        { def_out "Filename for saving partial DEF file" }
        { nets "Name/id of nets to write to DEF " }
    }
    public method save_def { args } {
        parse_args
        set db [get_tm DB]
        set rc [$db save_def -list_net_id [get_arg nets] -file [get_arg def_out] -mini_lef ]
        if { $rc != 0 } {
            error "Error $rc during save_def"
        }
    }


    ## PARAMETER setup
    VOB::add_args PDB_WRITE_SPEF { 
        { outfile ofile 1}
        { gz bool 0 1}
        { N nstr 0 }
        { use_ids bool 0}
        { no_name_map bool 0}
        { corner nstr 0 }
        { ext_corner_name nstr 0 }
        { prime_time bool NOT_psta 0}
        { psta bool NOT_prime_time 0}
        { quartz_time bool NOT_prime_time 0}
        { single_pi bool 0 0}
        { nets nstr 0 }
        { net_id num 0 0}
        { exclude_cells nstr 0 }
        { no_backslash bool 0}
        { flatten bool 0}
        { parallel bool 0}
    }

    VOB::add_help PDB_WRITE_SPEF { 
        { _fcn "Write parasitics to SPEF" }
        { outfile "filename for SPEF file." }
        { gz "write file in gzipped format." }
        { N "Spef format: s = Synopsys/StarRC, m = Magma" }
        { use_ids "Use ids in spef file" }
        { no_name_map "Don't write name map" }
        { corner "Which corner to write to spef file" }
        { ext_corner_name "Which corner(s) to write to spef file" }
        { prime_time "Write Synopsys PrimeTime compatible spef" }
        { psta "Write Parallax compatible spef" }
        { quartz_time "Write Magma QuartzTime compatible spef" }
        { nets "Nets to write" }
        { net_id "Net to write" }
        { exclude_cells "Do not write data for instances with these cells" }
        { no_backslash "Strip backslashes from names/nets" }
        { flatten "flat hierarchical extraction" }
        { parallel "parallel hierarchical extraction" }
    }

    VOB::add_alias PDB_WRITE_SPEF {
        {outfile file}
    }
    public method write_spef { args } {
        if { [state db_loaded] != 1 } { 
            warning "Must load a design before writing SPEF!"
            return 1
        }
        parse_args

        echo [cmdline]

        set ext [get_tm Ext]
        if { [get_arg gz] } {
            notice "Writing SPEF file [get_arg outfile].gz"
        } else {
            notice "Writing SPEF file [get_arg outfile]"
        }
        set cmd [build_cmd $ext "write_spef"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc while writing SPEF file [get_arg outfile]." "" -1
        }
        set_state db_write_spef 1
        cmd_rlog
    }

    ## PARAMETER setup
    VOB::add_args PDB_READ_SPEF { 
        { file ifile 1}
        { net nstr 0 }
        { force bool 0 }
        { use_ids bool 0 }
        { N nstr 0 }
        { cc_threshold pnum 0}
        { db_corner_name nstr 0}
        { calibrate_base_corner nstr 0}
        { spef_corner num 0}
        { length_unit num 0}
        { fix_loop num 0 }
        { m_map hbool 0}
        { stamp_wire bool 0 }
        { no_cap_num_collapse bool 0 }
        { cap_node_map_file nstr 0 }
    }

    VOB::add_help PDB_READ_SPEF { 
        { file "SPEF filename to read." }
        { net "List of net IDs to read" }
        { force "delete extraction data, if present, before reading spef" }
        { use_ids "Use net ids instead of names" }
        { cc_threshold "coupling caps smaller than value will be grounded" }
        { db_corner_name "Process corner to read into" }
        { calibrate_base_corner "Corner to calibrate against" }
        { spef_corner "Which spef corner to read" }
        { length_unit "Scale units in SPEF by this factor." }
        { fix_loop "0: skip loops, 1: break loops" }
        { N "Spef format: s = Synopsys/StarRC, m = Magma" }
        { stamp_wire "set capNode _node to junction_id" }
        { no_cap_num_collapse "do not collapse cap node numbers" }
        { cap_node_map_file "write capNodeMap to this file" }
    }
    public method read_spef { args } {
        if { [state db_loaded] != 1 } { 
            warning "Must load a design before reading SPEF!"
            return 1
        }
        parse_args

        echo [cmdline]
        #echo [settings]

        set fn [get_arg file]
        if { ! [file exists $fn] } {
            error "File $fn does not exist in read_spef." "" -1
        }

        set ext [get_tm Ext]
        notice "Reading SPEF file $fn"
        set cmd [build_cmd $ext "read_spef"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc while reading SPEF file $fn." "" -1
        }

        ## to keep internal logic consistent
        set_state db_parasitics 1

        set_state db_read_spef 1
        cmd_rlog
    }

    VOB::add_args PDB_DIFF_NET_CAP {
        { ref_file ifile 1 }
        { out_file nstr 0 }
        { log_file nstr 0 }
    }

    VOB::add_help PDB_DIFF_NET_CAP {
        { ref_file "Reference filename with cap values and net name pairs" }
        { out_file "File to send results to - defaults to stdout" }
        { log_file "File to send warnings to - defaults to stdout" }
    }

    public method diff_net_cap { args } {
        if { [state db_loaded] != 1 } { 
            error "Must load a design before diffing net Cap!" "" -1
            return 1
        }
        parse_args

        set db [get_tm DB]
        set cmd [build_cmd $db "diff_net_cap"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc during diff_net_cap." "" -1
        }
    }


    ## PARAMETER setup
    VOB::add_args PDB_DIFF_SPEF { 
          { file ifile 1}
          { m_map hbool 0}
          { low_guard num 0 1.0 }
          { db_corner_name nstr 0 "MinMax0" }
    }

    VOB::add_help PDB_DIFF_SPEF { 
        { file "SPEF filename to read." }
        { low_guard "Only report percentage changes greater than low_guard" }
        { db_corner_name "Name of corner to diff against - only required if not default" }
    }
    public method diff_spef { args } {
        if { [state db_loaded] != 1 } { 
            error "Must load a design before diffing SPEF!" "" -1
            return 1
        }
        get_ext_status
        if { [state tmg_extracted] != 1 && [state db_parasitics] == ""} {
            error "Can only diff against an extracted design!" "" -1
        }

        parse_args

        echo [cmdline]

        set fn [get_arg file]
        if { ! [file exists $fn] } {
            error "File $fn does not exist in diff_spef." "" -1
        }

        set ext [get_tm Ext]
        notice "Diffing SPEF file $fn, results in diff_spef.out."
        set cmd [build_cmd $ext "diff_spef"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc while diffing against SPEF file $fn." "" -1
        }
        set_state db_diff_spef 1
        cmd_rlog
    }

    #################################################################
    #
    # ECO methods
    #
    #################################################################

    protected variable eco_list;
    array set eco_list {};

    VOB::add_args PDB_START_TRANSACTION_LOG {
        {block nstr 0}
        {tag nstr 1}
    }
    VOB::add_help PDB_START_TRANSACTION_LOG {
        {block "Name of DB block to track"}
        {tag "Tag to associate to transaction log instance"}
    }
    public method start_transaction_log { args} {
        parse_args
        echo [cmdline]

        set tag [get_arg tag]
        if { [::info exists eco_list($tag)] == 1 } {
            error "Transaction log $tag is already active - please close it first!"
        }


        set eco [new_eco_inst]

        set block ""
        if { [check_arg block] != 0 } {
            set ::eco_block [get_arg block]
        } else {
            set db [get_tm DB]
            $db get -top -object= ::eco_block
        }
        foreach blk [array names eco_list *block] {
            if { $eco_list($blk) eq $::eco_block } {
                error "Transaction log for block $::eco_block is already active!"
            }
        }

        set eco_list($tag,inst) $eco
        set eco_list($tag,block) $::eco_block
        $eco begin -block $::eco_block
    }

    VOB::add_args PDB_WRITE_TRANSACTION_LOG {
        {tag nstr 1}
        {file ofile 1}
        {include_power_nets bool 0}
        {use_location bool 0}
    }
    VOB::add_help PDB_WRITE_TRANSACTION_LOG {
        {tag "Tag associated with transaction log instance"}
        {file "Name of file to write to (in overwrite mode)"}
        {include_power_nets "Include transactions relating to power/gnd nets" }
        {use_location "Use instance location coords instead of origin" }
    }
    public method write_transaction_log { args} {
        parse_args
        echo [cmdline]

        set tag [get_arg tag]
        if { [::info exists eco_list($tag,inst)] == 0 } {
            error "Transaction log $tag does not exist - please create it first!"
        }
        rm_arg tag
        set eco $eco_list($tag,inst)
        set cmd [build_cmd $eco "write"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc while writing transaction log $tag" "" -1
        }
    }

    VOB::add_args PDB_END_TRANSACTION_LOG {
        {tag nstr 1}
    }
    VOB::add_help PDB_END_TRANSACTION_LOG {
        {tag "Tag associated with transaction log instance"}
    }
    public method end_transaction_log { args} {
        parse_args
        echo [cmdline]

        set tag [get_arg tag]
        if { [::info exists eco_list($tag,inst)] == 0 } {
            error "Transaction log $tag does not exist - please create it first!"
        }
        set eco $eco_list($tag,inst)
        $eco end
        array unset eco_list "$tag,inst"
        array unset eco_list "$tag,block"
    }

    #################################################################
    #
    # Parallel Extraction.. Move to Ext?
    #
    #################################################################

    ## in microns^2 (0.06 mm^2)
    protected variable idealmintilesize 60000
    protected variable finalmintilesize 400
    protected variable goalterms 40000
    protected variable maxtiles 256

    protected variable blocks ""
    protected variable curdir [pwd]
    protected variable num_active_rmt 0;

    private method get_area { args } {
        set te [get_tm TE]
        $te get_die_area -area= ::area
        notice "Chip area: $::area"
        set_state chip_area $::area
    }
    
    VOB::add_args PDB_CREATE_TILES {
        {num_tiles num 0 -1}
    }

    VOB::add_help PDB_CREATE_TILES {
    }
    public method create_tiles { args } {
        parse_args
        set_state num_tiles [get_arg num_tiles]
        init_tiles

        set te [get_tm TE]

        #if { $demandtiling } {
            #$te set_demand_tile -level 1
        #}

        #$te set_corner_count -count 1

        $te make_regular -nproc [state num_tiles]
        $te snap_tiles
        notice "Tiles:"
        $te print_tiles
        #$te make_tile_blocks -no_power
        $te make_tile_blocks 
        $te get_tile_block_names -string= ::blocks

        set_state tile_blocks $::blocks
        set_state rem_tiles $::blocks
        notice "Tile blocks: [state tile_blocks]"
        cmd_rlog
    }

    VOB::add_args PDB_FLATTEN { 
        { blocks flist {} {} }
        { create_boundary_regions bool 0 0}
        { create_bterm_map bool 0 0}
    }

    VOB::add_help PDB_FLATTEN { 
        { blocks "list of db blocks to be processed in parallel" }
        { create_boundary_regions bool 0 0}
        { create_bterm_map bool 0 0}
    }

    public method flatten { args } {
        notice "Start db flattening [runtime]"
        set db [get_tm DB]
	$db flatten 
        notice "Finish db flattening at [runtime_incr]"
    }
    VOB::add_args PDB_INIT_JOBS { 
        { blocks flist {} {} }
    }

    VOB::add_help PDB_INIT_JOBS { 
        { blocks "list of db blocks to be processed in parallel" }
    }

    public method prepare_jobs { args } {
	set blocks [join $args]
        notice "Start Preparing jobs for Parallel Extraction at [runtime]"
        puts "BLOCKS: $blocks, Names: $blocks"
        set_state tile_blocks $blocks
        set_state rem_tiles $blocks

	set TN $blocks
        set_state tile_block $TN
        set_state rem_tiles $TN
        notice "Finish Preparing Blocks at [runtime_incr]"
    }
    public method init_jobs { args } {
        notice "Start Preparing Blocks for Parallel Extraction at [runtime]"
        set db [get_tm DB]
        set ext [get_tm Ext]
        $db get_children -chip -block_list= ::tile_list -block_name_list= ::tile_names
        puts "BLOCKS: $::tile_list, Names: $::tile_names"
        set_state tile_blocks $::tile_names
        set_state rem_tiles $::tile_names
        $db get -top -object= ::mblock

        set TL {}
        foreach tile $::tile_names {
            db find -block $tile -object= ::blk
            $db get_block_stats -block $::blk -wire_count= ::wcnt
            puts "Block $tile has $::wcnt wires"
            lappend TL [list $tile $::wcnt]
        }
        puts "Full list: $TL"
        set TL [lsort -integer -decreasing -index 1 $TL]
        puts "Full sorted list: $TL"
        set TN ""
        foreach entry $TL {
            foreach {tile wcnt} $entry {
                append TN "$tile "
            }
        }
        puts "Sorted Block Names: $TN"
        set_state tile_block $TN
        set_state rem_tiles $TN
        notice "Finish Preparing Blocks at [runtime_incr]"
    }
    public method create_new_tiles { extRules {max_res 50.0} } {
        notice "Start Create Tiles at [runtime]"
        set db [get_tm DB]
        set ext [get_tm Ext]
        $ext extract -tiling 77 -signal_table 3 -ext_model_file $extRules -max_res $max_res
        $ext extract -tiling 10 -signal_table 3 -ext_model_file $extRules -max_res $max_res
        $db get_children -chip -block_list= ::tile_list -block_name_list= ::tile_names
        puts "TILES: $::tile_list, Names: $::tile_names"
        set_state tile_blocks $::tile_names
        set_state rem_tiles $::tile_names
        $db get -top -object= ::mblock

        set TL {}
        foreach tile $::tile_names {
            db find -block $tile -object= ::blk
            $ext set_block -block $::blk

            ## for stats
            #$ext extract -tiling 8 -signal_table 2
            ##

            ## for stats + filling
            $ext extract -tiling 8 -signal_table 3
            ##

            #$db find_property -name _estimatedWireCnt -value= ::wcnt -obj $::blk
            $db get_block_stats -block $::blk -wire_count= ::wcnt
            puts "Block $tile has $::wcnt wires"
            lappend TL [list $tile $::wcnt]
        }
        puts "Full list: $TL"
        set TL [lsort -decreasing -index 1 $TL]
        puts "Full sorted list: $TL"
        set TN ""
        foreach entry $TL {
            foreach {tile wcnt} $entry {
                append TN "$tile "
            }
        }
        puts "Sorted Tile Names: $TN"
        set_state tile_block $TN
        set_state rem_tiles $TN
        notice "Finish Create Tiles at [runtime_incr]"
    }
    VOB::add_args PDB_PROCESS_TILES {
        {tile_script nstr 0 ""}
        {ext_model_file ifile 0 ""}
        {exec_name nstr 0 ""}
        {exec_parms nstr 0 ""}
        {lefs nslist 0 ""}
        {internal num 0 0}
        {debug num 0 0 }
        {flatten hbool 0 0}
        {ext_args str 0 ""}
    }
    VOB::add_help PDB_PROCESS_TILES {
        {tile_script "name of script to execute"}
        {ext_model_file "extraction rules file"}
        {exec_name "name of executable to run"}
        {exec_parms "parameters to pass to executable"}
        {lefs "list of LEF files"}
        {internal "run internal extractor 1: old tiling -- 3: new tiling -- 2: BB"}
        {debug "write spef after each assembly for BB only"}
        {flatten "Write SPEF of internal nets after assembly and add parasitics to top; for BB only"}
        {ext_args "args to extraction"}
    }

    public method process_tiles { args } {
        ## initialize msh
        notice "Start block parallel processing at [runtime]"
        set t [start_timer]
        parse_args
        set_state tile_script [get_arg tile_script]
        set_state exec_name [get_arg exec_name]
        set_state exec_parms [get_arg exec_parms]
        if { [check_arg lefs] } {
            set_state tile_lefs [get_arg lefs]
        }
        set_state fin_tiles ""
        set_state sched_tiles ""

        init_tile_balancing

        if { [check_arg flatten] } {
        	set_state flatten [get_arg flatten]
	}
        set_state debug [get_arg debug]
        set_state int_extraction [get_arg internal]
        set_state int_args [get_arg ext_args]

        get_tm MSH
        set nproc $::msh::OPT(nproc)
        if { $nproc < 1 } {
            error "Need at least one remote machine!"
        }

        if { [get_arg internal] == 0 } {
            cd tiles
            exec ln -sf [get_arg ext_model_file] ext_rules
            cd ..
        }

        set_state pdb_remotes {}
        set stiles 0
        Publish chip.tech
        Publish chip.libs


        for { set i 0 } { $i < $nproc } { incr i } {
            new_rmt R$i 1 athext
            set rmt [state ns]::R$i
            $rmt configure -mycallback [::itcl::code $this addRmt $rmt]
            $rmt start
            set host_type($rmt) ""
        }
        notice "Waiting for Remotes"
        ## give 'em 10 seconds to start doing something
        set cnt 0
        while { [state pdb_remotes] eq "" } {
            set ::pdbmutex 0
            after 1000 { set ::pdbmutex 1}
            vwait ::pdbmutex
            incr cnt
            puts "cnt: $cnt"
            if { $cnt == 100 } {
                for { set i 0 } { $i < $nproc } { incr i } {
                    set rmt [state ns]::R$i
                    ## disable callbacks
                    $rmt configure -mycallback ""
                }
                error "No remotes started in 100 seconds!" "" -1
            }
        }
        tile_stats

        set cnt 0
        while { ([state rem_tiles] != "") || ([state sched_tiles] != "") } {
            update
            after 100
            set mycnt [schedule_rmts 1]
            incr cnt
            if { ([expr {$cnt % 50}] == 0) || ($mycnt > 0) } {
                tile_stats
            }
        }

        puts "OUT OF TOTAL LOOP!"
        cmd_rlog
        if { [state int_extraction] == 1 } {
            assemble_tiles
        } 
	if { [state int_extraction] == 2 } {
		if { [state flatten] } {
        		set ext [get_tm Ext]
			$ext write_spef_nets -flatten -parallel
		}
	}
	
        set_state tmg_extracted 1
        cmd_rlog
        end_timer $t "Process Tiles"
        notice "Finished block parallel processing at [runtime_incr]"
    }
    public method run_remote_jobs { script_name } {
        ## initialize msh
        notice "Start Script parallel processing at [runtime]"
        set t [start_timer]
        #parse_args
        #set_state exec_name [get_arg exec_name]
        #set_state exec_parms [get_arg exec_parms]
        #set_state tile_lefs [get_arg lefs]
        set_state fin_tiles ""
        set_state sched_tiles ""
        set_state exec_name $script_name

        init_tile_balancing

        get_tm MSH
        set nproc $::msh::OPT(nproc)
        if { $nproc < 1 } {
            error "Need at least one remote machine!"
        }

        set_state pdb_remotes {}
        set stiles 0

        for { set i 0 } { $i < $nproc } { incr i } {
            new_rmt R$i 1 athext
            set rmt [state ns]::R$i
            $rmt configure -mycallback [::itcl::code $this addRmt $rmt]
            $rmt start
            set host_type($rmt) ""
        }
        notice "Waiting for Remotes"
        ## give 'em 10 seconds to start doing something
        set cnt 0
        while { [state pdb_remotes] eq "" } {
            set ::pdbmutex 0
            after 1000 { set ::pdbmutex 1}
            vwait ::pdbmutex
            incr cnt
            puts "cnt: $cnt"
            if { $cnt == 100 } {
                for { set i 0 } { $i < $nproc } { incr i } {
                    set rmt [state ns]::R$i
                    ## disable callbacks
                    $rmt configure -mycallback ""
                }
                error "No remotes started in 100 seconds!" "" -1
            }
        }
        tile_stats

        set cnt 0
        while { ([state rem_tiles] != "") || ([state sched_tiles] != "") } {
            update
            after 100
            set mycnt [schedule_scripts 1]
            incr cnt
            if { ([expr {$cnt % 50}] == 0) || ($mycnt > 0) } {
                tile_stats
            }
        }

        puts "OUT OF TOTAL LOOP!"
        cmd_rlog
        end_timer $t "Process Remote Jobs"
        notice "Finished block parallel processing at [runtime_incr]"
    }


    
    protected method tile_stats_summary_xml { } {
        notice "<STATS> <moment> [runtime_incr_ext_2] </moment> <scheduled>[llength [state sched_tiles]] </scheduled> <finished> [llength [state fin_tiles]] </finished> <queued>[llength [state rem_tiles]]  </queued> </STATS>"

        #notice "<STATS> <scheduled> <tiles> [llength [state sched_tiles]] </tiles> <moment> [runtime_incr_ext_2] </moment> </scheduled> </STATS>"
        #notice "<STATS> <finished> <tiles> [llength [state fin_tiles]] </tiles> <moment> [runtime_incr_ext_2] </moment> </finished> </STATS>"
        #notice "<STATS> <queued> <tiles> [llength [state rem_tiles]] </tiles> <moment> [runtime_incr_ext_2] </moment> </queued> </STATS>"
        notice ""
    }
    protected method tile_stats_summary { } {
        notice "STATS scheduled [llength [state sched_tiles]] moment [runtime_incr_ext_2]"
        notice "STATS finished [llength [state fin_tiles]] moment [runtime_incr_ext_2]"
        notice "STATS queued [llength [state rem_tiles]] moment [runtime_incr_ext_2]"
        notice ""
    }
    protected method tile_stats { } {
        notice "[llength [state sched_tiles]] tiles currently scheduled:\n[state sched_tiles]"
        set ll [llength [state fin_tiles]]
        if { $ll > 0 } {
            notice "$ll tiles finished:\n[state fin_tiles]."
        }
        set ll [llength [state rem_tiles]]
        if { $ll > 0 } {
            notice "$ll tiles in queue:\n[state rem_tiles]"
        }
        notice ""
    	tile_stats_summary_xml
    }

    private method add_sched_tile { tile } {
        puts "add_sched_tile $tile"
        set s [state sched_tiles]
        lappend s $tile
        set_state sched_tiles $s
    }

    private method remove_sched_tile { tile } {
        #puts "remove_sched_tile $tile"
        set s [state sched_tiles]
        set idx [lsearch $s $tile]
        if { $idx >= 0 } {
            set s [lreplace $s $idx $idx]
            set_state sched_tiles $s
        }
    }

    private method addRmt { rmt } {
        notice "addRMT: $rmt at [runtime]/  [state pdb_remotes]"
        lappend_state pdb_remotes $rmt
        #puts "addRMT: $rmt /  [state pdb_remotes]"
    }

    private method schedule_scripts { {cnt 0} } {
        set mycnt 0
        ## scan remotes
        #puts "SR!!!"
        foreach rmt [state pdb_remotes] {
            notice "Status of rmt $rmt: [$rmt status]"
            if { [$rmt status] == 0 } {
                ## done

                scheduleAnyScript $rmt
                lremove_state pdb_remotes $rmt
                incr mycnt
            }
            if { ($cnt > 0) && ($cnt == $mycnt) } {
                break;
            }
        }
        return $mycnt
    }
    private method schedule_rmts { {cnt 0} } {
        set mycnt 0
        ## scan remotes
        #puts "SR!!!"
        foreach rmt [state pdb_remotes] {
            notice "Status of rmt $rmt: [$rmt status]"
            if { [$rmt status] == 0 } {
                ## done

                scheduleTile $rmt
                lremove_state pdb_remotes $rmt
                incr mycnt
            }
            if { ($cnt > 0) && ($cnt == $mycnt) } {
                break;
            }
        }
        return $mycnt
    }

    protected variable queue_size 0
    protected variable host_type
    protected method init_tile_balancing { } {
        set queue_size [llength [state rem_tiles]]
        array set host_type {}
    }

    ## Highly Athena Dependent
    protected method classifyHost { rmt } {
        set host [$rmt get_remote_host]
        puts "Next Tile for host: $host"
        set pref [lindex [split $host "0123456789"] 0]
        switch $pref {
            qcore { return 2.0 }
            opt { return 2.2 }
            bw  { return 1.9 }
            ammo  { return 1.0 }
            #default { error "Cannot classify host $host" }
            default { return 1.0 }
        }
    }


    protected variable sp_fastest 1.0
    protected variable sp_slowest 99.0
    protected variable sp_diff 0.0
    protected method updateMachineSpeed { speed } {
        if { $speed > $sp_fastest } {
            set sp_fastest $speed
            set sp_diff [expr ($sp_fastest - $sp_slowest) / 2]
        }
        if { $speed < $sp_slowest } {
            set $sp_slowest $speed
            set sp_diff [expr ($sp_fastest - $sp_slowest) / 2]
        }
    }

    protected method nextTile { rmt } {

        set rem_tiles [state rem_tiles]
        set ll [llength $rem_tiles]

        if { $ll == 0 } {
            notice "No more tiles to extract!"
            return ""
        }

        set speed [classifyHost $rmt]
        updateMachineSpeed $speed

        #puts "MS: $speed"
        #puts "RT: $rem_tiles"

        set tile ""
        if { ([expr $sp_fastest - $speed] <= 0.1) || ($ll < 5) } {
            ## fast machine or nearly out of tiles - take tile off top
            puts "Fast machine: tile off top!"
            set tile [lindex $rem_tiles 0]
            set rem_tiles [lrange $rem_tiles 1 end]
        } else {
            #puts "NT: else!"
            set sdiff [expr ($sp_fastest - $speed)]
            set idx [expr {int($ll / 4) }]

            if { $sdiff  < $sp_diff } {
                ## medium fast  - take tile after first 1/4
                puts "Medium fast machine: tile near top!"
                set tile [lindex $rem_tiles $idx]
                set rem_tiles [lreplace $rem_tiles $idx $idx]
            } else {
                ## medium slow - take tile before last 1/4
                puts "Medium slow machine: tile near middle!"
                set idx2 [expr {int($ll / 2) }]
                set tile [lindex $rem_tiles $idx2]
                set rem_tiles [lreplace $rem_tiles $idx2 $idx2]
            }
        }

        set_state rem_tiles $rem_tiles
        puts "Returning tile $tile"
        return $tile
    }

    protected method scheduleTile { rmt } {

        set rl [split $rmt :]
        set remote [lindex $rl end]
        set otile [$rmt get_name]
        set db [get_tm DB]
        set ext [get_tm Ext]
        if { $otile == "" } {
            ## First time!!
            puts "First subscribe for node $rmt!"

            #$rmt mdebug
            #$rmt send_command sys "eval { gdb n14:21.0 -x ~/.c }"

            $rmt send_command acmd "Subscribe chip.tech"
            $rmt send_command acmd "Subscribe chip.libs"
            $rmt send_command sys "eval {::zr.zlog enable_debug_log -module ZTC}"
            $rmt send_command sys "eval {::zr.zlog enable_debug_log -module ZTIMER}"
            #$rmt send_command acmd "cd tiles"
            $rmt send_command acmd "array unset ::env MSHTID"
            incr num_active_rmt
            puts "Num active rmts: $num_active_rmt"

        } else {
            puts "Tile $otile completed (node $remote)"
            puts "Subscribe to tile $otile.res.block -- int_extraction: [state int_extraction]"
            if { [state int_extraction] == 1 } {
		puts "assembly mode: old_te"
                Subscribe -dbargs "-parasitics" $otile.res.block
                #Subscribe -async -dbargs "-parasitics" $otile.res.block
                #Wait $otile.res.block
                set te [get_tm TE]
                $te assemble -relative_c_threshold 0.01 -tile [string range $otile 1 end] 
            } elseif { [state int_extraction] == 3 } {
                #db clear -block $::blk
                #db create -block $otile -parent $::mblock
                Subscribe -dbargs "-parasitics -no_nets" $otile.res.block
                db find -block $otile -object= ::blk
		puts "Finished reading parasitics for block $otile $::blk - tile asmbl"
                #db save_def -block $::blk -file $otile.def
                $ext assembly -main_block $::mblock -block $::blk
                #db clear -block $::blk
                #db destroy -block $::blk
            } else {
		puts "assembly mode: BBext"
                #db clear -block $::blk
                #db create -block $otile -parent $::mblock
#Dimitris BlockBased Extraction
                Subscribe -dbargs "-parasitics -no_nets" $otile.res.block
                db find -block $otile -object= ::blk
		puts "Finished reading parasitics for block $otile $::blk"
                #db save_def -block $::blk -file $otile.def
                $ext assembly -block $::blk

		if { [state flatten] } {
			$ext flatten -block $::blk -spef
		} else {
			if { [state debug]>0 } {
                		$ext set_block -block $::blk
                		$ext write_spef -file $otile.spef
			}
		}

                #$ext assembly -main_block $::mblock -block $::blk
                #db clear -block $::blk
                #db destroy -block $::blk
            }
            $rmt send_command acmd "Unpublish $otile.res.block"
            Unpublish $otile.mblock

            set ftiles [state fin_tiles]
            lappend ftiles $otile
            set_state fin_tiles $ftiles

            remove_sched_tile $otile
            $rmt set_name ""
        } 

        set tile [nextTile $rmt]
        puts "Tile for rmt $rmt: $tile"
        if { $tile eq "" } {
            #itcl::delete object $rmt
            update
            incr num_active_rmt -1
            puts "Num active rmts: $num_active_rmt"
            if { $num_active_rmt == 0 } {
                set ::pdbmutex 1
            }
            return
        }

        notice "scheduleRMT $tile at [runtime] on remote $remote"
        $rmt set_name $tile
        add_sched_tile $tile

        #puts "Publish $tile.mblock"
        Publish $tile.mblock

        if { [state int_extraction] == 0 } {
            puts "External extraction"

            file mkdir tiles/$tile
            $db find -block $tile -object= ::BN
            #$db write -block $::BN -file tiles/$tile/remote_in.db

            $rmt send_command acmd "cd $tile"
            $rmt send_command acmd "Subscribe $tile.mblock"
            $rmt send_command acmd "set \[::apps::milos::cur_ns]::STATE(db_loaded) 1"
            $rmt send_command acmd "set ::tnum $tile"
            if { [state tile_lefs] == "" } {
                #$rmt send_command pdb "save_design -use_ids -def_out tile.def -lef_out tile.lef"
                $rmt send_command pdb "save_design -def_out tile.def -lef_out tile.lef"
                write_file tile_lefs ""
            } else {
                #$rmt send_command pdb "save_design -use_ids -def_out tile.def"
                $rmt send_command pdb "save_design -def_out tile.def"
                write_file tiles/tile_lefs [state tile_lefs]
            }
            #$rmt send_command acmd "source [state tile_script]"
            $rmt send_command acmd "set ::tcmd \"exec [state exec_name] [state exec_parms] [state tile_script]\""
            $rmt send_command acmd "puts \"exec cmd: \$::tcmd\""
            $rmt send_command acmd "::eval \$::tcmd"
            #$rmt send_command pdb "read_spef -use_ids -file tile.spef.gz"
            #$rmt send_command pdb "read_spef -db_corner_name MinMax0 -file tile.spef.gz"
            $rmt send_command pdb "read_spef -file tile.spef.gz"

            ## write debugging db
            $rmt send_command acmd "::db find -block $tile -object= ::BN"
            $rmt send_command acmd "::db get -top -object= ::BN"
            #$rmt send_command acmd "::db write -block \$::BN -parasitics -file ${tile}_remote_out.db"
            #$rmt send_command acmd "::db write -file full_remote_out.db"
            $rmt send_command acmd "Publish -dbargs -parasitics $tile.res.block"
            $rmt send_command acmd "\$::apps::milos::mycf del_tm Ext"
            $rmt send_command acmd "cd .."
        } elseif { [state int_extraction] == 1 } {
            puts "Internal extraction 1"
            $rmt send_command acmd "Subscribe $tile.mblock"
            $rmt send_command acmd "set \[::apps::milos::cur_ns]::STATE(db_loaded) 1"
            if { [state tile_script] ne "" } {
                $rmt send_command "source [state tile_script]"
            }
            $rmt send_command ext "extract [state int_args]"
            #$rmt send_command pdb "write_spef -outfile $tile/int.spef.gz"
            $rmt send_command acmd "Publish -dbargs -parasitics $tile.res.block"
            $rmt send_command acmd "\$::apps::milos::mycf del_tm Ext"
        # DIMITRIS 012810D  elseif  [state int_extraction] == 2  
        } elseif { [state int_extraction] > 1 } {
            ## fill in the tile..
            puts "FILLING tile $tile"
            puts "BBD::tile $tile"
            if { 0 } {
            $db find -block $tile -object= ::blk
            $ext set_block -block $::blk
            set cmd "$ext extract [state int_args] -tiling 8 -signal_table 3"
            ::eval $cmd
            }

            $rmt send_command acmd "Subscribe $tile.mblock"
            $rmt send_command acmd "set \[::apps::milos::cur_ns]::STATE(db_loaded) 1"
            puts "SENDING SB CMD!"
            #$rmt send_command "set tile $tile"

            $rmt send_command ext "set_block $tile"

            if { [state tile_script] ne "" } {
                $rmt send_command "source [state tile_script]"
            }
            #$rmt send_command db "order_wires"
            $rmt send_command ext "extract [state int_args]"
            #$rmt send_command pdb "write_spef -outfile $tile.int.spef"
            $rmt send_command acmd "Publish -dbargs \"-parasitics -no_nets\" $tile.res.block"

	    # 030210D DIMITRIS MEMORY REDUCTION TEST
            $rmt send_command db "clear -block $tile"
	    # 030210D END

            #$rmt send_command acmd "\$::apps::milos::mycf del_tm Ext"
        }
    }
    protected method scheduleAnyScript { rmt } {

        set rl [split $rmt :]
        set remote [lindex $rl end]
        set otile [$rmt get_name]
        if { $otile == "" } {
            ## First time!!
            #$rmt mdebug
            #$rmt send_command sys "eval { gdb n14:21.0 -x ~/.c }"

            #$rmt send_command acmd "Subscribe chip.tech"
            #$rmt send_command acmd "Subscribe chip.libs"
            #$rmt send_command sys "eval {::zr.zlog enable_debug_log -module ZTC}"
            #$rmt send_command sys "eval {::zr.zlog enable_debug_log -module ZTIMER}"
            #$rmt send_command acmd "cd tiles"

            $rmt send_command acmd "array unset ::env MSHTID"
            incr num_active_rmt
            puts "Num active rmts: $num_active_rmt"

        } else {
            puts "Block $otile completed (node $remote)"

		#puts "assembly NOT required"
                #Subscribe -dbargs "-parasitics" $otile.res.block
                #set te [get_tm TE]
                #$te assemble -relative_c_threshold 0.01 -tile [string range $otile 1 end] 

            #$rmt send_command acmd "Unpublish $otile.res.block"
            #Unpublish $otile.mblock

            set ftiles [state fin_tiles]
            lappend ftiles $otile
            set_state fin_tiles $ftiles

            remove_sched_tile $otile
            $rmt set_name ""
        } 

        set tile [nextTile $rmt]
        puts "Block for rmt $rmt: $tile"
        if { $tile eq "" } {
            #itcl::delete object $rmt
            update
            incr num_active_rmt -1
            puts "Num active rmts: $num_active_rmt"
            if { $num_active_rmt == 0 } {
                set ::pdbmutex 1
            }
            return
        }

        notice "scheduleRMT $tile at [runtime] on remote $remote"
        $rmt set_name $tile
        add_sched_tile $tile

        #puts "Publish $tile.mblock"
        #Publish $tile.mblock

            puts "External script [state exec_name]"

            #file mkdir tiles/$tile
            #file mkdir $tile

            $rmt send_command acmd "cd $curdir/$tile"
            #$rmt send_command acmd "Subscribe $tile.mblock"
            $rmt send_command acmd "set ::tnum $tile"
            #$rmt send_command acmd "source [state tile_script]"

	    #---------------------------------------------------------------------------------------------------
            $rmt send_command acmd "exec [state exec_name] [state exec_parms] [state tile_script]"
            #$rmt send_command acmd "set ::tcmd \"exec [state exec_name] [state exec_parms] [state tile_script]\""
            #$rmt send_command acmd "puts \"exec cmd: \$::tcmd\""
            #$rmt send_command acmd "::eval \$::tcmd"
	    #---------------------------------------------------------------------------------------------------
            #$rmt send_command pdb "read_spef -file tile.spef.gz"
    }


    private method init_tiles { } {
        get_area
        if { [state num_tiles] <= 0 } {
            notice "Using auto tiling."

            set te [get_tm TE]
            $te get_term_count -count= ::term_count

            notice "TC: $::term_count"

            set ntiles [expr int($::term_count / $goalterms) + 1]
            notice "Ntiles by tile size: $ntiles"

            set idealmin [expr 3*$::msh::OPT(nproc)]
            if { $idealmin < 32 } {
                set idealmin 32
            }

            if { $ntiles < $idealmin } {
                set ntiles $idealmin
            }

            set tilesize [expr double([state chip_area]) / double($ntiles)]

            notice "Tile Size (die: [state chip_area]: $tilesize"

            if { $tilesize < $idealmintilesize } {
                set ntiles [expr int([state chip_area] / $idealmintilesize)]
                notice "Too many tiles for optimum performance (tiles too small)"
                notice "Adjusting number of tiles to $ntiles"
            }
        } else {
            notice "Using user-specified tile count: [state num_tiles]"
            set ntiles [state num_tiles]
        }

        if { $ntiles < 1 } {
            set ntiles 1
        }

        if { $ntiles > $maxtiles } {
            set ntiles $maxtiles
        }

        set tilesize [expr double([state chip_area]) / double($ntiles) ]

        notice "tilesize for $ntiles tiles: $tilesize"
        if { $tilesize < $finalmintilesize } {
            set tilenum [expr [state chip_area] / $finalmintilesize]
            warning "Tile size too small for accurate performance."
            warning "Maximum tile limit for this design: $tilenum"
            set ntiles $tilenum
        }

        if { $ntiles < 1 } {
            set ntiles 1
        }

        if { $ntiles > $maxtiles } {
            set ntiles $maxtiles
        }

        notice "Final tile count: $ntiles"
        set_state num_tiles $ntiles
    }

    private method assemble_tiles { } {
        notice "Integrating Tiles"
        set te [get_tm TE]
        $te assemble -wrap -relative_c_threshold 0.01

        set_state assembled 1
    }


    VOB::add_args PDB_DEFINE_EXT_CORNERS {
    }
    VOB::add_help PDB_DEFINE_EXT_CORNERS {
    }
    public method define_ext_corners { args } {

	set ec [get_extraction_corners]
	if { [llength $ec] >= 1 } {
		cmd_rlog "Extraction corners have been already defined: $ec"
	} else {
		set idx 0
        	set x [get_tm Ext]
		foreach ii $args {
        		if { [check_extraction_corner $ii] ne "" } {
            			error "Corner $ii has already been defined!" "" -1
        		}
        		set rc [$x define_process_corner -ext_model_index $idx -name $ii]
			incr idx
        		if { $rc != 0 } {
            			error "Error $rc from define_ext_corners" "" -1
        		}
		}
		set_state db_has_extraction_corners 1
	}
    }
    VOB::add_args PDB_EXTRACT_GRID {
        {exclude_cells str 0 ""}
        {hier bool 0 0}
        {skip_power_stubs bool 0 0}
        {skip_m1_caps bool 0 0}
        {res_factor num 0 1.0 }
        {cc_factor num 0 1.0}
        {gndc_factor num 0 1.0}
        {power_source_file ifile 0 ""}
    }
    VOB::add_help PDB_EXTRACT_GRID {
        { exclude_cells "Exclude cells from the power grid netlisting" }
        { hier "Hierarchical Extraction" }
        { skip_power_stubs "Only connect Power Pins to Grid" }
        { skip_m1_caps "no cap nodes for m1 grid" }
        { res_factor "multiply resistance by res_factor" }
        { cc_factor "multiply coupling capacitance by cc_factor" }
        { gndc_factor "multiply ground capacitance by gndc_factor" }
        { power_source_file "Power source file with coordinates" }

    }

    public method extract_grid { args } {
        set t [start_timer]
        parse_args

	set skip_m1_caps_word ""
        set skip_m1_caps [get_arg skip_m1_caps]
	if { $skip_m1_caps==1 } {
		set skip_m1_caps_word " -skip_m1_caps"
	}
        set skip_power_stubs [get_arg skip_power_stubs]
	set exclude_cells_list [get_arg exclude_cells]
	set exclude_option ""
	if { $exclude_cells_list != "" } {
		set exclude_option " -exclude_cells \"$exclude_cells_list\""
	}
	set power_coords_file ""
        set power_source_file [get_arg power_source_file]
	if { $power_source_file != "" } {
		set power_coords_file " -power_source_coords \"$power_source_file\""
	}
	set ext [get_tm Ext]
	set db [get_tm DB]

	set res_factor_opt [get_arg res_factor]
	set cc_factor_opt [get_arg cc_factor]
	set gndc_factor_opt [get_arg gndc_factor]

	set scale_options ""
	if { $res_factor_opt != 1.0 || $cc_factor_opt != 1.0 || $gndc_factor_opt != 1.0 } {
		if { $res_factor_opt != 1.0 } {
			append scale_options " -res_factor $res_factor_opt"
		}
		if { $cc_factor_opt != 1.0 } {
			append scale_options " -cc_factor $cc_factor_opt"
		}
		if { $gndc_factor_opt != 1.0 } {
			append scale_options " -gndc_factor $gndc_factor_opt"
		}
		#puts "scale_options= $scale_options"
		set ext_cmd "$ext adjust_rc $scale_options"
		notice "$ext_cmd"
		set rc [::eval $ext_cmd]
        	if { $rc != 0 } {
            		error "Error $rc during extraction" "" -1
        	}
	}
	set fp [open "Extract.info" w]
	set ext_options " -power_grid -lef_rc -max_res 0.0 $skip_m1_caps_word $exclude_option $power_coords_file"
        set hier_flag [get_arg hier]
	if { $hier_flag==1 } {
		notice "$::tile_names"
    		run_power_ext_top $ext_options
		foreach tile $::tile_names {
    			run_power_ext_cmd $ext_options $tile
			cmd_rlog
		}
		#foreach tile $::tile_names {
        		#$db find -block $tile -object= ::blk
			#$db save_def -file $tile.def
			#$db write -file $tile.db
		#}
	} else {

	set ext_cmd "$ext extract -power_grid -lef_rc -max_res 0.0 $skip_m1_caps_word $exclude_option $power_coords_file"
	#set ext_cmd "$ext extract -lef_rc -max_res 0.0 -power_grid -exclude_cells $exclude_cells_list"
	notice "Start Power Net extraction [runtime] : $ext_cmd"
	set rc [::eval $ext_cmd]
        if { $rc != 0 } {
            	error "Error $rc during extraction" "" -1
        }
	notice "End  block extraction [runtime_incr]"
	}
	#write_spef -outfile $spef_file
    }
    public method run_power_ext_cmd { ext_options blockName } {
	set ext [get_tm Ext]
	set db [get_tm DB]
        $db find -block $blockName -object= ::blk
	set ext_cmd "$ext set_block -block $::blk"
	set rc [::eval $ext_cmd]
        if { $rc != 0 } {
            	error "Error $rc during extraction" "" -1
        }
	set ext_cmd "$ext extract $ext_options"
	notice "Start Power Net extraction for Block $blockName $::blk [runtime]"
	set rc [::eval $ext_cmd]
        if { $rc != 0 } {
            	error "Error $rc during extraction" "" -1
        }
	notice "End  block extraction [runtime_incr]"
	#write_spef -outfile $spef_file
    }
    public method run_power_ext_top { ext_options } {
	set ext [get_tm Ext]
	set ext_cmd "$ext extract $ext_options"
	notice "Start Power Net extraction for Top Block [runtime]"
	set rc [::eval $ext_cmd]
        if { $rc != 0 } {
            	error "Error $rc during extraction" "" -1
        }
	notice "End  block extraction [runtime_incr]"
	#write_spef -outfile $spef_file
    }

    VOB::add_args PDB_EXTRACT {
        {ext_model_file ifile 0 ""}
        {max_res num 0 0.0 }
        {min_cc num 0 0.1 }
        {ext_args str 0 ""}
        {rmt_script str 0 ""}
        {hier bool 0 0}
        {distributed bool 0 0}
        {threads num 0 0}
        {spef_file ofile 0 ""}
        {corner str 0 ""}
        {debug hnum 0 0}
    }
    VOB::add_help PDB_EXTRACT {
        {ext_model_file "extraction rules file"}
        {max_res "max combined res allowed for rsegs in series"}
        {min_cc "max combined res allowed for rsegs in series"}
        {parallel "distributed or distributed"}
        {hier "block based design"}
        {corner "if empty, all corners will be extracted"}
        {spef_file "write a flat spef"}
    }

    public method extract { args } {
        set t [start_timer]
        parse_args

	set ec [get_extraction_corners]
	if { [llength $ec] == 0 } {
            	error "No corners have been defined!" "" -1"
	}

        set extRules [get_arg ext_model_file]
        set hier [get_arg hier]
        set distributed [get_arg distributed]
	set max_res [get_arg max_res]
	set min_cc [get_arg min_cc]
	set spef_file [get_arg spef_file]
	set rmtScript [get_arg rmt_script]

	# Will make so can extract any corner list
	# for now we extract all corners

	set corner [get_arg corner]
	if { $rmtScript eq "" } {
		#have to write on running dir
		set rmtScript "./tmp.rmt.ext.tcl"
		set idx 0
		set fp [open $rmtScript w]
		foreach ii $ec {
			puts $fp "ext define_ext_corner -name $ii -model_idx $idx"
			incr idx
		}
		close $fp
	}
	#puts "extRules= $extRules  hier=$hier  distributed=$distributed"

	set ext [get_tm Ext]
	if { $distributed==0 } { 
		notice "Start block extraction, non parallel [runtime]"
		set ext_cmd "$ext extract -ext_model_file $extRules -corner_cnt 1 \
			-max_res $max_res -coupling_threshold $min_cc \
			-signal_table 3 -over_cell"

		set rc [::eval $ext_cmd]

        	if { $rc != 0 } {
            		error "Error $rc during extraction" "" -1
        	}
		notice "End  block extraction [runtime_incr]"
		cmd_rlog

		write_spef -outfile $spef_file
		notice "End  write_spef [runtime_incr]"
	} elseif { $hier==1 } {
		notice "Start Hierarchical Block Based extraction, Distributed [runtime]" 

		cmd_rlog "Extracting top level"
		set ext_cmd "$ext extract -ext_model_file $extRules -corner_cnt 1 \
			-max_res 0.0 -signal_table 3 -over_cell"

		set rc [::eval $ext_cmd]

        	if { $rc != 0 } {
            		error "Error $rc during extraction" "" -1
        	}
		cmd_rlog

		if { $spef_file eq "" } {
			init_jobs
			process_tiles -internal 2 -tile_script $rmtScript \
        		-ext_args "-ext_model_file $extRules -signal_table 3 -max_res 0.0"
		} else {
		write_spef -outfile $spef_file -flatten -parallel
		init_jobs
		process_tiles -flatten -internal 2 -tile_script $rmtScript \
        		-ext_args "-ext_model_file $extRules -signal_table 3 -max_res 0.0"
		}
		notice "End Extraction [runtime_incr]"
		
	} else {
        	notice "Start distributed block extraction [runtime]"
		# Tiled distributed block extraction 
		#set ec [get_extraction_corners]
		#puts "ec=$ec"
		create_new_tiles $extRules $max_res
		cmd_rlog
		#do_threaded_extract 3

		process_tiles -internal 3 -tile_script $rmtScript \
			-ext_args "-ext_model_file $extRules -signal_table 3 -max_res 0.0 -over_cell"
		cmd_rlog
        	notice "End extraction [runtime_incr]"
		write_spef -outfile $spef_file

		notice "End  write_spef [runtime_incr]"
	}
    }
    VOB::add_args PDB_MPLACE {
        {dir_list nslist }
        {rmt_script str }
        {debug hnum 0 0}
    }
    VOB::add_help PDB_MPLACE {
        {dir_list "list of dirs that contain input/output files"}
        {rmt_script "csh script name that will run in each dir"}
    }

    public method mplace { args } {
        set t [start_timer]
        parse_args

	set rmtScript [get_arg rmt_script]
	set dirList [get_arg dir_list]

	# Will make so can extract any corner list
	# for now we extract all corners

	notice "Start Parallel/Distributed Placement [runtime]" 
	notice "\n\tdir_list : $dirList"
	notice "\trmtScript : $rmtScript \n"

	cmd_rlog "Placement ..."
	prepare_jobs $dirList
	#run_remote_jobs -tile_script $rmtScript
	run_remote_jobs $rmtScript

	#init_jobs
	#process_tiles -flatten -internal 2 -tile_script $rmtScript \
        		#-ext_args "-ext_model_file $extRules -signal_table 3 -max_res 0.0"
	notice "End Placement [runtime_incr]"
		
    }
    VOB::add_args PDB_EXTRACT_TILES {
        {tile_script nstr 0 ""}
        {ext_model_file ifile 0 ""}
        {exec_name nstr 0 ""}
        {exec_parms nstr 0 ""}
        {lefs nslist 0 ""}
        {type nstr 0 "internal"}
        {ext_args str 0 ""}
        {debug hnum 0 0}
    }
    VOB::add_help PDB_EXTRACT_TILES {
        {tile_script "name of script to execute"}
        {ext_model_file "extraction rules file"}
        {exec_name "name of executable to run"}
        {exec_parms "parameters to pass to executable"}
        {lefs "list of LEF files"}
        {type "type: threaded, internal, external"}
        {ext_args "args to extraction"}
    }

    public method extract_tiles { args } {
        ## initialize msh
        set t [start_timer]
        parse_args

        set_state tile_script [get_arg tile_script]
        set_state exec_name [get_arg exec_name]
        set_state exec_parms [get_arg exec_parms]
        if { [check_arg lefs] } {
            set_state tile_lefs [get_arg lefs]
        }
        set_state fin_tiles ""
        set_state sched_tiles ""

        set_state int_args [get_arg ext_args]

        get_tm MSH
        set nproc $::msh::OPT(nproc)
        if { $nproc < 1 } {
            error "Need at least one remote machine!"
        }

        set type [get_arg type]
        switch $type {
            internal - external - threaded { }
            default { error "Extraction type $type not recognized - must be internal, external or threaded" "" -1 
            }
        }

        if { $type eq "external" } {
            file mkdir tiles
            cd tiles
            exec ln -sf [get_arg ext_model_file] ext_rules
            cd ..
            if { [state tile_lefs] == "" } {
                write_file tile_lefs ""
            } else {
                write_file tiles/tile_lefs [state tile_lefs]
            }
        } else {
            set_state ext_model_file [get_arg ext_model_file]
        }

        set_state pdb_remotes {}
        set stiles 0
        if { $type eq "threaded" } {
            ## call threaded extraction
            do_threaded_extract $nproc
        } else {
            Publish chip.tech
            Publish chip.libs

            set proxylist {}

            for { set i 0 } { $i < $nproc } { incr i } {

                set proxy [ [EXTProxy_${type} PR$i] this]
                $proxy configure -callback [::itcl::code $this addRmt $proxy]
                $proxy start
                #$proxy set_debug [get_arg debug]
                lappend proxylist $proxy
            }


            notice "Waiting for Remotes"
            ## give 'em 10 seconds to start doing something
            set cnt 0
            while { [state pdb_remotes] eq "" } {
                set ::pdbmutex 0
                after 1000 { set ::pdbmutex 1}
                vwait ::pdbmutex
                incr cnt
                puts "cnt: $cnt"
                if { $cnt == 100 } {
                    foreach proxy $proxylist {
                        $proxy exit
                    }
                    error "No remotes started in 100 seconds!" "" -1
                }
            }
            tile_stats

            set cnt 0
            while { ([state rem_tiles] != "") || ([state sched_tiles] != "") } {
                update
                after 100
                schedule_proxies 1
                incr cnt
                if { [expr {$cnt % 50}] == 0 } {
                    tile_stats
                }
            }

            puts "OUT OF TOTAL LOOP!"
        }
        assemble_tiles
        set_state tmg_extracted 1
        cmd_rlog
        end_timer $t "Process Tiles"
    }

    private method schedule_proxies { {cnt 0} } {
        set mycnt 0
        ## scan remotes
        #puts "S_P!!!"
        foreach rmt [state pdb_remotes] {
            if { [$rmt status] == 0 } {
                ## done
                lremove_state pdb_remotes $rmt
                scheduleProxy $rmt
                incr mycnt
            }
            if { ($cnt > 0) && ($cnt == $mycnt) } {
                break;
            }
        }
    }



    private method scheduleProxy { rmt } {

        set otile [$rmt cget -block]
        if { $otile == "" } {
            ## First time!!
            puts "First subscribe for node $rmt!"

            ## won't change for the whole run!
            $rmt configure -tile_script [state tile_script]
            $rmt configure -ext_args [state int_args]
            $rmt configure -tile_lefs [state tile_lefs]
            $rmt configure -exec_parms [state exec_parms]
            $rmt configure -exec_name [state exec_name]
            $rmt configure -ext_model_file [state ext_model_file]
            #$rmt debug 1

            incr num_active_rmt
            puts "Num active rmts: $num_active_rmt"

        } else {
            puts "Block $otile completed (proxy $rmt)"
            puts "Get tile $otile.res.block"

            $rmt get_tile
            $rmt assemble

            puts "Got tile $otile.res.block!"

            set ftiles [state fin_tiles]
            lappend ftiles $otile
            set_state fin_tiles $ftiles

            remove_sched_tile $otile

            $rmt configure -block ""
        } 

        set rem_tiles [state rem_tiles]
        set ll [llength $rem_tiles]
        if { $ll == 0 } {
            notice "No more tiles to extract!"
            $rmt exit
            itcl::delete object $rmt
            incr num_active_rmt -1
            puts "Num active rmts: $num_active_rmt"
            if { $num_active_rmt == 0 } {
                set ::pdbmutex 1
            }
            return
        }
        set tile [lindex $rem_tiles 0]
        set rem_tiles [lrange $rem_tiles 1 end]
        set_state rem_tiles $rem_tiles
        notice "Scheduling tile $tile on remote $rmt"
        #add_sched_tile $tile

        $rmt configure -block $tile
        $rmt send_tile
        $rmt extract
    }

    protected method joblist { } {
        set jl {}
        foreach blk [state tile_blocks] {
            set jid $blist($blk,jobid)
            if { $jid != 0 } {
                lappend jl $jid
            }
        }
        return $jl
    }

    protected variable blist
    protected method do_threaded_extract { num } {
        ## get all blocks
        package require Thread
        array set blist {}
        set db [get_tm DB]
        foreach blk [state tile_blocks] {
            #set b [string range $blk 1 end]
            db find -block $blk -object= ::BN
            set blist($blk,blk) $::BN
            set blist($blk,jobid) 0
        }

        puts "TEST: $::auto_path"
        set tpid [tpool::create -minworkers 1 -maxworkers $num]

	puts "tpid= $tpid   rem_tiles=[state rem_tiles]  sched_tiles=[state sched_tiles]"
        while { ([state rem_tiles] != "") || ([state sched_tiles] != "") } {

            set rem_tiles [state rem_tiles]
            set blk [lindex $rem_tiles 0]
            set_state rem_tiles [lrange $rem_tiles 1 end]
            add_sched_tile $blk

            set script "set ::block $blist($blk,blk)\n"
            append script "set ::auto_path \"[set ::auto_path]\"\n"

            #append script " source /u/mattias/.tclshrc \n"
            #append script "    lspackages\n"

            append script "package require Itcl\n"
            append script "package require ade::main 1.0\n"
            append script "package require ade::license 1.0\n"
            append script "puts \"CMDS: \[info commands]\n\"\n"
            append script "set tmcmds \[namespace eval ::zroute info commands *_tm]\n "
            append script "puts \"TM: \$tmcmds\"\n"


            append script "sys eval {Ext $blk}\n"
            append script "$blk set_block -block \$::block\n"
            append script "$blk extract -ext_model_file [state ext_model_file] [state int_args]\n"
            append script "$blk delete\n"
            append script "set ::block"
            puts "THREAD SCRIPT: $script"
            set blist($blk,jobid) [tpool::post $tpid $script]

            tile_stats

            set fin_jobs [tpool::wait $tpid [joblist] pend_jobs]
            foreach job $fin_jobs {
                set fblk [tpool::get $tpid $job]
                puts "Finished Job: $fblk"
                set blist($fblk,jobid) 0
                set te [get_tm TE]
                $te assemble -relative_c_threshold 0.01 -tile [string range $fblk 1 end] 

                rem_sched_tile $blk
                lappend_state fin_tiles $fblk
            }

            tile_stats
        }
    }

}


