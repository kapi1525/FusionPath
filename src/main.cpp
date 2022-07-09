#include "slim_windows.h"
#include <string>   // For std::wstring
#include <cstdio>   // For std::wprintf



std::wstring try_get_key_value(HKEY base_key, std::wstring key_path, std::wstring value_name) {
    DWORD buffer_size = MAX_PATH;
    wchar_t buffer[MAX_PATH];

    LSTATUS status = RegGetValueW(
        base_key,
        key_path.c_str(),
        value_name.c_str(),
        RRF_RT_REG_SZ | RRF_SUBKEY_WOW6432KEY,
        NULL,
        buffer,
        &buffer_size
    );

    if(!status) {
        return std::wstring(buffer);
    } else {
        return std::wstring(L"");
    }
}


// wmain instead of main since this is a UNICODE application
int wmain(int argc, wchar_t const* argv[]) {
    // HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Clickteam\Fusion 2.5\Settings    InstallPath
    std::wprintf(L"%s\n", try_get_key_value(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Clickteam\\Fusion 2.5\\Settings", L"InstallPath").c_str());

    return 0;
}
