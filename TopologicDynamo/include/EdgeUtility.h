#pragma once

#include "Edge.h"

namespace Topologic {
	namespace Support {

		/// <summary>
		/// 
		/// </summary>
		public ref class EdgeUtility
		{
		public:
			/// <summary>
			/// 
			/// </summary>
			/// <param name="vertices"></param>
			/// <returns></returns>
			static Edge^ ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="centerPoint"></param>
			/// <param name="radius"></param>
			/// <param name="xAxisX"></param>
			/// <param name="xAxisY"></param>
			/// <param name="xAxisZ"></param>
			/// <param name="normalX"></param>
			/// <param name="normalY"></param>
			/// <param name="normalZ"></param>
			/// <returns></returns>
			static Edge^ ByCircle(
				Vertex^ centerPoint, double radius,
				double xAxisX, double xAxisY, double xAxisZ,
				double normalX, double normalY, double normalZ);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="edge"></param>
			/// <param name="vertex"></param>
			/// <returns></returns>
			static double ParameterAtVertex(Edge^ edge, Vertex^ vertex);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="edge"></param>
			/// <param name="parameter"></param>
			/// <returns></returns>
			static Vertex^ VertexAtParameter(Edge^ edge, double parameter);

		public protected:
			EdgeUtility() {}
		};
	}
}