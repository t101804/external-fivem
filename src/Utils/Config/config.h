#pragma once
#include "../header.h"
#include "../../Game/Sdk/sdk.h"
#include "../../../ext/ImGui 1.90/imgui.h"
#include <string>

struct Cfg
{
    // System
    bool Run = true;
    bool ShowMenu = false;
    std::string DriverName = "jokowidrv";

    // GameData
    HWND GameHwnd;
    RECT GameRect{};
    POINT GamePoint{};

    // AimBot
    bool AimBot = false;
    bool AimSilent = false;
    bool Aim_NPC = true;
    bool Aim_Prediction = true;
    bool Aim_DrawFov = true;
    bool Aim_OnlyVisible = true;

    float Aim_Fov = 150.f;
    float Aim_Smooth = 1.f;
    float Aim_MaxDistance = 150.f;
    float Aim_Predict = 2000.f;

    BoneId Aim_Bone = HEAD; // Bone
    int Aim_Type = 3; // FOV/Dist/FovDist
    int AimKeyType = 1; // and/or   
    ImColor Fov_color = { 1.f, 1.f, 1.f, 1.f };

    // Visual
    bool ESP = true;
    bool ESP_NPC = true;
    bool ESP_Box = true;
    bool ESP_Weapon = true;
    bool ESP_BoxFilled = false;
    bool ESP_Line = false;
    bool ESP_Name = false;
    bool ESP_Skeleton = true;
    bool ESP_Distance = false;
    bool ESP_HealthBar = true;
    int ESP_BoxType = 1;
    float ESP_MaxDistance = 500.f;
    //float ESP_MinDistance = 9999.f

    // adjusting Visual
    ImColor ESP_NPC_COLOR = { 1.f, 1.f, 1.f, 1.f };
    ImColor ESP_VISIBLE_COLOR = { 1.f, 0.f, 0.f, 1.f };
    ImColor ESP_PLAYER_COLOR = { 1.f, 0.5f, 0.f, 1.f };
    ImColor ESP_GOD_COLOR = { 1.f, 0.f, 0.f, 1.f };
    ImColor ESP_Skeleton_COLOR = { 1.f, 1.f, 1.f, 1.f };
    ImColor ESP_Filled_COLOR = { 0.f, 0.f, 0.f, 0.2f };
    ImColor FOV_User_COLOR = { 1.f, 1.f, 1.f, 1.f };
    ImColor CrosshairColor = { 0.f, 1.f, 0.f, 1.f };

    // Local
    bool GodMode = false;
    bool InvisibleMode = false;
    bool HealMe = false;
    bool NoRecoil = false;
    bool NoSpread = false;

    // adjusting the memory writing of this
    float NoRecoil_Total = 0.f;
    float NoSpread_Total = 0.f;

    // System
    bool StreamProof = false;
    bool Crosshair = false;
    int CrosshairType = 0;
    int CrosshairSize = 3;

    // Key
    int MenuKey = VK_INSERT;
    int AimSilentKey1 = VK_MBUTTON;
    int AimKey0 = VK_XBUTTON2;
    int AimKey1 = VK_LBUTTON;
    int AimSilentKey0 = VK_XBUTTON1;
};

extern Cfg GlobalsConfig;