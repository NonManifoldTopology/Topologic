#pragma once

#include "Shell.h"

namespace Topologic {
	namespace Support {

		/// <summary>
		/// 
		/// </summary>
		public ref class ShellUtility
		{
		public:
			/// <summary>
			/// Create a shell by lofting through a set of wires.
			/// </summary>
			/// <param name="wires">A set of wires</param>
			/// <returns name="Shell">The created shell</returns>
			static Shell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

			/// <summary>
			/// Create a shell by a list of vertices and a list of face indices.
			/// </summary>
			/// <param name="vertices">A list of vertices.</param>
			/// <param name="faceIndices">A list of face indices. Every element must be within the range of the vertices argument.</param>
			/// <returns name="Shell">The created shell</returns>
			[IsVisibleInDynamoLibrary(false)]
			static Shell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);

			/// <summary>
			/// Create a shell by subdividing and planarizing a curved face.
			/// </summary>
			/// <param name="face"></param>
			/// <param name="iteration"></param>
			/// <param name="numUPanels"></param>
			/// <param name="numVPanels"></param>
			/// <param name="tolerance"></param>
			/// <param name="capBottom">Cap the holes if the surface is closed either in the minimum u- or v-direction but open on the other</param>
			/// <param name="capTop">Cap the holes if the surface is closed either in the maximum u- or v-direction but open on the other</param>
			/// <returns name="Face[]"></returns>
			static List<Face^>^ ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, int numUPanels, int numVPanels, double tolerance, bool capBottom, bool capTop);

			/// <summary>
			/// Create a shell by subdividing and planarizing a curved face. NOTE: This constructor will be moved to a dedicated package.
			/// </summary>
			/// <param name="face"></param>
			/// <param name="iteration"></param>
			/// <param name="uValues"></param>
			/// <param name="vValues"></param>
			/// <param name="tolerance"></param>
			/// <param name="capBottom">Cap the holes if the surface is closed either in the minimum u- or v-direction but open on the other</param>
			/// <param name="capTop">Cap the holes if the surface is closed either in the maximum u- or v-direction but open on the other</param>
			/// <returns name="Shell"></returns>
			//[MultiReturn(gcnew array<String^> { "Vertices", "Edges", "Wires", "Faces", "Shell" })]
			//static Dictionary<String^, Object^>^ ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop);
			static Shell^ ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop);

		public protected:
			ShellUtility() {}
		};
	}
}