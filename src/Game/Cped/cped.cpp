#include "cped.h"



bool CPed::GetPlayer(uintptr_t& address)
{
    address = address;
    return address == NULL ? false : true;
}

bool CPed::Update()
{
    Vehicle = read_mem<uintptr_t>(address + sdk.m_pVehicle);
    PlayerInfo = read_mem<uintptr_t>(address + sdk.m_pInfo);
    uintptr_t weapon = read_mem<uintptr_t>(address + sdk.m_pWeaponManager);
    CurrentWeapon = read_mem<uintptr_t>(weapon + 0x20);
    if (CurrentWeapon) {
        // get ped weapon name if weapoon equipped
        m_sWeaponName = GetCurrentWeaponName();
    }
    m_fHealth = read_mem<float>(address + sdk.m_fHealth);
    m_pVecLocation = read_mem<Vector3>(address + sdk.m_vecLocation);

    if (IsDead()) {
        return false;
    }
    if (IsPlayer()) {
        //auto it = PlayerIdToName.find(m_pId);
        //if (it != PlayerIdToName.end() && m_pId != -1) {

        //}
        //else {
        //    m_pName = 
        //}
        m_pId = read_mem<int>(address + sdk.m_pNetId);
    }

    //m_isVisible = IsVisible();
    m_fArmor = read_mem<float>(address + sdk.m_fArmor);
    m_fMaxHealth = read_mem<float>(address + sdk.m_fHealthMax);
    m_bMatrix = read_mem<Matrix>(address + sdk.m_pBoneMatrix);
   
    return true;
}


std::string CPed::GetCurrentWeaponName() {
    auto WeaponHash = read_mem<uint32_t>(CurrentWeapon + 0x10);
    if (WeaponHash) {
        std::string weapon_name = get_weapon_name(WeaponHash);
        return weapon_name;
    }
    else {
        return "unknown weapon";
    }
}



bool CPed::IsVisible() {
    BYTE VisibilityFlag = read_mem<BYTE>(address + sdk.m_pVisibleFlag);
    if (VisibilityFlag == 36 || VisibilityFlag == 0 || VisibilityFlag == 4)
        return false;

    return true;
}

bool CPed::IsDead()
{
    return m_fHealth <= 0.f || Vec3_Empty(m_pVecLocation);
}

bool CPed::IsPlayer()
{
    return PlayerInfo != NULL;
}

bool CPed::InVehicle()
{
    return read_mem<uint8_t>(address + sdk.m_bPedTask) & (uint8_t)ePedTask::TASK_DRIVING;
}

//Vector3 CPed::GetBonePosVec3() {
//    read_mem<uint64_t>(address. )
//}

bool CPed::IsGod()
{
    return read_mem<bool>(address + sdk.m_bGodMode) == true;
}

//if (GlobalsConfig.GodMode && !pLocal->IsGod())
//{
//    Logging::debug_print("activated godmode");
//    write_mem<bool>(pLocal->address + sdk.m_bGodMode, true);
//}
//else if (!GlobalsConfig.GodMode && pLocal->IsGod()) {
//    Logging::debug_print("deactivated godmode");
//    write_mem<bool>(pLocal->address + sdk.m_bGodMode, false);
//}
void CPed::SetInvisibleMode(BYTE mode){
    
 write_mem<bool>(address + sdk.m_bInvisibleMode, mode);
       
}
struct Bone {
    Vector3 pos{};
    int junk0{};
};

struct AllBone {
    Bone bone[9]{};
};

Vector3 CPed::GetBonePosition(int bone_id)
{
    auto boneMatrix = read_mem<Matrix>(address + sdk.m_pBoneMatrix);
    Vector3 bonePosition = read_mem<Vector3>(address + (sdk.m_pBoneList+0x10*bone_id));
    Vector3 BonePos = Vec3_Transform(&bonePosition, &boneMatrix);
    return BonePos;
}

Vector3 CPed::GetBoneByID(BoneId id)
{
    Vector3 pos = read_mem<Vector3>(address + sdk.m_pBoneList + (id * 0x10));

    return Vec3_Transform(&pos, &m_bMatrix);
}

std::vector<Vector3> CPed::GetBoneList()
{
    std::vector<Vector3> list;
    AllBone b_list = read_mem<AllBone>(address + sdk.m_pBoneList), * BoneList = &b_list;

    for (int b = 0; b < 9; b++)
    {
        if (Vec3_Empty(BoneList->bone[b].pos))
            list.push_back(Vector3());

        Vector3 pos = BoneList->bone[b].pos;
        list.push_back(Vec3_Transform(&pos, &m_bMatrix));
    }

    return list;
}

Vector3 CPed::GetVelocity()
{
    return InVehicle() ? read_mem<Vector3>(Vehicle + sdk.m_vecVehicleVelocity) : read_mem<Vector3>(address + sdk.m_vecVelocity);
}