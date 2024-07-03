#!/usr/bin/env bash

set -e

# Define colors
color_green='\x1B[38;5;46m'
color_yellow='\x1B[38;5;226m'
color_cyan='\x1B[38;5;51m'
color_white='\x1B[38;5;255m'
color_reset='\x1B[0m'

# Create virtual environment
python -m venv venv

# Activate virtual environment
source "${PWD}/venv/bin/activate"

# Upgrade pip
pip install --upgrade pip

# Install package and dependencies
pip install --editable "${PWD}"

# Inform user
printf "\n\n${color_green}Virtual environment setup complete and requirements installed.${color_reset}\n\nTo activate the virtual environment, run:\n"
printf "\n\t${color_yellow}\$  ${color_reset}${color_white}source ${color_reset}${color_cyan}venv/bin/activate${color_reset}\n\n"
