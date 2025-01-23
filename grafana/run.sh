#!/usr/bin/env bash

##
## This script builds and starts the containers 
##

set -e

function isInstalled(){
  local input="${1}"
  if ! command -v "${input}" >/dev/null 2>&1; then
    printf "\nERROR: cannot find '%s' in PATH\n\n" "${input}"
    printf "Did you run the './setup.sh' script?\n"
    return 1
  fi
}

function docker_compose_up(){
  printf "Building and starting the containers ...\n"
  if docker-compose up --build --detach --watch; then
    printf "Built and started containers\n"
    docker-compose ps
  else
    printf "\nERROR: Failed to build and start containers\n"
    return 1
  fi
}

isInstalled "docker-compose"
docker_compose_up
