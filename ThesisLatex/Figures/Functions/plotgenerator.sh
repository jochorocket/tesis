#!/bin/bash

for file in *.tex
do
    echo "Processing $file ...";
    pdflatex --shell-escape $file
    pdfcrop ${file%.tex}.pdf ${file%.tex}.pdf
    convert -density 300 -units PixelsPerInch ${file%.tex}.pdf ${file%.tex}.png
done
