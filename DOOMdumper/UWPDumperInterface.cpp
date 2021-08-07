/* 
This file (UWPDumperInterface.cpp) is a part of DOOMdumper.

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

#include <UWPInjector.hpp>
#include <iostream>

#include "DOOMdumper.hpp"

bool dumpWithStatus(const int pid, const std::string path, const bool verboseish)
{
	bool ExitStatus = false;
	int proc_id = pid;

	UWPDumper::UWPInjector injector(proc_id, path, verboseish);

	try {
		injector.DumperInject();

		std::wstring msg;
		UWPDumper::DumperError err;
		float progress;
		std::cout << RESET << "\n";

		ProgressBar pb;
		while (injector.ValidThread())
		{
			while (injector.PopMessage(msg, err, progress))
			{
				if (!nocolors) {
					pb.Clear();
				}

				std::wcout << msg;

				if (progress != 0 && !nocolors) {
					pb.Display(progress);
				}

				if (err != UWPDumper::DumperError::none) {
					throw err;
				}
			}
		}
		std::cout << GREEN << "Done\n" << RESET;
		ExitStatus = true;
	}
	catch (UWPDumper::DumperError& e) {
		switch (e)
		{
		case UWPDumper::DumperError::subdir:
			std::cerr << RED << "\nERROR: Couldn't create subdirs; Dump path doesn't exist.\n" << RESET;
			break;

		case UWPDumper::DumperError::read:
			std::cerr << RED << "\nERROR: Couldn't read game files.\n" << RESET;
			break;

		case UWPDumper::DumperError::write:
			std::cerr << RED << "\nERROR: Couldn't open files for writing.\n" << RESET;
			break;

		case UWPDumper::DumperError::copy:
			std::cerr << RED << "\nERROR: Failed to copy file data to dump.\n" << RESET;
			break;

		default:
			std::cerr << RED << "\nERROR: An exception, detailed above, occurred while dumping.\n" << RESET;
			break;
		}
		std::cerr << RED << "Failed dumping...\n" << RESET;
	}
	catch (UWPDumper::InjectorError& e) {
		std::cerr << RED << "\nFailed injection\n" << RESET;
	}

	return ExitStatus;
}