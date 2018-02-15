// 
// nwcore: common.h
// NEWorld: A Free Game with Similar Rules to Minecraft.
// Copyright (C) 2015-2018 NEWorld Team
// 
// NEWorld is free software: you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// NEWorld is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General 
// Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with NEWorld.  If not, see <http://www.gnu.org/licenses/>.
// 

#pragma once
// STL include

#include <array>
#include <cmath>
#include <mutex>
#include <atomic>
#include <chrono>
#include <future>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include "nwstdlib/nwstddef.h"
// Math Library
#include "nwmath/nwvector.h"
#include "nwmath/nwmatrix.h"
// Physics Library
#include "nwphysics/nwaabb.h"
// Maintenance Library
#include "maintenance/nwdebug.h"
#include "maintenance/nwlogger.hpp"
#include "maintenance/nwexception.hpp"
// Json Library
#include "nwjson/nwjson.h"
// NEWorld Standard Library
#include "nwstdlib/nwconcepts.hpp"
#include "nwstdlib/nwstrings.h"
#include "nwstdlib/nwconsole.hpp"
#include "nwstdlib/nwfilesys.h"
#include "nwstdlib/nwlibrary.h"
// Concurrency Library
#include "concurrency/nwconcurrency.hpp"
