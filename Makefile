all:	makelibsrc	makeprograms

makelibsrc:
	cd lib && rm -rf *.a
	cd include && rm -rf *.h
	cd libsrc/pb && ${MAKE}	install
	cd libsrc/bluetoothAction && ${MAKE}	install
	cd libsrc/ssocket && ${MAKE}	install
	cd libsrc/matrix && ${MAKE}	install
	cd libsrc/otim && ${MAKE}	install
	cd libsrc/tools && ${MAKE} install
	cd libsrc/Imagem && ${MAKE}	install
	cd libsrc/camera && ${MAKE}	install

makeprograms:
	cd program && ${MAKE}	clean && ${MAKE}
	cd program/main && ${MAKE}	clean && ${MAKE}
	cd program/calibrador	&& ${MAKE}	clean && ${MAKE}
	cd program/interface && ${MAKE}	clean && ${MAKE}

.PHONY: clean realclean

clean:
	cd program && ${MAKE} realclean
	cd libsrc/bluetoothAction && ${MAKE}	realclean
	cd libsrc/camera && ${MAKE}	realclean
	cd libsrc/Imagem && ${MAKE}	realclean
	cd libsrc/matrix && ${MAKE}	realclean
	cd libsrc/otim && ${MAKE}	realclean
	cd libsrc/ssocket && ${MAKE}	realclean
	cd libsrc/pb && ${MAKE}	realclean
