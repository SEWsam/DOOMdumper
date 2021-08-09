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
inline constexpr char Updated[] = "2021-08-08";
inline constexpr uint64_t Min_Free = 91268055040; // Minimum space, in bytes, required to dump DOOM Eternal

extern const winrt::PackageVersion GAME_VERSION;
extern const std::string GAME_VERSION_STR;
extern const int GAME_VERSION_INT;

extern bool misc_debug; 
extern DebugStream dbgs;

/* UWPDUmperInterface.cpp */
bool dumpWithStatus(const int pid, const std::string path, const bool verboseish);


/* Extra tools */

class ProgressBar
{
private:
	bool has_displayed = false;
public:
	void Display(int p)
	{
		has_displayed = true;
		std::cout << "\033[36m";

		std::cout << "Progress: [";
		for (int i = 2; i < 100; i += 2) {
			if (i <= p) {
				std::cout << "=";
			}
			else {
				std::cout << " ";
			}

		}
		std::cout << "] " << std::to_string(p) << " %\n\033[0m";


	}

	void Clear()
	{
		if (has_displayed) std::cout << "\033[1A\033[2K";
	}
};

