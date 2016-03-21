HOME = ..
INCS = -I$(GPPINCL)  -I$(HOME)/libbtree  -I$(HOME)/libgist -I.
objects = main.o load.o RDFParser.o Triple.o TurtleParser.o bt_driver.o
CC  = g++

LDFLAGS = -L$(HOME)/lib

BTLIBS = -lbtree -lgist

DEST = ..
BTPROG = $(DEST)/hload
OPERATEOBJ = ../operate/operate.o
all: HLOAD

HLOAD:$(HOME)/lib/libgist.a $(HOME)/lib/libbtree.a $(objects) $(OPERATEOBJ)
	$(LINKER) -o $(BTPROG) $(LDFLAGS) $(objects) $(OPERATEOBJ) $(BTLIBS)

#main.o:main.cpp  load.h
#		$(CC) -c $(CFLAGS) $(INCS) main.cpp

#load.o:load.cpp load.h Triple.h TurtleParser.h RDFParser.h 
#		$(CC) -c $(CFLAGS) $(INCS) load.cpp

#RDFParser.o:RDFParser.cpp RDFParser.h TurtleParser.h
#		$(CC) -c $(CFLAGS) RDFParser.cpp

#Triple.o:Triple.cpp Triple.h
#		$(CC) -c $(CFLAGS) Triple.cpp

#TurtleParser.o:TurtleParser.cpp TurtleParser.h Type.h
#		$(CC) -c $(CFLAGS) TurtleParser.cpp
%.o:%.cpp
	$(CC) -c  $(CFLAGS) $(INCS) $*.cpp
#operate.o:operate.cpp operate.h ../libgist/gist.h ../libgist/gist_defs.h
#		$(CC) -c $(CFLAGS) $(INCS) operate.cpp
clean:
	rm -f $(objects)
