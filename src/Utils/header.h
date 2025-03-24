#pragma once
// load an external source to load simple math like vector3 and etc.
#include "../../ext/SimpleMath/SimpleMath.h"

// load an internal source such as config and the log and communicate to the driver
#include "Config/config.h"
#include "Log/log.h"
#include "Driver/driver.h"

// include an official windows
#include <Windows.h>
#include <Tlhelp32.h>
//#include <Windows.h>
#include <cstdint>
#include <psapi.h>
#include <string>
#include <ostream>
#include <thread>
#include <iostream>

// use namespace simplemath
using namespace DirectX::SimpleMath;