
public method run_tmg_remote_script { {undo 0} } {
	mydebug "--------------------------> run_tmg_remote_script: BEGIN\n"
	if { [llength $rmt_list] == 0 } {
		return


#cterms ctlf

	Event <<TIMING_UPDATE_START>> $INCR_CNT

	update_incr_full_timing $rmt_tmg_info $suffix

	set num_scenarios 0
	if { $BBD_OPT eq "ON" } {
		set num_scenarios 1
	}
	foreach rmt $rmt_list {
		mydebug "\t<<<$rmt run_remote_tmg_cmd $rmt_tmg_info 0 $num_scenarios"
		#$rmt run_remote_tmg_cmd $rmt_tmg_info 0 $num_scenarios
		$rmt run_remote_tmg_cmd "example" {a b c d fgh 2}
		mydebug "\t>>>$rmt run_remote_tmg_cmd "
	}
        #if { $BBD_OPT ne "ON" } {
        #foreach rmt $rmt_list {
            ##020810D set dstring "[$rmt getAlias].$INCR_CNT.atfl"
            #set dstring "[$rmt getAlias].$INCR_CNT.$suffix"
                #mydebug "\tPublish $dstring"
            #$rmt publish $dstring
                #mydebug "\tSubscribe $dstring!"
            #Subscribe $dstring
            #mydebug "\tDone Subscribe $dstring!"
            #$rmt unpublish $dstring
        #}
        #} elseif { $rmt_tmg_info eq "aterms" } {
                #foreach rmt $rmt_list {
                        #$rmt store_timing_cache $num_scenarios
                #}
        #}

	check_multi_error
	incr INCR_CNT

	
	Event <<TIMING_UPDATE_END>> $INCR_CNT

	mydebug "--------------------------> run_tmg_remote_script: END\n"
}
