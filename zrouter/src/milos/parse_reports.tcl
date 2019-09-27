proc parse_to_keyword { stream keyword } {

	set ll ""
        while { ![eof $stream] } {

        	gets $stream ll
		set word0 [lindex $ll 0]

		if { $word0 eq $keyword } {
			return $ll
		}
	}
	return ""
}
proc parse_passed_clock { stream } {

	set found 0
	set ll ""
        while { ![eof $stream] } {

        	gets $stream ll
		set word0 [lindex $ll 0]

		if { $found>0 } {
			if { $word0 eq "clock" } {
				continue
			} else {
				break
			}
		}
		if { $word0 ne "clock" } {
			continue
		}
		set found 1
	}
	if { $found == 0 } {
		set ll ""
	}
	return $ll
}
proc get_name_id { name } {
	global idtable
	global idcount

	if { [info exists idtable($name)] == 0 } {
		incr idcount
		set idtable($name) idcount
		return "$idcount $idcount"
	}
	return "$idtable($name) 0"
}


proc read_tmp_report { filename prev_idcount } {

	global idtable
	global idcount
        
	set fi 0
	set look_for_arrival 0
        set lines ""
        set stream [open $filename]
        while { ![eof $stream] } {

            	gets $stream line
		set word0 [lindex $line 0]

		if { $look_for_arrival > 0 } {
			if { $word0 ne "data" } {
				set prev_line $line
				continue
			}
			if { [lindex $line 1] ne "arrival" } {
				continue
			}
			set look_for_arrival 0

			#puts "FROM : $from_inst(0) $from_inst(1)"
			#puts "TO   : $prev_line"

			set to_iterm [lindex $prev_line 0]
			set line [parse_passed_clock $stream]
			if {$line eq ""} {
				puts "Cannot find clock pin for to_term $to_iterm"
				continue
			}
			set to_clock [lindex $line 0]
			set line [parse_to_keyword $stream "slack"]
			if { $line eq "" } {
				puts "Cannot find slack for $from_inst(1)"
				continue
			}
			set slack [lindex $line 2]
			if { $prev_idcount>=0 } {
				set f1 [get_name_id $from_inst(0)]
				set f2 [get_name_id $from_inst(1)]
				set t1 [get_name_id $to_clock]
				set t2 [get_name_id $to_iterm]
				if { [lindex $f1 1]>0 } {
					append lines "N: [lindex $f1 0] $from_inst(0)" "\n"
				}
				if { [lindex $f2 1]>0 } {
					append lines "N: [lindex $f2 0] $from_inst(1)" "\n"
				}
				if { [lindex $t1 1]>0 } {
					append lines "N: [lindex $t1 0] $to_clock" "\n"
				}
				if { [lindex $t2 1]>0 } {
					append lines "N: [lindex $t2 0] $to_iterm" "\n"
				}
				append lines "P: $slack \
						[lindex $f1 0] [lindex $f2 0] \
						[lindex $t1 0] [lindex $t2 0]" "\n"
				continue
			}
			append lines "P: $slack $from_inst(0) $from_inst(1) $to_clock $to_iterm\n"
			continue
		}
		set start ""
		set end ""
		if { $word0 eq "Startpoint:" } {
			set start [lindex $line 1]

            		gets $stream line
			set word0 [lindex $line 0]

			if { $word0 eq "Endpoint:" } {
				set end [lindex $line 1]
			}
			set line [parse_passed_clock $stream]
			if {$line ne ""} {
				set fi 0
				set from_inst($fi) [lindex $line 0]
				incr fi

            			gets $stream line
				set from_inst($fi) [lindex $line 0]

				set look_for_arrival 1
			}
		}
	}
	close $stream
	# exec rm $filename
	return $lines
    }
global idcount
set idcount 0
set aaa [read_tmp_report 111 $idcount]
puts "\n\n\n\n"
puts $aaa

