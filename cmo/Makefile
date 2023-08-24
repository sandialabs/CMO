include ../utilib/Makefile
##
## The main makefile for CMO
##

install:
	(cd src/cmo; make cleanall; make driver)
	(cd src/ga; make mealign)
	(cd exec; ln -s ../src/cmo/cmo_driver .)
	(cd exec; ln -s ../src/ga/cmo_ga .)
	(cd exec; ln -s ../bin/PMAprot_ds .)
	(cd exec; ln -s ../../pico/bin/serialLP .)
