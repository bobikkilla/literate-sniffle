#!/bin/bash

# Write a bash script. The script is run with a single parameter.
# The parameter is an absolute or relative path to a directory. The parameter must end with '/', for example:
# script05.sh /var/log/
# The script must output the following information about the directory specified in the parameter:

# Total number of folders, including subfolders
# Top 5 folders with largest size in descending order (path and size)
# Total number of files
# Number of configuration files (with .conf extension), text files, executable files, log files (files with .log extension), archives, symbolic links
# Top 10 files with largest size in descending order (path, size and type)
# Top 10 executable files with largest size in descending order (path, size and hash)
# Execution time of the script
LC_NUMERIC=C
start_time=$(date +%s.%N)

# Check if a directory is provided
if [ -z "$1" ]; then
  echo "Usage: $0 [/path/to/directory/]"
  exit 1
fi

target_directory="$(realpath "$1")"

# Ensure the specified path is a valid directory
if [ ! -d "$target_directory" ]; then
  echo "Error: '$target_directory' is not a valid directory."
  exit 1
fi

echo "Total number of folders (including all nested ones) = $(find "$target_directory" -type d | wc -l)"

echo "TOP 5 folders of maximum size arranged in descending order (path and size):"

du -sh "$target_directory"/* 2>/dev/null | sort -hr | head -n 5 | awk 'BEGIN {rank=0} {rank++; printf "%d - %s, %s\n", rank, $2, $1}'

# to output files only in specified directory add -maxdepth 1 flag to find
echo "Total number of files = $(find "$target_directory" -type f | wc -l)"

# num of specific files 
echo "Number of:  "
# again if we want only specified directory files use flag -maxdepth 1 
echo "Configuration files (with the .conf extension) = $(find "$target_directory" -type f -name "*.conf" | wc -l)"
echo "Text files = $(find "$target_directory" -type f -name "*.txt" | wc -l)"
echo "Executable files = $(find "$target_directory" -type f -executable | wc -l)"
echo "Log files (with the extension .log) = $(find "$target_directory" -type f -name "*.log" | wc -l)"
echo "Archive files = $(find "$target_directory" -type f \( -name "*.tar" -o -name "*.gz" -o -name "*.zip" -o -name "*.bz2" -o -name "*.rar" \) | wc -l)"
echo "Symbolic links = $(find "$target_directory" -type l | wc -l)"

echo "TOP 10 files of maximum size arranged in descending order (path, size and type): "
# list of top 10 size files
find "$target_directory" -type f -exec du -sh {} + 2>/dev/null | sort -hr | head -n 10 | \
awk '{
    file=$2; size=$1;
    split(file, parts, ".");
    ext=(parts[length(parts)] == "" ? "unknown" : parts[length(parts)]);
    printf "%s - %s, %s, %s\n", ++rank, file, size, ext
}' rank=0

echo "TOP 10 executable files of the maximum size arranged in descending order (path, size and MD5 hash of file): "
# list of top 10 size exec files
find "$target_directory" -type f -executable 2>/dev/null | \
while read -r file; do
  # Get the size of the file in human-readable format
  size=$(du -sh "$file" 2>/dev/null | cut -f1)
  # Calculate the MD5 hash of the file
  md5=$(md5sum "$file" 2>/dev/null | awk '{print $1}')
  # Output the file details
  if [ -n "$size" ] && [ -n "$md5" ]; then
    echo "$size $file $md5"
  fi
done | \
# Sort by size in reverse order and display only the top 10
sort -hrk1 | head -n 10 | \
awk 'BEGIN {rank=0} {rank++; printf "%d - %s, %s, %s\n", rank, $2, $1, $3}'


end_time=$(date +%s.%N)

exec_time=$(echo "$end_time - $start_time" | bc)

# Output the result
printf "Script execution time: %.2lf seconds\n" "$exec_time"