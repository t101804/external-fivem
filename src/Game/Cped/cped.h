#pragma once

#include "../header.h"
#include "../Sdk/sdk.h"

class CPed
{
public:
	// Pointer
	uintptr_t address;
	uintptr_t Vehicle;
	uintptr_t PlayerInfo;
	uintptr_t CurrentWeapon;

	int Index;
	int m_pId;
	std::string m_pName;
	std::string m_sWeaponName;
	float m_fArmor;
	float m_fHealth;
	bool m_isVisible;
	//bool was_invisible;
	float m_fMaxHealth;
	Vector3 m_pVecLocation;
	Vector3 m_pVecVelocity;
	Matrix m_bMatrix;

	std::unordered_map<int, std::string> PlayerIdToName;


	bool GetPlayer(uintptr_t& address);
	bool Update();
	bool IsDead();
	bool IsVisible();
	void SetInvisibleMode(BYTE mode);
	bool IsPlayer();
	bool InVehicle();
	bool IsGod();
	std::string GetCurrentWeaponName();

	Vector3 GetBoneByID(BoneId id);
	Vector3 GetVelocity();
	Vector3 GetBonePosition(int bone_id);
	std::vector<Vector3> GetBoneList();
private:
	std::string get_weapon_name(DWORD hash) {
		//removed xoring, cba to do properly.
		static auto dagger = std::string("dagger");
		static auto bat = std::string("bat");
		static auto bottle = std::string("bottle");
		static auto crowbar = std::string("crowbar");
		static auto unarmed = std::string("unarmed");
		static auto flashlight = std::string("flashlight");
		static auto golfclub = std::string("golfclub");
		static auto hammer = std::string("hammer");
		static auto hatchet = std::string("hatchet");
		static auto knuckle = std::string("knuckle");
		static auto knife = std::string("knife");
		static auto machete = std::string("machete");
		static auto switchblade = std::string("switchblade");
		static auto nightstick = std::string("nightstick");
		static auto wrench = std::string("wrench");
		static auto battleaxe = std::string("battleaxe");
		static auto poolcue = std::string("poolcue");
		static auto pistol = std::string("pistol");
		static auto pistolmk2 = std::string("pistol_mk2");
		static auto combatpistol = std::string("combatpisto");
		static auto appistol = std::string("appistol");
		static auto stungun = std::string("stungun");
		static auto pistol50 = std::string("pistol50");
		static auto snspistol = std::string("snspisto");
		static auto snspistolmk2 = std::string("snspistol_mk2");
		static auto heavypistol = std::string("heavypisto");
		static auto vintagepistol = std::string("vintagepisto");
		static auto flaregun = std::string("flaregun");
		static auto marksmanpistol = std::string("marksmanpisto");
		static auto revolver = std::string("revolver");
		static auto revolvermk2 = std::string("revolver_mk2");
		static auto doubleaction = std::string("doubleaction");
		static auto microsmg = std::string("microsmg");
		static auto smg = std::string("smg");
		static auto smgmk2 = std::string("smg_mk2");
		static auto assaultsmg = std::string("assaultsmg");
		static auto combatpdw = std::string("combatpdw");
		static auto machinepistol = std::string("machinepisto");
		static auto minismg = std::string("minismg");
		static auto pumpshotgun = std::string("pumpshotgun");
		static auto pumpshotgun_mk2 = std::string("pumpshotgun_mk2");
		static auto sawnoffshotgun = std::string("sawnoffshotgun");
		static auto assaultshotgun = std::string("assaultshotgun");
		static auto bullpupshotgun = std::string("bullpupshotgun");
		static auto musket = std::string("musket");
		static auto heavyshotgun = std::string("heavyshotgun");
		static auto dbshotgun = std::string("dbshotgun");
		static auto autoshotgun = std::string("autoshotgun");
		static auto assaultrifle = std::string("assaultrifle");
		static auto assaultrifle_mk2 = std::string("assaultrifle_mk2");
		static auto carbinerifle = std::string("carbinerifle");
		static auto carbinerifle_mk2 = std::string("carbinerifle_mk2");
		static auto advancedrifle = std::string("advancedrifle");
		static auto specialcarbine = std::string("specialcarbine");
		static auto specialcarbine_mk2 = std::string("specialcarbine_mk2");
		static auto bullpuprifle = std::string("bullpuprifle");
		static auto bullpuprifle_mk2 = std::string("bullpuprifle_mk2");
		static auto compactrifle = std::string("compactrifle");
		static auto machine_gun = std::string("machine_gun");
		static auto combatmg = std::string("combatmg");
		static auto combatmg_mk2 = std::string("combatmg_mk2");
		static auto gusenberg = std::string("gusenberg");
		static auto sniperrifle = std::string("sniperrifle");
		static auto heavysniper = std::string("heavysniper");
		static auto heavysniper_mk2 = std::string("heavysniper_mk2");
		static auto marksmanrifle = std::string("marksmanrifle");
		static auto marksmanrifle_mk2 = std::string("marksmanrifle_mk2");
		static auto rpg = std::string("rpg");
		static auto grenadelauncher = std::string("grenadelauncher");
		static auto grenadelauncher_smoke = std::string("grenadelauncher_smoke");
		static auto minigun = std::string("minigun");
		static auto firework = std::string("firework");
		static auto railgun = std::string("railgun");
		static auto hominglauncher = std::string("hominglauncher");
		static auto compactlauncher = std::string("compactlauncher");
		static auto grenade = std::string("grenade");
		static auto bzgas = std::string("bzgas");
		static auto smokegrenade = std::string("smokegrenade");
		static auto flare = std::string("flare");
		static auto molotov = std::string("molotov");
		static auto stickybomb = std::string("stickybomb");
		static auto proxmine = std::string("proxmine");
		static auto snowball = std::string("snowbal");
		static auto pipebomb = std::string("pipebomb");
		static auto ball = std::string("bal");
		static auto petrolcan = std::string("petrolcan");
		static auto fireextinguisher = std::string("fireextinguisher");
		static auto parachute = std::string("parachute");

		switch (hash)
		{
		case 0x92A27487:
			return dagger.c_str(); break;
		case 0x958A4A8F:
			return bat.c_str(); break;
		case 0xF9E6AA4B:
			return bottle.c_str(); break;
		case 0x84BD7BFD:
			return crowbar.c_str(); break;
		case 0xA2719263:
			return unarmed.c_str(); break;
		case 0x8BB05FD7:
			return flashlight.c_str(); break;
		case 0x440E4788:
			return golfclub.c_str(); break;
		case 0x4E875F73:
			return hammer.c_str(); break;
		case 0xF9DCBF2D:
			return hatchet.c_str(); break;
		case 0xD8DF3C3C:
			return knuckle.c_str(); break;
		case 0x99B507EA:
			return knife.c_str(); break;
		case 0xDD5DF8D9:
			return machete.c_str(); break;
		case 0xDFE37640:
			return switchblade.c_str(); break;
		case 0x678B81B1:
			return nightstick.c_str(); break;
		case 0x19044EE0:
			return wrench.c_str(); break;
		case 0xCD274149:
			return battleaxe.c_str(); break;
		case 0x94117305:
			return poolcue.c_str(); break;
		case 0x1B06D571:
			return pistol.c_str(); break;
		case 0xBFE256D4:
			return pistolmk2.c_str(); break;
		case 0x5EF9FEC4:
			return combatpistol.c_str(); break;
		case 0x22D8FE39:
			return appistol.c_str(); break;
		case 0x3656C8C1:
			return stungun.c_str(); break;
		case 0x99AEEB3B:
			return pistol50.c_str(); break;
		case 0xBFD21232:
			return snspistol.c_str(); break;
		case 0x88374054:
			return snspistolmk2.c_str(); break;
		case 0xD205520E:
			return heavypistol.c_str(); break;
		case 0x83839C4:
			return vintagepistol.c_str(); break;
		case 0x47757124:
			return flaregun.c_str(); break;
		case 0xDC4DB296:
			return marksmanpistol.c_str(); break;
		case 0xC1B3C3D1:
			return revolver.c_str(); break;
		case 0xCB96392F:
			return revolvermk2.c_str(); break;
		case 0x97EA20B8:
			return doubleaction.c_str(); break;
		case 0x13532244:
			return microsmg.c_str(); break;
		case 0x2BE6766B:
			return smg.c_str(); break;
		case 0x78A97CD0:
			return smgmk2.c_str(); break;
		case 0xEFE7E2DF:
			return assaultsmg.c_str(); break;
		case 0xA3D4D34:
			return combatpdw.c_str(); break;
		case 0xDB1AA450:
			return machinepistol.c_str(); break;
		case 0xBD248B55:
			return minismg.c_str(); break;
		case 0x1D073A89:
			return pumpshotgun.c_str(); break;
		case 0x555AF99A:
			return pumpshotgun_mk2.c_str(); break;
		case 0x7846A318:
			return sawnoffshotgun.c_str(); break;
		case 0xE284C527:
			return assaultshotgun.c_str(); break;
		case 0x9D61E50F:
			return bullpupshotgun.c_str(); break;
		case 0xA89CB99E:
			return musket.c_str(); break;
		case 0x3AABBBAA:
			return heavyshotgun.c_str(); break;
		case 0xEF951FBB:
			return dbshotgun.c_str(); break;
		case 0x12E82D3D:
			return autoshotgun.c_str(); break;
		case 0xBFEFFF6D:
			return assaultrifle.c_str(); break;
		case 0x394F415C:
			return assaultrifle_mk2.c_str(); break;
		case 0x83BF0278:
			return carbinerifle.c_str(); break;
		case 0xFAD1F1C9:
			return carbinerifle_mk2.c_str(); break;
		case 0xAF113F99:
			return advancedrifle.c_str(); break;
		case 0xC0A3098D:
			return specialcarbine.c_str(); break;
		case 0x969C3D67:
			return specialcarbine_mk2.c_str(); break;
		case 0x7F229F94:
			return bullpuprifle.c_str(); break;
		case 0x84D6FAFD:
			return bullpuprifle_mk2.c_str(); break;
		case 0x624FE830:
			return compactrifle.c_str(); break;
		case 0x9D07F764:
			return machine_gun.c_str(); break;
		case 0x7FD62962:
			return combatmg.c_str(); break;
		case 0xDBBD7280:
			return combatmg_mk2.c_str(); break;
		case 0x61012683:
			return gusenberg.c_str(); break;
		case 0x5FC3C11:
			return sniperrifle.c_str(); break;
		case 0xC472FE2:
			return heavysniper.c_str(); break;
		case 0xA914799:
			return heavysniper_mk2.c_str(); break;
		case 0xC734385A:
			return marksmanrifle.c_str(); break;
		case 0x6A6C02E0:
			return marksmanrifle_mk2.c_str(); break;
		case 0xB1CA77B1:
			return rpg.c_str(); break;
		case 0xA284510B:
			return grenadelauncher.c_str(); break;
		case 0x4DD2DC56:
			return grenadelauncher_smoke.c_str(); break;
		case 0x42BF8A85:
			return minigun.c_str(); break;
		case 0x7F7497E5:
			return firework.c_str(); break;
		case 0x6D544C99:
			return railgun.c_str(); break;
		case 0x63AB0442:
			return hominglauncher.c_str(); break;
		case 0x781FE4A:
			return compactlauncher.c_str(); break;
		case 0x93E220BD:
			return grenade.c_str(); break;
		case 0xA0973D5E:
			return bzgas.c_str(); break;
		case 0xFDBC8A50:
			return smokegrenade.c_str(); break;
		case 0x497FACC3:
			return flare.c_str(); break;
		case 0x24B17070:
			return molotov.c_str(); break;
		case 0x2C3731D9:
			return stickybomb.c_str(); break;
		case 0xAB564B93:
			return proxmine.c_str(); break;
		case 0x787F0BB:
			return snowball.c_str(); break;
		case 0xBA45E8B8:
			return pipebomb.c_str(); break;
		case 0x23C9F95C:
			return ball.c_str(); break;
		case 0x34A67B97:
			return petrolcan.c_str(); break;
		case 0x60EC506:
			return fireextinguisher.c_str(); break;
		case 0xFBAB5776:
			return parachute.c_str(); break;
		default:
			return unarmed.c_str(); break;
		}
	}

}; 
