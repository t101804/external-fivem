#pragma once
#include "../../header.h"

#include "../game.h"

struct EntityToScreen {
    Vector2 pBase; // position of enemy
    std::vector<Vector2> Bones; // bones of enemy vector2 style
    std::vector<Vector3> BoneList; // all of bones of enemy vector3 style
};

struct ESPResources {
    float headToNeck; // head to neck
    float pTop; // top
    float pBottom; // bottom
    float pHeight; // height
    float pWidth; // width
    float bScale; // bone scale
};

struct Visual_s {
    Matrix ViewMatrix; // our view matrix
    FiveM Interact; // interacting with overlay
    Vector2 Center; // center of our view screen
};


class Visual {


public: 
    static void RenderEsp();
    inline static Visual_s Screen{};
// function
private:
    // initialize Esp_s
    static bool InitializeEsp();
    // initialize EntityToScreen
    //static bool BoneToScreen(CPed* pEntity);
    // initialize ESPResources
    static ESPResources ComputeESPResources();
// struct with initialize empty
private:
    inline static FiveM Interact{};
 
    inline static EntityToScreen EntityScreen{};
    inline static CPed* pLocal = &Globals::local;
    //static const CPed* pLocal;
	////EntityToScreen BoneToScreen(CPed* pEntity, Matrix ViewMatrix);
 //   Vector2 pBase = {};
 //   std::vector<Vector2> bScreen;
};