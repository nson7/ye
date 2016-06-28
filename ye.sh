#!/bin/bash

# emacs
TERM=xterm-256color emacs -nw $1

# etags
find . -name "*.[chCH]" -print | etags -
