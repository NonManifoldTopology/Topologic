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

#include <msclr/marshal_cppstd.h>
#include "StringAttributeFactory.h"

#include <TopologicCore/include/StringAttribute.h>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ StringAttributeFactory::Create(String ^ key, Object ^ value)
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