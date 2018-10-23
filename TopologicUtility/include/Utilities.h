#pragma once

#ifdef TOPOLOGIC_SUPPORT_EXPORTS
#define TOPOLOGIC_SUPPORT_API __declspec(dllexport)
#else
#define TOPOLOGIC_SUPPORT_API __declspec(dllimport)
#endif