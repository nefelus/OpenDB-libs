proc get_end_point_slacks { outfile } {
  set endpoints [all_registers -data_pins]

# for the moment let's forget about IOs
#  set endpoints [add_to_collection $endpoints [all_outputs]]

  foreach_in_collection endpoint $endpoints {
    set slackRiseMin [get_attribute $endpoint min_rise_slack]
    set slackRiseMax [get_attribute $endpoint max_rise_slack]
    set slackFallMin [get_attribute $endpoint min_fall_slack]
    set slackFallMax [get_attribute $endpoint max_fall_slack]
    set slewRiseMin  [get_attribute $endpoint actual_rise_transition_min]
    set slewRiseMax  [get_attribute $endpoint actual_rise_transition_max]
    set slewFallMin  [get_attribute $endpoint actual_fall_transition_min]
    set slewFallMax  [get_attribute $endpoint actual_fall_transition_max]

    puts $outfile "$endpoint $slackRiseMin $slackRiseMax $slackFallMin $slackFallMax $slewRiseMin $slewRiseMax $slewFallMin $slewFallMax"
  }
}
