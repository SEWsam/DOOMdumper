/* 
This file (Processes.hpp) is a part of DOOMdumper.

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

bool killProcessByPid(const DWORD pid);
bool matchProcessName(const DWORD pid, const WCHAR* match_name);
int getPidByName(const WCHAR* name);
int promptForProcess(const WCHAR* name);