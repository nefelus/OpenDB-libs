package provide LMSH 1.0

#
#       Class: MSHC
# Description: Client side access to MSH
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2004
#
itcl::class MSHC {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable cmsh "msh -info -w"          ;# the msh command
    public variable options ""                   ;# additional options msh/mds
    # -------------------------------------------------- Public Methods ----
    # spawn a remote command, create a "local" proxy
    public method Spawn {cmd {local ""} {stdin ""} argv} {
        set rj [$mrc Run $cmd $stdin "" $argv]
        $peer MakeProxy $rj $local
    }
    # -- status commands:
    public method Jobs {} {
        if { $DISABLED } { return "" }
        return all
    }
    
    public method Tasks {} {
        if { $DISABLED } { return "" }
        $mrc Tasks
    }

    public method JobTasks {j} {
        if { $DISABLED } { return "" }
        $mrc Tasks
    }
    
    public method TaskStatus {tasks} {
        if { $DISABLED } { return "state NONE" }
        $mrc TaskStatus $tasks
    }
    public method TasksStatus {tasks} {
        if { $DISABLED } { return "" }
        $mrc TasksStatus $tasks
    }
    
    public method JobStatus {j} {
        if { $DISABLED } { return "state NONE" }
        list state running
    }

    public method Info {} {
        if { $DISABLED } { return "" }
        $mrc Info
    }

    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable peer "" ;# rpc connection to the remote server
    protected variable mrc ""     ;# proxy of the remote VRC object
    protected variable address "" ;# the peer address
    protected variable MSH        ;# msh attributes
    protected variable DISABLED 0 ;# true if we do not run on unix 
    protected variable mshmonitor    "" ;#
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        msh_setup $cmsh
    }
    destructor {
        # of course we do not delete $mrc
        if { $peer != "" } {
            $peer Run msh quit
            $peer delete
        }
        if { $mshmonitor != "" } {
            $mshmonitor delete
        }
    }
    # -------------------------------------------------- Private Methods ---
    protected method msh_setup {cmsh} {
        set mshmonitor [LProcMonitor #auto -wait 1]

        # -- start msh and wait for it to get its attributes

        array set OPT $options
        set cmd $cmsh
	# -- exclude profile for now
	# set optionmap {rje -rje dmsh -d dmds -dm mprofile -p} 
	set optionmap {rje -rje dmsh -d dmds -dm}
	foreach {opt flag} $optionmap {
	    if { $OPT($opt) != "" } { 
		append cmd " $flag $OPT($opt)"
	    }
	}
        append cmd  " -cp $OPT(cpath)"
	# -- make sure a default logdir is passed
	# -- append msh subdirectory to it
	if [info exists OPT(logdir)] {
	    append cmd " -l $OPT(logdir)/msh"
	}

        # -- pass multiprocessing level to msh
        if [info exists OPT(nproc)] {
            append cmd " -N $OPT(nproc)"
        }
        # -- additional options (will override previous ones)
	if { $OPT(mshopts) != "" } { 
	    append cmd " $OPT(mshopts)"
	}

	if { $OPT(mdsopts) != "" } { 
	    append cmd " -mds \"$OPT(mdsopts)\""
	}

        set msh [$mshmonitor Start $cmd]
        array set MSH [$mshmonitor Info $msh]
        set DISABLED  [$mshmonitor cget -disabled]

	# -- if for some reason it failed to start ...
        if { $DISABLED }  { return }

        set peer [LRPCClient $this.peer]

        debug MSH:A mshc "Connecting to $MSH(msh)"

        if [catch {$peer Connect $MSH(msh)} err]  {
            error "msh: failed to connect to $MSH(msh) : $err"
        }
        if  { [$peer Run msh echo x] != "x" } {
            warning mshc "remote msh failed loopback test"
        }
        # create a proxy of the MRC
        set mrc $this.mmrc
        $peer MakeProxy $MSH(mrc) $mrc
        
        # pass the MSH & MDS address through the environment
        global env
        set env(MSH) $MSH(msh)
        set env(MDS) $MSH(mds)
	if [info exists MSH(smpx)] {
	    set env(SMPX) $MSH(smpx)
	}

        # pass the environment to msh
        set env(CWD) [pwd]
        eval $mrc [list configure -env [array get env]]

        $mshmonitor Bind <<done>> $this eMSHDied
    }
    public method eMSHDied {fp} {
        notice msh  "server $fp died"
        $mshmonitor delete
        set mshmonitor ""
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
