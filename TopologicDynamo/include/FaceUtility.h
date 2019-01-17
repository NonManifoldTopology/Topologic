#pragma once

#include "Face.h"

namespace Topologic {
	namespace Utilities {
	/// <summary>
	/// Utility provides extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>

		/// <summary>
		/// FaceUtility includes geometric methods relevant to a Face.
		/// </summary>
		public ref class FaceUtility
		{
		public:
			/// <summary>
			/// Return the area of a face.
			/// </summary>
			/// <param name="face">The Face</param>
			/// <returns>The area of the face</returns>
			static double Area(Face^ face);

			/// <summary>
			/// Create a face (of any type) by a set of vertices.
			/// </summary>
			/// <param name="vertices">A Set of Vertices</param>
			/// <returns name="Face">A Face</returns>
			static Face^ ByVertices(Face^ face, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<Vertex^>^>^ vertices);

			/// <summary>
			/// Returns the UV parameters at a given vertex on a face.
			/// </summary>
			/// <param name="vertex">A Vertex on a Face</param>
			/// <returns name="UV">The UV parameters</returns>
			static Autodesk::DesignScript::Geometry::UV^ UVParameterAtVertex(Face^ face, Vertex^ vertex);

			/// <summary>
			/// Returns the vertex at a given parameter of the face.
			/// </summary>
			/// <param name="face">A Face</param>
			/// <param name="u">The U parameter of the Face</param>
			/// <param name="v">The V parameter of the Face</param>
			/// <returns name="Vertex">The Vertex at the given parameter of the Face</returns>
			static Vertex^ VertexAtParameter(Face^ face, double u, double v);

			/// <summary>
			/// Returns the normal (vector) at a parameter of a face.
			/// </summary>
			/// <param name="uv">The UV parameter of the Face</param>
			/// <returns>The normal (vector) at a parameter of a face</returns>
			static Autodesk::DesignScript::Geometry::Vector^ NormalAtParameter(Face^ face, Autodesk::DesignScript::Geometry::UV^ uv);

			/// <summary>
			/// Trim a face with a wire. The portion of the face inside the wire will be returned.
			/// </summary>
			/// <param name="wire">The Wire with which the Face will be trimmed</param>
			/// <returns name="Face">The trimmed Face</returns>
			static Face^ TrimByWire(Face^ face, Wire^ wire);

			/// <summary>
			/// Triangulate a face given a deflection value.
			/// </summary>
			static List<Face^>^ Triangulate(Face^ face, double deflection);

		public protected:
			FaceUtility() {}
		};
	}
}