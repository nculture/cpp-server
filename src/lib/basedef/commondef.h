#ifndef BASEDEF_COMMONDEF_H
#define BASEDEF_COMMONDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include <stdexcept>
#include <string>
#include <cstding>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <algorithm>
#include <time.h>
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <memory>

namespace basedef {

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define charptr char*
#define const_charptr const char*

#define shared_ptr std::tr1::shared_ptr

typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;

typedef INT_PTR intptr;
typedef UINT_PTR uintptr;

typedef UINT_PTR SOCKET;
typedef int socklen_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

#define ARRAYSZ(v) (sizeof(v) / sizeof(v[0]))
#define ARRAY_SIZE(v) (sizeof(v) / sizeof(v[0]))

#undef min
#define min min
#undef max
#define max max

template <typename T>
inline const T& min(const T& a, const T& b)
{
	return b < a ? b : a;
}

template <typename T>
inline const T& max(const T& a, const T& b)
{
	return a < b ? b : a;
}

inline int32 getProcessPID()
{
	return (int32)GetCurrentProcessId();
}

inline uint64 genUUID64()
{
	static uint64 tv = (uint64)getSystemTime();
	static uint32 lastNum = 0;

	uint64 now = (uint64)getSystemTime();
	if(now != tv)
	{
		tv = now;
		lastNum = 0;
	}

	static uint64 rnd = 0;
	if(rnd == 0)
	{
		srand(getSystemTime());
		rnd = ((uint64)rand() % 65535) + 1;
	}

	return (rnd << 48) + (tv << 16) + lastNum++;
}

inline void sleep(uint32 ms)
{
	::Sleep(ms);
}

}

#endif