#include "visual.h"
#include "../../globals.h"
#include "aim.h"


//the calculation :
//float HeadToNeck = bScreen[NECK].y - bScreen[HEAD].y;
//float pTop = bScreen[HEAD].y - (HeadToNeck * 2.5f);
//float pBottom = bScreen[LEFTFOOT].y > bScreen[RIGHTFOOT].y ? bScreen[LEFTFOOT].y : bScreen[RIGHTFOOT].y;
//float pHeight = pBottom - pTop;
//float pWidth = pHeight / 3.5f;
//float bScale = pWidth / 1.5f
ESPResources Visual::ComputeESPResources() {
    ESPResources res;
    res.headToNeck = EntityScreen.Bones[NECK].y - EntityScreen.Bones[HEAD].y;
    res.pTop = EntityScreen.Bones[HEAD].y - (res.headToNeck * 2.5f);
    res.pBottom = (EntityScreen.Bones[LEFTFOOT].y > EntityScreen.Bones[RIGHTFOOT].y) ? EntityScreen.Bones[LEFTFOOT].y : EntityScreen.Bones[RIGHTFOOT].y;
    res.pHeight = res.pBottom - res.pTop;
    res.pWidth = res.pHeight / 3.5f;
    res.bScale = res.pWidth / 1.5f;
    return res;
}


//void Visual::DrawBox(const Vector2& base, const ESPResources& res, ImColor color) {
//    if (GlobalsConfig.ESP_BoxFilled) {
//        ImGui::GetBackgroundDrawList()->AddRectFilled(
//            ImVec2(base.x - res.pWidth, res.pTop),
//            ImVec2(base.x + res.pWidth, res.pBottom),
//            GlobalsConfig.ESP_Filled_COLOR);
//    }
//    switch (GlobalsConfig.ESP_BoxType) {
//    case 0: // Simple Box
//        interact.DrawLine(ImVec2(base.x - res.pWidth, res.pTop), ImVec2(base.x + res.pWidth, res.pTop), color, 1.f);
//        interact.DrawLine(ImVec2(base.x - res.pWidth, res.pTop), ImVec2(base.x - res.pWidth, res.pBottom), color, 1.f);
//        interact.DrawLine(ImVec2(base.x + res.pWidth, res.pTop), ImVec2(base.x + res.pWidth, res.pBottom), color, 1.f);
//        interact.DrawLine(ImVec2(base.x - res.pWidth, res.pBottom), ImVec2(base.x + res.pWidth, res.pBottom), color, 1.f);
//        break;
//    case 1: // Cornered Box
//        interact.DrawLine(ImVec2(base.x - res.pWidth, res.pTop), ImVec2(base.x - res.pWidth + res.bScale, res.pTop), color, 1.f);
//        interact.DrawLine(ImVec2(base.x + res.pWidth, res.pTop), ImVec2(base.x + res.pWidth - res.bScale, res.pTop), color, 1.f);
//        interact.DrawLine(ImVec2(base.x - res.pWidth, res.pTop), ImVec2(base.x - res.pWidth, res.pTop + res.bScale), color, 1.f);
//        interact.DrawLine(ImVec2(base.x - res.pWidth, res.pBottom), ImVec2(base.x - res.pWidth, res.pBottom - res.bScale), color, 1.f);
//        interact.DrawLine(ImVec2(base.x + res.pWidth, res.pTop), ImVec2(base.x + res.pWidth, res.pTop + res.bScale), color, 1.f);
//        interact.DrawLine(ImVec2(base.x + res.pWidth, res.pBottom), ImVec2(base.x + res.pWidth, res.pBottom - res.bScale), color, 1.f);
//        interact.DrawLine(ImVec2(base.x - res.pWidth, res.pBottom), ImVec2(base.x - res.pWidth + res.bScale, res.pBottom), color, 1.f);
//        interact.DrawLine(ImVec2(base.x + res.pWidth, res.pBottom), ImVec2(base.x + res.pWidth - res.bScale, res.pBottom), color, 1.f);
//        break;
//    default:
//        break;
//    }
//    
//}

