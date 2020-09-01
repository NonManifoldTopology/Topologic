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
			static IList<Edge^>^ AdjacentEdges(Vertex^ vertex, Topology^ parentTopology);

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