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
