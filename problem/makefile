DOCUMENT=main
SVGGFX=$(wildcard graphics/*.svg)
PDFGFX=$(addprefix graphics/,$(notdir $(SVGGFX:.svg=.pdf)))

all: $(PDFGFX) $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex -f -quiet --shell-escape -latexoption=-interaction=batchmode

graphics/%.pdf: graphics/%.svg
	-inkscape $< --export-text-to-path --export-pdf=$@

watch: $(PDFGFX) $(DOCUMENT).tex
	latexmk -pdf -pvc $(DOCUMENT).tex -f --shell-escape  -latexoption=-interaction=batchmode

test: $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex -quiet

clean:
	rm -f *.log *.out *.fls *.fdb_latexmk *.aux *.toc
