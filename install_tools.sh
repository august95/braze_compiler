#!/bin/bash
echo "==> Installing build tools"
sudo apt-get update
sudo apt-get install -y build-essential gcc make gcc-multilib g++-multilib nasm
