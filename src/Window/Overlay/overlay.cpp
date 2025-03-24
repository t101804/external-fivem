#include "overlay.h"
#include <d3d11.h>

void LoadStyle();
//TOPMOST DETECT in many games so dont use it dude
//LONG MenuStyle = WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
//LONG ESPStyle = WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

ID3D11Device* g_pd3dDevice = NULL;
ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
bool CreateDeviceD3D(HWND hWnd);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool Overlay::InitOverlay(const std::wstring targetName, int mode)
{
    if (mode == WINDOW_TITLE || mode == WINDOW_CLASS) {
        TargetHwnd = WINDOW_TITLE ? FindWindowW(nullptr, targetName.c_str()) : FindWindowW(targetName.c_str(), nullptr);

        if (!TargetHwnd) {
            Logging::error_print("cant init overlay, target app not found");
            return false;
        }
        
    }
    else if (mode == PROCESS) {
        TargetHwnd = GetTargetWindow(targetName);

        if (!TargetHwnd) {
            Logging::error_print("cant init overlay, target app not found");
            return false;
        }
    }
    else {
        Logging::error_print("WRONG Mode of initing overlay");
        return false;
    }

    return CreateOverlay();
}



void Overlay::DestroyOverlay()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(Hwnd);
    UnregisterClassA(wc.lpszClassName, wc.hInstance);
}

