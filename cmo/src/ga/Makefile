include ../../../sgopt/Makefile

OPTIMIZATION=-g

GAALIGN=PGAprot_ds.o PGAprot.o contactmap.o NKCMOProblem.o IPGAprot.o #graphics.o
MEALIGN=PMAprot_ds.o PGAprot.o PGALocalSearch.o contactmap.o NKCMOProblem.o #graphics.o


cleanup:
	$(RM) *.o

all:	$(MEALIGN) $(GAALIGN)

gaalign: 	$(GAALIGN)
	$(LINK.C) $(LDLIBS_FLAGS) $(LDLIBS) $(INCLUDES)  $(OPTIMIZATION) -o PGAprot_ds $(GAALIGN) $(LDLIBS)
	mv PGAprot_ds ../../bin


mealign:	$(MEALIGN) 
	$(LINK.C) $(LDLIBS_FLAGS) $(LDLIBS) $(INCLUDES) $(OPTIMIZATION) -o  PMAprot_ds $(MEALIGN) $(LDLIBS)
	mv PMAprot_ds ../../bin
