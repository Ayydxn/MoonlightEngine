#!/bin/bash

echo "Setting up Moonlight Engine for Linux/MacOS..."

# Check if Python is installed. 
if ! command -v python3 &> /dev/null; then
    echo "Python is not installed! Please install Python and rerun this script afterwards."
    
    # Simulating the PAUSE command
    read -n 1 -s -r -p "Press any key to exit..."
    echo
    exit 1
fi

# Run the Setup script
python3 Scripts/Python/Setup.py

# Pause at the end
read -n 1 -s -r -p "Press any key to exit..."
echo
exit 0