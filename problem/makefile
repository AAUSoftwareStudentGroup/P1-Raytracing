DOCUMENT=main
SVGGFX=$(wildcard graphics/*.svg)
PDFGFX=$(addprefix graphics/,$(notdir $(SVGGFX:.svg=.pdf)))
UNAME := $(shell uname)

ifeq ($(OS),Windows_NT)
	MOVE := move
else
	MOVE := mv
endif

all: main process

main: $(PDFGFX) $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex -f -quiet --shell-escape -latexoption=-interaction=batchmode	

process: $(PDFGFX) sections/processanalyse.tex
	cd sections && latexmk -pdf processanalyse.tex -f -quiet --shell-escape -latexoption=-interaction=batchmode
	$(MOVE) sections/processanalyse.pdf .

graphics/%.pdf: graphics/%.svg
	-inkscape $< --export-text-to-path --export-pdf=$@

watch: clean $(PDFGFX) $(DOCUMENT).tex
	latexmk -pdf -pvc $(DOCUMENT).tex -f --shell-escape  -latexoption=-interaction=batchmode

test: $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex -quiet

clean:
	rm -f $(PDFGFX)
	rm -f *.log *.out *.fls *.fdb_latexmk *.aux *.toc *.pdf
