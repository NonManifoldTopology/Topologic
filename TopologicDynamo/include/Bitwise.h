#pragma once

#include <TopologicCore/include/Bitwise.h>

namespace Topologic {
	namespace Utility {
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