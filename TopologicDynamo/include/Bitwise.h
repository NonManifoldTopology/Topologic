#pragma once

#include <TopologicCore/include/Bitwise.h>

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
			static int And(int argument1, int argument2);

			static int Or(int argument1, int argument2);

			static int Xor(int argument1, int argument2);

			static int Not(int argument);

		protected:
			Bitwise() {} 
		};
	}
}