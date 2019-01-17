#pragma once

#include "TopologicalQuery.h"

#include <TopologicCore/include/Topology.h>

#pragma make_public(TopologicCore::Topology)
#pragma make_public(TopologicCore::TopologyPtr)

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace Topologic
{
	ref class Face;
	ref class Vertex;
	ref class Context;
	ref class TopologyFactory;

	/// <summary>
	/// A Topology is an abstract superclass that includes constructors, properties and methods used by other subclasses that extend it.
	/// </summary>
	public ref class Topology abstract : public TopologicalQuery
	{
	public:
		/// <summary>
		/// Create a topology by geometry.
		/// </summary>
		/// <param name="geometry"></param>
		/// <returns name="Topology"></returns>
		static Topology^ ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry);

		/// <summary>
		/// Create a topology by a list of vertices and a 2D list of indices of the vertices in the first argument. An array of 1 index makes a vertex. An array of 2 indices makes an edge. An array of 3 indices either makes a wire, or, when the array has at least 4 vertices and the first index is the same as the last index, a face will be created instead. NOTE: This node currently can only create planar faces.
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="vertexIndices"></param>
		/// <returns name="Topology[]"></returns>
		static List<Topology^>^ ByVerticesIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::List<int>^>^ vertexIndices);

		/// <summary>
		/// Create a shell by a polysurface.
		/// </summary>
		/// <param name="polySurface"></param>
		/// <returns></returns>
		static Topology^ ByPolySurface(Autodesk::DesignScript::Geometry::PolySurface^ polySurface);

		/// <summary>
		/// Create a shell or a cluster by a set of connected faces.
		/// </summary>
		/// <param name="faces">A set of faces.</param>
		/// <returns name="Topology">The created topology</returns>
		static Topology^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces);

		/// <summary>
		/// Returns the dimensionality of the topological entity.
		/// </summary>
		/// <returns name="int">The topological entity's dimentionality</returns>
		property int Dimensionality
		{
			int get();
		}

		/// <summary>
		/// Create a geometry from the topology.
		/// </summary>
		/// <returns name="Geometry">The host geometry counterpart of the topological entity</returns>
		[IsVisibleInDynamoLibrary(false)]
		property Object^ Geometry_
		{
			virtual Object^ get() abstract;
		}

		property Object^ Geometry
		{
			virtual Object^ get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="keys"></param>
		/// <param name="values"></param>
		/// <returns name="Topology"></returns>
		Topology^ SetKeysValues(List<String^>^ keys, List<Object^>^ values);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		Object^ ValueAtKey(String^ key);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="attributes"></param>
		/// <returns></returns>
		[IsVisibleInDynamoLibrary(false)]
		Topology^ AddAttributesNoCopy(Dictionary<String^, Object^>^ attributes);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns name="Value"></returns>
		Object^ AttributeValue(String^ name);

		/// <summary>
		/// 
		/// </summary>
		property List<List<Object^>^>^ KeysValues
		{
			List<List<Object^>^>^ get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="keys"></param>
		/// <returns></returns>
		Topology^ RemoveKeys(List<String^>^ keys);

		/// <summary>
		/// Returns the non-constituent members of the input topological entity.
		/// </summary>
		/// <returns name="Topology[]">The topological entities containing the input topology as a non-constituent member</returns>
		property List<Topology^>^ Contents
		{
			List<Topology^>^ get();
		}

		[IsVisibleInDynamoLibrary(false)]
		property List<Topology^>^ SubContents
		{
			List<Topology^>^ get();
		}

		/// <summary>
		/// Returns the topological entities containing the input topology as a content.
		/// </summary>
		/// <returns name="Context[]">The non-constituent members of the input topological entity</returns>
		property List<Context^>^ Contexts
		{
			List<Context^>^ get();
		}

		/// <summary>
		/// Add this topology as a non-constituent member to another topology.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		Topology^ AddContent(Topology^ topology);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="content"></param>
		/// <param name="typeFilter"></param>
		/// <returns></returns>
		Topology^ AddContent(Topology^ content, [DefaultArgument("255")] int typeFilter);

		/// <summary>
		/// Remove a non-constituent member from the topology.
		/// </summary>
		/// <param name="content"></param>
		/// <returns name="Topology"></returns>
		Topology^ RemoveContent(Topology^ content);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="apertureTopologies"></param>
		/// <returns name="Topology"></returns>
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

		/// <summary>
		/// Perform the Difference operation between the first and second topological entities.
		/// </summary>
		/// <param name="topology">The second topological entity</param>
		/// <returns name="Topology">The result of the Difference operation</returns>
		Topology^ Difference(Topology^ topology);

		/// <summary>
		/// Impose the second topological entity to the first one.
		/// </summary>
		/// <param name="tool">Topologic.Topology</param>
		/// <returns name="Topology">Topologic.Topology</returns>
		Topology^ Impose(Topologic::Topology^ tool);

		/// <summary>
		/// Imprint the second topological entity to the first one.
		/// </summary>
		/// <param name="tool">Another topology</param>
		/// <returns name="Topology">The result of the Imprint operation</returns>
		Topology^ Imprint(Topology^ tool);

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
		/// <param name="tool"></param>
		/// <returns name="Topology">The result of the Slice operation</returns>
		Topology^ Slice(Topology^ tool);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="tool"></param>
		/// <returns name="Topology">The result of the Divide operation</returns>
		Topology^ Divide(Topology^ tool);

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

		[IsVisibleInDynamoLibrary(false)]
		Topology^ Trim(Topology^ trim);

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
		/// <param name="selector"></param>
		/// <param name="typeFilter"></param>
		/// <returns></returns>
		Topology^ SelectSubtopology(Topology^ selector,
			[Autodesk::DesignScript::Runtime::DefaultArgument("255")] int typeFilter);
		
		generic <class T>
		where T: Topology
		[IsVisibleInDynamoLibrary(false)]
		T Copy();

		Topology^ ShallowCopy();


		/// <summary>
		/// Returns the instance type as a string.
		/// </summary>
		property String^ TypeAsString
		{
			String^ get();
		}

		property int Type
		{
			int get();
		}

		[IsVisibleInDynamoLibrary(false)]
		void RegisterFactory(const TopologicCore::Topology::Ptr& kpCoreTopology, TopologyFactory^ topologyFactory);

		[IsVisibleInDynamoLibrary(false)]
		void RegisterFactory(String^ rkGUID, TopologyFactory^ topologyFactory);

	public protected:
		static Topology^ ByCoreTopology(const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology);

		Object^ CleanupGeometryOutput(List<Object^>^ geometry);

	protected:
		Topology();
		virtual ~Topology();
	};
}