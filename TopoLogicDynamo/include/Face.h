#pragma once

#include <Topology.h>

#include <TopologicCore/include/Face.h>

namespace Topologic {
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Shell;
	ref class Cell;

	/// <summary>
	/// 
	/// </summary>
	public ref class Face : Topology
	{
	public:
		/// <summary>
		/// Gets the list of faces adjacent to this face.
		/// </summary>
		/// <param name="parentTopology"></param>
		/// <returns name="Face[]">The faces adjacent to this face</returns>
		List<Face^>^ AdjacentFaces(Topology^ parentTopology);

		/// <summary>
		/// Gets the list of cells incident to this face.
		/// </summary>
		/// <param name="parentTopology"></param>
		/// <returns name="Cell[]">The cells incident to this face</returns>
		List<Cell^>^ Cells(Topology^ parentTopology);

		/// <summary>
		/// Gets the list of shells incident to this face.
		/// </summary>
		/// <param name="parentTopology"></param>
		/// <returns name="Shell[]">The shells incident to this face</returns>
		List<Shell^>^ Shells(Topology^ parentTopology);

		/// <summary>
		/// Gets the list of vertices constituent to this face. 
		/// </summary>
		/// <returns name="Vertex[]">The vertices consituent to this face</returns>
		List<Vertex^>^ Vertices();

		/// <summary>
		/// Gets the list of edges constituent to this face.
		/// </summary>
		/// <returns name="Edge[]">The edges consituent to this face</returns>
		List<Edge^>^ Edges();

		/// <summary>
		/// Gets the list of wires constituent to this face.
		/// </summary>
		/// <returns name="Wire[]">The wires consituent to this face</returns>
		List<Wire^>^ Wires();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="double"></returns>
		double Area();

		/// <summary>
		/// A factory method that creates a face by a closed wire.
		/// </summary>
		/// <param name="wire">A closed wire. Must be (and internally verified if it is) a Dynamo polygon or a Topologic wire.</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is neither a Dynamo polygon nor a Topologic wire</exception>
		/// <returns name="Face">/// </returns>
		static Face^ ByWire(Wire^ wire);

		/// <summary>
		/// Creates a face by a list of edges. A wire will be internally created.
		/// </summary>
		/// <param name="edges">The edges. </param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is not a Topologic edge</exception>
		/// <returns name="Face">the created face</returns>
		static Face^ ByEdges(IEnumerable<Edge^>^ edges);

		/// <summary>
		/// Creates a face by a surface.
		/// </summary>
		/// <param name="surface">The surface</param>
		/// <returns name="Face">The created face</returns>
		static Face^ BySurface(Autodesk::DesignScript::Geometry::Surface^ surface);

		/// <summary>
		/// Returns the shared edges between two faces. 
		/// </summary>
		/// <param name="face">Another face</param>
		/// <returns name="Edge[]">The shared edges</returns>
		List<Edge^>^ SharedEdges(Face^ face);

		/// <summary>
		/// Returns the shared vertices between two faces.
		/// </summary>
		/// <param name="face">Another face</param>
		/// <returns name="Vertex[][]">The shared vertices</returns>
		List<Vertex^>^ SharedVertices(Face^ face);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Wire[]"></returns>
		Wire^ OuterBoundary();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Wire[]"></returns>
		List<Wire^>^ InnerBoundaries();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertex"></param>
		/// <returns name="UV"></returns>
		Autodesk::DesignScript::Geometry::UV^ UVParameterAtPoint(Vertex^ vertex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="uv"></param>
		/// <returns name="Vertex"></returns>
		Vertex^ PointAtParameter(Autodesk::DesignScript::Geometry::UV^ uv);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="uv"></param>
		/// <returns></returns>
		Autodesk::DesignScript::Geometry::Vector^ NormalAtParameter(Autodesk::DesignScript::Geometry::UV^ uv);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="wire"></param>
		/// <returns name="Face"></returns>
		Face^ Trim(Wire^ wire);

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
		/// <param name="kpCoreFace"></param>
		Face(const std::shared_ptr<TopologicCore::Face>& kpCoreFace);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pWire"></param>
		Face(Wire^ pWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoSurface"></param>
		Face(Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Autodesk::DesignScript::Geometry::Surface^ Surface();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Autodesk::DesignScript::Geometry::Mesh^ TriangulatedMesh();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Face();

		/// <summary>
		/// Initialises the face given a NurbsSurface argument. Called by the respective constructor.
		/// </summary>
		/// <param name="pDynamoNurbsSurface">A Dynamo NURBS surface</param>
		void Init(Autodesk::DesignScript::Geometry::NurbsSurface^ pDynamoNurbsSurface,
			array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoPerimeterCurves);

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Face>* m_pCoreFace;
	};
}