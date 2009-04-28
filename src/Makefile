LIB = wxremote/libwxremote.so ogc/libogc.so iosg/libiosg.so cpw/libcpw.so wcs/libwcs.so ogcwcs/libogc_wcs.so cpwutils/libcpwutils.so wms/libwms.so remote/libremote.so

FILES = /usr/lib/libwxremote.so /usr/lib/libogc.so /usr/lib/libiosg.so /usr/lib/libcpw.so /usr/lib/libwcs.so /usr/lib/libogc_wcs.so /usr/lib/libcpwutils.so /usr/lib/libwms.so /usr/lib/libremote.so

SUBDIRS = wxremote ogc iosg cpw wcs ogcwcs cpwutils wms remote

all:
	@for i in $(SUBDIRS); do \
        echo "make all in $$i..."; \
	(cd $$i; make all); done

install:
	@for i in $(SUBDIRS); do \
        echo "Installing $$i..."; \
	(cd $$i; make install); done


uninstall:
	@for i in $(SUBDIRS); do \
        echo "Uninstalling $$i..."; \
	(cd $$i; make uninstall); done

clean:
	rm $(LIB)

mrpropper:
	@for i in $(SUBDIRS); do \
        echo "Cleaning in $$i..."; \
	(cd $$i; make clean); done

