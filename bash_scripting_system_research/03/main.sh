#!/bin/bash
# Write a bash script. Use the script from Part 2 and remove the part where the data is saved to a file. 
# The script is run with 4 parameters. The parameters are numeric. From 1 to 6, for example:
# script03.sh 1 3 4 5

# Colour designations: (1 - white, 2 - red, 3 - green, 4 - blue, 5 - purple, 6 - black)

# Parameter 1 is the background of the value names (HOSTNAME, TIMEZONE, USER etc.)
# Parameter 2 is the font colour of the value names (HOSTNAME, TIMEZONE, USER etc.)
# Parameter 3 is the background of the values (after the '=' sign)
# Parameter 4 is the font colour of the values (after the '=' sign)

# The font and background colours of one column must not match.
# If matching values are entered, there must be a message describing the problem and offering to call the script again. 
# After the message output, the program should exit correctly.

# font color
get_color_code() {
  case $1 in
    1) echo "97" ;;  # White
    2) echo "31" ;;  # Red
    3) echo "32" ;;  # Green
    4) echo "34" ;;  # Blue
    5) echo "35" ;;  # Purple
    6) echo "30" ;;  # Black
    *) echo "Invalid color number: $1" >&2; return 1 ;;
  esac
}

# background color
get_bg_color_code() {
  case $1 in
    1) echo "107" ;;  # White
    2) echo "41" ;;   # Red
    3) echo "42" ;;   # Green
    4) echo "44" ;;   # Blue
    5) echo "45" ;;   # Purple
    6) echo "40" ;;   # Black
    *) echo "Invalid background color number: $1" >&2; return 1 ;;
  esac
}

# Check if the number of parameters is correct
if [ $# != 4 ]; then
  echo "Usage: $0 <background_value_names> <font_value_names> <background_values> <font_values>"
  echo "Color codes: 1 - white, 2 - red, 3 - green, 4 - blue, 5 - purple, 6 - black"
  exit 1
fi

# Assign parameters
bg_value_names=$1
font_value_names=$2
bg_values=$3
font_values=$4

# Check if font and background colors match for value names
if [ "$bg_value_names" == "$font_value_names" ]; then
  echo "Error: Background and font colors for value names cannot match. Please call the script again with different colors."
  exit 1
fi

# Check if font and background colors match for values
if [ "$bg_values" == "$font_values" ]; then
  echo "Error: Background and font colors for values cannot match. Please call the script again with different colors."
  exit 1
fi

# Get ANSI color codes
bg_value_names_code=$(get_bg_color_code "$bg_value_names") || exit 1
font_value_names_code=$(get_color_code "$font_value_names") || exit 1
bg_values_code=$(get_bg_color_code "$bg_values") || exit 1
font_values_code=$(get_color_code "$font_values") || exit 1

# Function to mask CIDR to subnet mask
mask_output() {
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

# Function to display system information
SYS_INFO() {
  # HOSTNAME
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mHOSTNAME\e[0m = \e[${bg_values_code};${font_values_code}m$HOSTNAME\e[0m"

  # TIMEZONE
  timezone=$(timedatectl | grep -oP 'Time zone: \K\S+')
  offset=$(TZ="$timezone" date +"%:::z")
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mTIMEZONE\e[0m = \e[${bg_values_code};${font_values_code}m$timezone UTC $offset\e[0m"

  # USER
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mUSER\e[0m = \e[${bg_values_code};${font_values_code}m$USER\e[0m"

  # OS
  source '/etc/os-release'
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mOS\e[0m = \e[${bg_values_code};${font_values_code}m$NAME $VERSION\e[0m"

  # DATE
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mDATE\e[0m = \e[${bg_values_code};${font_values_code}m$(date +%d\ %B\ %Y\ %T)\e[0m"

  # UPTIME
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mUPTIME\e[0m = \e[${bg_values_code};${font_values_code}m$(uptime -p)\e[0m"

  # UPTIME_SEC
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mUPTIME_SEC\e[0m = \e[${bg_values_code};${font_values_code}m$(awk '{print $1}' /proc/uptime)\e[0m"

  # IP
  ip_address=$(ip -4 addr show | awk '/inet / && !/127.0.0.1/ {print $2; exit}')
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mIP\e[0m = \e[${bg_values_code};${font_values_code}m${ip_address%%/*}\e[0m"

  # MASK
  mask_output | while read -r line; do
    echo -e "\e[${bg_value_names_code};${font_value_names_code}m${line%=*}\e[0m = \e[${bg_values_code};${font_values_code}m${line#*= }\e[0m"
  done

  # GATEWAY
  echo -e "\e[${bg_value_names_code};${font_value_names_code}mGATEWAY\e[0m = \e[${bg_values_code};${font_values_code}m$(ip route | awk '/default/ {print $3}')\e[0m"

  # RAM
  free -b | awk '
  /^Mem:/ {
      total=$2/1024/1024/1024;
      used=$3/1024/1024/1024;
      free=$4/1024/1024/1024;
      printf "\033['"$bg_value_names_code"';'"$font_value_names_code"'mRAM_TOTAL\033[0m = \033['"$bg_values_code"';'"$font_values_code"'m%.3f GB\033[0m\n", total;
      printf "\033['"$bg_value_names_code"';'"$font_value_names_code"'mRAM_USED\033[0m = \033['"$bg_values_code"';'"$font_values_code"'m%.3f GB\033[0m\n", used;
      printf "\033['"$bg_value_names_code"';'"$font_value_names_code"'mRAM_FREE\033[0m = \033['"$bg_values_code"';'"$font_values_code"'m%.3f GB\033[0m\n", free;
  }
  '

  # SPACE_ROOT
  df / | awk '
  /dev/ {
      total=$2/1024;
      used=$3/1024;
      free=$4/1024;
      printf "\033['"$bg_value_names_code"';'"$font_value_names_code"'mSPACE_ROOT\033[0m = \033['"$bg_values_code"';'"$font_values_code"'m%.2f MB\033[0m\n", total;
      printf "\033['"$bg_value_names_code"';'"$font_value_names_code"'mSPACE_ROOT_USED\033[0m = \033['"$bg_values_code"';'"$font_values_code"'m%.2f MB\033[0m\n", used;
      printf "\033['"$bg_value_names_code"';'"$font_value_names_code"'mSPACE_ROOT_FREE\033[0m = \033['"$bg_values_code"';'"$font_values_code"'m%.2f MB\033[0m\n", free;
  }
  '
}

# Call the function
SYS_INFO