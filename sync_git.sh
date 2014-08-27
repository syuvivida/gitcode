#!/bin/bash

if [ -z $2 ]; then
    echo "Usage: $0 [fragment] [comment]"
    exit 1
fi

git add $1
git commit -m "$2"
#git pull --rebase upstream master
git push origin master

exit
