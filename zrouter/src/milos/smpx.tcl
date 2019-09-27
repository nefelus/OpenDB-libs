## smpx.tcl
## Multiplexing module for Milos
## (c) 2007 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::smpx 0.1

itcl::class SMPXClient {

    inherit LSMPXClient;

    constructor { {address 7679@localhost} } {
        LSMPXClient::constructor -address $address
    } {
        #puts "SMPXClient constructor: $args"
        #puts "Before: [$this configure]"
        #eval configure $args
        puts "After: [$this configure]"
    }

    public method register { type addr } {
        _init
        if [catch {d Register $type $addr} rc] {
            error "Error registering plugin type $type at address $addr: $rc"
        }
        notice "Successfully registered plugin type $type at address $addr!"
    }

    public method reserve { type } {
        _init
        d Reserve $type
        notice "Reserving type $type"
        Wait S; 
        notice "Got token for type $type"
        parray S
        notice "Returning: $S(address)"
        return $S(address)
    }

    public method release { type addr } {
        d UnRegister $type $addr
    }

    protected method _init { } {
    }
    protected method d { args } {
        debug SMPX:A "Command: $args"
        set rc [uplevel $args]
        debug SMPX:A "Return: $rc"
        set rc
    }
}
