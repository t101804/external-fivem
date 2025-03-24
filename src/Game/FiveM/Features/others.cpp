#include "../../globals.h"
#include "others.h"

void Others::GodMode() {
    CPed* pLocal = &Globals::local;
    if (GlobalsConfig.GodMode && !pLocal->IsGod())
    {
        Logging::debug_print("activated godmode");
        write_mem<bool>(pLocal->address + sdk.m_bGodMode, true);
    }
    else if (!GlobalsConfig.GodMode && pLocal->IsGod()) {
        Logging::debug_print("deactivated godmode");
        write_mem<bool>(pLocal->address + sdk.m_bGodMode, false);
    }
}

void Others::Invisible() {
    CPed* pLocal = &Globals::local;
    static bool was_invisible = false;
    if (GlobalsConfig.InvisibleMode && !was_invisible)
    {
        Logging::debug_print("activated invisible mode");
        pLocal->SetInvisibleMode(0x1);
        was_invisible = true;
    }
    else if (!GlobalsConfig.InvisibleMode && was_invisible) {
        Logging::debug_print("deactivated invisible mode");
        pLocal->SetInvisibleMode(0x3);
        was_invisible = false;
    }
   
}