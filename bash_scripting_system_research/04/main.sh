#!/bin/bash

DEFAULT_COLUMN1_BACKGROUND=6  # Black (default)
DEFAULT_COLUMN1_FONT_COLOR=1  # White (default)
DEFAULT_COLUMN2_BACKGROUND=2  # Red (default)
DEFAULT_COLUMN2_FONT_COLOR=4  # Blue (default)

# font colors
get_color_code() {
  case $1 in
    1) echo "97" ;;  # White
    2) echo "31" ;;  # Red
    3) echo "32" ;;  # Green
    4) echo "34" ;;  # Blue
    5) echo "35" ;;  # Purple
    6) echo "30" ;;  # Black
    *) echo "Invalid color number: $1" >&2; exit 1 ;;
  esac
}

# background colors
get_bg_color_code() {
  case $1 in
    1) echo "107" ;;  # White
    2) echo "41" ;;   # Red
    3) echo "42" ;;   # Green
    4) echo "44" ;;   # Blue
    5) echo "45" ;;   # Purple
    6) echo "40" ;;   # Black
    *) echo "Invalid background color number: $1" >&2; exit 1 ;;
  esac
}

# is cfg file exist
cd ./04
if [ ! -f "task4.conf" ]; then
  echo "Error: Configuration file is not found!"
  exit 1
fi

# Load the configuration file
source task4.conf

# Check and set defaults for each parameter
# Track whether parameters were explicitly set
declare -A defaults=(
  [column1_background]=1 [column1_font_color]=1
  [column2_background]=1 [column2_font_color]=1
)

for param in column1_background column1_font_color column2_background column2_font_color; do
  if [[ -z "${!param}" ]]; then
    # Dynamically get the default value
    default_var="DEFAULT_${param^^}"
    declare "$param=${!default_var}"
    defaults[$param]=0
  fi
done

# Check for non-same colors
if [ "$column1_background" == "$column1_font_color" ]; then
  echo "Error: Background and font colors for column 1 cannot match."
  exit 1
fi

if [ "$column2_background" == "$column2_font_color" ]; then
  echo "Error: Background and font colors for column 2 cannot match."
  exit 1
fi

# getting color codes
bg_value_names_code=$(get_bg_color_code "$column1_background") || exit 1
font_value_names_code=$(get_color_code "$column1_font_color") || exit 1
bg_values_code=$(get_bg_color_code "$column2_background") || exit 1
font_values_code=$(get_color_code "$column2_font_color") || exit 1

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

# main func
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

SYS_INFO

print_color_name() {
  case $1 in
    1) echo "white" ;;
    2) echo "red" ;;
    3) echo "green" ;;
    4) echo "blue" ;;
    5) echo "purple" ;;
    6) echo "black" ;;
    *) echo "unknown" ;;
  esac
}

# printing the color scheme
echo ""
for param in column1_background column1_font_color column2_background column2_font_color; do
  param_name=$(echo "$param" | sed 's/_/ /g')
  if [[ "${defaults[$param]}" -eq 0 ]]; then
    default_var="DEFAULT_${param^^}"
    color_name=$(print_color_name "${!default_var}")
    echo "${param_name^} = default ($color_name)"
  else
    color_name=$(print_color_name "${!param}")
    echo "${param_name^} = ${!param} ($color_name)"
  fi
done