default: crystal-plot

crystal-plot: crystal_phase.c plot.py
	mkdir -p GIFs
	mkdir -p PNGs
	gcc crystal_phase.c -g -std=gnu99 -lm -O2 -o crystal
	./crystal
	python2.7 plot.py video
	rm PNGs/*
	rmdir PNGs
