/* 
This file (main.cpp) is a part of DOOMdumper.

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
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <filesystem>
#include <fstream>
#include <optional>
#include <boost/locale.hpp>
#include <windows.h>
#include <winreg.h>

#include "DOOMdumper.hpp"
#include "DebugTools.hpp"


namespace fs = std::filesystem;

const std::string UPDATED = "2021-06-02";
const uint64_t MIN_FREE = 83751819391;  // Minimum space, in bytes, required to dump DOOM Eternal
const winrt::PackageVersion GAME_VERSION{ 1, 0, 6, 0 };
const std::string GAME_VERSION_STR
    = std::to_string(GAME_VERSION.Major) + "."
    + std::to_string(GAME_VERSION.Minor) + "."
    + std::to_string(GAME_VERSION.Build) + "."
    + std::to_string(GAME_VERSION.Revision);
const int GAME_VERSION_INT = std::stoi(boost::replace_all_copy(GAME_VERSION_STR, ".", ""));

// ANSI color stuff
std::string RESET = "\033[0m";
std::string BLUE_INFO = "\033[46m\033[38m";
std::string RED = "\033[31m";
std::string YELLOW = "\033[33m";
std::string GREEN = "\033[32m";


std::wstring stringToWstring(const std::string string)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &string[0], int(string.size()), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &string[0], int(string.size()), &wstrTo[0], size_needed);
    return wstrTo;
}

int enableDevMode()
{
    HKEY hkey;
    DWORD value = 1;

    auto l_key = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AppModelUnlock", 0, KEY_SET_VALUE, &hkey);
    if (l_key != EXIT_SUCCESS)
    {
        std::cerr << RED << "Failed to open registry to enable Developer Mode.\n" << RESET;
        return l_key;
    }

    auto l_set_result = RegSetValueExA(hkey, "AllowDevelopmentWithoutDevLicense", 0, REG_DWORD, LPBYTE(&value), sizeof(DWORD));
    if (l_set_result != EXIT_SUCCESS)
    {
        std::cerr << RED << "Failed to write to registry to enable Developer Mode.\n" << RESET;
        return l_set_result;
    }

    auto l_closure_result = RegCloseKey(hkey);
    if (l_closure_result != EXIT_SUCCESS)
    {
        std::cerr << RED << "Failed to close registry after enabling Developer Mode.\n" << RESET;
    }

    return l_closure_result;
}

bool confirmPrompt(const std::string msg)
{
    std::string input;
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);
    std::cout.imbue(loc);

    while (true) {
        std::cout << msg << " (yes/no): ";
        std::getline(std::cin, input);
        if (boost::locale::to_lower(input) == "yes") {
            return true;
        }
        else if (boost::locale::to_lower(input) == "no") {
            return false;
        }
        else {
            std::cout << "Please enter either 'yes' or 'no'.\n\n";
        }
    }

}

std::string formattedSize(uint64_t bytesize)
{
    const std::string units[] = {"B", "KB", "MB", "GB", "TB", "PB"};  // PB should be enough...

    int i = 0;
    while (bytesize > 1024) {
        if (i > 6) {
            break;
        }
        
        bytesize /= 1024;
        ++i;
    }

    std::string formatted = std::to_string(bytesize);
    formatted += units[i];
    return formatted;
}

int main(int argc, char** argv)
{
    bool verbosemode = false;

    if (argc > 1) {
        std::string argv_1(argv[1]);
        if (argv_1 == "--help" || argc > 2) {
            std::cout << "UWP-DOOMdumper (cpp) by SEWsam, updated " << UPDATED << "\n";
            std::cout << "Help -- Command line options\n"
                << "\n";
            std::cout << "USAGE: " << argv[0] << " [OPTIONS]\n"
                << "\n";
            std::cout << "OPTIONS:\n";
            std::cout << "    --help : Show this screen.\n";
            std::cout << "    --verbose : Print extra info that may help with troubleshooting.\n";
            return 1;
        }

        if (argv_1 == "--verbose") {
            verbosemode = true;
        }
        else {
            std::cout << "'" << argv_1 << "' is not a valid option. Use option '--help' for more details.";
            return 1;
        }
    } // TODO: colors?

    // Enable VT100 Emu
    DWORD ConsoleMode;
    GetConsoleMode(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &ConsoleMode
    );
    SetConsoleMode(
        GetStdHandle(STD_OUTPUT_HANDLE),
        ConsoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
    );
    SetConsoleOutputCP(437);


    DebugStream dbgs(verbosemode);

#pragma region Entry screen
    std::system("title DOOMdumper");
    std::cout << BLUE_INFO << "                                                  " << RESET << "\n";
    std::cout << BLUE_INFO << "  DOOMdumper                                      " << RESET << "\n";
    std::cout << BLUE_INFO << "    by SEWsam, updated " << UPDATED << "                 " << RESET << "\n";
    std::cout << BLUE_INFO << "                                                  " << RESET << "\n";
    std::cout << BLUE_INFO << "    DOOM ETERNAL MODS ON GAME PASS                " << RESET << "\n";
    std::cout << BLUE_INFO << "                                                  " << RESET << "\n";
    std::cout << BLUE_INFO << "    A tool that enables access to the game files  " << RESET << "\n";
    std::cout << BLUE_INFO << "    of your UWP DOOM Eternal installation, by     " << RESET << "\n";
    std::cout << BLUE_INFO << "    reinstalling your game to a custom location   " << RESET << "\n";
    std::cout << BLUE_INFO << "                                                  " << RESET << "\n";
    dbgs.dbgCout(true) << YELLOW << "Verbose Mode is ON\n" << RESET;

    std::cout << std::endl;
    std::cout << YELLOW
              << "This program will allow you to utilize EternalModInjector, with the\n";
    std::cout << "gamepass/Windows Store version of DOOM Eternal.\n";
    std::cout << "Your game will be copied to a location of your choosing, then the\n";
    std::cout << "original copy will be removed. You'll need " << RED << formattedSize(MIN_FREE)
              << YELLOW << " free for this.\n" << RESET;

    std::cout << std::endl;
    std::cout << YELLOW
              << "A WARNING ABOUT GAME UPDATES: Currently, there is no way to get updates\n";
    std::cout << "through the Microsoft Store if your game is modded. To update, you will\n";
    std::cout << "need to " << RED << "uninstall and reinstall" << YELLOW << " the game, then"
              << " run the lastest tools to\n";
    std::cout << "use mods again.\n" << RESET;
    std::cout << std::endl;

    std::system("pause");
    std::cout << std::endl;
#pragma endregion

    int devmode_result = enableDevMode();
    if (devmode_result == ERROR_ACCESS_DENIED)
    {
        std::cerr << RED << "ERROR: DOOMdumper needs to run as administrator.\n" << RESET;
        std::system("pause");
        return 1;
    }
    else if (devmode_result != EXIT_SUCCESS)
    {
        dbgs.dbgCout() << "Enabling devmode failed with status: " << devmode_result << "\n";
        std::cout << RED << "\nExiting...\n" << RESET;
        return 1;
    }

    std::string path;
    std::optional<winrt::Package> pkg_result = getPackage();    // DOOMEternal blah blah blah //L"Shinen.TheTouryst_9y1eezmggh3fe"
    if (pkg_result) {
        winrt::Package pkg = *pkg_result;

        std::cout << "Checking game status...\n";
        if (!validatePackage(pkg)) {
            std::cout << RED << "\nExiting...\n" << RESET;
            std::system("pause");
            return 1;
        }
        if (!pkg.IsDevelopmentMode()) { 
            std::cout << "DOOM Eternal needs to be dumped to use mods...\n";
            path = getPath();
            if (path.empty()) {  // Impossible to dump...
                std::cout << RED << "\nExiting...\n" << RESET;
                std::system("pause");
                return 1;
            }

            int pid;
            pid = promptForProcess();

            std::cout << "About to dump game. This can take " << RED << "20-40" << RESET << " minutes depending on if you have a HDD or SSD.\n";
            if (!confirmPrompt("Proceed?")) {
                std::cout << "\nExiting...\n";
                std::system("pause");
                return 0;
            }
            if (!dumpWithStatus(pid, path)) {
                std::cout << RED << "\nExiting...\n" << RESET;
                std::system("pause");
                return 1;
            }
            killProcessByPid(pid);

            std::cout << "Preparing new installation...\n";
            if (!removePackage(pkg)) {
                //todo: record path of dump for retry
                std::cout << RED << "\nExiting...\n" << RESET;
                std::system("pause");
                return 1;
            }
            if (!registerPackage(stringToWstring(path) + L"appxmanifest.xml")) {
                //todo: record path of dump for retry (take note of point of failure)
                std::cout << RED << "\nExiting...\n" << RESET;
                std::system("pause");
                return 1;
            }

            fs::path install_marker_path(path);
            install_marker_path /= "doom_dumper";
            std::ofstream install_marker_ofs(install_marker_path);
            install_marker_ofs.close();

        }
        else {
            path = getRegisteredPath(pkg);
            std::cout << "DOOM Eternal is already 'moddable'. Updating EternalModInjector...\n";
        }

    }
    else
    {
        std::cout << RED << "Failed to get installation status for DOOM Eternal. Is it not installed?\n";
        std::cout << "Exiting...\n" << RESET;
        std::system("pause");
        return 1;
    }

    std::cout << "Extracting EternalModInjector...\n";
    extractInjector(path);

    std::cout << YELLOW << "\nPLEASE READ THIS -- When you next launch you game you **may** see that you \"Don't Own\" the campaign.\n"
                        << "This is not the case, and you should install the campaign by clicking 'Campaign', then 'Go Now' in the game.\n\n"
                        << RESET;

    std::cout << GREEN << "All tasks completed!\n" << RESET;
    std::system("pause");
    return 0;
}


// TODO: Check if old game version was previously dumped (AppData config?)