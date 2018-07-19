#pragma once

#include "TopologicalQuery.h"
#include <TopologicCore/include/Topology.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

//#pragma make_public(TDF_Attribute)

namespace Topologic
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
		/// <returns name="Topology"></returns>
		static Topology^ ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Topology"></returns>
		static Topology^ ByContext();

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="vertexCoordinates"></param>
		/// <param name="vertexIndices"></param>
		/// <returns name="Topology"></returns>
		static Topology^ ByVertexIndex(List<array<double, 3>^>^ vertexCoordinates, List<List<int>^>^ vertexIndices);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="vertexIndices"></param>
		/// <returns name="Topology[]"></returns>
		static List<Topology^>^ ByVertexIndex(List<Vertex^>^ vertices, List<List<int>^>^ vertexIndices);

		/// <summary>
		/// Returns the dimensionality of the Topological entity.
		/// </summary>
		/// <returns name="int">The Topological entity's dimentionality</returns>
		property int Dimensionality
		{
			int get();
		}

		/// <summary>
		/// <para/>Creates the host geometry counterpart of the Topological entity. The mapping is as follows. 
		/// <para/>- Vertex: Point;
		/// <para/>- Edge: either a Curve or its subclasses, other than Polycurve or its subclasses; 
		/// <para/>- Wire: PolyCurve or Polygon; 
		/// <para/>- Face: Surface or NurbsSurface;
		/// <para/>- Shell: PolySurface; 
		/// <para/>- Cell: either a Solid or its subclasses; 
		/// <para/>- CellComplex: a list of Solids; 
		/// <para/>- Cluster: a list of Topological entities without a constituent parent.
		/// </summary>
		/// <returns name="Geometry">The host geometry counterpart of the Topological entity</returns>
		property Object^ Geometry
		{
			virtual Object^ get() abstract;
		}

		/// <summary>
		/// 
		/// </summary>
		property List<Object^>^ Attributes
		{
			List<Object^>^ get();
		}

		/// <summary>
		/// Returns the Topological entities containing the input topology as a non-constituent member
		/// </summary>
		/// <returns name="Topology[]">The Topological entities containing the input topology as a non-constituent member</returns>
		List<Topology^>^ Contents();

		/// <summary>
		/// Returns the Topological entities containing the input topology as a non-constituent member
		/// </summary>
		/// <param name="allLevel"></param>
		/// <returns name="Topology[]">The Topological entities containing the input topology as a non-constituent member</returns>
		List<Topology^>^ ContentsV2(
			[Autodesk::DesignScript::Runtime::DefaultArgument("true")] bool allLevel);

		/// <summary>
		/// Returns the non-constituent members of the input Topological entity.
		/// </summary>
		/// <returns name="Context[]">The non-constituent members of the input Topological entity</returns>
		List<Context^>^ Contexts();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="contentTopology"></param>
		/// <returns name="Topology"></returns>
		Topology^ AddContent(Topology^ contentTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology"></returns>
		Topology^ RemoveContent(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="context"></param>
		/// <returns name="Topology"></returns>
		Topology^ AddContext(Context^ context);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="context"></param>
		/// <returns name="Topology"></returns>
		Topology^ RemoveContext(Context^ context);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology"></returns>
		List<Topology^>^ SharedTopologies(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="maxLevel"></param>
		/// <param name="maxPaths"></param>
		/// <returns name="Topology[][]"></returns>
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
		/// Perform the Difference operation between the first and second Topological entities.
		/// </summary>
		/// <param name="topology">The second Topological entity</param>
		/// <returns name="Topology">The result of the Difference operation</returns>
		Topology^ Difference(Topology^ topology);
		
		/// <summary>
		/// Impose the second Topological entity to the first one.
		/// </summary>
		/// <param name="topology">Topologic.Topology</param>
		/// <returns name="Topology">Topologic.Topology</returns>
		Topology^ Impose(Topologic::Topology^ topology);

		/// <summary>
		/// Imprint the second Topological entity to the first one.
		/// </summary>
		/// <param name="topology">Another topology</param>
		/// <returns name="Topology">The result of the Imprint operation</returns>
		Topology^ Imprint(Topology^ topology);

		/// <summary>
		/// Perform the Intersection operation between the first and the second Topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Intersection operation</returns>
		Topology^ Intersection(Topology^ topology);

		/// <summary>
		/// Merge the first and the second Topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ Merge(Topology^ topology);

		/// <summary>
		/// Self=merge.
		/// </summary>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ SelfMerge();

		/// <summary>
		/// Slice the first Topological entity with the second.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Slice operation</returns>
		Topology^ Slice(Topology^ topology);

		/// <summary>
		/// Union the first and the second Topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Union operation</returns>
		Topology^ Union(Topology^ topology);

		/// <summary>
		/// Perform the XOR operation between the first and the second Topological entities.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the XOR operation</returns>
		Topology^ XOR(Topology^ topology);

		/// <summary>
		/// Save a Topological entity to a BRep file.
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="bool">True if the BRep file is succesffully created, otherwise false</returns>
		bool SaveToBRep(String^ path);

		/// <summary>
		/// Load a Topological entity from a BRep file.
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="Topology">The loaded topology</returns>
		static Topology^ LoadFromBRep(String^ path);

		/// <summary>
		/// Print the Topological information of the input entity.
		/// </summary>
		/// <returns name="String">The Topological information of the input entity</returns>
		String^ Analyze();

		/// <summary>
		/// Returns all sub-entities that have no other parents than this cluster, i.e. do not belong to other entities.
		/// </summary>
		/// <returns name="Topology[]">The immediate members</returns>
		List<Topology^>^ ImmediateMembers();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		bool IsSame(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="queryTopology"></param>
		/// <returns></returns>
		Topology^ ClosestSimplestSubshape(Topology^ queryTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		double Distance(Topology^ topology);

		/*[IsVisibleInDynamoLibrary(false)]
		TDF_Attribute* FindAttribute(String^ ID);*/

	public protected:
		static Topology^ ByCoreTopology(const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology);

	protected:
		/// <summary>
		/// 
		/// </summary>
		//void AttachAttribute(TDF_Attribute* pAttribute);

		Topology();
		virtual ~Topology();
	};
}