set d /fs/designs/s1
db load_lef -file $d/lef_verify
db load_def -file $d/def
db save_def -file out1.def
db get -top -object= block
db load_def_eco -replacewires -block $block -file out1.def
db save_def -file out2.def
exec diff out1.def out2.def
