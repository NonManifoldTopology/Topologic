#pragma once

#include "Cell.h"

namespace Topologic {
	namespace Utilities {
	/// <summary>
	/// Utility provides extended functionality including further geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>

		/// <summary>
		/// CellUtility includes geometric methods relevant to a Cell.
		/// </summary>
		public ref class CellUtility
		{
		public:
			/*/// <summary>
			/// Create a cell by a list of vertices and a list of face indices.
			/// </summary>
			/// <param name="vertices">A list of vertices</param>
			/// <param name="faceIndices">A list of face indices</param>
			/// <returns name="Cell">The created cell</returns>
			static Cell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);*/

			/// <summary>
			/// Create a cell by lofting through a set of wires.
			/// </summary>
			/// <param name="wires">A set of wires</param>
			/// <returns name="Cell">The created cell</returns>
			static Cell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

			/// <summary>
			/// Returns the volume of a cell.
			/// </summary>
			/// <param name="cell">The cell</param>
			/// <returns>The volume of the cell</returns>
			static double Volume(Cell^ cell);

			/// <summary>
			/// Check if a vertex is contained in a cell or not.
			/// </summary>
			/// <param name="cell">The cell</param>
			/// <param name="vertex">The vertex</param>
			/// <returns>True or false</returns>
			static bool Contains(Cell^ cell, Vertex^ vertex);

			/// <summary>
			/// Returns six values associated to the x, y, z coordinates of the bounding box of the topological entity.
			/// </summary>
			static List<double>^ GetMinMax(Cell^ cell);

		public protected:
			CellUtility() {}
		};
	}
}