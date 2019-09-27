set MODULES { sys pdb rpt rte tmg opt mme ext }

foreach cmd $MODULES {
    catch { $cmd } output
    puts $output
    puts ""
}

foreach cmd $MODULES {
    $cmd help -long
    puts ""
}

exec cp milos.log cmd_sum.log
exit
