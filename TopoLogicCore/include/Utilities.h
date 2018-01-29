#pragma once

#ifdef TOPOLOGIC_EXPORTS
#define TOPOLOGIC_API __declspec(dllexport)
#else
#define TOPOLOGIC_API __declspec(dllimport)
#endif