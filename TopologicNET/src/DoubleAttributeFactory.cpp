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

#include "DoubleAttributeFactory.h"
#include "Attribute.h"

#include <TopologicCore/include/DoubleAttribute.h>

#include <memory>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ DoubleAttributeFactory::Create(String ^ key, Object ^ value)
		{
			double doubleValue = safe_cast<double>(value);
			return gcnew Attribute(std::dynamic_pointer_cast<TopologicCore::Attribute>(std::make_shared<TopologicCore::DoubleAttribute>(doubleValue)));
		}

		bool DoubleAttributeFactory::CheckType(Type ^ type)
		{
			typedef double ^ DoubleHandle;
			System::Type^ doubleHandleType = DoubleHandle::typeid;

			return type == doubleHandleType;
		}

		bool DoubleAttributeFactory::CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicCore::DoubleAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ DoubleAttributeFactory::CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			double* pDoubleValue = static_cast<double*>(pValue);
			return *pDoubleValue;
		}
	}
}