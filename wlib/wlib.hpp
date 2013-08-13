#ifndef _WLIB_H
#define _WLIB_H

#include "stdafx.h"
#include "windowing.hpp"
#include "window.hpp"
#include "dialog.hpp"
#include "toolbar.hpp"
#include "splitter.hpp"
#include "rebar.hpp"
#include "treeview.hpp"
#include "listview.hpp"
#include "listbox.hpp"
#include "util.hpp"
#include "imagelist.hpp"
#include "filedialog.hpp"
#include "trackbar.hpp"
#include "combobox.hpp"
#include "registry.hpp"
#include "serializer.hpp"
#include "mutex.hpp"
#include "criticalsection.hpp"
#include "timer.h"
#include "binary.h"
#include "stringutil.hpp"
#include "sir/sir.h"
#include "validus/ValidusLibrary.h"

/*
 * Utility Macros
 */
#define SAFE_DELETE(p) \
  if (p) { \
    delete p; \
    p = NULL; \
  } \

#endif // !_WLIB_H