bool Overlay::CreateOverlay()
{
    // Get ClientSize
    GetClientRect(GlobalsConfig.GameHwnd, &GlobalsConfig.GameRect);
    ClientToScreen(GlobalsConfig.GameHwnd, &GlobalsConfig.GamePoint);

    // Create Overlay
    wc = { sizeof(WNDCLASSEXA), 0, WndProc, 0, 0, NULL, NULL, NULL, NULL, Title, Class, NULL };
    RegisterClassExA(&wc);
    Hwnd = CreateWindowExA(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, wc.lpszClassName, wc.lpszMenuName, WS_POPUP | WS_VISIBLE, GlobalsConfig.GamePoint.x, GlobalsConfig.GamePoint.y, GlobalsConfig.GameRect.right, GlobalsConfig.GameRect.bottom, NULL, NULL, wc.hInstance, NULL);
    SetLayeredWindowAttributes(Hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(Hwnd, &margin);
  
    if (!CreateDeviceD3D(Hwnd))
    {
        CleanupDeviceD3D();
        UnregisterClassA(wc.lpszClassName, wc.hInstance);

        return false;
    }

    ShowWindow(Hwnd, SW_SHOWDEFAULT);
    UpdateWindow(Hwnd);

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    Logging::debug_print("load the imgui styles");
    LoadStyle();

    ImGui_ImplWin32_Init(Hwnd);

    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    return true;
}


HWND Overlay::GetTargetWindow(const std::wstring processName)
{
    DWORD PID = NULL;
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    while (Process32Next(snapShot, &entry))
    {
        if (!processName.compare(entry.szExeFile))
        {
            PID = entry.th32ProcessID;
            break;
        }
    }

    CloseHandle(snapShot);

    HWND hwnd = GetTopWindow(NULL);
    do {
        if (GetWindowLong(hwnd, 8) != 0 || !IsWindowVisible(hwnd))
            continue;
        DWORD ProcessID;
        GetWindowThreadProcessId(hwnd, &ProcessID);
        if (PID == ProcessID)
            return hwnd;
    } while ((hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) != NULL);

    return NULL;
}


bool IsKeyDown(int VK)
{
    return (GetAsyncKeyState(VK) & 0x8000) != 0;
}



template<typename... Args>
void TextCentered(const char* fmt, Args... args) {
    char buffer[512];
    std::snprintf(buffer, sizeof(buffer), fmt, args...);

    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(buffer).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", buffer);
}

//void Debug_overlay::Watermark() {
//    char buffer[128];
//
//    std::snprintf(buffer, sizeof(buffer), "%.3f ms/frame (%.1f FPS) by fivemeow",
//        1000.0 / static_cast<double>(ImGui::GetIO().Framerate),
//        static_cast<double>(ImGui::GetIO().Framerate));
//
//    ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImColor(0.0f, 1.0f, 0.0f), buffer);
//}

void CheckboxMargin(const std::string& text, bool* checkmark, float margin = 11.0f)
{
    // Set the cursor's X position to the desired left margin
    ImGui::SetCursorPosX(margin);
    ImGui::Checkbox(text.c_str(), checkmark);
}

void ColorEditMargin(const std::string& text, float* col, float margin = 11.0f)
{
    // Set the cursor's X position to the desired left margin
    ImGui::SetCursorPosX(margin);
    ImGui::ColorEdit4(text.c_str(), col);
    //ImGui::Checkbox(text.c_str(), checkmark);
}


//void Fov() {
//    ImGui_ImplDX11_NewFrame();
//    ImGui_ImplWin32_NewFrame();
//
//    ImGui::NewFrame();
//
//    const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
//
//    ImGui::GetStyle().AntiAliasedFill = true;
//    ImGui::GetStyle().AntiAliasedLines = true;
//
//    ImGui::Begin("##overlay", nullptr, flags);
//
//    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
//    ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
//
//    ImGui::GetIO().DisplaySize.x;
//    ImGui::GetIO().DisplaySize.y;
//
//}

void Overlay::RenderMenuGui()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImGuiStyle& style = ImGui::GetStyle();
    static float DefaultSpacing = style.ItemSpacing.y;
    static int Index = 0;
    static int BindingID = 0;

    ImGui::SetNextWindowBgAlpha(0.975f);
    ImGui::SetNextWindowSize(ImVec2(725.f, 450.f));

    ImGui::Begin("External FiveM - meowdiocre", &GlobalsConfig.ShowMenu, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
    ImGui::BeginChild("##LeftChild", ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, ImGui::GetContentRegionAvail().y));
    ImGui::Spacing();

    //ImGui::draw
    TextCentered("Visual");
    ImGui::Separator();
    ImGui::Spacing();
    CheckboxMargin("ESP", &GlobalsConfig.ESP);
    TextCentered("ESP Options");
    ImGui::Separator();
    ImGui::Spacing();

    // allign this checkboxes into 3:3

    CheckboxMargin("Box", &GlobalsConfig.ESP_Box);

    CheckboxMargin("BoxFilled", &GlobalsConfig.ESP_BoxFilled);
    CheckboxMargin("Line", &GlobalsConfig.ESP_Line);
    CheckboxMargin("Name", &GlobalsConfig.ESP_Name);
    CheckboxMargin("Skeleton", &GlobalsConfig.ESP_Skeleton);
    CheckboxMargin("Distance", &GlobalsConfig.ESP_Distance);
    CheckboxMargin("HealthBar", &GlobalsConfig.ESP_HealthBar);
    ImGui::Spacing();
    ColorEditMargin("NPC", &GlobalsConfig.ESP_NPC_COLOR.Value.x);
    ColorEditMargin("Player", &GlobalsConfig.ESP_PLAYER_COLOR.Value.x);
    ColorEditMargin("Visible", &GlobalsConfig.ESP_VISIBLE_COLOR.Value.x);
    ImGui::Separator();
    ImGui::Spacing();
    TextCentered("Misc Options");
    CheckboxMargin("God Mode", &GlobalsConfig.GodMode);
    CheckboxMargin("Invisible Mode", &GlobalsConfig.InvisibleMode);
    
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine(); 
    ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetContentRegionAvail().x / 1.0f, ImGui::GetContentRegionAvail().y));
    ImGui::Spacing();
    TextCentered("Aim");
    ImGui::Separator();
    ImGui::Spacing();
    CheckboxMargin("Aimbot", &GlobalsConfig.AimBot);
    CheckboxMargin("SilentAim", &GlobalsConfig.AimSilent);
    ImGui::SliderFloat("Smooth", &GlobalsConfig.Aim_Smooth, 1.f, 20.f);
    CheckboxMargin("Crosshair", &GlobalsConfig.Crosshair);
    ImGui::SetCursorPosX(11.0f);
    const char* OptAimBone[] = {
    "Head", "Left Foot", "Right Foot", "Left Ankle", "Right Ankle",
    "Left Hand", "Right Hand", "Neck", "Hip"
    };
    int currentBoneIndex = static_cast<int>(GlobalsConfig.Aim_Bone);
    if (ImGui::Combo("AimBone", &currentBoneIndex, OptAimBone, IM_ARRAYSIZE(OptAimBone)))
    {
        GlobalsConfig.Aim_Bone = static_cast<BoneId>(currentBoneIndex); // Convert index back to enum
    }
    ImGui::Checkbox("DrawFOV", &GlobalsConfig.Aim_DrawFov);
    // const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags
    ImGui::SliderFloat("FOV", &GlobalsConfig.Aim_Fov, 10.f, 1000.f);
    //Logging::debug_print(GlobalsConfig.Aim_Bone)
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Weapon");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Checkbox("NoRecoil", &GlobalsConfig.NoRecoil);
    ImGui::SliderFloat("NoRecoilAdjust", &GlobalsConfig.NoRecoil_Total, 0.f, 100.f);
    ImGui::Checkbox("NoSpread", &GlobalsConfig.NoSpread);
    ImGui::SliderFloat("NoSpreadAdjust", &GlobalsConfig.NoSpread_Total, 0.f, 100.f);
    ImGui::Separator();
    ImGui::Spacing();
    TextCentered("%.3f ms/frame (%.1f FPS)",
        1000.0 / static_cast<double>(ImGui::GetIO().Framerate),
        static_cast<double>(ImGui::GetIO().Framerate));
    ImGui::EndChild();
    ImGui::End();
}

