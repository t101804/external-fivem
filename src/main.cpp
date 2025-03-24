#include "Utils/header.h"
#include "Window/Overlay/overlay.h"
#include "Game/FiveM/game.h"
#include "Window/Overlay/Debug/debug_overlay.hpp"

FiveM* fivem = new FiveM;

int main() {
	std::thread([&]() { fivem->Start(); }).detach();
	Sleep(3000);
	if (!fivem->Overlay()) {
		Logging::error_print("cant initializing and injecting the overlay");
	}
	delete fivem;
	return 0;
}