#ifndef _ALTHREAT_LANG_H
#define _ALTHREAT_LANG_H

#include "config.h"

#if ENABLE_NLS

#include <locale.h>
#include <libintl.h>

#define LOCALE_PACKAGE "AlThreat"
#define LOCALE_DIR DATADIR"/locale/"

#define _(String) gettext(String)
#define N_(String) String

#else

#define _(String) String
#define N_(String) String

#endif

#endif
