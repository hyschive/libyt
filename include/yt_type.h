#ifndef __YT_TYPE_H__
#define __YT_TYPE_H__



/*******************************************************************************
/
/  Data types used by libyt
/
********************************************************************************/


// short names for unsigned types
typedef unsigned int       uint;
typedef unsigned long int  ulong;


// enumerate types
enum yt_verbose { YT_VERBOSE_OFF=0, YT_VERBOSE_INFO=1, YT_VERBOSE_WARNING=2, YT_VERBOSE_DEBUG=3 };
enum yt_ftype   { YT_FTYPE_UNKNOWN=0, YT_FLOAT=1, YT_DOUBLE=2 };


// structures
#include "yt_type_param_libyt.h"
#include "yt_type_param_yt.h"
#include "yt_type_grid.h"



#endif // #ifndef __YT_TYPE_H__
