/* 
This file (Processes.cpp) is a part of DOOMdumper.

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
#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <tchar.h>

#include "DOOMdumper.hpp"

bool killProcessByPid(const DWORD pid)
{
    bool result = false;

    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, pid); // | SYNCHRONIZE 

    if (process != NULL) {
        HMODULE hmod;
        DWORD cb_needed;

        if (EnumProcessModules(process, &hmod, sizeof(hmod),
            &cb_needed))
        {

            TerminateProcess(process, 9);
            //DWORD wait_result = WaitForSingleObject(process, 3000);
            //if (wait_result != WAIT_OBJECT_0)
            //{} // result = true;
            result = true;
        }
    }

    CloseHandle(process);
    return result;
}

bool matchProcessName(const DWORD pid, const WCHAR* match_name)
{
    bool result = false;
    WCHAR proc_name[MAX_PATH] = L"<Unknown>";

    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (process != NULL) {
        HMODULE hmod;
        DWORD cb_needed;

        if (EnumProcessModules(process, &hmod, sizeof(hmod),
            &cb_needed))
        {
            GetModuleBaseName(process, hmod, proc_name,
                sizeof(proc_name) / sizeof(TCHAR));

            if (wcscmp(proc_name, match_name) == 0) {
                result = true;
            }
        }
    }

    CloseHandle(process);
    return result;
}

int getPidByName(const WCHAR* name) {
    DWORD processes[1024], cbNeeded, cbProcesses;

    BOOL procBool = EnumProcesses(processes, sizeof(processes), &cbNeeded);
    if (!procBool) {
        return -1;  // unable to iterate processes.
    }

    cbProcesses = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < cbProcesses; i++) {
        if (matchProcessName(processes[i], name)) {
            return processes[i];
        }
    }

    return 0;
}

int promptForProcess(const WCHAR* name)
{
    int pid = 0;
    while (true) {
        std::cout << "Please open DOOM Eternal, then ";
        std::system("pause");

        pid = getPidByName(name);
        if (pid == 0) {
            std::cout << YELLOW << "Could not find DOOM Eternal running process. Trying again . . .\n\n" << RESET;
        }
        else {
            std::cout << "Sucessfully found DOOM Eternal running process!\n";
            std::cout << YELLOW << "Please do NOT close DOOM Eternal.\n\n" << RESET;
            break;
        }
    }

    return pid;
}