#include <msclr/marshal_cppstd.h>

#include "AttributeFactoryManager.h"
#include "AttributeFactory.h"
#include "IntAttributeFactory.h"
#include "DoubleAttributeFactory.h"
#include "StringAttributeFactory.h"
#include "Topology.h"

#include <TopologicCore/include/IntAttribute.h>
#include <TopologicCore/include/DoubleAttribute.h>
#include <TopologicCore/include/StringAttribute.h>
#include <TopologicCore/include/AttributeManager.h>

namespace Topologic
{
	namespace Attributes
	{
		AttributeFactoryManager::AttributeFactoryManager()
		{
			// TODO: how to add user-defined attributes?
			m_attributeFactoryDict->Add(gcnew String(TopologicCore::IntAttributeGUID::Get().c_str()), gcnew IntAttributeFactory());
			m_attributeFactoryDict->Add(gcnew String(TopologicCore::DoubleAttributeGUID::Get().c_str()), gcnew DoubleAttributeFactory());
			m_attributeFactoryDict->Add(gcnew String(TopologicCore::StringAttributeGUID::Get().c_str()), gcnew StringAttributeFactory());
		}

		void AttributeFactoryManager::SetAttribute(Topology ^ topology, String ^ key, Object ^ value)
		{
			AttributeFactory^ attributeFactory = GetFactory(value);
			Attribute^ attribute = attributeFactory->Create(key, value);
			std::string cppKey = msclr::interop::marshal_as<std::string>(key);
			std::shared_ptr<TopologicCore::Topology> pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

			// Add the attribute
			TopologicCore::AttributeManager::GetInstance().Add(pCoreTopology, cppKey, attribute->UtilitiesAttribute);
		}

		AttributeFactory ^ AttributeFactoryManager::GetFactory(const std::shared_ptr<TopologicCore::Attribute> kpUtilitiesAttribute)
		{
			for each(KeyValuePair<String^, AttributeFactory^>^ entry in m_attributeFactoryDict)
			{
				bool isValueCorrect = entry->Value->CheckType(kpUtilitiesAttribute);
				if (isValueCorrect)
				{
					return entry->Value;
				}
			}

			throw gcnew Exception("Attribute is not currently supported.");
		}

		AttributeFactory ^ AttributeFactoryManager::GetFactory(Object ^ value)
		{
			System::Type^ entryValueType = value->GetType();
			for each(KeyValuePair<String^, AttributeFactory^>^ entry in m_attributeFactoryDict)
			{
				bool isValueCorrect = entry->Value->CheckType(entryValueType);
				if (isValueCorrect)
				{
					return entry->Value;
				}
			}

			throw gcnew Exception("Attribute value of type " + entryValueType->Name + " is not currently supported.");
		}
	}
}