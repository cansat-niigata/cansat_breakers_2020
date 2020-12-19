#!/bin/bash



CMDNAME=`basename $0`

while getopts u:h: OPT
do
	case $OPT in
		"u" ) FLAG_U=1 ; VALUE_UNAME=$OPTARG ;;
		"h" ) VALUE_HNAME=$OPTARG ;;
		* ) echo "Usage: $CMDNAME [-u username] [-h hostname]" 1>&2
		    exit 1 ;;
	esac
done

if [ $FLAG_U = 1 ]; then
	TMP=`powershell.exe "Resolve-DnsName $VALUE_HNAME | ConvertTo-Json" | jq -r '.[1].IP4Address'`
	ssh $VALUE_UNAME@$TMP
else
	powershell.exe "Resolve-DnsName $VALUE_HNAME | ConvertTo-Json" | jq -r '.[1].IP4Address'
fi
