#include "utl.hpp"
#include <switch.h>

bool canSwkbd() {
    if (appletGetAppletType() == AppletType_Application) return true;
    u64 totalMemorySize;
    u64 usedMemorySize;
    if (R_FAILED(svcGetInfo(&totalMemorySize, InfoType_TotalMemorySize, CUR_PROCESS_HANDLE, 0)))
        return true;
    if (R_FAILED(svcGetInfo(&usedMemorySize, InfoType_UsedMemorySize, CUR_PROCESS_HANDLE, 0)))
        return true;
    if(totalMemorySize - usedMemorySize < 0x8000000)
        return false;
    return true;
}

u64 getInt(const nlohmann::json &idValue) {
    switch(idValue.type()){
        case nlohmann::detail::value_t::string:
            printf("int: %d\n", std::stoi(idValue.get<std::string>()));
            return std::stoi(idValue.get<std::string>());
            break;
        case nlohmann::detail::value_t::number_integer:
        case nlohmann::detail::value_t::number_unsigned:
            return idValue.get<u64>();
            break;
        default: break;
    }
    printf("int: -1\n");
    return -1;
}

std::string getString(const nlohmann::json &strValue) {
    if (strValue.is_string()) {
        auto val = strValue.get<std::string>();
        printf("str: %s\n", val.c_str());
        return val;
    }
    printf("str:\n");
    return "";
}
