default: collide

collide: crystal_phase.c vertex.c sflake_test.c contour.c snowflake.c vertex.h
	gcc sflake_test.c vertex.c contour.c crystal_phase.c snowflake.c -lm -o collide


crystal-plot: crystal_phase.c plot.py 
	mkdir -p GIFs
	mkdir -p PNGs
	gcc sflake_test.c contour.c crystal_phase.c snowflake.c crystal_phase.c -g -std=gnu99 -lm -O2 -o crystal
	./crystal
	python2.7 plot.py video
	rm PNGs/*
	rmdir PNGs

clean:
	rm -f crystal
	rm -f collide

