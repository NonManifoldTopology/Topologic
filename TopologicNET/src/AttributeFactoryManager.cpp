#include <msclr/marshal_cppstd.h>

#include "AttributeFactoryManager.h"
#include "AttributeFactory.h"
#include "IntAttributeFactory.h"
#include "DoubleAttributeFactory.h"
#include "StringAttributeFactory.h"
#include "Topology.h"

#include <TopologicUtilities/include/IntAttribute.h>
#include <TopologicUtilities/include/DoubleAttribute.h>
#include <TopologicUtilities/include/StringAttribute.h>
#include <TopologicUtilities/include/AttributeManager.h>

namespace Topologic
{
	namespace Attributes
	{
		AttributeFactoryManager::AttributeFactoryManager()
		{
			// TODO: how to add user-defined attributes?
			m_attributeFactoryDict->Add(gcnew String(TopologicUtilities::IntAttributeGUID::Get().c_str()), gcnew IntAttributeFactory());
			m_attributeFactoryDict->Add(gcnew String(TopologicUtilities::DoubleAttributeGUID::Get().c_str()), gcnew DoubleAttributeFactory());
			m_attributeFactoryDict->Add(gcnew String(TopologicUtilities::StringAttributeGUID::Get().c_str()), gcnew StringAttributeFactory());
		}

		void AttributeFactoryManager::SetAttribute(Topology ^ topology, String ^ key, Object ^ value)
		{
			AttributeFactory^ attributeFactory = GetFactory(value);
			Attribute^ attribute = attributeFactory->Create(key, value);
			std::string cppKey = msclr::interop::marshal_as<std::string>(key);
			std::shared_ptr<TopologicCore::Topology> pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

			// Add the attribute
			TopologicUtilities::AttributeManager::GetInstance().Add(pCoreTopology, cppKey, attribute->UtilitiesAttribute);
		}

		AttributeFactory ^ AttributeFactoryManager::GetFactory(const std::shared_ptr<TopologicUtilities::Attribute> kpUtilitiesAttribute)
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