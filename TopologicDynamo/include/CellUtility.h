#pragma once

#include "Cell.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	

		/// <summary>
		/// CellUtility includes geometric methods relevant to a Cell.
		/// </summary>
		public ref class CellUtility
		{
		public:
			/*/// <summary>
			/// Creates a Cell by a list of Vertices and a list of Face indices.
			/// </summary>
			/// <param name="vertices">A list of Vertices</param>
			/// <param name="faceIndices">A list of Face indices</param>
			/// <returns name="Cell">The created Cell</returns>
			static Cell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);*/

			/// <summary>
			/// Creates a Cell by lofting through a set of Wires.
			/// </summary>
			/// <param name="wires">A set of Wires</param>
			/// <returns name="Cell">The created Cell</returns>
			static Cell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

			/// <summary>
			/// Returns the volume of a Cell.
			/// </summary>
			/// <param name="cell">The Cell</param>
			/// <returns>The volume of the Cell</returns>
			static double Volume(Cell^ cell);

			/// <summary>
			/// Check if a vertex is contained in a Cell or not.
			/// </summary>
			/// <param name="cell">The Cell</param>
			/// <param name="vertex">The Vertex</param>
			/// <returns>True or false</returns>
			static bool Contains(Cell^ cell, Vertex^ vertex);

			/// <summary>
			/// Returns six values associated to the x, y, z coordinates of the bounding box of the Cell.
			/// </summary>
			/// <param name="cell">The Cell</param>
			/// <returns>Six values associated to the x, y, z coordinates of the bounding box of the topological entity</returns>
			[IsVisibleInDynamoLibrary(false)]
			static List<double>^ GetMinMax(Cell^ cell);

		public protected:
			CellUtility() {}
		};
	}
}