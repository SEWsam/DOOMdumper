/* 
This file (Utils.hpp) is a part of DOOMdumper.

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

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winreg.h>


class ProgressBar
{
private:
	bool has_displayed = false;
public:
    void Display(int p);
	void Clear();
};

class GameVersion
{
private:
	uint16_t Major;
	uint16_t Minor;
	uint16_t Build;
	uint16_t Revision;
public:
	winrt::PackageVersion pkg_version;
    // todo: allow creation of class from a winrt::PackageVersion
    GameVersion(uint16_t major, uint16_t minor, uint16_t build, uint16_t revision);

    const std::string String();
    const int Int();

    //todo: add operators
};

std::wstring stringToWstring(const std::string string);
bool confirmPrompt(const std::string msg);
std::string formattedSize(uint64_t bytesize);


/*
    ehh i could probably avoid having two functions for reading AND writing but whatever
*/
// Registry set template
template <typename T, DWORD regT>
int regSet(HKEY hive, const char* key, const char* value_name, T value, size_t value_size)
{
    HKEY hkey;

    auto l_key = RegOpenKeyExA(hive, key, 0, KEY_SET_VALUE, &hkey);
    if (l_key == ERROR_FILE_NOT_FOUND)
    {
        dbgs.dbgCout() << "Key '" << key << "' does not exist. Creating it.\n";
        l_key = RegCreateKeyExA(hive, key, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, nullptr, &hkey, nullptr);
    }

    if (l_key != EXIT_SUCCESS)
    {
        std::cerr << RED << "Failed to open registry.\n" << RESET;
        return l_key;
    }

    auto l_set_result = RegSetValueExA(hkey, value_name, 0, regT, (LPBYTE)value, value_size);
    if (l_set_result != EXIT_SUCCESS)
    {
        std::cerr << RED << "Failed to write to registry.\n" << RESET;
        return l_set_result;
    }

    auto l_closure_result = RegCloseKey(hkey);
    if (l_closure_result != EXIT_SUCCESS)
    {
        std::cerr << RED << "Failed to close registry.\n" << RESET;
    }

    return l_closure_result;
}


// inline helper functions
inline void printLicenseHelp()
{
    std::cout << "\033[33m\nPLEASE READ THIS -- When you next launch you game you **may** see that you \"Don't Own\" the campaign.\n"
              << "If you DO own the campaign(s), you should enter the links below into a browser.\n\n\033[0m";

    std::cout << "\033[33mCampaign: \033[0mms-windows-store://pdp/?productId=9PC4V8W0VCWT\n"
              << "\033[33mTAG1:     \033[0mms-windows-store://pdp/?productId=9P2MSCGJPKJC\n"
              << "\033[33mTAG2:     \033[0mms-windows-store://pdp/?productId=9NB788JLSR97\n";
}

inline int enableDevMode()
{
    dbgs.dbgCout() << "Enabling devmode...\n";
    int value = 1;
    int result = regSet<int*, REG_DWORD>(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AppModelUnlock",
        "AllowDevelopmentWithoutDevLicense", &value, sizeof(DWORD));
    dbgs.dbgCout() << "Enabling devmode should have finished.\n";
    return result;
}

inline int storeDumpPath(std::string path)
{
    dbgs.dbgCout() << "Storing dump path...\n";
    const char* c_path = path.c_str();
    const size_t pathlen = strlen(c_path) * sizeof(char);
    int result = regSet<const char*, REG_SZ>(HKEY_LOCAL_MACHINE, "SOFTWARE\\DOOMdumper", "DumpPath", c_path, pathlen);
    dbgs.dbgCout() << "Storing dump path should have finished\n";
    return result;
}