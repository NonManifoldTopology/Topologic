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

#include "Face.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	
		/// <summary>
		/// FaceUtility includes geometric methods relevant to a Face.
		/// </summary>
		public ref class FaceUtility
		{
		public:
			/// <summary>
			/// Returns the area of a Face.
			/// </summary>
			/// <param name="face">The Face</param>
			/// <returns>The area of the Face</returns>
			static double Area(Face^ face);

			/// <summary>
			/// Creates a Face (of any type) by a set of Vertices.
			/// </summary>
			/// <param name="vertices">A Set of Vertices</param>
			/// <returns name="Face">The created Face</returns>
			static Face^ ByVertices(System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<Vertex^>^>^ vertices);

			/// <summary>
			/// Returns the UV parameters at a given Vertex on a Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="vertex">A Vertex on a Face</param>
			/// <returns name="UV">The UV parameters</returns>
			static IEnumerable<double>^ ParametersAtVertex(Face^ face, Vertex^ vertex);

			/// <summary>
			/// Returns the normal (vector) given the UV parameters of a Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="u">The U parameter of the Face</param>
			/// <param name="v">The U parameter of the Face</param>
			/// <returns>The normal (vector) at a parameter of a Face</returns>
			static IEnumerable<double>^ NormalAtParameters(Face^ face, double u, double v);

			/// <summary>
			/// Returns the Vertex at a given parameter of the Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="u">The U parameterof the Vertex on the Face</param>
			/// <param name="v">The V parameter of the Vertex on the Face</param>
			/// <returns name="Vertex">The Vertex at the given parameter of the Face</returns>
			static Vertex^ VertexAtParameters(Face^ face, double u, double v);

			/// <summary>
			/// Trims a Face with a Wire. The portion of the Face inside the Wire will be returned.
			/// </summary>
			/// <param name="face">The Face to be trimmed</param>
			/// <param name="wire">The Wire with which the Face will be trimmed</param>
			/// <param name="reverseWire">If true, reverse the orientation of the wire</param>
			/// <returns name="Face">The trimmed Face</returns>
			static Face^ TrimByWire(Face^ face, Wire^ wire, bool reverseWire);

			/// <summary>
			/// Triangulates a Face given a deflection value.
			/// </summary>
			/// <param name="face">The Face to be triangulated</param>
			/// <param name="deflection">The deflection value to be applied for triangulation</param>
			/// <returns name="Face">The triangulated Face</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static IEnumerable<Face^>^ Triangulate(Face^ face, double deflection);

			/// <summary>
			/// Returns a list of Shells that are the adjacent to the input Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <returns name="Shell[]">A list of Shells adjacent to the input Face</returns>
			static IEnumerable<Shell^>^ AdjacentShells(Face^ face, Topology^ parentTopology);

			/// <summary>
			/// Returns a list of Cells that are the adjacent to the input Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <returns name="Cell[]">A list of Cells adjacent to the input Face</returns>
			static IEnumerable<Cell^>^ AdjacentCells(Face^ face, Topology^ parentTopology);

			/// <summary>
			/// Returns a Vertex inside the Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="tolerance">A positive tolerance value</param>
			/// <returns name="Vertex">A Vertex inside the Face</returns>
			static Vertex^ InternalVertex(
				Face^ face,
#ifdef TOPOLOGIC_DYNAMO
				[DefaultArgument("0.0001")]
#endif
			double tolerance);

			/// <summary>
			/// Checks if a Vertex is located inside a Face within a tolerance value.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="vertex">A Vertex</param>
			/// <param name="tolerance">A positive tolerance value</param>
			/// <returns name="bool">True if the Vertex is inside the Face within at tolerance value, otherwise False</returns>
			static bool IsInside(Face^ face, Vertex^ vertex,
#ifdef TOPOLOGIC_DYNAMO
			[DefaultArgument("0.0001")]
#endif
			double tolerance);

		public protected:
			FaceUtility() {}
		};
	}
}