#pragma once

#include <TopologicCore/include/Bitwise.h>

using namespace System::Collections::Generic;

namespace Topologic {
	namespace Utilities {
	/// <summary>
	/// Utility provides extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
		
		/// <summary>
		/// Bitwise operators such as AND, OR, XOR and NOT are included in this class.
		/// </summary>

		public ref class Bitwise
		{
		public:
			/*static int And(int argument1, int argument2);

			static int Or(int argument1, int argument2);

			static int Xor(int argument1, int argument2);*/

			static int And(List<int>^ arguments);

			static int Or(List<int>^ arguments);

			static int Xor(List<int>^ arguments);


			static int Not(int argument);

		protected:
			Bitwise() {} 
		};
	}
}