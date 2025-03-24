#include "game.h"
#include "Features/visual.h"
#include "Features/aim.h"
#include "Features/others.h"


Overlay* overlay = new Overlay;

bool FiveM::Overlay() {
	if (!overlay->InitOverlay(L"grcWindow", WINDOW_CLASS)) 
	{
		Logging::error_print("cant init overlay");
		return false;
	}
	overlay->OverlayLoop();
	overlay->DestroyOverlay();
	delete overlay;
	Logging::debug_print("deleting overlay");
	return true;
}

void FiveM::Start() {
	DetectGame();
	if (!Globals::Setup()) {
		Logging::error_print("cant setup globals");
		return;
	}
	if (!sdk.InitOffset()) {
		Logging::error_print("cant init offset, unknown game version");
		return;
	}
	Globals::EntityRenderUpdate();
}

void FiveM::RunCheat() {
	if (Globals::GameWorld == 0) {
		return;
	}
    if (GlobalsConfig.AimBot && GlobalsConfig.Aim_DrawFov) {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(GlobalsConfig.GameRect.right / 2.f, GlobalsConfig.GameRect.bottom / 2.f), GlobalsConfig.Aim_Fov, GlobalsConfig.Fov_color);
    }
	Others::GodMode();
	Others::Invisible();
    Aim::NoRecoil();
	Aim::NoSpread();
    if (GlobalsConfig.ESP) {
        Visual::RenderEsp();
    }
    if (GlobalsConfig.ShowMenu) {
        Overlay::RenderMenuGui();
    }
}


// just private function for utils 


bool FiveM::getGameId(const char* target, int InitMode) {
	if (InitMode == WINDOW_TITLE || InitMode == WINDOW_CLASS) {
		HWND hWindow = InitMode == WINDOW_TITLE ? FindWindowA(nullptr, target) : FindWindowA(target, nullptr);

		if (!hWindow) {
			Logging::error_print("sorry i cant get %s, do you sure its already opened?", target);
			return false;
		}
		GetWindowThreadProcessId(hWindow, &Globals::GamePid);
		return true;
	}
	else {
		// maybe implementing an target if the target was exe and not window
		return false;
	}
}

void FiveM::DetectGame() {
	if (!FiveM::getGameId("grcWindow", WINDOW_CLASS)) {
		Logging::error_print("sorry i dont get the game");
	}
	Logging::debug_print("pid target found : %ld", Globals::GamePid);
}
