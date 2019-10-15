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

#include "IntAttributeFactory.h"

#include <TopologicCore/include/IntAttribute.h>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ IntAttributeFactory::Create(String ^ key, Object ^ value)
		{
			long long int intValue = safe_cast<long long int>(value);
			return gcnew Attribute(std::dynamic_pointer_cast<TopologicCore::Attribute>(std::make_shared<TopologicCore::IntAttribute>(intValue)));
		}

		bool IntAttributeFactory::CheckType(Type ^ type)
		{
			typedef long long int ^ IntHandle;
			System::Type^ intHandleType = IntHandle::typeid;

			return type == intHandleType;
		}

		bool IntAttributeFactory::CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicCore::IntAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ IntAttributeFactory::CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			long long int* pIntValue = static_cast<long long int*>(pValue);
			return *pIntValue;
		}
	}
}