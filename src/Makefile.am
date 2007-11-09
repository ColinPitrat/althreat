INCLUDES = -I$(top_srcdir)/widgets
LDADD = $(top_builddir)/src/widgets/libT3F_Widgets.a 

SUBDIRS = widgets

bin_PROGRAMS = AlThreat AlThreat_install
AlThreat_CPPFLAGS = -DDATADIR="\"$(pkgdatadir)\"" -DSRCDIR="\"$(srcdir)\""
AlThreat_SOURCES = AlThreat.cc Arme.cc Bonus.cc Configuration.cc Ennemi.cc Game.cc Level.cc Menu.cc MenuJeu.cc Options.cc Projectile.cc Sprite.cc SpriteData.cc TypeBonus.cc TypeEnnemi.cc Vaisseau.cc graphics.cc Arme.h Bonus.h Configuration.h Ennemi.h Game.h Level.h Menu.h MenuJeu.h Options.h Projectile.h Sprite.h SpriteData.h TypeBonus.h TypeEnnemi.h Vaisseau.h config.h graphics.h Joystick.h lang.h
AlThreat_install_SOURCES = AlThreat_install.in

AlThreat_install: AlThreat_install.in
	sed 's*_althreat_installdir_*$(bindir)*' $(top_builddir)/src/AlThreat_install.in > AlThreat_install
	chmod a+x AlThreat_install

scripts_dir=$(bindir)
EXTRA_DIST=AlThreat_install
