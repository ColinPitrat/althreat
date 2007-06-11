INCLUDES = -I/usr/include/SDL -I$(top_srcdir)/widgets
LDADD = -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer $(top_builddir)/src/widgets/libT3F_Widgets.a 

SUBDIRS = src

AlThreat_data = armes decors fonts images levels sons vaisseaux
EXTRA_DIST = $(AlThreat_data)

#$(mkinstalldirs) $(datadir)
install-data-local:
	test -d $(DESTDIR)/$(datadir) || mkdir $(DESTDIR)/$(datadir) && chmod u+w $(DESTDIR)/$(datadir)
	test -d $(DESTDIR)/$(pkgdatadir) || mkdir -p $(DESTDIR)/$(pkgdatadir) && chmod u+w $(DESTDIR)/$(pkgdatadir)
	cd $(srcdir) && cp -pr $(AlThreat_data) $(DESTDIR)/$(pkgdatadir)/ 
	chmod -R u+w $(DESTDIR)/$(pkgdatadir)

uninstall-local:
	rm -rf $(DESTDIR)/$(pkgdatadir)
