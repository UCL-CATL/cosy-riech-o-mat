#!/bin/sh

# For me, to have clang completion in Vim:
~/.vim/bin/cc_args.py gcc -W -Wall -o test-open-valve -g test-open-valve.c
~/.vim/bin/cc_args.py gcc -W -Wall -o test-parallel-port -g test-parallel-port.c

#gcc -W -Wall -o test-open-valve -g test-open-valve.c
