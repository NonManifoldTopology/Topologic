#pragma once

#include "Cell.h"

namespace Topologic {
	namespace Support {

		/// <summary>
		/// 
		/// </summary>
		public ref class CellUtility
		{
		public:
			/// <summary>
			/// 
			/// </summary>
			/// <param name="vertices"></param>
			/// <param name="faceIndices"></param>
			/// <returns name="Cell"></returns>
			static Cell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);

			/// <summary>
			/// Creates a cell by lofting through a set of wires.
			/// </summary>
			/// <param name="wires">A set of wires</param>
			/// <returns name="Cell">The created cell</returns>
			static Cell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="cell"></param>
			/// <returns></returns>
			static double Volume(Cell^ cell);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="cell"></param>
			/// <param name="vertex"></param>
			/// <returns></returns>
			static bool Contains(Cell^ cell, Vertex^ vertex);

		public protected:
			CellUtility() {}
		};
	}
}