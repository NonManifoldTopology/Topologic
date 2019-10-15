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

			AttributeFactory^ GetFactory(Object^ value);

		private:
			AttributeFactoryManager();
			AttributeFactoryManager(const AttributeFactoryManager%) { throw gcnew System::InvalidOperationException("Singleton cannot be copy-constructed"); }

			static AttributeFactoryManager m_instance;
			Dictionary<String^, AttributeFactory^>^ m_attributeFactoryDict = gcnew Dictionary<String^, AttributeFactory^>();
		};
	}
}