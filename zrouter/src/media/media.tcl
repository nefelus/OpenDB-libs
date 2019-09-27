package provide ade::media 1.0

proc ade_media {file} {
    global auto_path
    foreach p $auto_path {
	set d [file join $p media]
	if ![file isdirectory $d] { continue }
	set f [file join $d $file]
	if [file exists $f] {
	    return $f
	}
    }
    error "$file: media file not found"
}
