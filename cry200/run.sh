#!/bin/bash

socat tcp-l:7777,fork,reuseaddr exec:./serrvice.py
