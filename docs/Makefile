TEX := $(wildcard *.tex)

.Phony: clean

default: owoled.pdf

./build/xml/index.xml:
	mkdir -p $(@D)
	doxygen

./build/api.tex: ./build/xml/index.xml
	python3 ./util/doxytex/gen-latex.py ./build/xml 1 > ./build/api.tex 

./build/owoled.pdf: ./build/api.tex $(TEX)
	#latexmk -use-make -xelatex -jobname=./build/owoled owoled.tex
	latexmk -use-make -pdf -pdflatex="xelatex -interaction=nonstopmode" -jobname=./build/owoled owoled.tex

./owoled.pdf: ./build/owoled.pdf
	cp ./build/owoled.pdf .

clean:
	rm -rf build
	rm -rf owoled.pdf
