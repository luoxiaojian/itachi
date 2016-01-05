#!/bin/bash

./statis ../bf/log/ ./$1/bfRes
./statis ../pf/log/ ./$1/pfRes
./statis ../run/log/ ./$1/runRes
./statis ../dufo/log/ ./$1/dufoRes
./statis ../uedf/log/ ./$1/uedfRes

