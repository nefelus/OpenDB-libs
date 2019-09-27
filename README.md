# OpenDB-libs

This repository is a header only pre-built version of OpenDB, the OpenROAD physical database.
This repo will be obsolete when OpenDB is opensourced.
The plan is to open source OpenDB under a BSD-3 license.
This prebuilt version is temporarily licensed as GPL as indicated in the license file.

To start, source the script set_ade_envi.bash after modifying it to correctly point to the directory holding the clone of OpenDB.
Then type ade to enter the OpenDB tcl shell. Be sure that running the script is done to affect your current shell and not a short lived subshell.

Eve though the .a's are prebuilt, you must "build" locally after cloning. This is the same build procedure that will be used when all of the code is open sourced.
cd zrouter/src; make clean; make; make install




