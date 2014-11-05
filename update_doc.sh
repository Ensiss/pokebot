#!/bin/bash

cd doc
doxygen
cd gh-pages
git add -A
git commit -m "update doxygen documentation"
git push origin gh-pages
