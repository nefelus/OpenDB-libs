#catch {exec lsfkill}

if { [file exists ex.db] } {
    pdb load_design -dbname ex.db
} else {
    pdb load_design -lefs /fs/designs/e/e.lef -def /fs/designs/e/e.def
    pdb save_db -dbname ex.db
}

tmg gui_start

set d  /fs/designs/nec2/libs
set d2 /fs/designs/nec_sigmodup/time_LIBS

set parms(ext_rules)  /fs/designs/extLibs/nec90mm.extRules
set parms(maxlibs) [ list \
  $d/CB130M_CMOS_12V_H9_MAX_DRIVE.lib \
  $d/CB130M_CMOS_12V_H9_MAX_SCAN.lib \
  $d/CB130M_CMOS_12V_H9_MAX_GATING.lib \
  $d/CB130M_CMOS_12V_H9_MAX_SPECIAL.lib \
  $d/CB130M_CMOS_12V_H9_MAX_IO.lib \
  $d/CB130M_CMOS_12V_H9_MAX_PRIM.lib \
  $d2/CB130M_CMOS_12V_M9_MAX_TESTACT.lib  \
  $d2/CB130M_CMOS_12V-M_COM_MAX.lib \
  $d2/CB130M_FAKE_MAX.lib \
]

set parms(minlibs) [ list \
  $d/CB130M_CMOS_12V_H9_MIN_DRIVE.lib \
  $d/CB130M_CMOS_12V_H9_MIN_SCAN.lib \
  $d/CB130M_CMOS_12V_H9_MIN_GATING.lib \
  $d/CB130M_CMOS_12V_H9_MIN_SPECIAL.lib \
  $d/CB130M_CMOS_12V_H9_MIN_IO.lib \
  $d/CB130M_CMOS_12V_H9_MIN_PRIM.lib \
  $d2/CB130M_CMOS_12V_M9_MAX_TESTACT.lib  \
  $d2/CB130M_CMOS_12V-M_COM_MAX.lib \
  $d2/CB130M_FAKE_MAX.lib \
]

## enable "disassembly" debug
#pdb e "::zr.zlog enable_debug_log -module ZTM"
#pdb e "::zr.zlog enable_debug_log -module MILOS_TMG_MM"
#pdb e "::zr.zlog enable_debug_log -module MILOS_RMT"


## start multimode setup
tmg init_multimode 

tmg reset_threshold -upper_pct 59 -lower_pct 41 -derate 0.18

tmg define_corner -corner SETUP \
-min_derate 1.0 -max_derate 1.22 \
-lib $parms(maxlibs)

tmg define_corner -corner SETUP2 \
-min_derate 1.0 -max_derate 1.5 \
-lib $parms(maxlibs)

tmg define_corner -corner CLONE \
-min_derate 1.0 -max_derate 1.22 \
-lib $parms(maxlibs)

tmg define_corner -corner HOLD \
-min_derate 0.8 -max_derate 1.7 \
-lib $parms(minlibs)



## this will be the local node (priority 1)
tmg create_scenario -name SETUP -corner SETUP -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules -priority 1

## now define remote/other nodes/corners
tmg create_scenario -name CLONE  -corner CLONE  -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules 

## now define remote/other nodes/corners
tmg create_scenario -name SETUP2  -corner SETUP2  -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules 

## now define remote/other nodes/corners
tmg create_scenario -name HOLD  -corner HOLD  -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules 

## finish multimode setup
tmg apply_multimode -name "dac"


## run some optimization command
opt size -change_limit 50000 -setup_incr 1 -slack_target 0.8

tmg finish_multimode
#exit
