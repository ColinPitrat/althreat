AC_INIT(AlThreat, 0.4)
AC_CONFIG_SRCDIR(src/AlThreat.cc)
AC_CONFIG_HEADER(src/config.h)

AM_INIT_AUTOMAKE
AM_GNU_GETTEXT([external])

AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL

AC_CHECK_FUNCS([strstr])
AC_CHECK_HEADERS([libintl.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADER_STDBOOL
AC_PREREQ
AC_PROG_AWK

AC_CHECK_LIB([SDL], [SDL_Init],, AC_MSG_ERROR([SDL library needed for AlThreat! see http://www.libsdl.org/]))
AC_CHECK_LIB([SDL_image], [IMG_Load],, AC_MSG_ERROR([SDL_image library needed for AlThreat! see http://www.libsdl.org/]))
AC_CHECK_LIB([SDL_mixer], [Mix_LoadMUS],, AC_MSG_ERROR([SDL_mixer library needed for AlThreat! see http://www.libsdl.org/]))
AC_CHECK_LIB([SDL_ttf], [TTF_Init],, AC_MSG_ERROR([SDL_ttf library needed for AlThreat! see http://www.libsdl.org/]))

SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :, AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LDFLAGS="$LDFLAGS $SDL_LIBS"

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS([memset setlocale])

AC_OUTPUT(Makefile src/Makefile src/widgets/Makefile armes/Makefile decors/Makefile fonts/Makefile images/Makefile levels/Makefile sons/Makefile vaisseaux/Makefile po/Makefile)
