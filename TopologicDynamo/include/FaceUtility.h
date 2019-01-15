#pragma once

#include "Face.h"

namespace Topologic {
	namespace Utilities {

		/// <summary>
		/// 
		/// </summary>
		public ref class FaceUtility
		{
		public:
			/// <summary>
			/// 
			/// </summary>
			/// <param name="face"></param>
			/// <returns></returns>
			static double Area(Face^ face);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="vertices"></param>
			/// <returns name="Face"></returns>
			static Face^ ByVertices(Face^ face, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<Vertex^>^>^ vertices);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="vertex"></param>
			/// <returns name="UV"></returns>
			static Autodesk::DesignScript::Geometry::UV^ UVParameterAtVertex(Face^ face, Vertex^ vertex);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="face"></param>
			/// <param name="u"></param>
			/// <param name="v"></param>
			/// <returns name="Vertex"></returns>
			static Vertex^ VertexAtParameter(Face^ face, double u, double v);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="uv"></param>
			/// <returns></returns>
			static Autodesk::DesignScript::Geometry::Vector^ NormalAtParameter(Face^ face, Autodesk::DesignScript::Geometry::UV^ uv);

			/// <summary>
			/// Trim a face with a wire. The portion of the face inside the wire will be returned.
			/// </summary>
			/// <param name="wire"></param>
			/// <returns name="Face"></returns>
			static Face^ TrimByWire(Face^ face, Wire^ wire);

			static List<Face^>^ Triangulate(Face^ face, double deflection);

		public protected:
			FaceUtility() {}
		};
	}
}