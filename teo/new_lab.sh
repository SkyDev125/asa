#!/bin/bash

# Find the highest numbered folder
last_folder=$(ls -d [0-9][0-9][0-9] | sort -n | tail -1)

# Extract the number and increment it
new_number=$(printf "%03d" $((10#${last_folder} + 1)))

# Create the new folder
mkdir "$new_number"

# Get the absolute path of the current directory
parent_dir=$(dirname "$(pwd)")

# Link the Makefile to the new folder using the absolute path
ln -s "$parent_dir/Makefile" "$new_number/Makefile"

# Copy the template files to the new folder
cp -r templates/* "$new_number"

echo "Created new folder: $new_number and symbolic link to Makefile"
