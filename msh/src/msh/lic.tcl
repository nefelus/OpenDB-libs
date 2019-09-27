package provide mshlic 1.0

#
# License Support
#

#       Class: ADELicense
# Description: ADE specific licensing policy
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2005
#
itcl::class ADELicense {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable envlicense "LM_LICENSE_FILE:ADS_LICENSE"
						;# license server env
    public variable server      ""              ;# license server
    public variable VERSION       1.0           ;# license file version
    public variable DEFAULT_SERVER 6150@tatu1
    # -------------------------------------------------- Public Methods ----
    public method Init {rr} {
	upvar $rr R

	set rc [lmgr_init R]

	if {$NOLICENSE} {
	    return 0
	}

	if { $rc != 0 } { return $rc }
	if { [$lmgr Features] == "" } { 
	    set R(msg) "license server '$server' has no features"
	    return 1
	}
	return 0
    }
    public method Checkout {feature rr} {
	upvar $rr R
	lmgr_checkout $feature R
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable lmgr ""
    protected variable LICENSE_MAP ""           ;# license mapping
    protected variable NOLICENSE 0              ;# operate with no License MGR?
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	map_licenses
    }
    destructor {
	if { $lmgr != "" } { $lmgr delete }
    }
    # -------------------------------------------------- Private Methods ---
    protected method lmgr_init {rr} {
        upvar $rr R
        if { $server == "" } {
            global env
            foreach var [split $envlicense :] {
                if [info exists env($var)] { break }
            }
            if ![info exists env($var)] {
                if { $DEFAULT_SERVER != "" } {
                    set env($var) $DEFAULT_SERVER
                    puts stderr "No license server specified"
                    puts stderr "Using $DEFAULT_SERVER (Athena Design only)"
                } else {
                    set R(msg) "no license set or env variable ($envlicense)"
                    return 1
                }
            }
            set server $env($var)
        }
	if { $server == "ADS_2006" } {
	    set R(msg) "License server not needed"
	    set NOLICENSE 1
	    return 0
	}
        set lmgr [LLicenseManagerFlexLM $this.lm]
        $lmgr Init $server R
    }

    # ----------------------------------------------- Interface Implementation
    protected method lmgr_checkout {feature rr} {
	upvar $rr R
	if { $NOLICENSE } {
	    set R(msg) "no need to checkout"
	    return 0
	}
	foreach {feat lfeat type} $LICENSE_MAP {
	    if { $feat == $feature} {
		return [lmgr_checkout1 $feature $lfeat $type R]
	    }
	}
	set R(msg) "checkout: $feature: unsupported feature"
	return 1
    }
    protected method lmgr_checkout1 {feature lfeat type rr} {
	upvar $rr R
	switch $type {
	    count {
		set rc [$lmgr Acquire $lfeat $VERSION]
		if { $rc < 0 } {
		    set R(msg) "checkout $feature:$lfeat: no more licenses"
		    return 2
		}
		set R(msg) "checkout $feature: checked out ok"
		set R(lid) $rc
		return 0
	    }
	    exists {
		set r [$lmgr FeatureInfo $lfeat]
		if { $r == "" } {
		    set R(msg) "checkout $feature:$lfeat: feature not available"
		    return 1
		}
		set R(msg) "checkout $feature: checked out ok"
		return 0
	    }
	    param {
		# -- scan all licenses for the parameter, specified in lfeat
		foreach feat [$lmgr Features] {
		    if { [scan $lfeat param] == 1 } {
			set R(param) $param
			set R(msg) "checkout $feature: checked out ok"
			return 0
		    }
		}
		set R(msg) "checkout $feature: feature not available"
		return 2
	    }
	    value {
		# -- the parameter is equal to the license count
		set r [$lmgr FeatureInfo $lfeat]
		if { $r == "" } {
		    set R(msg) "checkout $feature:$lfeat: feature not available"
		    return 1
		}
		set F(max) 0
		array set F $r
		set R(param) $F(max)
		set R(msg) "checkout $feature: checked out ok ($F(max))"
		return 0
	    }
	}
	set R(msg) "checkout $feature: $type: illegal type"
	return 2
    }

    protected method map_licenses {} {
	set LICENSE_MAP {
            ade.msh    ADE_Tools:Milos_Tools:TurboRC_Tools  exists
            ade.mds    ADE_Tools:Milos_Tools:TurboRC_Tools  exists
            ade.msh.mpl ADE_Multi_Processing:Milos_Multi_Processing:TurboRC_Multi_Processing  value
	}
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
