#!/bin/bash

SESSIONNAME="CLIO"
tmux has-session -t $SESSIONNAME &> /dev/null

if [ $? != 0 ]
 then
    tmux new-session -s $SESSIONNAME -d
    tmux new-window -t $SESSIONNAME:1 -n 'dev'
    tmux split-window -v
    tmux select-pane -t 1
    tmux send-keys "reset && picocom -b 921600 /dev/ttyACM0" C-m
    tmux select-pane -t 0
    tmux send-keys "make flash" 
    tmux send-keys C-o
    tmux new-window -t $SESSIONNAME:2 -n 'git'
    tmux send-keys "gitl" C-m
fi

tmux attach -t $SESSIONNAME:1
