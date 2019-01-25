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
	ref class CellComplex;
	ref class Cell;
	ref class Shell;
	ref class Face;
	ref class Wire;
	ref class Edge;
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
		/// Creates a Topology by geometry.
		/// </summary>
		/// <param name="geometry">A geometry</param>
		/// <param name="tolerance">A tolerance value</param>
		/// <returns name="Topology">The created Topology</returns>
		static Topology^ ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry, [DefaultArgument("0.001")] double tolerance);

		/// <summary>
		/// Creates a Topology by a list of Vertices and a 2D list of indices of the Vertices in the first argument. An array of 1 index makes a Vertex. An array of 2 indices makes an Edge. An array of 3 indices either makes a Wire, or, when the array has at least 4 vertices and the first index is the same as the last index, a Face will be created instead. NOTE: This node currently can only create planar Faces.
		/// </summary>
		/// <param name="vertices">A list of Vertices</param>
		/// <param name="vertexIndices">A 2D list of the indices of the Vertices</param>
		/// <returns name="Topology[]">A Topology</returns>
		static List<Topology^>^ ByVerticesIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::List<int>^>^ vertexIndices);

		/// <summary>
		/// Creates a Shell by a polysurface [NOTE: This method is deleted].
		/// </summary>
		/// <param name="polySurface">A polysurface</param>
		/// <returns>The created Topology</returns>
		[IsVisibleInDynamoLibrary(false)]
		static Topology^ ByPolySurface(Autodesk::DesignScript::Geometry::PolySurface^ polySurface);

		/// <summary>
		/// Creates a Shell or a Cluster by a set of connected Faces.
		/// </summary>
		/// <param name="faces">A set of faces</param>
		/// <returns name="Topology">The created Topology</returns>
		static Topology^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces);

		/// <summary>
		/// Returns the dimensionality of the Topology.
		/// </summary>
		/// <returns name="int">The Topology's dimensionality</returns>
		property int Dimensionality
		{
			int get();
		}

		/// <summary>
		/// Creates a geometry from the Topology.
		/// </summary>
		/// <returns name="Geometry">The host geometry counterpart of the topological entity</returns>
		[IsVisibleInDynamoLibrary(false)]
		property Object^ Geometry_
		{
			virtual Object^ get() abstract;
		}

		/// <summary>
		/// Creates a geometry from Topology.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ Geometry
		{
			virtual Object^ get();
		}

		/// <summary>
		/// Returns a Topology 
		/// </summary>
		/// <param name="attributes">A dictionary</param>
		/// <returns>The original Topology with a dictionary</returns>
		[IsVisibleInDynamoLibrary(false)]
		Topology^ AddAttributesNoCopy(Dictionary<String^, Object^>^ attributes);

		/// <summary>
		/// Returns the non-constituent members of the input Topology.
		/// </summary>
		/// <returns name="Topology[]">A list of Topologies contained in the input Topology as non-constituent members</returns>
		property List<Topology^>^ Contents
		{
			List<Topology^>^ get();
		}

		/// <summary>
		/// Returns the sub-contents (contents of the constituent members) of a Topology.
		/// </summary>
		/// <returns name="Context[]">A list of the sub-contents (contents of the constituent members) of a Topology</returns>
		[IsVisibleInDynamoLibrary(false)]
		property List<Topology^>^ SubContents
		{
			List<Topology^>^ get();
		}

		/// <summary>
		/// Returns the Topologies containing the input topology as a content.
		/// </summary>
		/// <returns name="Context[]">A list of the non-constituent members containing the input Topology</returns>
		property List<Context^>^ Contexts
		{
			List<Context^>^ get();
		}

		/// <summary>
		/// Adds this Topology as a content (non-constituent member) to another Topology.
		/// </summary>
		/// <param name="topology">The Topology of the content</param>
		/// <returns>The new Topology</returns>
		Topology^ AddContent(Topology^ topology);
		
		/// <summary>
		/// Adds this Topology as a content (non-constituent member) to another Topology of a specific type.
		/// </summary>
		/// <param name="content">The Topology of the content</param>
		/// <param name="typeFilter">The type of the context Topology</param>
		/// <returns>The new Topology</returns>
		Topology^ AddContent(Topology^ content, [DefaultArgument("255")] int typeFilter);

		/// <summary>
		/// Removes a content (non-constituent member) from a Topology.
		/// </summary>
		/// <param name="content">The Topology of the content</param>
		/// <returns name="Topology">The new Topology</returns>
		Topology^ RemoveContent(Topology^ content);

		/// <summary>
		/// Adds Apertures to a Topology.
		/// </summary>
		/// <param name="apertureTopologies">A list of Topologies associated to the apertures</param>
		/// <returns name="Topology">The new Topology</returns>
		Topology^ AddApertures(System::Collections::Generic::IEnumerable<Topology^>^ apertureTopologies);

		/// <summary>
		/// Adds a context to a Topology.
		/// </summary>
		/// <param name="context">A context</param>
		/// <returns name="Topology">The new Topology</returns>
		Topology^ AddContext(Context^ context);

		/// <summary>
		/// Removes a context from a Topology.
		/// </summary>
		/// <param name="context">A context</param>
		/// <returns name="Topology">The new Topology</returns>
		Topology^ RemoveContext(Context^ context);

		/// <summary>
		/// Returns the shared Topologies between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns name="Topology[]">A list of shared Topologies between the input Topology and another Topology</returns>
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
		/// Performs the Difference operation between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns name="Topology">The result of the Difference operation</returns>
		Topology^ Difference(Topology^ topology);

		/// <summary>
		/// Imposes another Topology on the input Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <returns name="Topology">The result of the Impose operation</returns>
		Topology^ Impose(Topologic::Topology^ tool);

		/// <summary>
		/// Imprints another Topology on the input Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <returns name="Topology">The result of the Imprint operation</returns>
		Topology^ Imprint(Topology^ tool);

		/// <summary>
		/// Performs the Intersection operation between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns name="Topology">The result of the Intersection operation</returns>
		Topology^ Intersect(Topology^ topology);

		/// <summary>
		/// Merges the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ Merge(Topology^ topology);

		/// <summary>
		/// Merges the sub-topologies of the input Topology.
		/// </summary>
		/// <returns name="Topology">The result of the SelfMerge operation</returns>
		Topology^ SelfMerge();

		/// <summary>
		/// Slices the input Topology with another Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <returns name="Topology">The result of the Slice operation</returns>
		Topology^ Slice(Topology^ tool);

		/// <summary>
		/// Divides the input Topology with another Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <returns name="Topology">The result of the Divide operation</returns>
		Topology^ Divide(Topology^ tool);

		/// <summary>
		/// Unions the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns name="Topology">The result of the Union operation</returns>
		Topology^ Union(Topology^ topology);

		/// <summary>
		/// Performs an XOR operation between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns name="Topology">The result of the XOR operation</returns>
		Topology^ XOR(Topology^ topology);

		/// <summary>
		/// Trims the input Topology with another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns name="Topology">The result of the Trim operation</returns>
		[IsVisibleInDynamoLibrary(false)]
		Topology^ Trim(Topology^ trim);

		/// <summary>
		/// Exports a Topology to a BRep file (.brep).
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="bool">True if the BRep file is succesffully created, otherwise false</returns>
		bool ExportToBRep(String^ path);

		/// <summary>
		/// Imports a Topology from a BRep file (.brep).
		/// </summary>
		/// <param name="path">The path to the BRep file</param>
		/// <returns name="Topology">The loaded Topology</returns>
		static Topology^ ImportFromBRep(String^ path);

		/// <summary>
		/// Prints the topological information of a Topology.
		/// </summary>
		/// <returns name="String">The topological information of the input Topology</returns>
		String^ Analyze();

		/// <summary>
		/// Returns all sub-topologies (immediate constituent members) of a Topology.
		/// </summary>
		/// <returns name="Topology[]">A list of the sub-topologies (immediate constituent members) of a Topology</returns>
		property List<Topology^>^ SubTopologies
		{
			List<Topology^>^ get();
		}

		/// <summary>
		/// Get the shells constituent to the Topology.
		/// </summary>
		/// <returns name="Shell[]"></returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Get the faces constituent to the Topology.
		/// </summary>
		/// <returns name="Face[]"></returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Get the wires constituent to the Topology.
		/// </summary>
		/// <returns name="Wire[]"></returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Get the edges constituent to the Topology.
		/// </summary>
		/// <returns name="Edge[]"></returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the vertices constituent to the Topology.
		/// </summary>
		/// <returns name="Vertex[]"></returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Get the cells constituent to the Topology.
		/// </summary>
		/// <returns name="Cell[]"></returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Get the cellComplexes constituent to the Topology.
		/// </summary>
		/// <returns name="CellComplex[]"></returns>
		property List<CellComplex^>^ CellComplexes
		{
			List<CellComplex^>^ get();
		}

		/// <summary>
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns>True if the Topologies are the same, otherwise false</returns>
		bool IsSame(Topology^ topology);

		/// <summary>
		/// Returns the sub-topology that is the closest and the simplest (i.e. has the lowest dimensionality) to the selector.
		/// </summary>
		/// <param name="selector">A Topology</param>
		/// <returns>The closest and simplest sub-topology to the selector</returns>
		Topology^ ClosestSimplestSubshape(Topology^ selector);

		/// <summary>
		/// Returns the sub-topology that is the closest to the selector and of a specific type.
		/// </summary>
		/// <param name="selector">A Topology</param>
		/// <param name="typeFilter">A type</param>
		/// <returns>The closest sub-topology to the selector of the specified type</returns>
		Topology^ SelectSubtopology(Topology^ selector,
			[Autodesk::DesignScript::Runtime::DefaultArgument("255")] int typeFilter);
		
		/// <summary>
		/// Copies a Topology and its contents.
		/// </summary>
		/// <returns>The copied Topology</returns>
		generic <class T>
		where T: Topology
		[IsVisibleInDynamoLibrary(false)]
		T Copy();

		/// <summary>
		/// Copies a Topology without its contents.
		/// </summary>
		/// <returns>The copied Topology</returns>
		Topology^ ShallowCopy();


		/// <summary>
		/// Returns the instance type as a string.
		/// </summary>
		/// <returns>The instance type as a string</returns>
		property String^ TypeAsString
		{
			String^ get();
		}

		/// <summary>
		/// Returns the type associated to the Topology.
		/// </summary>
		/// <returns>The type associated to the Topology</returns>
		property int Type
		{
			int get();
		}

		[IsVisibleInDynamoLibrary(false)]
		void RegisterFactory(const TopologicCore::Topology::Ptr& kpCoreTopology, TopologyFactory^ topologyFactory);

		[IsVisibleInDynamoLibrary(false)]
		void RegisterFactory(String^ rkGUID, TopologyFactory^ topologyFactory);

		/*/// <summary>
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
		/// 
		/// </summary>
		/// <param name="keys"></param>
		/// <param name="values"></param>
		/// <returns name="Topology"></returns>
		Topology^ SetKeysValues(List<String^>^ keys, List<Object^>^ values);*/

		Topology^ SetDictionary(Dictionary<String^, Object^>^ dictionary);

		property System::Collections::Generic::Dictionary<String^, Object^>^ Dictionary
		{
			System::Collections::Generic::Dictionary<String^, Object^>^ get();
		}

	public protected:
		static Topology^ ByCoreTopology(const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology);

		Object^ CleanupGeometryOutput(List<Object^>^ geometry);

	protected:
		Topology();
		virtual ~Topology();
	};
}