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
#include <filesystem>
#include <fstream>
#include <optional>
#include <windows.h>
#include <winreg.h>
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>

#include "DOOMdumper.hpp"
#include "DebugTools.hpp"
#include "Packages.hpp"
#include "FileSys.hpp"
#include "Processes.hpp"
#include "Utils.hpp"


namespace fs = std::filesystem;

const std::string UPDATED = "2021-08-07";
const uint64_t MIN_FREE = 91268055040;  // Minimum space, in bytes, required to dump DOOM Eternal
const winrt::PackageVersion GAME_VERSION{ 1, 0, 10, 0 };
const std::string GAME_VERSION_STR
    = std::to_string(GAME_VERSION.Major) + "."
    + std::to_string(GAME_VERSION.Minor) + "."
    + std::to_string(GAME_VERSION.Build) + "."
    + std::to_string(GAME_VERSION.Revision);
const int GAME_VERSION_INT = std::stoi(boost::replace_all_copy(GAME_VERSION_STR, ".", ""));

// ANSI color stuff
bool nocolors = false; // todo: why do i need this?? also these can be macros considering i already manually enbale vt100 emulation
std::string RESET = "\033[0m";
std::string BLUE_INFO = "\033[46m\033[38m";
std::string BLUE = "\033[36m";
std::string RED = "\033[31m";
std::string YELLOW = "\033[33m";
std::string GREEN = "\033[32m";

bool misc_debug = false;
DebugStream dbgs;


int main(int argc, char** argv)
{
    bool print_links_then_exit = false;
    bool sync_with_reg_then_exit = false;

    if (argc > 1) {
        for (int i = 1; i < argc; i++ )
        {
            if (strcmp(argv[i], "--help") == 0 || argc > 4) {
                std::cout << "DOOMdumper by SEWsam, updated " << UPDATED << "\n";
                std::cout << "Help | Command line options\n"
                    << "\n";
                std::cout << "USAGE: " << argv[0] << " [OPTIONS]\n"
                    << "\n";
                std::cout << "OPTIONS:\n";
                std::cout << "      --help : Show this screen.\n";
                std::cout << "      --verbose : Print extra info that may help with troubleshooting.\n";
                std::cout << "      --no-colors : Don't use colors in output (and disable progress bar).\n";
                std::cout << "  -l, --licenses : Print game license download help.\n";
                std::cout << "\n";
                std::cout << "Extra options you shouldn't need to use:\n";
                std::cout << "      --sync-with-reg : Stores/replaces the currently registered path in the registry.\n";

                std::system("pause");
                return 1;
            }
            else if (strcmp(argv[i], "--verbose") == 0) {
                misc_debug = true;
                dbgs.enabled(true);
            }
            else if (strcmp(argv[i], "--no-colors") == 0) {
                // also disables progress bar
                nocolors = true;
                RESET = "";
                BLUE_INFO = "";
                BLUE = "";
                RED = "";
                YELLOW = "";
                GREEN = "";
            }
            else if ( strcmp(argv[i], "-l") == 0 ||
                      strcmp(argv[i], "--licenses") == 0 )
            {
                print_links_then_exit = true;
            }
            else if (strcmp(argv[i], "--sync-with-reg") == 0) {
                sync_with_reg_then_exit = true;
            }
            else {
                std::cout << "'" << argv[i] << "' is not a valid option. Use option '--help' for more details.\n";
                std::system("pause");
                return 1;
            }
        }
    }

    if (!nocolors)
    {
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
    }

    if (print_links_then_exit)
    {
        printLicenseHelp();
        std::system("pause");
        return 0;
    }

    if (sync_with_reg_then_exit)
    {
        int ret = 0;
        std::string path;
        std::optional<winrt::Package> pkg_result = getPackage();

        if (pkg_result) {
            winrt::Package pkg = *pkg_result;
            path = getRegisteredPath(pkg);
            std::cout << YELLOW << "Registered Install path: " << path << RESET << "\n";
            int result = storeDumpPath(path);

            if (result == ERROR_ACCESS_DENIED) {
                std::cerr << RED << "ERROR: You need to run this as admin.\n" << RESET;
            }
            else if (result != 0) {
                std::cerr << RED << "ERROR: code " << result << RESET << "\n";
            }
            else {
                std::cout << GREEN << "Synced registry data with currently registered DOOM Eternal installation!\n" << RESET;
            }
        }
        else {
            std::cout << RED << "Failed to get package object for DOOM Eternal. Is it not installed?\n";
            std::cout << "Exiting...\n" << RESET;
            std::system("pause");
            ret = 1;
        }

        std::system("pause");
        return ret;
    }

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
    std::optional<winrt::Package> pkg_result = getPackage();
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

            // this helps chrispy and probably me in the future
            storeDumpPath(path);
           
            // So doomdumper knows that this folder contains a sideloaded DOOMEternal, and that it is ok to replace later on.
            fs::path install_marker_path(path);
            install_marker_path /= "doom_dumper";
            std::ofstream install_marker_ofs(install_marker_path);
            install_marker_ofs.close();

            if (!dumpWithStatus(pid, path, misc_debug)) {
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

    std::cout << "\n";
    std::cout << BLUE << "Your game is installed to " << GREEN << path << "\n" << RESET;

    printLicenseHelp();

    std::cout << GREEN << "All tasks completed!\n" << RESET;
    std::system("pause");
    return 0;
}


// TODO: Find a way to launch MS Store automatically in a less confusing way than before.
// TODO: Possibly implement MSIXVC backups for those who are willing to sacrifice the space on slow internet.
// TODO: Unlikely to be possible, but hopefully find a way to "patch" the game installation to use the MutablePackageDirectory property.
