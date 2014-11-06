#!/bin/bash

if [ $(pwd | tr '/' '\n' | tail -1) = "tools" ] ; then
    cd ..
fi
cd doc
doxygen
cd gh-pages
git add -A
git commit -m "update doxygen documentation"
git push origin gh-pages
