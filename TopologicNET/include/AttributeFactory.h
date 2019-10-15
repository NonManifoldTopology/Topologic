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

#pragma once

using namespace System;
#ifdef TOPOLOGIC_DYNAMO
using namespace Autodesk::DesignScript::Runtime;
#endif

#include <Attribute.h>

namespace TopologicUtilities
{
	class Attribute;
}

namespace Topologic
{
	namespace Attributes
	{
		/// <summary>
		/// A public abstract factory class to create an Attribute. This class should be inherited when new Attribute classes are implemented.
		/// </summary>
		public ref class AttributeFactory abstract
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
			virtual Attribute^ Create(String^ key, Object^ value) abstract;

			/// <summary>
			/// Checks the type of the value.
			/// </summary>
			/// <param name="type">The type</param>
			/// <returns name="bool">True if the value is of the correct type, otherwise false</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual bool CheckType(Type^ type) abstract;

		public protected:
			virtual bool CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute) abstract;

			virtual Object^ CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute) abstract;

		protected:
			AttributeFactory() {}
		};
	}
}