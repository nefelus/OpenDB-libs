::sys::enableMemCount 1 5

puts "Running [info script] on [exec hostname]"

db rlog

set data_dir /home/centos/Resizer/test/

db load_lef -file $data_dir/liberty1.lef
db rlog -tag "LoadLef"

db load_def -file $data_dir/reg1.def
db rlog -tag "LoadDef"

db write -file bus1.db
db rlog -tag "WriteDb"

db save_lef -file out.LEF
db rlog -tag "SaveLEF"

db save_def -file out.DEF
db rlog -tag "SaveDEF"

