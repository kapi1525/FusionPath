#include "slim_windows.hpp"

#include <cstdio>
#include <string>

#include "registry_key.hpp"



// wmain instead of main since this is a UNICODE application
int wmain(int argc, wchar_t const* argv[]) {
    (void)argc;
    (void)argv;

    // HKEY_LOCAL_MACHINE/SOFTWARE/WOW6432Node/Clickteam/Fusion 2.5/Settings    InstallPath
    fusionpath::registry_key key;

    key.open_child_key_path(L"SOFTWARE/Clickteam/Fusion 2.5/Settings");
    if(key.is_valid()) {
        std::wprintf(L"%s\n", key.get_value(L"InstallPath").c_str());
    }
    
    return 0;
}
