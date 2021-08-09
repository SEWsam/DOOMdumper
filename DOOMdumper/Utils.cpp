/* 
This file (Utils.cpp) is a part of DOOMdumper.

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
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>

#include <Windows.h>
#include <winreg.h>

#include "Utils.hpp"
#include "DOOMdumper.hpp"

std::wstring stringToWstring(const std::string string)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &string[0], int(string.size()), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &string[0], int(string.size()), &wstrTo[0], size_needed);
    return wstrTo;
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
    const std::string units[] = { "B", "KB", "MB", "GB", "TB", "PB" };  // PB should be enough...

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
