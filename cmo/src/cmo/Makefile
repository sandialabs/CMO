
include ../../../utilib/Makefile


# Nat's
CC	=	g++ 

TLDLIBS_FLAGS	=	-L/usr/X11R6/lib -L/usr/local/lib  -L/Net/pecos/proj/opt/apps/CMO/GA/Ygl -L/usr/openwin/lib 
TLDLIBS		=	-lYgl -lXext -lXm -lXt -lX11 -lm
TINCLUDES	=	-I/usr/X11R6/include -I../ga -I../Ygl


INCLUDES += -I../mw -I/usr/local/mpich/include -L/usr/local/mpich/lib

LDLIBS += #-lnsl #-lmpich


TEST	=  graphics.o contactmap.o

test:	main.o $(TEST)
	$(CC)   -o test $(TEST) main.o  $(TLDLIBS_FLAGS)   $(TLDLIBS) 


main.o:	main.C
	$(CC)  $(TINCLUDES) -c -DUSEGRAPHICS -o main.o main.C 


graphics.o:	graphics.C
	$(CC)   $(TINCLUDES) -c -o graphics.o   graphics.C


contactmap.o:	contactmap.C
	$(CC)  $(TINCLUDES) -c  -DUSEGRAPHICS -ocontactmap.o contactmap.C 


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
	make cmo_driver.o
	$(LINK.C) -o cmo_driver cmo_driver.o $(DRIVER_OBJS) $(LDLIBS) 

