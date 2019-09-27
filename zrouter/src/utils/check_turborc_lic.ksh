#!/bin/ksh -p

unset ENV

function die {
        echo "ca: $*" 1>&2
        exit 1
}
TOP=$(dirname $(dirname $0))
if [[ $TOP = . ]]
then
        BTOP=$(pwd | awk -F\/ '{print $NF}')
	TOP=$(pwd | awk -F/$BTOP '{print $1}')
fi

ADSTOP=$(grep ads_top $TOP/conf/msh.conf | awk '{print $2}')
ADSLIC=$(grep ads_lic $TOP/conf/msh.conf | awk '{print $2}')
ADSPORT=$(grep ads_port $TOP/conf/msh.conf | awk '{print $2}')

if [[ $ADSLIC = @('""'|''|'#') ]] || [[ $ADSPORT = @('""'|''|'#') ]]
then
        print "TurboRC License have not been configured! Please run configure from install directory"
        exit 1
fi
ISDAEMON=$($TOP/license/bin/lmstat -c $ADSPORT@$ADSLIC | grep UP)
if [[ $ISDAEMON = '' ]]
then
        print "License DAEMON at $ADSPORT@$ADSLIC is down"
        exit 1
fi

FEAT=$($TOP/license/bin/lmstat -a -c $ADSPORT@$ADSLIC | grep turborc)

integer ISTURBORC=$($TOP/license/bin/lmstat -a -c $ADSPORT@$ADSLIC | grep turborc | wc -l)
if [[ $ISTURBORC != 2 ]]
then
        print "License Features for TurboRC do not exist"
        exit 1
fi

print "TurboRC license is correctly installed and running!"
print "	ads_top		$ADSTOP"
print "	ads_lic		$ADSLIC"
print "	ads_port	$ADSPORT"
print ""
print "$FEAT"