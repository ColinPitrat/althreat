INCLUDES = -I/usr/include/SDL -I$(top_srcdir)/widgets
#AM_LDFLAGS = -Wl,-rpath,/usr/lib -L/usr/lib 
LDADD = -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer $(top_builddir)/src/widgets/libT3F_Widgets.a

SUBDIRS = widgets

bin_PROGRAMS = AlThreat
AlThreat_CPPFLAGS = -DDATADIR="\"$(pkgdatadir)\"" -DSRCDIR="\"$(srcdir)\""
AlThreat_SOURCES = AlThreat.cc Arme.cc Bonus.cc Configuration.cc Ennemi.cc Game.cc Level.cc Menu.cc MenuJeu.cc Options.cc Projectile.cc Sprite.cc SpriteData.cc TypeBonus.cc TypeEnnemi.cc Vaisseau.cc graphics.cc Arme.h Bonus.h Configuration.h Ennemi.h Game.h Level.h Menu.h MenuJeu.h Options.h Projectile.h Sprite.h SpriteData.h TypeBonus.h TypeEnnemi.h Vaisseau.h config.h graphics.h
