#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace TopologicCore
{
	class Attribute;
}

namespace Topologic
{
	ref class Topology;

	namespace Attributes
	{
		ref class AttributeFactory;

		/// <summary>
		/// An AttributeFactoryManager stores the subclasses of AttributeFactory, identified by their IDs. This class is used to identify the attribute type. The ID is in the GUID format, represented as a String. A user-created AttributeFactory must be registered with a unique ID to this class.
		/// </summary>
		ref class AttributeFactoryManager
		{
		public protected:
			static property AttributeFactoryManager^ Instance {
				AttributeFactoryManager^ get() { return %m_instance; }
			}

			void SetAttribute(Topology^ topology, String^ key, Object^ value);

			AttributeFactory^ GetFactory(const std::shared_ptr<TopologicCore::Attribute> kpUtilitiesAttribute);

		private:
			AttributeFactoryManager();
			AttributeFactoryManager(const AttributeFactoryManager%) { throw gcnew System::InvalidOperationException("Singleton cannot be copy-constructed"); }

			AttributeFactory^ GetFactory(Object^ value);

			static AttributeFactoryManager m_instance;
			Dictionary<String^, AttributeFactory^>^ m_attributeFactoryDict = gcnew Dictionary<String^, AttributeFactory^>();
		};
	}
}