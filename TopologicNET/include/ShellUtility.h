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

#include "Shell.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	

		/// <summary>
		/// ShellUtility includes geometric methods relevant to a Shell.
		/// </summary>
		public ref class ShellUtility
		{
		public:
			/// <summary>
			/// Creates a Shell by lofting through a set of Wires.
			/// </summary>
			/// <param name="wires">A set of Wires</param>
			/// <returns name="Shell">The created Shell</returns>
			static Shell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

			/*/// <summary>
			/// Creates a Shell by a list of vertices and a list of Face indices.
			/// </summary>
			/// <param name="vertices">A list of Vertices.</param>
			/// <param name="faceIndices">A list of Face indices. Every element must be within the range of the Vertices argument.</param>
			/// <returns name="Shell">The created shell</returns>
			[IsVisibleInDynamoLibrary(false)]
			static Shell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);*/

			///// <summary>
			///// Creates a Shell by subdividing and planarizing a curved Face.
			///// </summary>
			///// <param name="face">A curved Face</param>
			///// <param name="iteration"></param>
			///// <param name="numUPanels"></param>
			///// <param name="numVPanels"></param>
			///// <param name="tolerance"></param>
			///// <param name="capBottom">Cap the bottom hole if the surface is closed either in the minimum u- or v-direction but open on the other</param>
			///// <param name="capTop">Cap the top hole if the surface is closed either in the maximum u- or v-direction but open on the other</param>
			///// <returns name="Face[]">A list of Faces</returns>
			//static List<Face^>^ ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, int numUPanels, int numVPanels, double tolerance, bool capBottom, bool capTop);

			///// <summary>
			///// Creates a shell by subdividing and planarizing a curved face. NOTE: This constructor will be moved to a dedicated package.
			///// </summary>
			///// <param name="face">A curved Face</param>
			///// <param name="iteration"></param>
			///// <param name="uValues"></param>
			///// <param name="vValues"></param>
			///// <param name="tolerance"></param>
			///// <param name="capBottom">Cap the holes if the surface is closed either in the minimum u- or v-direction but open on the other</param>
			///// <param name="capTop">Cap the holes if the surface is closed either in the maximum u- or v-direction but open on the other</param>
			///// <returns name="Shell">The created Shell</returns>
			////[MultiReturn(gcnew array<String^> { "Vertices", "Edges", "Wires", "Faces", "Shell" })]
			////static Dictionary<String^, Object^>^ ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop);
			//static Shell^ ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop);

		public protected:
			ShellUtility() {}
		};
	}
}