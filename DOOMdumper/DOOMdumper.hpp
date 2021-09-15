/* 
This file (DOOMdumper.hpp) is a part of DOOMdumper.

Copyright (C) 2021 SEWsam

DOOMdumper is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DOOMdumper is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DOOMdumper If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <iostream>
#include <optional>
#include <filesystem>
#include <winrt/Windows.ApplicationModel.h>

#include "DebugTools.hpp"
#include "Utils.hpp"

// ANSI color stuff
#define RESET "\033[0m"
#define BLUE_INFO "\033[46m\033[38m"
#define BLUE "\033[36m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

namespace fs = std::filesystem;
namespace winrt { using namespace Windows::ApplicationModel; }

// Global Version constants
inline constexpr char Updated[] = "2021-09-15";
inline constexpr uint64_t Min_Free = 91268055040; // Minimum space, in bytes, required to dump DOOM Eternal

extern const GameVersion game_version;

extern bool misc_debug; 
extern DebugStream dbgs;

/* UWPDUmperInterface.cpp */
bool dumpWithStatus(const int pid, const std::string path, const bool verboseish);

