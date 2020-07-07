#! /bin/bash


FILE="main.c"
CURRENT_DIR="${1}"

n=1
while read -r l1 l2 l3 && [ $n -lt 20 ]; 
do
	if [[ $l2 == *"Version:"* ]];
	then
		VERSION_NUMBER=$l3
		break
	fi
	let n=n+1
done < $FILE

NOW=$(date +"%m-%d-%Y")

DEFAULT_VERSION_DATE="Talon_${VERSION_NUMBER}_${NOW}.hex"
DEFAULT_VERSION="Talon_${VERSION_NUMBER}.hex"
INPUT_VERSION_DATE="${2}${VERSION_NUMBER}_${NOW}.hex"
INPUT_VERSION="${2}${VERSION_NUMBER}.hex"

CUR=$(pwd)

cp "${1}" "../Generated_Binary/${DEFAULT_VERSION_DATE}" && echo Generated "'${DEFAULT_VERSION_DATE}'"