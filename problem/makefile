DOCUMENT=main

all: $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex -f -quiet -latexoption=-interaction=batchmode

watch: $(DOCUMENT).tex
	latexmk -pdf -pvc $(DOCUMENT).tex -f -latexoption=-interaction=batchmode

test: $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex -quiet

clean:
	rm -f *.log *.out *.fls *.fdb_latexmk *.aux *.toc
