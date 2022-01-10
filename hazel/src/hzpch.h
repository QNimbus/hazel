#pragma once

#include <stdint.h>

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "hazel/core/Log.h"
#include "hazel/core/Random.h"

#define HZ_PROFILE 0
#ifdef HZ_PROFILE
	#pragma warning(push, 0)
	#include "hazel/debug/Profiling.h"
	#pragma warning(pop)
#endif

#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif