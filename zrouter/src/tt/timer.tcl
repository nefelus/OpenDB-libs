package require Itcl

if { [info var _Timer_true] != "" } {
    return
}
set _Timer_true 1

itcl::class Timer {
    private variable start 0
    private variable stop  0
    private variable difftime 0

    public method start { } {
        set start [clock clicks -milliseconds]
        set stop 0
    }

    public method stop { } {
        set stop [clock clicks -milliseconds]
        set difftime [expr $stop - $start]
    }

    public method format_time { difftime } {
        set tmp [expr $difftime / 10]
        set exectime [expr $tmp / 100]
        set tmp [expr ($tmp - $exectime * 100)]
        set frac [expr round($tmp/10.0)]
        if { $frac == "10" } { 
            incr exectime
            set frac "0"
        }
        return  "[clock format $exectime -format "%T" -gmt 1].$frac"
    }

    public method get_difftime { } {
        return $difftime
    }

    public method get_time { } {
        return [format_time $difftime]
    }
}

