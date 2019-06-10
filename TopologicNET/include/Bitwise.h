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
			/// Performs a bitwise And operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int And(List<int>^ arguments);

			/// <summary>
			/// Performs a bitwise Or operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int Or(List<int>^ arguments);

			/// <summary>
			/// Performs a bitwise Xor operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int Xor(List<int>^ arguments);

			/// <summary>
			/// Performs a bitwise Not operation between the arguments (in their binary form).
			/// </summary>
			/// <param name="arguments">A list of arguments</param>
			/// <returns>The bitwise result</returns>
			static int Not(int argument);

		protected:
			Bitwise() {} 
		};
	}
}