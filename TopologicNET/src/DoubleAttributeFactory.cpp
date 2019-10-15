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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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