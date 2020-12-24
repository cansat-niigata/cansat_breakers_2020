#!/bin/bash

#これをつかうとwsl1上からpowershellを介して名前解決できます

CMDNAME=`basename $0`
FLAG_U=0


while getopts u:h OPT
do
	case $OPT in
		u ) FLAG_U=1 ; VALUE_UNAME=$OPTARG ;;
		h ) echo "Usage: $CMDNAME [-u username] hostname" 1>&2
		exit 1 ;;
	esac
done

shift $(($OPTIND - 1))
VALUE_HNAME=$1

if [ -z $VALUE_HNAME ]; then
	echo "Usage: $CMDNAME [-u username] hostname" 1>&2
	exit 1
fi

if [ $FLAG_U = 1 ]; then
	TMP=`powershell.exe "Resolve-DnsName $VALUE_HNAME | ConvertTo-Json" | jq -r '.[1].IP4Address'`
	ssh $VALUE_UNAME@$TMP
else
	powershell.exe "Resolve-DnsName $VALUE_HNAME | ConvertTo-Json" | jq -r '.[1].IP4Address'
fi
