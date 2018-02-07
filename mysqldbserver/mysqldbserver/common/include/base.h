#ifndef _BASE_H_
#define _BASE_H_

#include <vector>
#include <string>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef False
#define	False  0
#endif

#ifndef True
#define	True   1
#endif

struct timeval;

#define I64FMTD "%llu"

#ifdef WIN32
typedef __int64				int64;
typedef __int32				int32;
typedef __int16				int16;
typedef __int8				int8;
typedef unsigned __int64	uint64;
typedef unsigned __int32	uint32;
typedef unsigned __int16	uint16;
typedef unsigned __int8		uint8;
#else
typedef __int64_t   int64;
typedef __int32_t   int32;
typedef __int16_t   int16;
typedef __int8_t    int8;
typedef __uint64_t  uint64;
typedef __uint32_t  uint32;
typedef __uint16_t  uint16;
typedef __uint8_t   uint8;
#endif


#ifdef WIN32
#define FILE_MAX_PATH  MAX_PATH
#define strcasecmp strcmp
#define strncasecmp strncmp
#define	snprintf _snprintf
#define	sleep Sleep
#define atoll _atoi64
#endif


/************************************************************************/
/* 时间点判断函数                                                                     */
/************************************************************************/
//一天的秒数
#define SECONDS_OF_DAY   (60*60*24)
//一小时的秒数
#define SECONDS_OF_HOUR   (60*60)
// 一分钏的秒数
#define SECONDS_OF_MIN		(60)
//一个星期的天数，具体星期1-6分别为对应数值，而星期日为0
#define WEEK_DAY_NUM                     7
//time函数起始时间的星期数，即1970.1.1日为星期四
#define WEEK_DAY_OFC_OMPUTETIME_BEGIN              4


#endif



