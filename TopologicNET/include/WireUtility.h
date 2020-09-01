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
			static IList<Shell^>^ AdjacentShells(Wire^ wire, Topology^ parentTopology);

			/// <summary>
			/// Returns a list of Cells that are the adjacent to the input Wire.
			/// </summary>
			/// <param name="wire">A Wire</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <returns name="Cell[]">A list of Cells adjacent to the input Wire</returns>
			static IList<Cell^>^ AdjacentCells(Wire^ wire, Topology^ parentTopology);

			/// <summary>
			/// Remove collinear edges in a Wire.
			/// </summary>
			/// <param name="wire">A Wire</param>
			/// <param name="tolerance">A positive tolerance value</param>
			/// <returns name="Wire">A Wire with collinear edges removed</returns>
			static Wire^ RemoveCollinearEdges(Wire^ wire, 
#ifdef TOPOLOGIC_DYNAMO
				[DefaultArgument("0.0001")]
#endif
			double tolerance);
		public protected:
			WireUtility() {}
		};
	}
}
