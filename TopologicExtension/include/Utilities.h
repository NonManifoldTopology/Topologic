#pragma once

#ifdef TOPOLOGIC_EXTENSION_EXPORTS
#define TOPOLOGIC_EXTENSION_API __declspec(dllexport)
#else
#define TOPOLOGIC_EXTENSION_API __declspec(dllimport)
#endif