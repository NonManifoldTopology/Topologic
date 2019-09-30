#pragma once

#include "Shell.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
		/// <summary>
		/// TopologyUtility includes geometric methods relevant to any Topology.
		/// </summary>
		public ref class TopologyUtility
		{
		public:

			/// <summary>
			/// Translates (moves) a Topology a certain distance according to XYZ values.
			/// </summary>
			/// <param name="topology">The Topology to be translated</param>
			/// <param name="x">The x value</param>
			/// <param name="y">The y value</param>
			/// <param name="z">The z value</param>
			/// <returns name="Topology">The Topology in its new location</returns>
			static Topology^ Translate(
				Topology^ topology, 
#ifdef TOPOLOGIC_DYNAMO
	[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")]
#endif
				double x,
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")]
#endif
				double y, 
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")]
#endif
				double z);

			/// <summary>
			/// Rotates a Topology given an origin, XYZ vectors and an angle.
			/// </summary>
			/// <param name="topology">The Topology to be rotated</param>
			/// <param name="origin">The origin</param>
			/// <param name="xVector">The X vector</param>
			/// <param name="yVector">The Y vector</param>
			/// <param name="zVector">The Z vector</param>
			/// <param name="degree">The angle in degrees</param>
			/// <returns name="Topology">The Topology after its rotation</returns>
			static Topology^ Rotate(Topology^ topology,
				Vertex^ origin,
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")]
#endif
				double xVector, 
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")]
#endif
				double yVector,
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")]
#endif
				double zVector,
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("0.0")]
#endif
				double degree);

			/// <summary>
			/// Scales a Topology according to an origin and XYZ factors.
			/// </summary>
			/// <param name="topology">The Topology to be scaled</param>
			/// <param name="origin">The origin</param>
			/// <param name="xFactor">The X factor</param>
			/// <param name="yFactor">The Y factor</param>
			/// <param name="zFactor">The Z factor</param>
			/// <returns name="Topology">The Topology after its scaling</returns>
			static Topology^ Scale(Topology^ topology, Vertex^ origin, 
#ifdef TOPOLOGIC_DYNAMO
				[Autodesk::DesignScript::Runtime::DefaultArgument("1.0")]
#endif
				double xFactor, 
#ifdef TOPOLOGIC_DYNAMO
					[Autodesk::DesignScript::Runtime::DefaultArgument("1.0")]
#endif
				double yFactor, 
#ifdef TOPOLOGIC_DYNAMO
					[Autodesk::DesignScript::Runtime::DefaultArgument("1.0")]
#endif
				double zFactor);

			/// <summary>
			/// Transforms a Topology according to translation and rotation factors.
			/// </summary>
			/// <param name="topology">A Topology</param>
			/// <param name="translationX"></param>
			/// <param name="translationY"></param>
			/// <param name="translationZ"></param>
			/// <param name="rotation11"></param>
			/// <param name="rotation12"></param>
			/// <param name="rotation13"></param>
			/// <param name="rotation21"></param>
			/// <param name="rotation22"></param>
			/// <param name="rotation23"></param>
			/// <param name="rotation31"></param>
			/// <param name="rotation32"></param>
			/// <param name="rotation33"></param>
			/// <returns name="Topology"></returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static Topology^ Transform(Topology^ topology,
				double translationX, double translationY, double translationZ,
				double rotation11, double rotation12, double rotation13,
				double rotation21, double rotation22, double rotation23,
				double rotation31, double rotation32, double rotation33);


			/// <summary>
			/// Returns a list of Topologies that are the adjacent to the input Topology.
			/// </summary>
			/// <param name="topology">A Topology</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <param name="typeFilter">The type of the context Topology</param>
			/// <returns>A list of Topologies adjacent to the input Topology</returns>
			static List<Topology^>^ AdjacentTopologies(Topology^ topology, Topology^ parentTopology, int typeFilter);
/*
			///
			static List<Edge^>^ AdjacentEdges(Topology^ topology, Topology^ parentTopology);

			static List<Face^>^ AdjacentFaces(Topology^ topology, Topology^ parentTopology);

			static List<Cell^>^ AdjacentCells(Topology^ topology, Topology^ parentTopology);*/

		public protected:
			TopologyUtility() {}

			/*static Topology^ Transform(Topology^ topology, 
				double zAxisX, double zAxisY, double zAxisZ,
				double xAxisX, double xAxisY, double xAxisZ);*/

		};
	}
}