HOME = ..
CC = g++
INCS = -I$(GPPINCL) -I$(HOME)/libgist -I. \
	-I$(HOME)/libbtree

SRCS = operate.cpp 
       
OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCS) $*.cpp

all: $(OBJS)

clean:
	rm -f $(OBJS) *~ #*# *pure*
#	makedepend

depend:
	makedepend $(DEFINES) $(INCS) $(SRCS)

.depend:
	$(MAKE) depend
	touch .depend

install: $(PROGRAM)
	echo Installing $(PROGRAM) in $(DEST)
	install -s $(PROGRAM) $(DEST)

# DO NOT DELETE THIS LINE -- make depend depends on it.
