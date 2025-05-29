#!/bin/bash
# Write a bash script. The script should output the following information:

# HOSTNAME = network name
# TIMEZONE = time zone as: America/New_York UTC -5 
# (time zone must be taken from the system and be correct for the current location)
# USER = current user who ran the script
# OS = type and version of operating system
# DATE = current time as: 12 May 2020 12:24:36
# UPTIME = system uptime
# UPTIME_SEC = system uptime in seconds
# IP = _ip address of the machine on any of the network interfaces
# MASK = network mask of any of the network interfaces as: xxx.xxx.xxx.xxx.
# GATEWAY = default gateway ip
# RAM_TOTAL = main memory size in GB with an accuracy of three decimal places as: 3.125 GB
# RAM_USED = used memory size in GB with an accuracy of three decimal places
# RAM_FREE = free memory size in GB, with an accuracy of three decimal places
# SPACE_ROOT = root partition size in MB, with an accuracy of two decimal places, as 254.25 MB
# SPACE_ROOT_USED = size of used space of the root partition in MB, with an accuracy of two decimal places
# SPACE_ROOT_FREE = size of free space of the root partition in MB, with an accuracy of two decimal places

# After outputting the values, suggest writing the data to a file (ask the user to answer Y/N).
# Responses Y and y are considered positive, all others - negative.

# If the user agrees, create a file in the current directory containing the information that had been outputted. 
# The file name must looks like: DD_MM_YY_HH_MM_SS.status (The time in the file name must indicate when the data was saved).

function timezone_output() {
    # extracting timezone name
    timezone=$(timedatectl | grep -oP 'Time zone: \K\S+')
    # Get the UTC offset in hours
    offset=$(TZ="$timezone" date +"%:::z")
    # Output the result
    echo "TIMEZONE = $timezone UTC $offset"
}

function OS_output() {
    source '/etc/os-release'
    echo "OS = $NAME $VERSION"
}

function _ip {
  ip_address=$(ip -4 addr show | awk '/inet / && !/127.0.0.1/ {print $2; exit}')
  echo "IP = ${ip_address%%/*}"
}

function mask_output() {
  local cidr=$(ip -4 addr show | awk '/inet / && !/127.0.0.1/ {print $2; exit}')
  cidr=${cidr#*/*}
  local mask=""
  for i in {1..4}; do
    if [ $cidr -ge 8 ]; then
      mask+="255."
      cidr=$((cidr - 8))
    else
      mask+=$((256 - 2 ** (8 - cidr))).
      cidr=0
    fi
  done
  echo "MASK = ${mask%.}"
}

function SYS_INFO {
    echo "HOSTNAME = $HOSTNAME"

    timezone_output

    echo "USER = $USER"

    OS_output

    echo "DATE = $(date +%d\ %B\ %Y\ %T)"

    echo "UPTIME = $(uptime -p)"

    echo "UPTIME_SEC = $(awk '{print $1}' /proc/uptime)"

    _ip

    mask_output

    echo "GATEWAY = $(ip route | awk '/default/ {print $3}')"

    free -b | awk '
    /^Mem:/ {
        total=$2/1024/1024/1024;
        used=$3/1024/1024/1024;
        free=$4/1024/1024/1024;
        printf "RAM_TOTAL = %.3f GB\nRAM_USED = %.3f GB\nRAM_FREE = %.3f GB\n", total, used, free
    }
    '

    df  / | awk '
    /dev/ {
        total=$2/1024;
        used=$3/1024;
        free=$4/1024;
        printf "SPACE_ROOT = %.2f MB\nSPACE_ROOT_USED = %.2f MB\nSPACE_ROOT_FREE = %.2f MB\n", total, used, free
    }
    '
}

SYS_INFO

read -p "Save information to a file? (Enter Y/N): " status

# Check if the input is 'y' or 'Y'
if [ "$status" = 'y' ] || [ "$status" = 'Y' ]; then
  # The file name must looks like: DD_MM_YY_HH_MM_SS.status 
	date=$(date +%d_%m_%y_%H_%M_%S)
  SYS_INFO > "$date.status"
fi
