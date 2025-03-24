#include "aim.h"
#include "../../globals.h"
#include "visual.h"


void Aim::NoSpread() {
    CPed* pLocal = &Globals::local;
    if (GlobalsConfig.NoSpread) {
        float totalSpread = read_mem<float>(pLocal->CurrentWeapon + sdk.m_fWeaponSpread);
        Logging::debug_print("changing this recoil %.2f into %.2f", totalSpread, GlobalsConfig.NoSpread_Total);
        write_mem<float>(pLocal->CurrentWeapon + sdk.m_fWeaponSpread, GlobalsConfig.NoSpread_Total);
        GlobalsConfig.NoSpread = false;

    }
}


void Aim::NoRecoil() {
    CPed* pLocal = &Globals::local;
    if (GlobalsConfig.NoRecoil) {
        float totalRecoil = read_mem<float>(pLocal->CurrentWeapon + sdk.m_fWeaponRecoil);
        Logging::debug_print("changing this recoil %.2f into %.2f", totalRecoil, GlobalsConfig.NoRecoil_Total);
        write_mem<float>(pLocal->CurrentWeapon + sdk.m_fWeaponRecoil, GlobalsConfig.NoRecoil_Total);
        GlobalsConfig.NoRecoil = false;
    }
}

void Aim::RestoreSilent() {
    std::vector<uint8_t> ReWriteTable =
    {
        0xF3, 0x41, 0x0F, 0x10, 0x19,
        0xF3, 0x41, 0x0F, 0x10, 0x41, 0x04,
        0xF3, 0x41, 0x0F, 0x10, 0x51, 0x08
    };

    driver_manager::write_virtual_memory(reinterpret_cast<PVOID>(Globals::BaseAddress + sdk.handlebullet), &ReWriteTable[0], ReWriteTable.size());


    std::vector<uint8_t> AngleReWriteTable =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00
    };

    driver_manager::write_virtual_memory(reinterpret_cast<PVOID>(Globals::BaseAddress + 0x34E), &AngleReWriteTable[0], AngleReWriteTable.size());
}
void Aim::Silent(Vector3 EndBulletPos) {
    static uint64_t HandleBulletAddr = Globals::BaseAddress + sdk.handlebullet;
    static uint64_t AllocPtr = Globals::BaseAddress + 0x34E;
    auto CalculateRelativeOffset = [](uint64_t CurrentAddress, uint64_t TargetAddress, int Offset = 5)
        {
            intptr_t RelativeOffset = static_cast<intptr_t>(TargetAddress - (CurrentAddress + Offset));

            return static_cast<uint32_t>(RelativeOffset);
        };
    union
    {
        float f;
        uint32_t i;
    } EndPosX, EndPosY, EndPosZ;
    EndPosX.f = EndBulletPos.x;
    EndPosY.f = EndBulletPos.y;
    EndPosZ.f = EndBulletPos.z;
    {
        std::vector<uint8_t> ReWriteTable =
        {
            0xE9, 0x00, 0x00, 0x00, 0x00
        };

        uint32_t JmpOffset = CalculateRelativeOffset(HandleBulletAddr, AllocPtr);
        ReWriteTable[1] = static_cast<uint8_t>(JmpOffset & 0xFF);
        ReWriteTable[2] = static_cast<uint8_t>((JmpOffset >> 8) & 0xFF);
        ReWriteTable[3] = static_cast<uint8_t>((JmpOffset >> 16) & 0xFF);
        ReWriteTable[4] = static_cast<uint8_t>((JmpOffset >> 24) & 0xFF);

        driver_manager::write_virtual_memory(reinterpret_cast<PVOID>(HandleBulletAddr), &ReWriteTable[0], ReWriteTable.size());
    }
    {
        uintptr_t currentAddress = (uintptr_t)AllocPtr;

        uintptr_t targetAddress = (uintptr_t)(HandleBulletAddr);

        intptr_t relativeOffset = static_cast<intptr_t>(targetAddress - (currentAddress + 28));

        uint32_t jmpOffset = static_cast<uint32_t>(relativeOffset);

        // Modified instructions
        std::vector<uint8_t> ReWriteTable =
        {
            0x41, 0xC7, 0x01, static_cast<uint8_t>(EndPosX.i), static_cast<uint8_t>(EndPosX.i >> 8), static_cast<uint8_t>(EndPosX.i >> 16), static_cast<uint8_t>(EndPosX.i >> 24),
            0x41, 0xC7, 0x41, 0x04, static_cast<uint8_t>(EndPosY.i), static_cast<uint8_t>(EndPosY.i >> 8), static_cast<uint8_t>(EndPosY.i >> 16), static_cast<uint8_t>(EndPosY.i >> 24),
            0x41, 0xC7, 0x41, 0x08, static_cast<uint8_t>(EndPosZ.i), static_cast<uint8_t>(EndPosZ.i >> 8), static_cast<uint8_t>(EndPosZ.i >> 16), static_cast<uint8_t>(EndPosZ.i >> 24),
            0xF3, 0x41, 0x0F, 0x10, 0x19,
            0xE9, 0x00, 0x00, 0x00, 0x00
        };

        // Replace the last 4 bytes in the vector with the new offset
        ReWriteTable[29] = static_cast<uint8_t>(jmpOffset & 0xFF);
        ReWriteTable[30] = static_cast<uint8_t>((jmpOffset >> 8) & 0xFF);
        ReWriteTable[31] = static_cast<uint8_t>((jmpOffset >> 16) & 0xFF);
        ReWriteTable[32] = static_cast<uint8_t>((jmpOffset >> 24) & 0xFF);
        driver_manager::write_virtual_memory(reinterpret_cast<PVOID>(AllocPtr), &ReWriteTable[0], ReWriteTable.size());
    }
}


