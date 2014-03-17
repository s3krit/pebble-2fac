#!/bin/bash
set -e
pebble build
pebble install --phone 192.168.0.5 --logs
