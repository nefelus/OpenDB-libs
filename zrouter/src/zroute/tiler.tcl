package provide ade::main 1.0

# TILER technology module TCL layer
#
# Provides all necessary code to support the TCL layer of the
# tiler technology module.
# 
# Note that this namespace lives inside the zroute namespace, since
# it makes no sense to use it externally.
# As a result, the tiler technology module author should bind the *_cmd
# commands to zroute::tiler:*_cmd

namespace eval zroute::tiler  {
# ---------------------------------------------------------- EXPORTED COMMANDS
    namespace export tiles_make tracks_assign pins_assign buses_make
    namespace export tile_maker
# ---------------------------------------------------- 
proc init {} {
    # -- define the usage of the inspect command
    namespace import [namespace parent]::args::*
}
# ---------------------------------------------------- 

proc tile_maker {chip size metH metV} {
    tiles_make $chip $size 0
    tracks_assign block $chip metH $metH metV $metV
    pins_assign block $chip metH $metH metV $metV
    buses_make block $chip
}

# proc tiles_make {chip x y}
# tiles_make_cmd $chip $x $y
proc tiles_make {args} {
    eval tiles_make_cmd $args
}

proc tracks_assign {args} {
    eval tracks_assign_cmd $args
}

proc pins_assign {args} {
    eval pins_assign_cmd $args
}

proc buses_make {args} {
    eval buses_make_cmd $args
}

# -------------------------------------------------------------- CLI related
}
