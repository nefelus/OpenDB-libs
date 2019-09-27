## ade script to get options
set zr_opts {}
lappend zr_opts [info command sta]

## Add other options here


## Stop adding options here
## Do not modify last line
if { [llength $zr_opts] == 0 } {
    puts "none"
} else {
    puts $zr_opts
}
exit 0
