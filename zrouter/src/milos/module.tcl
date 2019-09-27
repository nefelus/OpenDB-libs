## module.tcl
## Empty module used to build new Milos modules
## (c) 2006 Athena Design Systems
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::module 0.1

itcl::class MODULE {

    inherit VOB

    ## Public Vars
    ##

    ## End Public Vars

    ## Protected Vars
    ##

    ## End Protected Vars

    protected method constructor { parent } { 
        VOB::constructor $parent
        set NAME "MODULE"
        set SELFNAME "MODULE$this"
        ## ONLY for public vars
        set STRINGVARS { }
        set NUMVARS  { }
        set_state module init
    }

    protected method _itcl_config { option args } @itcl-builtin-configure


    public method example { args } {
        ::eval _itcl_config $args
    }

    protected method vob_settings { } {
        set var [chain]
        ## handle local non-public vars, if any
        ## append var "Other vars: "
        return $var

    }

    ## used to "close the hole"
    protected method info { } 
}
