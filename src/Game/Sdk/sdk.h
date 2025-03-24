#pragma once

#include <cstdint>

// for more detailes bone esp
//enum FragInst
//{
//    b2060 = 0x1400,
//    b2189 = 0x1400,
//    b2372 = 0x1400,
//    b2545 = 0x1450,
//    b2612 = 0x1450,
//    b2699 = 0x1450,
//    b2802 = 0x1430,
//    b2944 = 0x1430
//};


class SDK {
public:
	uintptr_t world;
	uintptr_t replay;
	uintptr_t viewport;
	uintptr_t camera;
    uintptr_t handlebullet;

    // pointer
    uintptr_t m_pVehicle = 0xD10;
    uintptr_t m_pInfo = 0x10A8;
    uintptr_t m_pWeaponManager = 0x10B8;


    // offset
    uintptr_t m_pBoneMatrix = 0x60;
    uintptr_t m_vecLocation = 0x90;
    uintptr_t m_bGodMode = 0x189;
    uintptr_t m_bInvisibleMode = 0x2C;
    uintptr_t m_fHealth = 0x280;
    uintptr_t m_fHealthMax = 0x284;
    uintptr_t m_vecVelocity = 0x300;
    uintptr_t m_pBoneList = 0x410;
    uintptr_t m_fArmor = 0x150C;
    uintptr_t m_bPedTask = 0x144B;

    // player info
    uintptr_t m_CName = 0xE0;
    uintptr_t m_iWantedLev = 0x8D8;
    uintptr_t m_pNetId = 0x88;
    uintptr_t m_pVisibleFlag = 0;

    // vehicle offset
    uintptr_t m_vecVehicleVelocity = 0;
    uintptr_t m_fVehicleHealth = 0;
    uintptr_t m_fVehicleEngineHealth = 0;

    // weapon
    uintptr_t m_fWeaponSpread = 0;
    uintptr_t m_fWeaponDamage = 0;
    uintptr_t m_fWeaponRecoil = 0x2F4;
    uintptr_t m_fWeaponRange = 0x28C;

    bool InitOffset();
};

enum class ePedTask {
    TASK_NONE,
    TASK_FOOT = 1 << 4,
    TASK_UNK = 1 << 5,
    TASK_DRIVING = 1 << 6
};

enum BoneId : int
{
    HEAD = 0,
    LEFTFOOT = 1,
    RIGHTFOOT = 2,
    LEFTANKLE = 3,
    RIGHTANKLE = 4,
    LEFTHAND = 5,
    RIGHTHAND = 6,
    NECK = 7,
    HIP = 8
};




extern SDK sdk;
