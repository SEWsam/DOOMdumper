/* 
This file (Packages.cpp) is a part of DOOMdumper.

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
#include <optional>

#include <winrt/Windows.Management.Deployment.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.h>

#include "DOOMdumper.hpp"
#include "DebugTools.hpp"

namespace winrt
{
	using namespace Windows::Management::Deployment;
	using namespace Windows::ApplicationModel;
	using namespace Windows::Foundation::Collections;
	using namespace Windows::Foundation;
}

std::optional<winrt::Package> getPackage(const std::wstring pfn) 
{
	DebugStream dbgs(true);
	winrt::hstring pkg_family{ pfn };

	winrt::PackageManager pkgmgr;
	winrt::IIterable<winrt::Package> results = pkgmgr.FindPackages(pkg_family); // Iterable - holds an iterator
	winrt::IIterator<winrt::Package> results_iter = results.First();  // The acutal iterator

	if (!results_iter.HasCurrent()) {
		return std::nullopt;
	}

	return results_iter.Current();
}

bool validatePackage(const winrt::Package& pkg)
{
	auto pkg_status = pkg.Status();
	auto pkg_id = pkg.Id();

	if (pkg_id.Version() != GAME_VERSION)
	{
		std::cerr << RED << "ERROR: The installed version of DOOM Eternal is NOT compatible with this version of DOOMdumper.\n"
			             << "This release of DOOMdumper is for DOOM Eternal Version " << GAME_VERSION_STR << "\n" 
					     << "Make sure you have the latest version of DOOMdumper. If your game is out of date, reinstall it.\n" << RESET;
	}

	if (pkg_status.DataOffline() || pkg_status.PackageOffline()) {
		std::cerr << RED << "ERROR: DOOM Eternal is not accessible. Was it installed on an external drive?\n" << RESET;
		return false;
	}

	if (pkg_status.DependencyIssue()) {
		std::cerr << RED << "ERROR: There is a dependency issue with DOOM Eternal. Try launching your game once, then run this agian.\n" << RESET;
		return false;
	}

	if (pkg_status.Servicing()) {
		std::cerr << RED << "ERROR: DOOM Eternal installation is in progress or an update may be installing.\n" << RESET;
		return false;
	}

	if (pkg_status.Disabled()) {
		std::cerr << RED << "ERROR: DOOM Eternal is disabled. You may have to reinstall it.\n" << RESET;
		return false;
	}

	if (pkg_status.LicenseIssue()) {
		std::cerr << RED << "ERROR: There is a license issue with DOOM Eternal . . .\n"
			<< "This could be caused by using the wrong account, expired gamepass, etc.\n" << RESET;
		return false;
	}

	if (pkg_status.NotAvailable() || pkg_status.NeedsRemediation()) {
		std::cerr << RED << "ERROR: DOOM Eternal's Package is unavailable for an unknown reason.\n" << RESET;
		return false;
	}

	return true;
}

bool removePackage(const winrt::Package& pkg)
{
	winrt::PackageId pkg_id = pkg.Id();
	winrt::hstring pkg_fullname{ pkg_id.FullName() };

	winrt::PackageManager pkgmgr;

	auto removal{ pkgmgr.RemovePackageAsync(pkg_fullname) };
	removal.get();

	if (removal.Status() == winrt::AsyncStatus::Completed)
	{
		std::cout << "Old installation sucessfully removed.\n";
		return true;
	}
	else if (removal.Status() == winrt::AsyncStatus::Canceled)
	{
		std::cerr << "ERROR: Removal canceled. (This is not normal, contact SEWsam)\n" << RESET;
		return false;
	}
	else if (removal.Status() == winrt::AsyncStatus::Error)
	{
		std::cerr << RED << "ERROR: Failed removing old DOOM Eternal installation.\n" << RESET;
		return false;
	}
	else
	{
		std::cerr << RED << "ERROR: Removal status unknown. (This is not normal, contact SEWsam)\n" << RESET;
		return false;
	}
}

bool registerPackage(const std::wstring wpath)
{
	winrt::Uri path{ wpath };

	winrt::PackageManager pkgmgr;
	winrt::RegisterPackageOptions register_options;
	register_options.DeveloperMode(true);
	auto registration{ pkgmgr.RegisterPackageByUriAsync(path, register_options) };
	registration.get();
	
	if (registration.Status() == winrt::AsyncStatus::Completed)
	{
		std::cout << "Succeeded setting up new game installion.\n";
		return true;
	}
	else if (registration.Status() == winrt::AsyncStatus::Canceled)
	{
		std::cerr << "ERROR: Registration canceled. (This is not normal, contact SEWsam)\n" << RESET;
		return false;
	}
	else if (registration.Status() == winrt::AsyncStatus::Error) 
	{
		std::cerr << RED << "ERROR: Failed setting up new DOOM Eternal installation.\n" << RESET;
		return false;
	}
	else
	{
		std::cerr << RED << "ERROR: Registration status unknown. (This is not normal, contact SEWsam)\n" << RESET;
		return false;
	}
}

std::string getRegisteredPath(const winrt::Package& pkg)
{
	if (pkg.IsDevelopmentMode())
		return winrt::to_string(pkg.InstalledPath());
	else
		return "";
}