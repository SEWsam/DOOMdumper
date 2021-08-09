/* 
This file (Packages.hpp) is a part of DOOMdumper.

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

#include <winrt/Windows.ApplicationModel.h>

namespace winrt { using namespace Windows::ApplicationModel; }


std::optional<winrt::Package> getPackage(const std::wstring pfn = L"BethesdaSoftworks.DOOMEternal-PC_3275kfvn8vcwc");
bool validatePackage(const winrt::Package& pkg);
bool removePackage(const winrt::Package& pkg);
bool registerPackage(const std::wstring wpath);
std::string getRegisteredPath(const winrt::Package& pkg);