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

u64 getInt(const nlohmann::json::value_type &idValue) {
    switch(idValue.type()){
        case nlohmann::detail::value_t::string:
            return std::stoi(idValue.get<std::string>());
            break;
        case nlohmann::detail::value_t::number_integer:
        case nlohmann::detail::value_t::number_unsigned:
            return idValue.get<u64>();
            break;
        default: break;
    }
    return -1;
}

std::string getString(const nlohmann::json::value_type &strValue) {
    if (strValue.is_string())
        return strValue.get<std::string>();
    else
        return "";
}
