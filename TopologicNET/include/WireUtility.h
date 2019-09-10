#pragma once

#include "Wire.h"

namespace Topologic {
	namespace Utilities {
		/// <summary>
		/// WireUtility includes geometric methods relevant to a Wire.
		/// </summary>
		public ref class WireUtility
		{
		public:
			/// <summary>
			/// Returns a list of Shells that are the adjacent to the input Wire.
			/// </summary>
			/// <param name="wire">A Wire</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <returns name="Shell[]">A list of Shells adjacent to the input Wire</returns>
			static List<Shell^>^ AdjacentShells(Wire^ wire, Topology^ parentTopology);

			/// <summary>
			/// Returns a list of Cells that are the adjacent to the input Wire.
			/// </summary>
			/// <param name="wire">A Wire</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <returns name="Cell[]">A list of Cells adjacent to the input Wire</returns>
			static List<Cell^>^ AdjacentCells(Wire^ wire, Topology^ parentTopology);

		public protected:
			WireUtility() {}
		};
	}
}
