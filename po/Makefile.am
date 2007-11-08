POFILES = fr_FR/LC_MESSAGES/AlThreat.po

MOFILES = $(POFILES:.po=.mo)

SRCFILES = ../src/AlThreat.cc \
	   ../src/Arme.cc \
	   ../src/Arme.h \
	   ../src/Bonus.cc \
	   ../src/Bonus.h \
	   ../src/Configuration.cc \
	   ../src/Configuration.h \
	   ../src/Ennemi.cc \
	   ../src/Ennemi.h \
	   ../src/Game.cc \
	   ../src/Game.h \
	   ../src/Joystick.h \
	   ../src/Level.cc \
	   ../src/Level.h \
	   ../src/Menu.cc \
	   ../src/Menu.h \
	   ../src/MenuJeu.cc \
	   ../src/MenuJeu.h \
	   ../src/Options.cc \
	   ../src/Options.h \
	   ../src/Projectile.cc \
	   ../src/Projectile.h \
	   ../src/Sprite.cc \
	   ../src/Sprite.h \
	   ../src/SpriteData.cc \
	   ../src/SpriteData.h \
	   ../src/Trajectoire.h \
	   ../src/TypeBonus.cc \
	   ../src/TypeBonus.h \
	   ../src/TypeEnnemi.cc \
	   ../src/TypeEnnemi.h \
	   ../src/Vaisseau.cc \
	   ../src/Vaisseau.h \
	   ../src/config.h \
	   ../src/graphics.cc \
	   ../src/graphics.h \
	   ../src/lang.h

DOMAIN = $(PACKAGE)

COPYRIGHT_HOLDER = -=(the3fold)=-

MSGID_BUGS_ADDRESS = althreat@googlegroups.com

LOCALEDIR=$(pkgdatadir)/locale

EXTRA_DIST = $(DOMAIN).pot $(POFILES)

MSGMERGE_OPTIONS = --quiet --update
XGETTEXT_OPTIONS = --keyword=_ --keyword=N_ --force-po
SUFFIXES = .po .mo

.po.mo:
	$(MSGFMT) -o $@ $<

$(POFILES): $(DOMAIN).pot
	@lang=`echo $@ | sed 's/\.po$$//'`; \
	test -f $${lang}.po && $(MSGMERGE) $(MSGMERGE_OPTIONS) $${lang}.po $(DOMAIN).pot

$(DOMAIN).pot: $(SRCFILES)
	$(XGETTEXT) $(XGETTEXT_OPTIONS) --default-domain=$(DOMAIN) \
	--copyright-holder='$(COPYRIGHT_HOLDER)' \
	--msgid-bugs-address='$(MSGID_BUGS_ADDRESS)' \
	-o $(DOMAIN).pot $(SRCFILES)

all-local: all-local-@USE_NLS@
all-local-no:
all-local-yes: $(DOMAIN).pot $(MOFILES)

install-data-local: install-data-local-@USE_NLS@
install-data-local-no: all-local
install-data-local-yes: all-local
	for file in $(MOFILES); do \
		$(INSTALL_DATA) -D $$file $(LOCALEDIR)/$$file; \
	done;

uninstall-local: uninstall-local-@USE_NLS@
uninstall-local-no:
uninstall-local-yes:
	for file in $(MOFILES); do \
		rm -f $(LOCALEDIR)/$$file; \
	done;

