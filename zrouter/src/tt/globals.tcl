## Testing Tool module - globals.tcl
## (c) Athena Design Systems, 2005
### Author: Mattias Hembruch
#
# Global variables used by the program

if { [info var _Globals_true] != "" } {
    return
}

set _Globals_true 1

set top "../../test"
set zargs ""
set rcglobal "~/.ttrc"
set rclocal "./.ttrc"

set prompt "> "
set find "/usr/bin/find"
set groupfile "groups.dat"
set testfile "test.exe"
#set testfile "test_run.tcl"
set difffile "test.diff"
set cleanfile "test_clean.tcl"
set attrfile "test.attr"
set fdescr "::Description::"
set fplatform "::Platform::"
set fruntime "::Runtime::"
set foption "::Option::"
set fzargs "::Zargs::"
set testlist {}
set gtestlist(0,0) ""
set groups {}
set ctype "exec"
set makefile "Makefile"
set makeinc "Makefile.rules"

## Options
set show_only 0
set summary 1
set verbose 0
set autocreate 0

set cmdsep "_"

set zp1 "#!/bin/bash --noprofile\n##Automatically generated test file\n##Do not Edit\n"
set zp2 "Z=`which ade 2> /dev/null`"
#set zp3 "ZR=\$\{Z:-\$ZR}\n\n"
set zp3 ""
set zp4 "if \[\[ -f \$Z ]]\nthen"
set zprefix "$zp1\n$zp2\n$zp3$zp4"
set zcmd "    \$\{Z} \$\{ZARGS\} << 'EOF'\n    catch \{"
set zps0 "    \} _testoutput_\n    puts \$_testoutput_\n"
set zps1 "    exit 0\n"
set zps2 "EOF\n\nelse"
set zps3 {    echo "ade not found in current PATH"}
set zps4 "    exit 1\nfi"
set zpostfix "$zps0$zps1\n$zps2\n$zps3\n$zps4"

set ttcmds {clean run diff show new upcvs upgold }
set ttdesc {"cleaned" "executed only" "diffed" "showed" "created" "updated cvs" "updated gold"}

## Command targets for make
set makeupgold "make upgold"
set makeupcvs "make upcvs"
set makerun "make test.log"
set makeclean "make cleantest"
set makediff "make cleandiff test.diff"

## Group descriptions
set tgdescr(athena) "Tests internal to Athena"
set tgdescr(toshiba) "Tests required to ship to Toshiba"
set tgdescr(sanity) "Sanity Tests - make sure it works"
set tgdescr(basic) "Basic Tests - a little more involved than sanity"
set tgdescr(extreme) "Extreme tests - long-running, stress tests"
set tgdescr(installation) "Tests shipped for testing of a ade install"

