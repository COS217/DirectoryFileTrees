#--------------------------------------------------------------------
# Makefile for Assignment 4, Part 2
# dt* targets are built using checkerDT
# rules to build dtBad*.o and nodeBad*.o from source will fail
# Author: Christopher Moretti
#--------------------------------------------------------------------

GCC = gcc217
#GCC = gcc217m

TARGETS = dtGood dtBad1a dtBad1b dtBad2 dtBad3 dtBad4

.PRECIOUS: %.o

all: $(TARGETS)

clean:
	rm -f $(TARGETS) meminfo*.out

clobber: clean
	rm -f dynarray.o path.o dt_client.o checkerDT.o nodeDTGood.o dtGood.o *~

dt%: dynarray.o path.o checkerDT.o nodeDT%.o dt%.o dt_client.o
	$(GCC) -g $^ -o $@

dynarray.o: dynarray.c dynarray.h
	$(GCC) -g -c $<

path.o: path.c dynarray.h path.h a4def.h
	$(GCC) -g -c $<

dt_client.o: dt_client.c dt.h a4def.h
	$(GCC) -g -c $<

checkerDT.o: checkerDT.c dynarray.h checkerDT.h nodeDT.h path.h a4def.h
	$(GCC) -g -c $<

nodeDTGood.o: nodeDTGood.c dynarray.h checkerDT.h nodeDT.h path.h a4def.h
	$(GCC) -g -c $<

dtGood.o: dtGood.c dynarray.h checkerDT.h nodeDT.h dt.h path.h a4def.h
	$(GCC) -g -c $<

#You can't re-build the .o files we provide, and
#you shouldn't be changing the header files they rely on
#but in case the headers' modification times have changed,
#update the .o files' modification times to still be newer.
nodeDT%.o: dynarray.h checkerDT.h nodeDT.h path.h a4def.h
	touch $@

dtBad%.o: dynarray.h checkerDT.h nodeDT.h dt.h path.h a4def.h
	touch $@
