#!/bin/bash

socat tcp-l:17777,fork,reuseaddr exec:./ggg
