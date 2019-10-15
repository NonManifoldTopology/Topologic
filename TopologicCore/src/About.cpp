// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

#include "About.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 8
#define EXTERNAL_BUILD_VERSION 7
#define INTERNAL_BUILD_VERSION 0

namespace TopologicCore
{
	std::string About::Version()
	{
		std::string version = std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(EXTERNAL_BUILD_VERSION);
		if (INTERNAL_BUILD_VERSION > 0)
		{
			version = version + "." + std::to_string(INTERNAL_BUILD_VERSION);
		}
		return version;
	}
}