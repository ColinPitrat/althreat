INCLUDES = -I/usr/include/SDL -I$(top_srcdir)/widgets
LDADD = -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer $(top_builddir)/src/widgets/libT3F_Widgets.a 

SUBDIRS = src

AlThreat_data = armes decors fonts images levels sons vaisseaux
EXTRA_DIST = $(AlThreat_data)

#$(mkinstalldirs) $(datadir)
install-data-local:
	test -d $(datadir) || mkdir $(datadir) && chmod u+w $(datadir)
	test -d $(pkgdatadir) || mkdir -p $(pkgdatadir) && chmod u+w $(pkgdatadir)
	cd $(srcdir) && cp -pr $(AlThreat_data) $(pkgdatadir)/ 
	chmod -R u+w $(pkgdatadir)

uninstall-local:
	rm -rf $(pkgdatadir)
