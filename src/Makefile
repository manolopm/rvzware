LIB = wxremote/libwxremote.so ogc/libogc.so iosg/libiosg.so cpw/libcpw.so libwcs/libwcs.so ogcwcs/libogc_wcs.so cpwutils/libcpwutils.so libwms/libwms.so remote/libremote.so

FILES = /usr/lib/libwxremote.so /usr/lib/libogc.so /usr/lib/libiosg.so /usr/lib/libcpw.so /usr/lib/libwcs.so /usr/lib/libogc_wcs.so /usr/lib/libcpwutils.so /usr/lib/libwms.so /usr/lib/libremote.so

SUBDIRS = wxremote ogc iosg cpw libwcs ogcwcs cpwutils libwms remote

all:
	@for i in $(SUBDIRS); do \
        echo "make all in $$i..."; \
	(cd $$i; make all); done

install:
	cp $(LIB) /usr/lib

uninstall:
	rm $(FILES)

clean:
	rm $(LIB)

mrpropper:
	@for i in $(SUBDIRS); do \
        echo "Cleaning in $$i..."; \
	(cd $$i; make clean); done

