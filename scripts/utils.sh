#!/usr/bin/env sh

###########################################################
# Globals
export MEDIAINFO_VERSION="18.05"

###########################################################
# OS infos
export OS_NAME=$(uname -s)
export OS_REV=$(uname -r)
export OS_MACH=$(uname -m)

if [ "${OS_NAME}" = "Linux" ]; then
	export JOBS=$(grep '^processor' /proc/cpuinfo | wc -l)
else
	export JOBS=2
fi

###########################################################
# Print

RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
LIGHT_BLUE='\033[1;34m'
NC='\033[0m' # No Color

print_important() {
	printf '%b' "${BLUE}${1}${NC}\n"
}

print_success() {
	printf '%b' "${GREEN}${1}${NC}\n" 1>&2
}

print_info() {
	printf '%b' "${LIGHT_BLUE}${1}${NC}\n"
}

print_warning() {
	printf '%b' "${ORANGE}${1}${NC}\n"
}

print_error() {
	printf '%b' "${RED}${1}${NC}\n" 1>&2
}

print_critical() {
	printf '%b' "${RED}${1}${NC}\n" 1>&2
	exit 1
}

###########################################################
# Other useful functions

lc() {
	echo "$1" | tr '[:upper:]' '[:lower:]'
}
