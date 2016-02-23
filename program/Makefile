all: makeprograms 

makelibsrc: 
	cd lib && rm -rf *.a
	cd include && rm -rf *.h
	cd libsrc && ${MAKE} 

makeprograms:
	cd program && ${MAKE}

.PHONY: clean realclean 

clean: 	
	cd program && ${MAKE} clean

realclean: 
	cd program && ${MAKE} realclean

