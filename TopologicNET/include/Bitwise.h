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

#include <TopologicCore/include/Bitwise.h>

using namespace System::Collections::Generic;

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	
		
		/// <summary>
		/// Bitwise operators such as AND, OR, XOR and NOT are included in this class.
		/// </summary>
		public ref class Bitwise
		{
		public:
			/// <summary>
			/// Performs a bitwise AND operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int AND(List<int>^ arguments);

			/// <summary>
			/// Performs a bitwise OR operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int OR(List<int>^ arguments);

			/// <summary>
			/// Performs a bitwise XOR operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int XOR(List<int>^ arguments);

			/// <summary>
			/// Performs a bitwise NOT operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int NOT(int argument);

		protected:
			Bitwise() {} 
		};
	}
}