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

#include "Attribute.h"
#include "AttributeFactoryManager.h"
#include "AttributeFactory.h"

namespace Topologic
{
	namespace Attributes
	{
		TopologicCore::Attribute::Ptr Attribute::UtilitiesAttribute::get()
		{
			return *m_pUtilitiesAttribute;
		}

		Attribute::Attribute(const TopologicCore::Attribute::Ptr& kpUtilitiesAttribute)
			: m_pUtilitiesAttribute(new TopologicCore::Attribute::Ptr(kpUtilitiesAttribute))
		{

		}

		Attribute::~Attribute()
		{

		}
	}
}