//bool Visual::BoneToScreen(CPed* pEntity, Matrix ViewMatrix) {
//    
//    
//    // Convert the entity's base world location to screen coordinates
//    if (!WorldToScreen(Esp_s.ViewMatrix, pEntity->m_pVecLocation, EntityScreen.pBase))
//    {
//        return false;
//    }
//    std::vector<Vector3> BoneList = pEntity->GetBoneList();
//    // Convert each bone's world coordinates to screen coordinates.
//    for (int j = 0; j < BoneList.size(); j++)
//    {
//        Vector2 screenPos{};
//        if (Vec3_Empty(BoneList[j]))
//        {
//            Logging::debug_print("vec bone list empty");
//            continue;
//        }
//        else if (!WorldToScreen(Esp_s.ViewMatrix, BoneList[j], screenPos))
//        {
//            Logging::debug_print("not in screen pos");
//            continue;
//        }
//        EntityScreen.Bones.push_back(screenPos);
//    }
//    // Check if the conversion succeeded for all bones
//    if (BoneList.size() != EntityScreen.Bones.size())
//    {
//        Logging::debug_print("size different bScreen and BoneList");
//        return false;
//    }
//    EntityScreen.BoneList = BoneList;
//}



//Overlay* overlay = new Overlay;

bool Visual::InitializeEsp() {
    static CPed target = CPed();
   

    Screen.Center = Vector2(GlobalsConfig.GameRect.right / 2.f, GlobalsConfig.GameRect.bottom / 2.f);
    Screen.ViewMatrix = read_mem<Matrix>(Globals::ViewPort + 0x24C);
    //Draw::Circle(Vector2D(width / 2, height / 2), 20, radius, Color::FromImColor(Settings::ESP::FOVCrosshair::color.Color()));

    if (!Visual::pLocal->Update()) {
        return false;
    }
}


