DOCUMENT=main

all: $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex -f

watch: $(DOCUMENT).tex
	latexmk -pdf -pvc $(DOCUMENT).tex

clean:
	rm -f *.log *.out *.fls *.fdb_latexmk *.aux *.toc
