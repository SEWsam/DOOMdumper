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

/* Shared Header File */
#pragma once

#include <iostream>
#include <optional>
#include <filesystem>
#include <Windows.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Management.Deployment.h>

namespace fs = std::filesystem;

namespace winrt
{
	using namespace Windows::ApplicationModel;
	using namespace Windows::Management::Deployment;
}

// Global Version constants
extern const std::string UPDATED;
extern const uint64_t MIN_FREE;
extern const winrt::PackageVersion GAME_VERSION;
extern const std::string GAME_VERSION_STR;

// ANSI color stuff
extern std::string RESET;
extern std::string BLUE_INFO;
extern std::string RED;
extern std::string YELLOW;
extern std::string GREEN;


/* main.cpp */
bool confirmPrompt(const std::string msg);
std::string formattedSize(uint64_t bytesize);

/* FileSys.cpp */
void extractInjector(const std::string& path);
bool getDefaultPath(fs::path& path);
bool preparePath(fs::path& path, uint64_t& free);
std::string getPath();

/* Packages.cpp */
std::optional<winrt::Package> getPackage(const std::wstring pfn = L"BethesdaSoftworks.DOOMEternal-PC_3275kfvn8vcwc");
bool validatePackage(const winrt::Package& pkg);
bool removePackage(const winrt::Package& pkg);
bool registerPackage(const std::wstring wpath);
std::string getRegisteredPath(const winrt::Package& pkg);

/* Processes.cpp */
bool killProcessByPid(const DWORD pid);
bool matchProcessName(const DWORD pid, const WCHAR* match_name);
int getPidByName(const WCHAR* name);
int promptForProcess(const WCHAR* name = L"DOOMEternalx64vk.exe");

/* UWPDUmperInterface.cpp */
bool dumpWithStatus(const int pid, const std::string path);


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

