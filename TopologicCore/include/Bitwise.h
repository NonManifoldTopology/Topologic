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

#pragma once

#include "Utilities.h"

#include <list>

namespace TopologicUtilities
{
	class Bitwise
	{
	public:
		static TOPOLOGIC_API int AND(const std::list<int>& rArguments);

		static TOPOLOGIC_API int OR(const std::list<int>& rArguments);

		static TOPOLOGIC_API int XOR(const std::list<int>& rArguments);

		static TOPOLOGIC_API int NOT(const int kArgument1);

		static TOPOLOGIC_API unsigned int NOT(const unsigned int kArgument1);
	};
}