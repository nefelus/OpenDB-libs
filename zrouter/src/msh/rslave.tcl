package provide LMSH 1.0

#       Class: ADSProxy
# Description: RSLAVE protocol proxy
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2006
#
#
itcl::class ADSProxy {
    inherit LRemoteProxy
    # -------------------------------------------------- Public Variables --
    public variable slaveclass ""     ;# the name of the remote class
    public variable init ""           ;# additional initialization
    # -------------------------------------------------- Public Methods ----
    # -- obtain the task status
    public method TaskStatus {} {
	$task Status
    }
    # -- kill the remote process
    public method Kill {} {
	ap_kill
    }

    # -- Explit startup of the remote
    # -- if delay is set, we will wait up to so many millseconds
    public method StartRemote {{delay ""}} {
	if { $task != "" } { 
	    error "remote already started (as task $task)"
	}
	startup
	if { !$async } {
	    $sema Wait $delay
	} else {
	    if { $delay != "" } { 
		warning rslave \
		    "StartRemote: delay ($delay) not supported in async mode"
	    }
	    # Event <<connected>> 1
	    # is posted when the remote slave connects
	}
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable task ""
    private   variable sema ""         ;# connection semaphore
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set sema [LSemaphore $this.sema]
    }
    destructor {
	$sema delete
	cleanup
    }
    # -------------------------------------------------- Private Methods ---
    # -- called by eRSlaveConnected? when the remote slave has connected
    # -- back to the proxy
    private method startup {} {
	set rclass $slaveclass
	if { $rclass == "" } {
	    # -- derive the name of the remote class
	    set rclass [$this info class]
	    regsub "^::" $rclass "" rclass
	    regsub -nocase {proxy$} $rclass Slave rclass
	}
	if { $init != "" } {
	    set cmd "$init;"
	}
	set key $this
	append cmd "ADSSlave::StartSlave $rclass $key"
	set task $this.task
	# -- now create a remote task and instantiate the slave there
	puts "spawn: $task $cmd"
	spawn $task $cmd
    }
    # -- cleanup your act
    private method cleanup {} {
	if { $task == "" } { return }
	$task delete
	set task ""
    }
    # ----------------------------------------------- Interface Implementation
    # -- kill the remote proxy
    protected method ap_kill {} {
	# -- no task has been allocated. No need to do much
	if { $task == "" } { 
	    return
	}
	# -- has the remote slave been connected?
	if { $address != "" } { 
	    Send "Die" <<die>>
	    return
	}
	# -- slave has not been connected yet. Just kill the remote
	# -- task
	$task delete
	set task ""
	# -- FIX ME: a possible reace condition might be here.
	# -- the remote node may attempt to register while it is being
	# -- killed
    }
    # ----------------------------------------------- CallBacks
    # -- called by the remote slave when it starts up
    public method eRSlaveRegistered {address me} {
	puts "remote slave at: $address $me"
	$this configure -address $address
	if { $async } {
	    Event <<connected>> 1
	    return
	}
	$sema Signal
    }
    # -- this is called to check if the remote slave is created & connected
    public method eRSlaveConnected? {} {
	if { $task == "" } { 
	    startup
	    eNotice "waiting for slave to start"
	    $sema Wait
	    eNotice "ready to send command"
	}
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: ADSSlave
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: Manolis M. Tsangaris, Athena Design Systems, 2006
#
itcl::class ADSSlave {
    inherit LRemoteSlave
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Wait { } {
	$sema Wait
    }
    # -- stop waiting
    public method Die {} {
	$sema Signal
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable sema ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set sema [LSemaphore $this.sema]
    }
    destructor {
	$sema delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # ----------------------------------------------- Initialization Proc
    # -- Start slave object, wait, then deallocate when done
    # -- called by the main proxy startup method as part of the initialization
    # -- of the remote slave
    # -- rclass: the class of the remote slave
    # --    key: the name of the local proxy
    # --
    public proc StartSlave {rclass key} {
	set rs [uplevel #0 $rclass ::#auto -address 7752-@]
	# -- register this slave to the main node
	# -- send the "registration command" for this slave to the master
	zroute::master_run \
	    [list after idle [list $key eRSlaveRegistered [$rs Address] $rs]]
	$rs Wait 
	$rs delete
    }
    # -------------------------------------------------- Common Initialization
}
