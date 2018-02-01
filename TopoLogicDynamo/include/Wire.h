#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Wire.h>

namespace TopoLogic {
	ref class Edge;

	public ref class Wire : Topology
	{
	public:
		/// <summary>
		/// Gets the list of edges constituent to this wire.
		/// </summary>
		/// <param name="topoLogicWire">A TopoLogic wire</param>
		/// <returns name="TopoLogic Edges">
		/// "TopoLogic Edges": the constituent edges
		/// </returns>
		/// <returns name="Curves">
		/// "Curves": the Dynamo curves counterpart of the edges
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ Edges(Wire^ topoLogicWire);

		/// <summary>
		/// Gets the list of faces incident to this wire.
		/// </summary>
		/// <param name="topoLogicWire">A TopoLogic wire</param>
		/// <returns name="TopoLogic Faces">
		/// "TopoLogic Faces": the created TopoLogic faces
		/// </returns>
		/// <returns name="Surfaces">
		/// "Surfaces": the Dynamo surfaces counterpart of the faces
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Faces", "Surfaces" })]
		static Dictionary<String^, Object^>^ Faces(Wire^ topoLogicWire);

		/// <summary>
		/// Checks if the wire is closed.
		/// </summary>
		/// <returns>True if the wire is closed, otherwise false</returns>
		bool IsClosed();

		/// <summary>
		/// Gets the list of vertices constituent to this wire.
		/// </summary>
		/// <param name="topoLogicWire">A TopoLogic wire</param>
		/// <returns name="TopoLogic Vertices">
		/// "TopoLogic Vertices": the constituent vertices
		/// </returns>
		/// <returns name="Points">
		/// "Points": the Dynamo points counterpart of the vertices
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertices", "Points" })]
		static Dictionary<String^, Object^>^ Vertices(Wire^ topoLogicWire);

		/// <summary>
		/// Creates a wire by a list of edges.
		/// </summary>
		/// <param name="edges">The edges. Must be (and internally verified if it is) a list of Dynamo or TopoLogic edges.</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is neither a list of Dynamo or TopoLogic edges</exception>
		/// <returns name="TopoLogic Wire">
		/// "TopoLogic Wire": the created TopoLogic wire
		/// </returns>
		/// <returns name="PolyCurve">
		/// "PolyCurve": the Dynamo polycurve counterpart of the wire
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Wire", "PolyCurve" })]
		static Dictionary<String^, Object^>^ ByEdges(List<Edge^>^ edges);

		/// <summary>
		/// Creates a wire by a polycurve (including a polygon)
		/// </summary>
		/// <param name="polycurve">The polycurve</param>
		/// <returns name="TopoLogic Wire">
		/// "TopoLogic Wire": the created TopoLogic wire
		/// </returns>
		/// <returns name="PolyCurve">
		/// "PolyCurve": the Dynamo polycurve counterpart of the wire
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Wire", "PolyCurve" })]
		static Dictionary<String^, Object^>^ ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve^ polycurve);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreWire"></param>
		Wire(TopoLogicCore::Wire* const kpCoreWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pEdges"></param>
		Wire(List<Edge^>^ pEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoPolycurve"></param>
		Wire(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoLogicCore::Topology* GetCoreTopology() override;

		// Utility classes

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Edge^>^ Edges();

	protected:
		virtual ~Wire();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Wire* m_pCoreWire;
	};
}