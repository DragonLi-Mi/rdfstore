LINKER = g++
export LINKER

CC = g++
export  CC

DEFINES = -DUNIX -DLIBGIST
export DEFINES

CFLAGS = -g -Wall -Wno-unused $(DEFINES)
export CFLAGS

GPPINCL = /usr/include
export GPPINCL

INCS =-I$(GPPINCL) -I/home/k/Documents/load/libgist -I.

LDFLAGS = -L ./lib

all: LIBGIST LIBBTREE OPERATE HLOAD

LIBGIST: 
	cd libgist ; $(MAKE) 

LIBBTREE:
	cd libbtree ; $(MAKE) 

OPERATE:
	cd operate ; $(MAKE)

HLOAD: LIBGIST LIBBTREE OPERATE
	cd src ; $(MAKE)
#	$(objects) ./lib/libgist.a ./lib/libbtree.a $(LINKER)
#	$(CFLAGS) -o hload $(objects) $(LDFLAGS)

#main.o:main.cpp load.h
#	$(CC) -c $(CFLAGS) main.cpp

#load.o:load.cpp load.h Triple.h TurtleParser.h RDFParser.h operate.h
#	$(CC) -c $(CFLAGS) $(INCS) load.cpp

#RDFParser.o:RDFParser.cpp RDFParser.h TurtleParser.h
#	$(CC) -c $(CFLAGS) RDFParser.cpp

#Triple.o:Triple.cpp Triple.h
#	$(CC) -c $(CFLAGS) Triple.cpp

#TurtleParser.o:TurtleParser.cpp TurtleParser.h Type.h
#	$(CC) -c $(CFLAGS) TurtleParser.cpp

#operate.o:operate.cpp operate.h gist.h gist_defs.h
#	$(CC) -c $(CFLAGS) $(INCS) operate.cpp

clean:
	
	cd libgist ; $(MAKE) clean 
	cd libbtree ; $(MAKE) clean 
	cd src ; $(MAKE) clean
	cd operate ; $(MAKE) clean  
	
#	makedepend


depend:
	makedepend $(DEFINES) $(INCS)

.depend:
	$(MAKE) depend
	touch .depend