Vector3 GetPrediction(CPed& target, CPed& local)
{
    Vector3 vOut{};
    Vector3 m_pVecVelocity = target.GetVelocity();

    if (Vec3_Empty(m_pVecVelocity))
        return Vector3();

    float distance = GetDistance(target.m_pVecLocation, local.m_pVecLocation);
    float BulletSpeed = read_mem<float>(local.CurrentWeapon + 0xE0);

    if (BulletSpeed == 0.f)
        return Vector3();
    else if (BulletSpeed < 100.f)
        BulletSpeed = 1800.f; 


    float time = distance / 2000.f; // berubah2
    vOut = m_pVecVelocity * time;

    return vOut;
}

bool Aim::AimSetup(CPed target)
{
    FiveM Interact;
    static bool SilentAplied = false;
    Vector3 targetPos = target.GetBoneByID(GlobalsConfig.Aim_Bone);

    if (Interact.IsKeyDown(GlobalsConfig.AimKey0) || Interact.IsKeyDown(GlobalsConfig.AimKey1)) {
        if (Vec3_Empty(target.GetBoneByID(GlobalsConfig.Aim_Bone))) {
            return false;
        }
        uintptr_t camera = read_mem<uintptr_t>(Globals::BaseAddress + sdk.camera);
        Vector3 viewAngle = read_mem<Vector3>(camera + 0x3D0); // 0x40
        Vector3 cameraPos = read_mem<Vector3>(camera + 0x60);
        Vector3 predict = GetPrediction(target, Globals::local);
        // maybe in future we need globals this
        Vector3 angle = CalcAngle(cameraPos, targetPos+predict);
        Vector3 delta = angle - viewAngle;
        Vector3 writeAngle = viewAngle + (delta / GlobalsConfig.Aim_Smooth);
        Vector2 targetPosOnScreen{};
        if (!WorldToScreen(Visual::Screen.ViewMatrix, targetPos, targetPosOnScreen))
        {
            return false;
        }
        float ScreenDistance = DistTo(targetPosOnScreen, Visual::Screen.Center);

        if (ScreenDistance > GlobalsConfig.Aim_Fov) {
            return false;
        }
        if (!Vec3_Empty(writeAngle)) {
            // fps and tps camera
            write_mem<Vector3>(camera + 0x40, writeAngle);
            write_mem<Vector3>(camera + 0x3D0, writeAngle);
          
        }
    }
  
    if (GlobalsConfig.AimSilent) {
    /*    if (target.m_fHealth < 100 || target.m_fArmor < 30) {
            Logging::debug_print("skipping this silent because the target already 100% HP or 30 armor");
        }
        else {*/
            // Logging::debug_print("your aiming at this %.2f and this hp: %.2f", target.m_fArmor, target.m_fHealth);
        /*    Logging::debug_print("locking this silent because the target already  %.2f HP", target.m_fHealth);*/

            Aim::Silent(targetPos);
        //}
        
    }

    return true;
}