void Overlay::OverlayLoop()
{
    while (GlobalsConfig.Run)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        OverlayManager("grcWindow");

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
      /*  if (GetKeyState(GlobalsConfig.AimSilentKey1) && !GlobalsConfig.AimSilent) {
            Logging::debug_print("menyalakan aim silent");
            GlobalsConfig.AimSilent = true;

        }
        else if (!GetKeyState(GlobalsConfig.AimSilentKey1) && GlobalsConfig.AimSilent) {
            Logging::debug_print("menonaktifkan aim silent");
            GlobalsConfig.AimSilent = false;
        }*/
        /*ImGui*/
        FiveM::RunCheat();
       /* if (GlobalsConfig.AimBot && GlobalsConfig.Aim_DrawFov) {
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(GlobalsConfig.GameRect.right / 2.f, GlobalsConfig.GameRect.bottom / 2.f), GlobalsConfig.Aim_Fov, Fov_color);
        }*/
        /*fivem->NoRecoil();
        fivem->NoSpread();*/
        //if (GlobalsConfig.ESP) {
        //    //fivem->RenderEsp();
        //}
        //if (GlobalsConfig.ShowMenu) {
        //   RenderMenuGui();
        //}
        //OverlayUserFunction();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }
}
void Overlay::OverlayManager(const char* targetName)
{
    // Window Check
    HWND CheckHwnd = FindWindowA(targetName, nullptr);
    if (!CheckHwnd) {
        GlobalsConfig.Run = false;
        Logging::debug_print("target name not found: ");
        return;
    }

    // StreamProof
    DWORD Flag = NULL;
    GetWindowDisplayAffinity(Hwnd, &Flag);
    if (GlobalsConfig.StreamProof && Flag == WDA_NONE)
        SetWindowDisplayAffinity(Hwnd, WDA_EXCLUDEFROMCAPTURE);
    else if (!GlobalsConfig.StreamProof && Flag == WDA_EXCLUDEFROMCAPTURE)
        SetWindowDisplayAffinity(Hwnd, WDA_NONE);

    // Window Style Changer
    HWND ForegroundWindow = GetForegroundWindow();
    LONG TmpLong = GetWindowLong(Hwnd, GWL_EXSTYLE);

   /* if (GlobalsConfig.ShowMenu && MenuStyle != TmpLong)
        SetWindowLong(Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
    else if (!GlobalsConfig.ShowMenu && ESPStyle != TmpLong)
        SetWindowLong(Hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);*/

    // ShowMenu
    //bool setSilentAim = true;
   

    static bool menu_key = false;
    if (IsKeyDown(GlobalsConfig.MenuKey) && !menu_key)
    {
        GlobalsConfig.ShowMenu = !GlobalsConfig.ShowMenu;

        if (GlobalsConfig.ShowMenu && ForegroundWindow != Hwnd)
            SetForegroundWindow(Hwnd);
        else if (!GlobalsConfig.ShowMenu && ForegroundWindow != CheckHwnd)
            SetForegroundWindow(CheckHwnd);

        menu_key = true;
    }
    else if (!IsKeyDown(GlobalsConfig.MenuKey) && menu_key)
    {
        menu_key = false;
    }

    // Window Resizer
    RECT TmpRect{};
    POINT TmpPoint{};
    GetClientRect(CheckHwnd, &TmpRect);
    ClientToScreen(CheckHwnd, &TmpPoint);

    // Resizer
    if (TmpRect.left != GlobalsConfig.GameRect.left || TmpRect.bottom != GlobalsConfig.GameRect.bottom || TmpRect.top != GlobalsConfig.GameRect.top || TmpRect.right != GlobalsConfig.GameRect.right || TmpPoint.x != GlobalsConfig.GamePoint.x || TmpPoint.y != GlobalsConfig.GamePoint.y)
    {
        GlobalsConfig.GameRect = TmpRect;
        GlobalsConfig.GamePoint = TmpPoint;
        SetWindowPos(Hwnd, nullptr, TmpPoint.x, TmpPoint.y, GlobalsConfig.GameRect.right, GlobalsConfig.GameRect.bottom, SWP_NOREDRAW);
    }
}



void LoadStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 1.0f;
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.WindowRounding = 11.5f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(20.0f, 20.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
    style.FrameRounding = 11.89999961853027f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
    style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
    style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
    style.IndentSpacing = 0.0f;
    style.ColumnsMinSpacing = 4.900000095367432f;
    style.ScrollbarSize = 11.60000038146973f;
    style.ScrollbarRounding = 15.89999961853027f;
    style.GrabMinSize = 3.700000047683716f;
    style.GrabRounding = 20.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09250493347644806f, 0.100297249853611f, 0.1158798336982727f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1120669096708298f, 0.1262156516313553f, 0.1545064449310303f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.971993625164032f, 1.0f, 0.4980392456054688f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7953379154205322f, 0.4980392456054688f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1821731775999069f, 0.1897992044687271f, 0.1974248886108398f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1545050293207169f, 0.1545048952102661f, 0.1545064449310303f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.1414651423692703f, 0.1629818230867386f, 0.2060086131095886f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1072951927781105f, 0.107295036315918f, 0.1072961091995239f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.1293079704046249f, 0.1479243338108063f, 0.1931330561637878f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1459212601184845f, 0.1459220051765442f, 0.1459227204322815f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.999999463558197f, 1.0f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1249424293637276f, 0.2735691666603088f, 0.5708154439926147f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8841201663017273f, 0.7941429018974304f, 0.5615870356559753f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9570815563201904f, 0.9570719599723816f, 0.9570761322975159f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9356134533882141f, 0.9356129765510559f, 0.9356223344802856f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.266094446182251f, 0.2890366911888123f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}

// idk
void CreateRenderTarget()
{

    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);

    pBackBuffer->Release();

}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

void CleanupDeviceD3D()
{

    CleanupRenderTarget();

    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}


bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK) {
        return false;
    }
        

    CreateRenderTarget();
    return true;
}



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
