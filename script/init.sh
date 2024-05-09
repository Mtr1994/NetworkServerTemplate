#!/bin/bash

export LD_LIBRARY_PATH=$PWD/:$LD_LIBRARY_PATH

sudo ldconfig -n $PWD
