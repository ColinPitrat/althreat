POFILES = $(top_srcdir)/po/fr_FR/LC_MESSAGES/AlThreat.po

MOFILES = $(top_builddir)/po/fr_FR/LC_MESSAGES/AlThreat.mo

SRCFILES = $(top_srcdir)/src/AlThreat.cc \
	   $(top_srcdir)/src/Arme.cc \
	   $(top_srcdir)/src/Arme.h \
	   $(top_srcdir)/src/Bonus.cc \
	   $(top_srcdir)/src/Bonus.h \
	   $(top_srcdir)/src/Configuration.cc \
	   $(top_srcdir)/src/Configuration.h \
	   $(top_srcdir)/src/Ennemi.cc \
	   $(top_srcdir)/src/Ennemi.h \
	   $(top_srcdir)/src/Game.cc \
	   $(top_srcdir)/src/Game.h \
	   $(top_srcdir)/src/Joystick.h \
	   $(top_srcdir)/src/Level.cc \
	   $(top_srcdir)/src/Level.h \
	   $(top_srcdir)/src/Menu.cc \
	   $(top_srcdir)/src/Menu.h \
	   $(top_srcdir)/src/MenuJeu.cc \
	   $(top_srcdir)/src/MenuJeu.h \
	   $(top_srcdir)/src/Options.cc \
	   $(top_srcdir)/src/Options.h \
	   $(top_srcdir)/src/Projectile.cc \
	   $(top_srcdir)/src/Projectile.h \
	   $(top_srcdir)/src/Sprite.cc \
	   $(top_srcdir)/src/Sprite.h \
	   $(top_srcdir)/src/SpriteData.cc \
	   $(top_srcdir)/src/SpriteData.h \
	   $(top_srcdir)/src/TypeBonus.cc \
	   $(top_srcdir)/src/TypeBonus.h \
	   $(top_srcdir)/src/TypeEnnemi.cc \
	   $(top_srcdir)/src/TypeEnnemi.h \
	   $(top_srcdir)/src/Vaisseau.cc \
	   $(top_srcdir)/src/Vaisseau.h \
	   $(top_srcdir)/src/config.h \
	   $(top_srcdir)/src/graphics.cc \
	   $(top_srcdir)/src/graphics.h \
	   $(top_srcdir)/src/lang.h

DOMAIN = $(PACKAGE)

COPYRIGHT_HOLDER = -=(the3fold)=-

MSGID_BUGS_ADDRESS = althreat@googlegroups.com

POTFILE = $(top_srcdir)/po/$(DOMAIN).pot

LOCALEdir=$(pkgdatadir)/locale
LOCALE_DATA = $(POTFILE) $(POFILES)

EXTRA_DIST = $(LOCALE_DATA)

MSGMERGE_OPTIONS = --quiet --update
XGETTEXT_OPTIONS = --keyword=_ --keyword=N_ --force-po

all-local: all-local-@USE_NLS@
all-local-no:
all-local-yes: $(MOFILES)

$(MOFILES): $(POFILES)
	$(MSGFMT) -o $@ $<

$(POFILES): $(POTFILE)
	test -f $@ && $(MSGMERGE) $(MSGMERGE_OPTIONS) $@ $(POTFILE)

$(POTFILE): $(SRCFILES)
	$(XGETTEXT) $(XGETTEXT_OPTIONS) --default-domain=$(DOMAIN) \
	--copyright-holder='$(COPYRIGHT_HOLDER)' \
	--msgid-bugs-address='$(MSGID_BUGS_ADDRESS)' \
	-o $(POTFILE) $(SRCFILES)
