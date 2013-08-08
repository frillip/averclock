#!/bin/bash

# pytime fast is unreliable, so if that doesn't work do the slow method

./pytime_fast.py $1 || ./pytime.py $1
