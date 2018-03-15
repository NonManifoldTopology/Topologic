#pragma once

#include <TopologicalQuery.h>
#include <TopoLogicCore/include/Topology.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopoLogic
{
	ref class Vertex;
	ref class Attribute;
	ref class Context;

	/// <summary>
	/// A Topology is an abstract superclass that constructors, properties and methods used by other subclasses that extend it.
	/// </summary>
	public ref class Topology abstract : public TopologicalQuery
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="geometry"></param>
		/// <returns></returns>
		Topology^ ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Topology^ ByContext();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertexCoordinates"></param>
		/// <param name="vertexIndices"></param>
		/// <returns></returns>
		Topology^ ByVertexIndex(List<array<double, 3>^>^ vertexCoordinates, List<List<int>^>^ vertexIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertexCoordinates"></param>
		/// <param name="vertexIndices"></param>
		/// <returns></returns>
		Topology^ ByVertexIndex(List<Vertex^>^ vertexCoordinates, List<List<int>^>^ vertexIndices);

		/// <summary>
		/// Returns the dimensionality of the topological entity.
		/// </summary>
		/// <returns name="int">The topological entity's dimentionality</returns>
		property int Dimensionality
		{
			int get();
		}

		/// <summary>
		/// <para/>Creates the host geometry counterpart of the topological entity. The mapping is as follows. 
		/// <para/>- Vertex: Point;
		/// <para/>- Edge: either a Curve or its subclasses, other than Polycurve or its subclasses; 
		/// <para/>- Wire: PolyCurve or Polygon; 
		/// <para/>- Face: Surface or NurbsSurface;
		/// <para/>- Shell: PolySurface; 
		/// <para/>- Cell: either a Solid or its subclasses; 
		/// <para/>- CellComplex: a list of Solids; 
		/// <para/>- Cluster: a list of topological entities without a constituent parent.
		/// </summary>
		/// <returns name="Geometry">The host geometry counterpart of the topological entity</returns>
		property Object^ Geometry
		{
			virtual Object^ get() abstract;
		}

		/// <summary>
		/// 
		/// </summary>
		property Dictionary<String^, Attribute^>^ Attributes
		{
			Dictionary<String^, Attribute^>^ get();
		}

		/// <summary>
		/// Returns the topological entities containing the input topology as a non-constituent member
		/// </summary>
		/// <returns name="Topology[]">The topological entities containing the input topology as a non-constituent member</returns>
		List<Topology^>^ Contents();

		/// <summary>
		/// Returns the non-constituent members of the input topological entity.
		/// </summary>
		/// <returns name="Context[]">The non-constituent members of the input topological entity</returns>
		List<Context^>^ Contexts();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		Topology^ AddContent(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		Topology^ RemoveContent(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="context"></param>
		/// <returns></returns>
		Topology^ AddContext(Context^ context);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="context"></param>
		/// <returns></returns>
		Topology^ RemoveContext(Context^ context);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		List<Topology^>^ SharedTopologies(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="maxLevel"></param>
		/// <param name="maxPaths"></param>
		/// <returns name="Topology[][]"></returns>
		List<List<Topology^>^>^ PathsTo(Topology^ topology, int maxLevel, int maxPaths);

		/// <summary>
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

		/// <summary>
		/// Perform the Difference operation between the first and second topological entities.
		/// </summary>
		/// <param name="topology">The second topological entity</param>
		/// <returns name="Topology">The result of the Difference operation</returns>
		Topology^ Difference(Topology^ topology);
		
		/// <summary>
		/// Impose the second topological entity to the first one.
		/// </summary>
		/// <param name="topology">TopoLogic.Topology</param>
		/// <returns name="Topology">TopoLogic.Topology</returns>
		Topology^ Impose(TopoLogic::Topology^ topology);

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
		Topology^ Intersection(Topology^ topology);

		/// <summary>
		/// Merge the first and the second topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ Merge(Topology^ topology);

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
		/// Perform the XOR operation between the first and the second topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the XOR operation</returns>
		Topology^ XOR(Topology^ topology);

		/// <summary>
		/// Save a topological entity to a BRep file.
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="bool">True if the BRep file is succesffully created, otherwise false</returns>
		bool SaveToBRep(String^ path);

		/// <summary>
		/// Load a topological entity from a BRep file.
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="Topology">The loaded topology</returns>
		static Topology^ LoadFromBRep(String^ path);

		/// <summary>
		/// Print the topological information of the input entity.
		/// </summary>
		/// <returns name="String">The topological information of the input entity</returns>
		String^ Analyze();

		/// <summary>
		/// Returns all sub-entities that have no other parents than this cluster, i.e. do not belong to other entities.
		/// </summary>
		/// <returns>The immediate members</returns>
		List<Topology^>^ ImmediateMembers();

	public protected:
		static Topology^ ByCoreTopology(TopoLogicCore::Topology * const kpCoreTopology);

	protected:
		Topology();
		virtual ~Topology();
	};
}