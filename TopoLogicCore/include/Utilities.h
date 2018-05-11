#pragma once

#ifdef Topologic_EXPORTS
#define Topologic_API __declspec(dllexport)
#else
#define Topologic_API __declspec(dllimport)
#endif