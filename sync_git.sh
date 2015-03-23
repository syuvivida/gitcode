#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [comment]"
    exit 1
fi

git add --all
git commit -m "$1"
git pull --rebase upstream master
git push origin master

exit
