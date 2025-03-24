#pragma once
#include "../../Cped/cped.h"
#include "../game.h"


class Aim {
public:
	static bool AimSetup(CPed target);
	
	// for now this have each public function but maybe in futures i will make it into 1
	// because this only writing a memory function
	static void NoRecoil();
	static void NoSpread();
private:
	//static Vector3 targetPos;
	static void Silent(Vector3);
	static void RestoreSilent();
};