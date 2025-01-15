#!/usr/bin/env bash

##
## This script is to be run on RaspberryPi OS to install all the needed packages
##

set -e

function apt_update(){
  printf "Updating packages ...\n"
  sudo -s<<<'apt -y update && apt -y upgrade && apt -y autoremove'
}

function install_git(){
  printf "Installing git ...\n"
  sudo apt -y install git
}

function install_docker(){
  printf "Installing Docker ...\n"
  curl -sSL "https://get.docker.com" | sh
  printf "Adding '%s' to 'docker' group ...\n" "${USER}"
  sudo usermod -aG docker "${USER}"
  printf "\n\nYou will need to log out and log back in to be added to docker group\n\n"
  sleep 2
  printf "Starting and enabling docker service ...\n"
  sudo systemctl enable --now docker.service
  printf "Installing docker-compose\n"
  sudo apt -y install docker-compose
}

apt_update
install_git
install_docker
