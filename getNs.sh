#!/bin/bash

#改行コードに注意!
#通常WSL1からは名前解決できないけどこれを使えばsshとUSBシリアル通信がWSL上で完結できるゾ！
#ついでに背景を透過できて便利

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
	TMP=`powershell.exe "Resolve-DnsName $VALUE_HNAME | ConvertTo-Json" | jq -r '.[0].IPAddress'`
	ssh $VALUE_UNAME@$TMP
else
	powershell.exe "Resolve-DnsName $VALUE_HNAME | ConvertTo-Json" | jq -r '.[0].IPAddress'
fi
