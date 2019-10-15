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

#include "Vertex.h"

namespace Topologic {
	namespace Utilities {


		/// <summary>
		/// VertexUtility includes geometric methods relevant to a Vertex.
		/// </summary>
		public ref class VertexUtility
		{
		public:
			/// <summary>
			/// Returns a list of Edges that are the adjacent to the input Vertex.
			/// </summary>
			/// <param name="vertex">A Vertex</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <returns name="Edge[]">A list of Edges adjacent to the input Vertex</returns>
			static List<Edge^>^ AdjacentEdges(Vertex^ vertex, Topology^ parentTopology);

			/// <summary>
			/// Returns the distance between a Vertex and a Topology.
			/// </summary>
			/// <param name="vertex">A Vertex</param>
			/// <param name="topology">A Topology</param>
			/// <returns>The distance between a Vertex and a Topology</returns>
			static double Distance(Vertex^ vertex, Topology^ topology);

		public protected:
			VertexUtility() {}
		};
	}
}