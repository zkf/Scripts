#! /usr/bin/env zsh

CMD=$1
N=$(optirun --no-xorg nvidia-xconfig --query-gpu-info | grep '  Number of Display Devices: ' | rev | cut -d' ' -f1 | rev)

if [[ $N > 0 ]]; then
    # check if it is already in use
    INACTIVE=$( zmodload zsh/net/socket;
          typeset REPLY;
          zsocket -d 5 /run/bumblebee.socket;
          echo -n "S" >&5 && read -u5 && print $REPLY | grep "inactive")
    if [[ "$INACTIVE" != "" ]]; then
        exx ${CMD}
    else
        primusrun $CMD
    fi
else
    primusrun $CMD
fi

