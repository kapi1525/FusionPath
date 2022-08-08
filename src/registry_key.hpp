#pragma once

#include "slim_windows.hpp"
#include <string>
#include <cstring>
#include <cwchar>


class registry_key {
public:
    registry_key();
    registry_key(HKEY base_key);
    ~registry_key();

    void open_child_key(std::wstring key_name);
    void open_child_key_path(std::wstring key_path_name);
    std::wstring get_value(std::wstring value);

    bool is_valid();

    constexpr static int key_access_rights = KEY_QUERY_VALUE | KEY_WOW64_32KEY;

protected:
    void close(HKEY key);

    HKEY handle;
};
