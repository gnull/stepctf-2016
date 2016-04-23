#!/bin/bash
g++ -m32 -o ggg ggg.cpp 
strip ggg
strip ggg -R .gnu.version
strip ggg -R .eh_frame
# strip ggg -R .bss
strip ggg -R .comment 
strip ggg -R .hash