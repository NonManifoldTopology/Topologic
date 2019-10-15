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

#include "Attribute.h"
#include "Utilities.h"

#include <string>

namespace TopologicCore
{
	class IntAttributeGUID {
	public:
		static std::string Get()
		{
			return std::string("1a8944c6-23e0-4607-9313-336f4ab3fd53");
		}
	};

	class IntAttribute :public Attribute
	{
	public:
		typedef std::shared_ptr<IntAttribute> Ptr;

		TOPOLOGIC_API IntAttribute(const long long int kValue);
		virtual void* Value();
		long long int IntValue() { return m_value; }

	protected:
		long long int m_value;
	};
}