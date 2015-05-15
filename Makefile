all: xep0047
xep0047: deplibs 
	cd xep-0047/build ; make LIBPATH=${PWD};

deplibs: openssl expat libstrophe-build

libstrophe-build: libstrophe.a

libstrophe.a:
	tar zxvf libstrophe.tgz
	-cd libstrophe ; ./bootstrap.sh ; ./configure openssl_CFLAGS=-I${PWD}/openssl-1.0.2/include openssl_LIBS=-L${PWD}  expat_CFLAGS=-I${PWD}/expat-2.1.0/lib expat_LIBS=-L${PWD}  ; make
	cp libstrophe/.libs/libstrophe.a .

openssl: libssl.a

libssl.a:
	tar zxvf openssl-1.0.2.tar.gz
	cd openssl-1.0.2 ; ./config ; make
	cp openssl-1.0.2/libssl.a .
	cp openssl-1.0.2/libcrypto.a .

expat: libexpat.a

libexpat.a:
	tar zxvf expat-2.1.0.tar.gz
	cd expat-2.1.0 ; ./configure ; make
	cp ./expat-2.1.0/.libs/libexpat.a .

clean: clean-openssl clean-expat clean-libstrophe clean-xep0047
	-rm libexpat.a libssl.a libcrypto.a libstrophe.a

clean-openssl:
	if [ -e openssl-1.0.2 ] ; then cd openssl-1.0.2 ; make clean ; fi

clean-expat:
	if [ -e expat-2.1.0 ] ; then cd expat-2.1.0 ; make clean ; fi

clean-libstrophe:
	cd libstrophe ; make clean

clean-xep0047:
	cd xep-0047/build; make clean

distclean: clean
	rm -rf expat-2.1.0 openssl-1.0.2
