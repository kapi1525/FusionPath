#pragma once

#include <cstdio>



#ifdef _DEBUG
    #define debug_log(wstr, ...) std::wprintf(wstr L"\n", __VA_ARGS__)
#else
    #define debug_log(wstr, ...)
#endif