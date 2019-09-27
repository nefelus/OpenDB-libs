
set d /fs/designs/s1

puts "loading lef"
db load_lef -file $d/lef_verify

puts "loading def"
db load_def -file $d/def

puts "saving db"
db write -file s1.db

puts "reading db"
db read -file s1.db

puts "saving lef"
db save_lef -file out1.lef

puts "saving def"
db save_def -file out1.def

db clear
puts "loading lef"
db load_lef -file out1.lef

puts "loading def"
db load_def -file out1.def

puts "saving db"
db write -file s2.db

puts "reading db"
db read -file s1.db

puts "saving lef"
db save_lef -file out2.lef

puts "saving def"
db save_def -file out2.def

puts "diffing lef"
exec diff out1.lef out2.lef

puts "diffing def"
exec diff out1.def out2.def
