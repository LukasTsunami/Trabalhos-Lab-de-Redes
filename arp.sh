#!/bin/sh
COUNTER=1
NUMBER_OF_IP_ADDRESSES_IN_A_SUBNETWORK=254
MY_IP_ADDRESS_P1=`ifconfig eth0 | head -2 | tail -1 | awk '{printf $2}' | cut -d. -f1`
MY_IP_ADDRESS_P2=`ifconfig eth0 | head -2 | tail -1 | awk '{printf $2}' | cut -d. -f2`
MY_IP_ADDRESS_P3=`ifconfig eth0 | head -2 | tail -1 | awk '{printf $2}' | cut -d. -f3`
MY_IP_ADDRESS_P4=`ifconfig eth0 | head -2 | tail -1 | awk '{printf $2}' | cut -d. -f4`
MY_ETHERNET_MAC_ADDRESS=`ifconfig eth0 | head -3 | tail -1 | awk '{print $2}'`
export COLLUMN_DELIMITER="\t\t"
export SPECIAL_CHARACTER_TO_DESTACATE_MY_ADDRESS="*"

insertMyAddressInArpTable(){
	local MY_MAC_ADDRESS____=$5
	local MY_IP_ADDRESS____=$1.$2.$3.$4
	local IP_TO_SHOW="$MY_IP_ADDRESS____$SPECIAL_CHARACTER_TO_DESTACATE_MY_ADDRESS"
	local MAC_TO_SHOW=$MY_MAC_ADDRESS____$SPECIAL_CHARACTER_TO_DESTACATE_MY_ADDRESS
	echo "$IP_TO_SHOW $COLLUMN_DELIMITER $MAC_TO_SHOW"
}

mountMyArpTableCopy(){
	local IP_ADDRESS=$1
	local MAC_ADDRESS=$2
	echo "$IP_ADDRESS $COLLUMN_DELIMITER $MAC_ADDRESS"
}

drawHeader(){
	echo "-------------------------------------------------------------------"
	echo "Trabalho de Lab de Redes: Montando minha tabela ARP"
	echo "Nome: Lucas Caponi da Silva"
	echo "OBS: O endereco com $SPECIAL_CHARACTER_TO_DESTACATE_MY_ADDRESS eh o da minha maquina"
	echo "-------------------------------------------------------------------"
	echo "Endereco logico \t Endereco fisico"
	echo "-------------------------------------------------------------------"
}

returnPingAnswer(){
	local COUNTER=$5
	local MY_IP_ADDRESS_P1=$1
	local MY_IP_ADDRESS_P2=$2
	local MY_IP_ADDRESS_P3=$3
	local MY_IP_ADDRESS_P4=$4
	echo `ping -c 1 $MY_IP_ADDRESS_P1'.'$MY_IP_ADDRESS_P2'.'$MY_IP_ADDRESS_P3'.'$COUNTER | grep '1 received'`
}

echo "$(drawHeader)"

while [ $COUNTER -le $NUMBER_OF_IP_ADDRESSES_IN_A_SUBNETWORK ]
do
	if [ $COUNTER -eq $MY_IP_ADDRESS_P4 ]; then
		echo "$(insertMyAddressInArpTable $MY_IP_ADDRESS_P1 $MY_IP_ADDRESS_P2 $MY_IP_ADDRESS_P3 $MY_IP_ADDRESS_P4 $MY_ETHERNET_MAC_ADDRESS)"
	else
		RECEIVED_PING_ANSWER=$(returnPingAnswer $MY_IP_ADDRESS_P1 $MY_IP_ADDRESS_P2 $MY_IP_ADDRESS_P3 $MY_IP_ADDRESS_P4 $COUNTER)
		if [ "$RECEIVED_PING_ANSWER" != "" ]; then
			IP_AT_ARP_TABLE=`arp -na | grep -w $MY_IP_ADDRESS_P1.$MY_IP_ADDRESS_P2.$MY_IP_ADDRESS_P3.$COUNTER | grep -Eo [0-9]+'\.'[0-9]+'\.'[0-9]+'\.'[0-9]+`
			ETHERNET_MAC_ADDRESS_AT_ARP_TABLE=`arp -na | grep -w $MY_IP_ADDRESS_P1.$MY_IP_ADDRESS_P2.$MY_IP_ADDRESS_P3.$COUNTER | grep -Eo [a-f]?[0-9]*[a-f]?:[a-f]?[0-9]*[a-f]?:[a-f]?[0-9]*[a-f]?:[a-f]?[0-9]*[a-f]?:[a-f]?[0-9]*[a-f]?:[a-f]?[0-9]*[a-f]?`
			echo "$(mountMyArpTableCopy $IP_AT_ARP_TABLE $ETHERNET_MAC_ADDRESS_AT_ARP_TABLE)"
		fi
	fi
		
	COUNTER=$(($COUNTER + 1))
done
