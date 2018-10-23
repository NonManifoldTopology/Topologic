#include <msclr/marshal_cppstd.h>
#include "AttributeManager.h"
#include "AttributeFactory.h"
#include "IntAttributeFactory.h"
#include "DoubleAttributeFactory.h"
#include "StringAttributeFactory.h"
#include "Topology.h"

#include <TopologicUtility/include/IntAttribute.h>
#include <TopologicUtility/include/DoubleAttribute.h>
#include <TopologicUtility/include/StringAttribute.h>
#include <TopologicUtility/include/AttributeManager.h>

namespace Topologic
{
	AttributeManager::AttributeManager()
	{
		// TODO: how to add user-defined attributes?
		m_attributeFactoryDict->Add(gcnew String(TopologicUtility::IntAttributeGUID::Get().c_str()), gcnew IntAttributeFactory());
		m_attributeFactoryDict->Add(gcnew String(TopologicUtility::DoubleAttributeGUID::Get().c_str()), gcnew DoubleAttributeFactory());
		m_attributeFactoryDict->Add(gcnew String(TopologicUtility::StringAttributeGUID::Get().c_str()), gcnew StringAttributeFactory());
	}

	void AttributeManager::SetAttribute(Topology ^ topology, String ^ key, Object ^ value)
	{
		AttributeFactory^ attributeFactory = GetFactory(value);
		Attribute^ attribute = attributeFactory->Create(key, value);
		std::string cppKey = msclr::interop::marshal_as<std::string>(key);
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = 
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		// Add the attribute
		TopologicUtility::AttributeManager::GetInstance().Add(pCoreTopology, cppKey, attribute->SupportAttribute);
	}

	AttributeFactory ^ AttributeManager::GetFactory(const std::shared_ptr<TopologicUtility::Attribute> kpSupportAttribute)
	{
		for each(KeyValuePair<String^, AttributeFactory^>^ entry in m_attributeFactoryDict)
		{
			bool isValueCorrect = entry->Value->CheckType(kpSupportAttribute);
			if (isValueCorrect)
			{
				return entry->Value;
			}
		}

		throw gcnew Exception("Attribute is not currently supported.");
	}

	AttributeFactory ^ AttributeManager::GetFactory(Object ^ value)
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