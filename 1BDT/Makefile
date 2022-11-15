#--------------------------------------------------------------------
# Makefile for Assignment 4, Part 1
# rules to build bdt{Good,Bad}*.o from source will fail
# Author: Christopher Moretti
#--------------------------------------------------------------------

TARGETS = bdtGood bdtBad1 bdtBad2 bdtBad3 bdtBad4 bdtBad5

.PRECIOUS: %.o

all: $(TARGETS)

clean:
	rm -f $(TARGETS) meminfo*.out

clobber: clean
	rm -f dynarray.o path.o bdt_client.o *M.o *~

bdtBad4: dynarrayM.o pathM.o bdtBad4.o bdt_clientM.o
	gcc217m -g $^ -o $@

bdtBad5: dynarrayM.o pathM.o bdtBad5.o bdt_clientM.o
	gcc217m -g $^ -o $@

bdt%: dynarray.o path.o bdt%.o bdt_client.o
	gcc217 -g $^ -o $@

dynarray.o: dynarray.c dynarray.h
	gcc217 -g -c $<

dynarrayM.o: dynarray.c dynarray.h
	gcc217m -g -c $< -o dynarrayM.o

path.o: path.c path.h a4def.h dynarray.h
	gcc217 -g -c $<

pathM.o: path.c path.h a4def.h dynarray.h
	gcc217m -g -c $< -o pathM.o

bdt_client.o: bdt_client.c bdt.h a4def.h
	gcc217 -g -c $<

bdt_clientM.o: bdt_client.c bdt.h a4def.h
	gcc217m -g -c $< -o bdt_clientM.o

#You can't re-build the .o files we provide, and
#you shouldn't be changing the header files they rely on
#but in case the headers' modification times have changed,
#update the .o files' modification times to still be newer.
bdtGood.o: dynarray.h bdt.h path.h a4def.h
	touch $@

bdtBad%.o: dynarray.h bdt.h path.h a4def.h
	touch $@
