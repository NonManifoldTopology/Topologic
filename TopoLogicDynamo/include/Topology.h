#pragma once

#include "TopologicalQuery.h"
#include "AttributeMap.h"

#include <TopologicCore/include/Topology.h>

#pragma make_public(TopologicCore::Topology)
#pragma make_public(TopologicCore::TopologyPtr)

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace Topologic
{
	ref class Vertex;
	ref class Attribute;
	ref class Context;
	ref class TopologyFactory;

	/// <summary>
	/// A Topology is an abstract superclass that constructors, properties and methods used by other subclasses that extend it.
	/// </summary>
	public ref class Topology abstract : public TopologicalQuery
	{
	public:
		/// <summary>
		/// Create a topology by geometry.
		/// </summary>
		/// <param name="geometry"></param>
		/// <returns name="Topology"></returns>
		//[IsVisibleInDynamoLibrary(false)]
		static Topology^ ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry);

		/// <summary>
		/// Create a topology by geometry.
		/// </summary>
		/// <param name="geometry"></param>
		/// <returns name="Topology"></returns>
		[IsVisibleInDynamoLibrary(false)]
		static Topology^ ByGeometryA(Autodesk::DesignScript::Geometry::DesignScriptEntity^ geometry);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Topology"></returns>
		[IsVisibleInDynamoLibrary(false)]
		static Topology^ ByContext();

		/*/// <summary>
		///
		/// </summary>
		/// <param name="vertexCoordinates"></param>
		/// <param name="vertexIndices"></param>
		/// <returns name="Topology"></returns>
		static Topology^ ByVertexIndex(List<array<double, 3>^>^ vertexCoordinates, List<List<int>^>^ vertexIndices);*/

		/// <summary>
		/// Create a topology by a list of vertices and a 2D list of indices of the vertices in the first argument. An array of 1 index makes a vertex. An array of 2 indices makes an edge. An array of 3 indices either makes a wire, or, when the array has at least 4 vertices and the first index is the same as the last index, a face will be created instead. NOTE: This node currently can only create planar faces.
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="vertexIndices"></param>
		/// <returns name="Topology[]"></returns>
		static List<Topology^>^ ByVerticesIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::List<int>^>^ vertexIndices);

		/// <summary>
		/// Returns the dimensionality of the topological entity.
		/// </summary>
		/// <returns name="int">The topological entity's dimentionality</returns>
		property int Dimensionality
		{
			int get();
		}

		/// <para/>Creates the host geometry counterpart of the topological entity. The mapping is as follows. 
		/// <para/>- Vertex: Point;
		/// <para/>- Edge: either a Curve or its subclasses, other than Polycurve or its subclasses; 
		/// <para/>- Wire: PolyCurve or Polygon; 
		/// <para/>- Face: Surface or NurbsSurface;
		/// <para/>- Shell: PolySurface; 
		/// <para/>- Cell: either a Solid or its subclasses; 
		/// <para/>- CellComplex: a list of Solids; 
		/// <para/>- Cluster: a list of topological entities without a constituent parent.

		/// <summary>
		/// Create a geometry from the topology.
		/// </summary>
		/// <returns name="Geometry">The host geometry counterpart of the topological entity</returns>
		property Object^ Geometry
		{
			virtual Object^ get() abstract;
		}

		/// <summary>
		/// 
		/// </summary>
		[IsVisibleInDynamoLibrary(false)]
		property Support::AttributeMap^ AttributeMap
		{
			Support::AttributeMap^ get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="attributeMap"></param>
		/// <returns></returns>
		[IsVisibleInDynamoLibrary(false)]
		Topology^ AddAttributeMap(Support::AttributeMap^ attributeMap);

		/// <summary>
		/// Returns the non-constituent members of the input topological entity.
		/// </summary>
		/// <param name="allLevels"></param>
		/// <returns name="Topology[]">The topological entities containing the input topology as a non-constituent member</returns>
		List<Topology^>^ Contents(
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool allLevels);

		/// <summary>
		/// Returns the topological entities containing the input topology as a content.
		/// </summary>
		/// <returns name="Context[]">The non-constituent members of the input topological entity</returns>
		List<Context^>^ Contexts();

		///// <summary>
		///// 
		///// </summary>
		///// <param name="contentTopology"></param>
		///// <param name="parentTopology"></param>
		///// <returns name="Topology"></returns>
		//[MultiReturn(gcnew array<String^> { "Topology", "ParentTopology" })]
		//Dictionary<String^, Object^>^ AddContent(Topology^ thisTopology, Topology^ contentTopology, [DefaultArgument("null")] Topology^ parentTopology);
		////Topology^ AddContent(Topology^ contentTopology, [DefaultArgument("null")] Topology^ parentTopology);

		/// <summary>
		/// Add this topology as a non-constituent member to another topology.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		Topology^ AddContent(Topology^ topology);

		/// <summary>
		/// Remove a non-constituent member from the topology.
		/// </summary>
		/// <param name="content"></param>
		/// <returns name="Topology"></returns>
		Topology^ RemoveContent(Topology^ content);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="apertureTopology"></param>
		/// <returns name="Topology"></returns>
		[IsVisibleInDynamoLibrary(false)]
		Topology^ AddAperture(Topology^ apertureTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="apertureTopologies"></param>
		/// <returns name="Topology"></returns>
		//[IsVisibleInDynamoLibrary(false)]
		Topology^ AddApertures(System::Collections::Generic::IEnumerable<Topology^>^ apertureTopologies);

		/// <summary>
		/// Add a context to the topology.
		/// </summary>
		/// <param name="context"></param>
		/// <returns name="Topology"></returns>
		Topology^ AddContext(Context^ context);

		/// <summary>
		/// Remove a context from the topology.
		/// </summary>
		/// <param name="context"></param>
		/// <returns name="Topology"></returns>
		Topology^ RemoveContext(Context^ context);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology"></returns>
		[IsVisibleInDynamoLibrary(false)]
		List<Topology^>^ SharedTopologies(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="maxLevel"></param>
		/// <param name="maxPaths"></param>
		/// <returns name="Topology[][]"></returns>
		[IsVisibleInDynamoLibrary(false)]
		List<List<Topology^>^>^ PathsTo(Topology^ topology, Topology^ parentTopology, int maxLevel, int maxPaths);

		/*/// <summary>
		///
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> {
			"A_A",
			"A_B",
			"B_A",
			"B_B",
			"Geometry A_A",
			"Geometry A_B",
			"Geometry B_A",
			"Geometry B_B"
		})]
		Dictionary<String^, Object^>^ BooleanImages(Topology^ topology);

		[MultiReturn(gcnew array<String^> {
			"Parts between A_A and B_A",
			"Parts between B_A and A_B",
			"Parts between A_B and B_B",
			"Parts geometry between A_A and B_A",
			"Parts geometry between B_A and A_B",
			"Parts geometry between A_B and B_B"
		})]
		Dictionary<String^, Object^>^ BooleanParts(Topology^ topology);
*/
		/// <summary>
		/// Perform the Difference operation between the first and second topological entities.
		/// </summary>
		/// <param name="topology">The second topological entity</param>
		/// <returns name="Topology">The result of the Difference operation</returns>
		Topology^ Difference(Topology^ topology);

		/// <summary>
		/// Impose the second topological entity to the first one.
		/// </summary>
		/// <param name="topology">Topologic.Topology</param>
		/// <returns name="Topology">Topologic.Topology</returns>
		Topology^ Impose(Topologic::Topology^ topology);

		/// <summary>
		/// Imprint the second topological entity to the first one.
		/// </summary>
		/// <param name="topology">Another topology</param>
		/// <returns name="Topology">The result of the Imprint operation</returns>
		Topology^ Imprint(Topology^ topology);

		/// <summary>
		/// Perform the Intersection operation between the first and the second topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Intersection operation</returns>
		Topology^ Intersect(Topology^ topology);

		/// <summary>
		/// Merge the first and the second topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ Merge(Topology^ topology);

		/// <summary>
		/// Merge the subtopologies under this topology.
		/// </summary>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ SelfMerge();

		/// <summary>
		/// Slice the first topological entity with the second.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Slice operation</returns>
		Topology^ Slice(Topology^ topology);

		/// <summary>
		/// Union the first and the second topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Union operation</returns>
		Topology^ Union(Topology^ topology);

		/// <summary>
		/// Perform an XOR operation between the first and the second topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the XOR operation</returns>
		Topology^ XOR(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		/// <returns name="Topology"></returns>
		[IsVisibleInDynamoLibrary(false)]
		Topology^ Translate(double x, double y, double z);

		/// <summary>
		/// Export the topological entity to a BRep file (.brep).
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="bool">True if the BRep file is succesffully created, otherwise false</returns>
		bool ExportToBRep(String^ path);

		/// <summary>
		/// Import a topological entity from a BRep file (.brep).
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="Topology">The loaded topology</returns>
		static Topology^ ImportFromBRep(String^ path);

		/// <summary>
		/// Print the topological information of the input entity.
		/// </summary>
		/// <returns name="String">The topological information of the input entity</returns>
		String^ Analyze();

		/// <summary>
		/// Returns all sub-topologies under this topology.
		/// </summary>
		/// <returns name="Topology[]">The sub-topologies of the input topology</returns>
		property List<Topology^>^ SubTopologies
		{
			List<Topology^>^ get();
		}

		/// <summary>
		/// Check if two topologies are the same. NOTE: This is a pointer check, not a topological or a geometry one.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		bool IsSame(Topology^ topology);

		/// <summary>
		/// Return the subtopology that is the closest and the simplest (i.e. has the lowest dimensionality) to the selector.
		/// </summary>
		/// <param name="selector"></param>
		/// <returns></returns>
		Topology^ ClosestSimplestSubshape(Topology^ selector);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[IsVisibleInDynamoLibrary(false)]
		double Distance(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Vertex"></returns>
		[IsVisibleInDynamoLibrary(false)]
		Vertex^ CenterOfMass();

		/// <summary>
		/// Returns the instance type as a string.
		/// </summary>
		property String^ Type
		{
			String^ get();
		}

		[IsVisibleInDynamoLibrary(false)]
		void RegisterFactory(const TopologicCore::Topology::Ptr& kpCoreTopology, TopologyFactory^ topologyFactory);

		[IsVisibleInDynamoLibrary(false)]
		void RegisterFactory(String^ rkGUID, TopologyFactory^ topologyFactory);

		/// <summary>
		/// Return the host topology of this topology. NOTE: Future feature.
		/// </summary>
		property List<Topology^>^ HostTopology__
		{
			List<Topology^>^ get();
		}

	public protected:
		static Topology^ ByCoreTopology(const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology);

	protected:
		Topology();
		virtual ~Topology();
	};
}