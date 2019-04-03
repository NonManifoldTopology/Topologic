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
			static List<double>^ ParametersAtVertex(Face^ face, Vertex^ vertex);

			/// <summary>
			/// Returns the normal (vector) at a parameter of a Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="u">The u parameter of the Face</param>
			/// <param name="v">The v parameter of the Face</param>
			/// <returns>The normal (vector) at a parameter of a Face</returns>
			static List<double>^ NormalAtParameters(Face^ face, double u, double v);

			/// <summary>
			/// Returns the Vertex at a given parameter of the Face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="u">The U parametric coordinate of the Vertex on the Face</param>
			/// <param name="v">The V parametric coordinate of the Vertex on the Face</param>
			/// <returns name="Vertex">The Vertex at the given parameter of the Face</returns>
			static Vertex^ VertexAtParameters(Face^ face, double u, double v);

			/// <summary>
			/// Trims a Face with a Wire. The portion of the Face inside the Wire will be returned.
			/// </summary>
			/// <param name="face">The Face to be trimmed</param>
			/// <param name="wire">The Wire with which the Face will be trimmed</param>
			/// <returns name="Face">The trimmed Face</returns>
			static Face^ TrimByWire(Face^ face, Wire^ wire);

			/// <summary>
			/// Triangulates a Face given a deflection value.
			/// </summary>
			/// <param name="face">The Face to be triangulated</param>
			/// <param name="deflection">The deflection value to be applied for triangulation</param>
			/// <returns name="Face">The triangulated Face</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static List<Face^>^ Triangulate(Face^ face, double deflection);

		public protected:
			FaceUtility() {}
		};
	}
}