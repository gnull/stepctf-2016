#!/bin/bash
clang -S -emit-llvm testkey.cpp -o testkey.ll
opt -S -load ./llvm-functionpass-kryptonite-obfuscater.so -kryptonite -heavy-add-obfu testkey.ll -o testkey.obfu.ll
llc -filetype=obj -O0 testkey.obfu.ll -o testkey.obfu.o
clang testkey.obfu.o -o testkey


