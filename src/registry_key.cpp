#include "registry_key.hpp"
#include "debug.hpp"



registry_key::registry_key() {
    handle = HKEY_LOCAL_MACHINE;
}

registry_key::registry_key(HKEY base_key) {
    handle = base_key;
}

registry_key::~registry_key() {
    close(handle);
}



void registry_key::open_child_key(std::wstring key_name) {
    HKEY temp_key = nullptr;

    LSTATUS status = RegOpenKeyExW(
        handle,
        key_name.c_str(),
        0,
        key_access_rights,
        &temp_key
    );

    if(status != ERROR_SUCCESS) {
        debug_log(L"RegOpenKeyExW failed: %d", status);
        temp_key = nullptr;
    }

    close(handle);
    handle = temp_key;
}


void registry_key::open_child_key_path(std::wstring key_path_name) {
    wchar_t* path = _wcsdup(key_path_name.c_str());

    constexpr const wchar_t delim[] = L"/\\";

    wchar_t* buffer;
    wchar_t* token = wcstok_s(path, delim, &buffer);

    while (token) {
        debug_log("Openning key %s." , token);
        open_child_key(token);
        if(!is_valid()) {
            break;
        }

        token = wcstok_s(NULL, delim, &buffer);
    }

    free(path);
}


std::wstring registry_key::get_value(std::wstring value) {
    LSTATUS status;

    // Check if value is correct type and get its size.
    DWORD type;
    DWORD size;

    status = RegQueryValueExW(
        handle,
        value.c_str(),
        NULL,
        &type,
        NULL,
        &size
    );

    if(status != ERROR_SUCCESS) {
        debug_log(L"RegQueryValueExW failed: %d.", status);
        return L"";
    }

    if(type != REG_SZ) {
        debug_log(L"Registry value has incorect type: %d.", type);
        return L"";
    }


    // Get actual string
    size += 1;  // To be safe if string isnt null terminated for some reason
    wchar_t* buffer = (wchar_t*)malloc(size);

    status = RegGetValueW(
        handle,
        NULL,
        value.c_str(),
        RRF_RT_ANY,
        NULL,
        buffer,
        &size
    );

    if(status != ERROR_SUCCESS) {
        debug_log(L"RegGetValueW failed: %d.", status);
        free(buffer);
        return L"";
    }

    std::wstring str(buffer);
    free(buffer);

    return str;
}



// if handle is NULL registry_key is not valid
bool registry_key::is_valid() {
    return handle;
}



void registry_key::close(HKEY key) {
    switch ((int)key) {

    // Ignore predefined keys (and null) as they cannot be closed
    case NULL:
    case (int)HKEY_CLASSES_ROOT:
    case (int)HKEY_CURRENT_CONFIG:
    case (int)HKEY_CURRENT_USER:
    case (int)HKEY_LOCAL_MACHINE:
    case (int)HKEY_PERFORMANCE_DATA:
    case (int)HKEY_PERFORMANCE_NLSTEXT:
    case (int)HKEY_PERFORMANCE_TEXT:
    case (int)HKEY_USERS:
        break;
    
    default: {
            LSTATUS status = RegCloseKey(key);
            if(status != ERROR_SUCCESS) {
                std::fprintf(stderr, "Failed to close handle to registry key! Error code: %i\n", status);
                abort();
            }
        }
        break;
    }
}