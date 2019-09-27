package provide MSH 1.0

#
#       Class: VRCSlave
# Description: Manage a remote slave
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
# This object corresponds to a remote slave
#
itcl::class VRCSlave {
    inherit LObject LBinder
    # Event <<free>>         ;# -- when the slave is free
    # Event <<idle-timeout>> ;# time to cleanup this slave
    # -------------------------------------------------- Public Variables --
    public variable address ""    ;# where to connect to
    public variable fp ""         ;# connection to the remote server
    public variable task "."      ;# task being run
    public variable host ""       ;# which host it runs on
    public variable idletimeout -1 ;# slave idle timeout
    public variable tag  "slave"  ;# slave name used in debugging
    # -------------------------------------------------- Public Methods ----
    public method Status {} {
        array set S [$peer Run state]
        set addr $address
        if { [string length $addr] > 20 } {
            set addr [lrange $address 0 19]
        }
        set h [string range $host 0 8]
        format "%20s %5s (%8s) %8s %9s"  $addr $S(state) $TID $S(pid) $h
    }
    public method Disconnected {} {
        dinfo "slave $address died"
    }
    public method Run  {_task} {
        configure -task $_task
        set TID [$task cget -tid]
        notice "starting task $TID: [$task Info]"
	$ITASK Cancel
        $peer Run start \
            [$task cget -cmd] $TID [$task cget -stdin] [$task cget -env] \
	    [$task cget -timeout]
        $task configure -host $host
        $task NewState DISP
    }
    public method TaskDone {out} {
        if { $task == "." } { return }

        $task NewState DONE
        # record the task output
        $task configure -output $out

	if { $idletimeout > 0 } {
	    $ITASK configure -timeout [expr $idletimeout*1000]
	    $ITASK Start
	}
    }
    # -- called by smgr when a keepalive is received
    public method IsAlive {fp} {
	set now [clock format [clock seconds] -format %H:%M]
	notice "$fp: $address: alive at $now"
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable peer
    protected variable TID "-"
    protected variable realhost              ;# the real host address
    protected variable ITASK                 ;# idle timeout detection
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

	set ITASK [LSTask $this.itask -cmd "$this eTimeout" -start 0]

        # -- NOTE: because of broken DNS, we will use the peer
        # -- address returned to us by the peername of the registration socket
        # -- we will ignore "address" or "host"
        # --
        set realhost [lindex [fconfigure $fp -peername] 0]
        set port [lindex [split $address @] 0]
        set address "$port@$realhost"

        set peer [LRPCClient $this.peer]
        notice "--------- "
        notice "managing slave on $realhost"
        $peer Connect $address
        if [peer_test] {
	    $peer Bind <<free>> $this Event <<free>>
	}
    }
    destructor {
	$ITASK delete
        dinfo "shutting down (on $host)" 
        $peer delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method peer_test {} {
	# -- we will not do a peer test, since it confuses idle detection
	# -- this better be done from the parent object
        # set rc [$peer Run test]
        # dinfo "link-test: $rc"
        # $peer Run start "date"
        # $peer Run status
	return 1
    }
    # -------------------------------------------------- debug
    protected method dinfo {msg} {
        LLog vslave info "$tag: $msg"
    }
    protected method notice {msg} {
        LLog vslave notice "$tag: $msg"
    }
    protected method warning {msg} {
        LLog vslave warning "$tag: $msg"
    }
    # ----------------------------------------------- Interface Implementation
    public method eTimeout {} {
	Event <<idle-timeout>>
    }
    # -------------------------------------------------- Common Initialization
}
