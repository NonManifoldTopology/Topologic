#pragma once

#ifdef TOPOLOGIC_EXTENSIONS_EXPORTS
#define TOPOLOGIC_EXTENSIONS_API __declspec(dllexport)
#else
#define TOPOLOGIC_EXTENSIONS_API __declspec(dllimport)
#endif