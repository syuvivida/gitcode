gitcode
=======

Use ./sync_git.sh

git remote add upstream git@github.com:henrytongCMS/gitcode.git

git add --all
git commit -m "comment"
git pull --rebase upstream master
git push origin master
