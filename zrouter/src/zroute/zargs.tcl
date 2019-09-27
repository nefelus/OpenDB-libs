package provide ade::main 1.0

namespace eval zroute { }

namespace eval zroute::args {

#
# Process and normalize arguments and options
#
variable USAGE
variable DOC

namespace export usage parse

# -- define the usage (and documentation) of a function
proc usage {function usage {doc ""}} {
	variable USAGE
	variable DOC
	args_norm $usage S
	set USAGE($function) [array get S]
	man $function $doc
}

#
# -- parse the command line arguments of a function, based on
# -- the usage previously defined
# -- return the parsed arguments into the array (last argument)
# -- return : empty string if ok, error message if a problem
# -- errors:  if function usage is not defined

proc parse {function argv pp} {
	upvar $pp P
	variable USAGE
	
	if { ![info exists USAGE($function)] } {
		error "zargs: $function: no usage defined"
	}
	if { ![args_parse $USAGE($function) $argv P] } {
		error "$P(error)\n[usage_print $function]"
	}
}


#
# --------------------------------------------------------------------
#
# argument usage is provided using the syntax below, as depicted in the examples
#
# -tiles ntiles:int "Number of Tiles to generate"        "none:0"
# opt   argument & type  explanation                    default (name & value)
#
# tiles ntiles:int "Number of Tiles to generate"        "none:0"
# this one indicates that arg 1 (not option) should be bound to tiles

proc args_norm {argv ss} {
	upvar $ss S
	set S(options) ""
	set S(args) ""
	foreach line [split $argv \n] {
		set line [string trim $line]
		if { $line == "" } { continue }
		foreach {opt arg info default} $line { break }
		foreach {argname argtype} [split $arg :] { break }
		if { ![string match *:* $default] } {
			set defname $default
		}  else {
			foreach {defname default}  [split $default :] { break }
		}

		# is it an option or an argument?
		switch -glob -- $opt {
			-*      { 
				set opt [string range $opt 1 end]
				lappend S(options) $opt
			}
			default { lappend S(args) $opt }
		}
		set S($opt) [list arg $arg argname $argname argtype $argtype info $info\
				default $default defname $defname]
		lappend S(defaults) $opt $default
	}
}

proc args_parse {usage argv pp} {
	upvar $pp P
	array set S $usage
	array set P $S(defaults)

	while { $argv != "" } {
		set opt  [lindex $argv 0]
		set arg  [lindex $argv 1]
		if ![string match -* $opt] { break }
		set opt [string range $opt 1 end]
		if ![info exists S($opt)] {
			set P(error) "unknown option -$opt"
			return 0
		}
		set P($opt) $arg
		set argv [lrange $argv 2 end]
	}
	foreach p $S(args) {
		if {$p == "*"} {
			set P($p)  $argv 
			set argv ""
		} else {
			set P($p) [lindex $argv 0]
			set argv [lrange $argv 1 end]
		}
	}
	return 1
}

proc usage_print {fn} {
	variable USAGE
	array set S $USAGE($fn)
	set out ""
	append out "$fn: usage: \[options\] $S(args)\n"
	append out "arguments:\n"
	foreach opt $S(args) {
		array set O $S($opt)
		append out [format "%10s %10s: %-30s (%s)\n" \
				$opt $O(arg) $O(info) $O(default)]
		unset O
	}
	append out "options:\n"
	foreach opt [lsort $S(options)] {
		array set O $S($opt)
		append out [format "%10s %10s: %-30s (%s)\n" \
				-$opt $O(arg) $O(info) $O(default)]
		unset O
	}
	set out
}

proc args_example {{argv ""}} {
	if { $argv == "" } {
		set argv "-tiles 10 -loops 5 chip0"
	}
	usage example {
		chip   chip       "chip to process"        ""
		-tiles ntiles:int "Number of Tiles to generate"        "none:0"
		-loops nloops:int "Number of optimization loops to do" "10"
	}
	if {![parse example $argv P ]} {
		error "$P(error)"
		return
	}
	parray P
}
}

