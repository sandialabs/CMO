
include ../../../utilib/Makefile


INCLUDES += -I../mw

cleanup:
	rm -rf *.o *~ 

cleanall: cleanup
	rm -rf SunW*
	(cd ../mw; make cleanup; rm -rf SunW*)

DRIVER_OBJS= \
../mw/MWGroup.o \
../mw/MWIndRC.o \
../mw/MWMpiComm.o \
../mw/MWprintf.o \
../mw/MWRMComm.o \
../mw/MWExec.o

driver: cleanup 
	(cd ../mw; make all)
	make sem_driver.o
	$(LINK.C) -o sem_driver sem_driver.o $(DRIVER_OBJS) $(LDLIBS) 

tmp: cleanup 
	(cd ../mw; make all)
	make tmp.o
	$(LINK.C) -o tmp tmp.o $(DRIVER_OBJS) $(LDLIBS) 

