package provide ade::ui 1.0

#
#       Class: ZMGR
# Description: ZUI controller
#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: ATHENA DESIGN SYSTEMS, 2004
#
# Manage the ZUI/ZCMD interactions
itcl::class ZMGR {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable loader ""        ; # the cad object loader
    public variable rpc    ""        ; # remote server to connect to
    public variable opts    ""  {     ; # various parameters from the argv
            array set OPT $opts
    }     
    # -------------------------------------------------- Public Methods ----
    public method Run {cmd} {
        eval eUICmd $cmd
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable zcmd ""
    protected variable zui  ""
    protected variable zconsole  ""
    protected variable OPT 
    protected variable gcmd ""              ;# gui language interpeter
    protected variable zc ""                ;# zui context
    protected variable zhist ""             ;# zui history
    protected variable zhelp ""             ;# zui help
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        set zhist [ZHistory $this.zhist]
        set zc [ZContext $this.zc]

        set zconsole [ZConsole .zcon#auto -rpc $rpc]
        $zconsole configure -geometry -0-50
        $zconsole Hide

        set zhelp [namespace code [ZHelp .zhelp#auto -title ZUI:Help]]
        $zhelp Hide

        set zui  [namespace eval :: [list  \
            LZUI .zui#auto -rpc $rpc -opts $opts] \
        ]

        $zui Bind <<show-console>> [namespace code $zconsole] UnHide

        set zcmd [namespace eval :: [list \
            ZCMD $this.cmd -loader $loader \
                -zhist $zhist -rpc $rpc -opts $opts -zc $zc -zhelp $zhelp] \
        ]

        set gcmd [GTCL $this.gcmd]

        $loader Bind <<info>>     $zui InfoShow
        $loader Bind <<progress>> $zui InfoShow

        $zui    Bind <<lcmd>>     $gcmd Run 
        $zui    Bind <<quit>>     $this Signal 0
        $zui    Bind <<command>>  $this eUICmd 


        # -- GCMD bindings
        # pass all commands to zcmd
        $gcmd Bind <<command>> $zcmd
        # process results locally (and eventually pass them to viz)
        $gcmd Bind <<result>>  $zui Viz

        # bind cmd result events to zui viz
        $zcmd Bind <<result>> $zui Viz
    }
    destructor {
        $zui  delete
        $zcmd delete
        $zconsole delete
        $gcmd delete
        $zc delete
        $zhist delete
        $zhelp delete
    }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- CallBacks
    # interpet a UI command
    public method eUICmd {cmd args} {
        switch $cmd {
            quit { Signal 0 }
            default {
                eval $zcmd $cmd $args
            }
        }
    }
    # -------------------------------------------------- Common Initialization
}
