#pragma once
#include "header.h"

// an initial entity type, but it will update in this scope
struct Entity {
	uint64_t address;
	uint64_t junk0;
};


// entity list type maximal 256
struct EntityList_t {
	Entity entity[256];
};



namespace Globals {

	// globals address
    inline uintptr_t GameWorld = 0;
    inline uintptr_t ViewPort = 0;
    inline uintptr_t BaseAddress = 0;
    //inline uintptr_t HandleBullet = 0;

	// others globals  variables 
    inline int GameBuild = 0;
    inline DWORD GamePid = 0;
    inline CPed local;
    inline std::vector<CPed> EntityList;

    // setup a app, including setuping a driver to attach and getting baseaddress for the app
    inline bool Setup() {

        std::string driver_initial_path = "\\\\.\\";
        std::string driver_name = GlobalsConfig.DriverName;
        std::string full_driver_path = driver_initial_path + driver_name;
        bool driver = driver_manager::find_driver(full_driver_path.c_str());
        if (!driver) {  
            Logging::debug_print("Failed to find driver");
            return false;
        }
		Logging::debug_print("driver found %s", full_driver_path.c_str());

        /*   const DWORD process_id = driver_manager::get_process_id(application_name);
        if (!process_id) {
            Logging::error_print("failed to get the game/application. are you sure the app is opened correctly? %s is missing", application_name);
            return false;
        }*/

        // attaching the driver into our target proccess
        if (Globals::GamePid == 0) {
            Logging::debug_print("Failed to find GamePid");
            return false;
        }
        driver_manager::attach_to_process(Globals::GamePid);
        std::wstring baseName = driver_manager::get_base_module(Globals::GamePid);
        std::string baseNameStr(baseName.begin(), baseName.end());
        GameBuild = ExtractNumber(baseNameStr);
        Logging::debug_print("pid: %ld, gamename : %s, gameversion: %d", Globals::GamePid, baseNameStr.c_str(), Globals::GameBuild);
        BaseAddress = driver_manager::get_module_base_address(Globals::GamePid, baseName.c_str());
        return true;
    }

    // to render all person/entity in the world
    inline void EntityRenderUpdate() {
        while (GlobalsConfig.Run) {
            if (BaseAddress == 0) {
                Logging::error_print("base_address error");
                continue;
            }


            GameWorld = read_mem<uintptr_t>(BaseAddress + sdk.world);
            if (GameWorld == 0) {
                Logging::error_print("cant get the fivem world");
                continue;
            }
            local.address = read_mem<uintptr_t>(GameWorld + 0x8);
            ViewPort = read_mem<uintptr_t>(BaseAddress + sdk.viewport);


            uintptr_t ReplayInterface = read_mem<uintptr_t>(BaseAddress + sdk.replay);
            uintptr_t EntityInterface = read_mem<uintptr_t>(ReplayInterface + 0x18);
            uintptr_t EntityListAddr = read_mem<uintptr_t>(EntityInterface + 0x100);
            EntityList_t base = read_mem<EntityList_t>(EntityListAddr), * list = &base;

            std::vector<CPed> StoredEntity;
            for (int i = 0; i < 256; i++)
            {
                if (list->entity[i].address != NULL && list->entity[i].address != local.address)
                {
                    CPed entity_ped = CPed();

                    entity_ped.address = list->entity[i].address;

                    // set the attributes like current weapon name health etc.
                    if (entity_ped.Update()) {
                        entity_ped.Index = i;
                        StoredEntity.push_back(entity_ped);
                    }
                }
            }

            // set an entity list that we already get before to this globals
            EntityList = StoredEntity;

            // just clear and sleep
            StoredEntity.clear();
            //Logging::debug_print("entity obtained");
            Sleep(500);
        }

    }
};