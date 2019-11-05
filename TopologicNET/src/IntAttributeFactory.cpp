// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "IntAttributeFactory.h"

#include <TopologicCore/include/IntAttribute.h>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ IntAttributeFactory::Create(Object ^ value)
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