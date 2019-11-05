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

#include <msclr/marshal_cppstd.h>
#include "StringAttributeFactory.h"

#include <TopologicCore/include/StringAttribute.h>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ StringAttributeFactory::Create(Object ^ value)
		{
			String^ strValue = safe_cast<String^>(value);
			std::string cppStrValue = msclr::interop::marshal_as<std::string>(strValue);
			return gcnew Attribute(std::dynamic_pointer_cast<TopologicCore::Attribute>(std::make_shared<TopologicCore::StringAttribute>(cppStrValue)));
		}

		bool StringAttributeFactory::CheckType(Type ^ type)
		{
			System::Type^ stringHandleType = String::typeid;

			return type == stringHandleType;
		}

		bool StringAttributeFactory::CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicCore::StringAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ StringAttributeFactory::CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			std::string* pStringValue = static_cast<std::string*>(pValue);
			return gcnew String(pStringValue->c_str());
		}
	}
}