#pragma once

#ifdef TOPOLOGIC_UTILITIES_EXPORTS
#define TOPOLOGIC_UTILITIES_API __declspec(dllexport)
#else
#define TOPOLOGIC_UTILITIES_API __declspec(dllimport)
#endif