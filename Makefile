all:	makelibsrc	makeprograms

makelibsrc:	clean
	cd lib && rm -rf *.a
	cd include && rm -rf *.h
	cd libsrc/bluetothAction && ${MAKE}	install
	cd libsrc/ssocket && ${MAKE}	install
	cd libsrc/matrix && ${MAKE}	install
	cd libsrc/otim && ${MAKE}	install
	cd libsrc/Imagem && ${MAKE}	install
	cd libsrc/camera && ${MAKE}	install

makeprograms:
	cd program && ${MAKE}	clean
	cd program && ${MAKE}
	cd calibrador	&& ${MAKE}	clean && ${MAKE}	install
	cd main && ${MAKE}	clean && ${MAKE}	install

.PHONY: clean realclean

# clean:
# 	cd program && ${MAKE} clean
clean:
	cd program && ${MAKE} realclean
	cd libsrc/bluetothAction && ${MAKE}	realclean
	cd libsrc/camera && ${MAKE}	realclean
	cd libsrc/Imagem && ${MAKE}	realclean
	cd libsrc/matrix && ${MAKE}	realclean
	cd libsrc/otim && ${MAKE}	realclean
	cd libsrc/ssocket && ${MAKE}	realclean