//a
void Visual::RenderEsp() {
    InitializeEsp();
    static CPed target = CPed();
    float MinFov = 9999.f;
    float MinDistance = 9999.f;

    for (auto& entity : Globals::EntityList)
    {
        CPed* pEntity = &entity;

        // Checks
        if (!pEntity->Update()) 
        {
            continue;
        }

        
         
        float pDistance = GetDistance(pEntity->m_pVecLocation, pLocal->m_pVecLocation);

        if (pDistance > GlobalsConfig.ESP_MaxDistance) 
        {
            continue;
        }
        else if (!GlobalsConfig.ESP_NPC && !pEntity->IsPlayer()) 
        {
            continue;
        }

      

        // Calculate Bones to the screen to calculate 
        // if its on view matrix or no to-do ( separate this from this function for better clarity 
        Vector2 pBase{};
        if (!WorldToScreen(Screen.ViewMatrix, pEntity->m_pVecLocation, pBase))
        {
            continue;
        }
        std::vector<Vector2> bScreen;
        std::vector<Vector3> BoneList = pEntity->GetBoneList();
        // WorldToScreen and push back the valid bones
        for (int j = 0; j < BoneList.size(); j++)
        {
            Vector2 vOut{};

            if (Vec3_Empty(BoneList[j]))
            {
                continue;
            }
            else if (!WorldToScreen(Screen.ViewMatrix, BoneList[j], vOut))
            {
                continue;
            }
            bScreen.push_back(vOut);
        }
        if (BoneList.size() != bScreen.size())
        {
            continue;
        }
        // initialize bone from entityscreen so we can use compute_esp
        EntityScreen.Bones = bScreen;
        // END OF EntityScreen


        // ESP Resource
        ESPResources EspPart = Visual::ComputeESPResources();

        // if entityIsVisible
        ImColor color = pEntity->IsPlayer() ? GlobalsConfig.ESP_PLAYER_COLOR : GlobalsConfig.ESP_NPC_COLOR;
        // GodMode Checker
        bool entityIsVisible = pEntity->IsVisible();
        if (pEntity->IsGod()) 
        {
            color = GlobalsConfig.ESP_GOD_COLOR;
        }
        if (!entityIsVisible) {
            color = GlobalsConfig.ESP_VISIBLE_COLOR;
        }

        // Line
        if (GlobalsConfig.ESP_Line) 
        {
            Interact.DrawLine(ImVec2(GlobalsConfig.GameRect.right / 2.f, GlobalsConfig.GameRect.bottom), ImVec2(pBase.x, EspPart.pBottom), color, 1.f);
        }
        // BoxEsp
        if (GlobalsConfig.ESP_Box)
        {
            if (GlobalsConfig.ESP_BoxFilled)
            {
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pBase.x - EspPart.pWidth, EspPart.pTop), ImVec2(pBase.x + EspPart.pWidth, EspPart.pBottom), GlobalsConfig.ESP_Filled_COLOR);
            }
            switch (GlobalsConfig.ESP_BoxType)
            {
            case 0: // simple
                Interact.DrawLine(ImVec2(pBase.x - EspPart.pWidth, EspPart.pTop), ImVec2(pBase.x + EspPart.pWidth, EspPart.pTop), color, 1.f);
                Interact.DrawLine(ImVec2(pBase.x - EspPart.pWidth, EspPart.pTop), ImVec2(pBase.x - EspPart.pWidth, EspPart.pBottom), color, 1.f);
                Interact.DrawLine(ImVec2(pBase.x + EspPart.pWidth, EspPart.pTop), ImVec2(pBase.x + EspPart.pWidth, EspPart.pBottom), color, 1.f);
                Interact.DrawLine(ImVec2(pBase.x - EspPart.pWidth, EspPart.pBottom), ImVec2(pBase.x + EspPart.pWidth, EspPart.pBottom), color, 1.f);
                break;
            case 1: // cornerd
                Interact.DrawLine(ImVec2((pBase.x - EspPart.pWidth), EspPart.pTop), ImVec2((pBase.x - EspPart.pWidth) + EspPart.bScale, EspPart.pTop), color, 1.f); // top
                Interact.DrawLine(ImVec2((pBase.x + EspPart.pWidth), EspPart.pTop), ImVec2((pBase.x + EspPart.pWidth) - EspPart.bScale, EspPart.pTop), color, 1.f);
                Interact.DrawLine(ImVec2(pBase.x - EspPart.pWidth, EspPart.pTop), ImVec2(pBase.x - EspPart.pWidth, EspPart.pTop + EspPart.bScale), color, 1.f); // left
                Interact.DrawLine(ImVec2(pBase.x - EspPart.pWidth, EspPart.pBottom), ImVec2(pBase.x - EspPart.pWidth, EspPart.pBottom - EspPart.bScale), color, 1.f);
                Interact.DrawLine(ImVec2(pBase.x + EspPart.pWidth, EspPart.pTop), ImVec2(pBase.x + EspPart.pWidth, EspPart.pTop + EspPart.bScale), color, 1.f); // right
                Interact.DrawLine(ImVec2(pBase.x + EspPart.pWidth, EspPart.pBottom), ImVec2(pBase.x + EspPart.pWidth, EspPart.pBottom - EspPart.bScale), color, 1.f);
                Interact.DrawLine(ImVec2((pBase.x - EspPart.pWidth), EspPart.pBottom), ImVec2((pBase.x - EspPart.pWidth) + EspPart.bScale, EspPart.pBottom), color, 1.f); // bottom
                Interact.DrawLine(ImVec2((pBase.x + EspPart.pWidth), EspPart.pBottom), ImVec2((pBase.x + EspPart.pWidth) - EspPart.bScale, EspPart.pBottom), color, 1.f);
                break;
            default:
                break;
            }
        }
        //CPed* pEntity = &entity;


        // Skeleton
        if (GlobalsConfig.ESP_Skeleton)
        {
            // Head
           
            Interact.Circle(ImVec2(bScreen[HEAD].x, bScreen[HEAD].y), EspPart.headToNeck, GlobalsConfig.ESP_Skeleton_COLOR);

            Vector3 bList[][2] = { { BoneList[NECK], BoneList[HIP] }, { BoneList[NECK], BoneList[LEFTHAND] }, { BoneList[NECK], BoneList[RIGHTHAND] },
                { BoneList[HIP], BoneList[LEFTANKLE] }, { BoneList[HIP], BoneList[RIGHTANKLE] } };

            // Body
            for (int j = 0; j < 5; j++)
            {
                Vector2 ScreenB1{}, ScreenB2{};
                if (Vec3_Empty(bList[j][0]) || Vec3_Empty(bList[j][1]))
                {
                    break;
                }

                if (!WorldToScreen(Screen.ViewMatrix, bList[j][0], ScreenB1) || !WorldToScreen(Screen.ViewMatrix, bList[j][1], ScreenB2))
                {
                    break;
                }
                Interact.DrawLine(ImVec2(ScreenB1.x, ScreenB1.y), ImVec2(ScreenB2.x, ScreenB2.y), GlobalsConfig.ESP_Skeleton_COLOR, 1.f);
            }
        }

        // Healthbar
        if (GlobalsConfig.ESP_HealthBar)
        {
            Interact.HealthBar((pBase.x - EspPart.pWidth) - 5.f, EspPart.pBottom, 2.f, -EspPart.pHeight, pEntity->m_fHealth, pEntity->m_fMaxHealth);

            if (pEntity->m_fArmor > 0.f)
            {
                Interact.ArmorBar((pBase.x + EspPart.pWidth) + 3.f, EspPart.pBottom, 2.f, -EspPart.pHeight, pEntity->m_fArmor, 100);
            }
        }

        // Distance
        if (GlobalsConfig.ESP_Distance)
        {
            std::string DistStr = std::to_string((int)pDistance) + "m";
            Interact.StringEx(ImVec2(pBase.x - ImGui::CalcTextSize(DistStr.c_str()).x / 2.f, EspPart.pBottom), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), DistStr.c_str());
        }

        // Name
        //if (GlobalsConfig.ESP_Name)
        //{
        //     //- wong offset? 
        //    std::string pName = read_mem<std::string>(pEntity->PlayerInfo + 0xFC);
        //    StringEx(ImVec2(pBase.x - ImGui::CalcTextSize(pName.c_str()).x / 2.f, pTop - 15.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), pName.c_str());
        //    
        //}
        if (GlobalsConfig.ESP_Weapon)
        {
             //- wong offset? 
            int entityId = pEntity->m_pId;
            std::string entityWeaponName = pEntity->m_sWeaponName;
           /* std::string entityNameWithWeapon = std::to_string(entityId)  + entityWeaponName;*/
            Interact.StringEx(ImVec2(pBase.x - ImGui::CalcTextSize(entityWeaponName.c_str()).x / 2.f, EspPart.pTop - 15.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), entityWeaponName.c_str());
            
        }


        // AimBot

        if (GlobalsConfig.AimBot||GlobalsConfig.AimSilent)
        {
            // jika musuh keliatan dan gak aktifin only visible
            // maka skip
            if (GlobalsConfig.Aim_OnlyVisible && !entityIsVisible) {
                continue;
            }
        
            if (GlobalsConfig.Aim_MaxDistance < pDistance)
            {

                continue;
            }
            else if (!GlobalsConfig.Aim_NPC && !pEntity->IsPlayer())
            {
                continue;
            }

        
            Vector2 fov_check{};
            
        
                
               
            if (!WorldToScreen(Screen.ViewMatrix, BoneList[GlobalsConfig.Aim_Bone], fov_check))
            {
                continue;
            }
            float FOV = abs((Screen.Center - fov_check).Length());
            if (FOV < GlobalsConfig.Aim_Fov)
            {
                if (pDistance < MinDistance)
                {
                    target = entity;
                    MinDistance = pDistance;
                      
                  
                    //break;
                }
                //target = entity;
                    

                //FiveM::AimBot(target);
            }
     
            
            
        }
    }
    if (target.address != NULL) {
        Aim::AimSetup(target);
    }
   
}
