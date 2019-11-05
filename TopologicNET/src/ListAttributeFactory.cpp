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
#ifdef max
#undef max
#endif
#include "ListAttributeFactory.h"
#include "AttributeFactoryManager.h"

#include <TopologicCore/include/ListAttribute.h>

#include <assert.h>

using namespace System::Collections;
using namespace System::Collections::Generic;

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ ListAttributeFactory::Create(Object ^ value)
		{
			ArrayList^ listValue = safe_cast<ArrayList^>(value);
			std::list<TopologicCore::Attribute::Ptr> coreAttributes;
			System::Collections::IEnumerator^ listEnumerator = listValue->GetEnumerator();
			while (listEnumerator->MoveNext())
			{
				Object^ value = safe_cast<Object^>(listEnumerator->Current);
				AttributeFactory^ attributeFactory = AttributeFactoryManager::Instance->GetFactory(value);
				Attribute^ attribute = attributeFactory->Create(value);
				coreAttributes.push_back(attribute->UtilitiesAttribute);
			}
			return gcnew Attribute(
				std::dynamic_pointer_cast<TopologicCore::Attribute>(
					std::make_shared<TopologicCore::ListAttribute>(coreAttributes)
					)
			);
		}

		bool ListAttributeFactory::CheckType(Type ^ type)
		{
			typedef ArrayList ListHandle;
			System::Type^ ListHandleType = ListHandle::typeid;

			return type == ListHandleType;
		}

		bool ListAttributeFactory::CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicCore::ListAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ ListAttributeFactory::CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			std::list<TopologicCore::Attribute::Ptr>* pListValue = static_cast<std::list<TopologicCore::Attribute::Ptr>*>(pValue);
			if (pListValue == nullptr)
			{
				assert(false);
				return nullptr;
			}

			ArrayList^ attributeList = gcnew ArrayList();
			for (const TopologicCore::Attribute::Ptr& kpCoreAttribute : *pListValue)
			{
				AttributeFactory^ attributeFactory = AttributeFactoryManager::Instance->GetFactory(kpCoreAttribute);
				Object^ obj = attributeFactory->CreateValue(kpCoreAttribute);
				attributeList->Add(obj);
			}
			return attributeList;
		}
	}
}