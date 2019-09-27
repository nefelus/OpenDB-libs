package provide ade::main 1.0
#
# ZDB technology module TCL layer
#
# Provides all necessary code to support the TCL layer of the
# db technology module.
# 
# Note that this namespace lives inside the zroute namespace, since
# it makes no sense to use it externally.
# As a result, the zdb technology module author should bind the *_cmd
# commands to zroute::zdb:*_cmd

# provides ext

namespace eval zroute::ext  {
# ---------------------------------------------------------- EXPORTED COMMANDS
    namespace export ext_make_model ext_extract ext_write_spef ext_init ext_clean

# ---------------------------------------------------- 
proc init {} {
    # -- define the usage of the inspect command
    namespace import [namespace parent]::args::*

    # define the usage of calls

    usage ext_init {
        -all flag "Delete all extract objects" "0"
        -model_only ext-flag "Delete only recent extract object" "1"
    }
    usage ext_clean {
        -all flag "Delete all extract objects" "1"
        -ext_only ext-flag "Delete only recent extract object" "0"
    }
    usage ext_make_model {
        -name       model-name      "model name for a process corner" "TYP"
        -from_db    lef-model       "read values from DB" "1"
        -file       process-file    "process parameters" ""
    }
    usage ext_extract {
        -name   model-name      "model name for a process corner" "TYP"
        -net    net-list        "list of net Ids to extract" "ALL"
    }
    usage ext_write_spef {
        -bbox   "x0 y0 x1 y1" "Area to extract"   ""
        -net    net-list    "list of net Ids to extract" "ALL"
        -file   filename    "output SPEF file" ""
    }
}

# ---------------------------------------------------- 
proc ext_init {args} {
    parse ext_init  $args C
    ext_init_cmd C
}
proc ext_make_model {args} {
    parse ext_make_model $args C
    ext_make_model_cmd C
}
proc ext_extract {args} {
    parse ext_extract $args C
    ext_extract_cmd C
}
proc ext_write_spef {args} {
    parse ext_write_spef $args C
    ext_write_spef_cmd C
}
proc ext_clean {args} {
    parse ext_clean $args C
    ext_clean_cmd C
}
# -------------------------------------------------------------- CLI related
}

