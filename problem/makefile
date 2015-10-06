DOCUMENT=main

all: $(DOCUMENT).tex
	latexmk -pdf $(DOCUMENT).tex

watch: $(DOCUMENT).tex
	latexmk -pdf -pvc $(DOCUMENT).tex

clean:
	rm *.log *.fls *.fdb_latexmk *.aux