#include "sdk.h"
#include "../globals.h"

SDK sdk;

bool SDK::InitOffset() {
	if (Globals::GameBuild == 2699) {
		// that need base addr
		world = 0x26684D8;
		replay = 0x20304C8;
		viewport = 0x20D8C90;
		camera = 0x20D9868;
		handlebullet = 0xFF9D90;


		m_pVehicle = 0xD30;
		m_pInfo = 0x10C8;
		m_pWeaponManager = 0x10D8;
		
		// Player
		m_fHealthMax = 0x2A0;
		m_vecVelocity = 0x320;
		m_pBoneList = 0x430;
		m_fArmor = 0x1530;
		m_bPedTask = 0x146B;
		m_pVisibleFlag = 0x147C;

		m_vecVehicleVelocity = 0x7F0;
		m_fVehicleEngineHealth = 0x908;

		m_fWeaponSpread = 0x84;
		return true;
	}
	else if (Globals::GameBuild == 2802) {
		world = 0x254D448;
		replay = 0x1F5B820;
		viewport = 0x1FBC100;
		camera = 0x1FBCCD8;

		// Vehicle
		m_vecVehicleVelocity = 0x7D0;
		m_fVehicleEngineHealth = 0x8E8;
		m_pVisibleFlag = 0x145C;

		// Weapon
		m_fWeaponSpread = 0x84;
		return true;
	} 
	else if (Globals::GameBuild == 2944) {
		world = 0x257BEA0;
		replay = 0x1F42068;
		viewport = 0x1FEAAC0;
		camera = 0x1FEB698;

		// Player
		m_pNetId = 0xE8;
		m_pVisibleFlag = 0x145C;

		// Vehicle
		m_vecVehicleVelocity = 0x7D0;
		m_fVehicleEngineHealth = 0x8E8;
		

		// Weapon
		m_fWeaponSpread = 0x74;
		return true;
	}
	else if (Globals::GameBuild == 3095) {
		world = 0x2593320;
		replay = 0x1F58B58;
		viewport = 0x20019E0;
		camera = 0x20025B8;

		// Player
		m_pNetId = 0xE8;
		m_pVisibleFlag = 0x145C;

		// Vehicle
		m_vecVehicleVelocity = 0x7E0;
		m_fVehicleEngineHealth = 0x910;
		
		// Weapon
		m_fWeaponSpread = 0x74;
		m_fWeaponDamage = 0xB0;
		return true;
	}
	else {
		return false;
	}
	

}



