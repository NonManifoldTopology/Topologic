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

#pragma once

#include "AttributeFactory.h"

namespace Topologic
{
	namespace Attributes
	{
		/// <summary>
		/// (private) A factory class to create a String Attribute.
		/// </summary>
		ref class StringAttributeFactory : AttributeFactory
		{
		public:
			/// <summary>
			/// Creates an Attribute from a key and a value.
			/// </summary>
			/// <param name="key">A key</param>
			/// <param name="value">A value</param>
			/// <returns name="Attribute">The created Attribute</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual Attribute^ Create(String^ key, Object^ value) override;

			/// <summary>
			/// Checks the type of the value.
			/// </summary>
			/// <param name="type">The type</param>
			/// <returns name="bool">True if the value is of the correct type, otherwise false</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual bool CheckType(Type^ type) override;

		public protected:
			virtual bool CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute) override;

			virtual Object^ CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute) override;

		protected:
		};
	}
}