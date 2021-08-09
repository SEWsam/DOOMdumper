/* 
This file (FileSys.cpp) is a part of DOOMdumper.

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
#include <Windows.h>
#include <filesystem>
#include <optional>
#include <boost/locale.hpp>
#include <zipper/unzipper.h>

#include "DOOMdumper.hpp"
#include "Utils.hpp"


void extractInjector(const std::string& path) // Note: there cannot be a blank 'Mods' folder in the zip.
{
    try
    {
        zipper::Unzipper unzipper("EternalModInjector-UWP.zip");

        if (!unzipper.extract(path))
        {
            std::cerr << RED << "Failed to extract EternalModInjector-UWP.zip.\n"
                             << "Try extracting it manually to your game directory ('" << path << "').\n" << RESET;
        }
        else
        {
            std::cout << "Extracted EternalModInjector!\n";
            fs::create_directory(path + "\\Mods");
        }

        unzipper.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << RED << "Couldn't load EternalModInjector-UWP.zip.\n"
                         << "If it's not in the same folder as doomdumper.exe, try downloading it again.\n" << RESET;
    }

}

bool getDefaultPath(fs::path& path)
{
    LPWSTR buf = new WCHAR[24]();
    DWORD buflen = 24;

    DWORD count = GetLogicalDriveStringsW(buflen, buf);

    LPWSTR drives = buf;
    fs::path final_path;
    while (*drives)
    {
        fs::path drive_path(drives);
        auto space = fs::space(drive_path);

        if (space.available > MIN_FREE) {
            final_path = drive_path.string();
            break;
        }

        while (*drives++);
    }
    delete[] buf;

    if (final_path.empty()) {
        std::cout << RED << "None of your drives have enough space to continue. You need: " << formattedSize(MIN_FREE) << "\n" << RESET;
        return false;
    }

    final_path /= "Games\\DOOMEternal\\";
    path = final_path;
    return true;
}

bool preparePath(fs::path& path, uint64_t& free)
{
    path.make_preferred();
    if (*path.string().rbegin() != '\\') {
        path += "\\";
    }

    if (!path.is_absolute()) {
        std::cout << YELLOW << "Invalid path. No drive letter specified.\n" << RESET;
        return false;
    }

    if (!fs::exists(path.root_path())) {
        std::cout << YELLOW << "Invalid path: The drive letter " << path.root_path() << " does not exist.\n" << RESET;
        return false;
    }

    if (fs::exists(path) && !fs::is_empty(path)) {
        if (fs::exists(path / "doom_dumper") 
            && confirmPrompt("Old DOOM Eternal Installation found in specified path. Replace it?")) 
        {
            fs::remove_all(path);
        }
        else {
            std::cout << YELLOW << "Specified path is not empty.\n" << RESET;
            return false;
        }
    }

    auto space = fs::space(path.root_path());
    free = space.available;

    if (free < MIN_FREE) {
        std::cout << YELLOW << "Not enough space in " << path
            << " (" << formattedSize(free) << " free, " << formattedSize(MIN_FREE) << " required)\n" << RESET;
        return false;
    }

    return true;
}

std::string getPath()
{
    // TODO: allow a drive letter as the only input, and create a path from that
    while (true) {
        std::string input_str;
        uint64_t free;

        std::cout << "Enter a path to re-install DOOM Eternal to (leave blank to choose automatically): ";
        std::getline(std::cin, input_str);

        fs::path path(input_str);

        if (path.empty()) {  // No input
            if (!getDefaultPath(path)) {
                return "";
            }
        }

        if (!preparePath(path, free)) {
            std::cout << "\n";
            continue;
        }

        std::string prompt = "Are you sure you would like to install your game to '" + path.string() +
            "', with " + formattedSize(free) + " free?";

        if (confirmPrompt(prompt)) {
            if (!fs::exists(path)) {
                try
                {
                    fs::create_directories(path);
                }
                catch (fs::filesystem_error& e)
                {
                    std::cerr << YELLOW << e.what() << "\n" << RESET;
                    continue;
                }
            }

            return path.string();
        }

        std::cout << "\n";
    }
}