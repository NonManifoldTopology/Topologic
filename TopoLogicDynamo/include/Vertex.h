#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Vertex.h>

namespace TopoLogic
{
	public ref class Vertex : Topology
	{
	public:
		/// <summary>
		/// Creates a vertex by a point.
		/// </summary>
		/// <param name="point">A point.</param>
		/// <returns name="TopoLogic Vertex">
		/// "TopoLogic Vertex": the created TopoLogic vertex
		/// </returns>
		/// <returns name="Point">
		/// "Point": the Dynamo point counterpart of the vertex
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertex", "Point" })]
		static Dictionary<String^, Object^>^ ByPoint(Autodesk::DesignScript::Geometry::Point^ point);

		/// <summary>
		/// Gets the list of edges incident to this vertex.
		/// </summary>
		/// <param name="topoLogicVertex">A TopoLogic vertex.</param>
		/// <returns name="TopoLogic Edges">
		/// "TopoLogic Edges": the edges incident to this vertex
		/// </returns>
		/// <returns name="Curves">
		/// "Curves": the Dynamo curves counterpart of the edges
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ Edges(Vertex^ topoLogicVertex);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	public protected:
		Vertex(TopoLogicCore::Vertex* const kpCoreVertex);
		Vertex(Autodesk::DesignScript::Geometry::Point^ pDynamoPoint);

		Autodesk::DesignScript::Geometry::Point^ Point();

		virtual TopoLogicCore::Topology* GetCoreTopology() override;

	protected:
		virtual ~Vertex();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Vertex* m_pCoreVertex;
	};
}