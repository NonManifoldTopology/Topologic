// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "TopologicalQuery.h"

#include <TopologicCore/include/Topology.h>

#pragma make_public(TopologicCore::Topology)
#pragma make_public(TopologicCore::TopologyPtr)

using namespace System;
using namespace System::Collections::Generic;

#ifdef TOPOLOGIC_DYNAMO
using namespace Autodesk::DesignScript::Runtime;
#else
using namespace System::Runtime::InteropServices;
#endif

namespace Topologic
{
	ref class Aperture;
	ref class CellComplex;
	ref class Cell;
	ref class Shell;
	ref class Face;
	ref class Wire;
	ref class Edge;
	ref class Vertex;
	ref class Context;

	namespace Factories
	{
		ref class TopologyFactory;
	}

	/// <summary>
	/// A Topology is an abstract superclass that includes constructors, properties and methods used by other subclasses that extend it.
	/// </summary>
	public ref class Topology abstract : public TopologicalQuery
	{
	public:
#ifdef TOPOLOGIC_DYNAMO
		/// <summary>
		/// Creates a Topology by geometry.
		/// </summary>
		/// <param name="geometry">A geometry</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Topology">The created Topology</returns>
		static Topology^ ByGeometry(Autodesk::DesignScript::Geometry::Geometry^ geometry, [DefaultArgument("0.0001")] double tolerance);

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
		/// <param name="faces">A set of Faces</param>
		/// <returns name="Topology">The created Topology</returns>
		[IsVisibleInDynamoLibrary(false)]
		static Topology^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces);
#endif

		/// <summary>
		/// Creates a Topology by a list of Vertices and a 2D list of indices of the Vertices in the first argument. An array of 1 index makes a Vertex. An array of 2 indices makes an Edge. An array of 3 indices either makes a Wire, or, when the array has at least 4 vertices and the first index is the same as the last index, a Face will be created instead. NOTE: This node currently can only create planar Faces.
		/// </summary>
		/// <param name="vertices">A list of Vertices</param>
		/// <param name="vertexIndices">A 2D list of the indices of the Vertices</param>
		/// <returns name="Topology[]">A Topology</returns>
		static IEnumerable<Topology^>^ ByVerticesIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ vertexIndices);

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
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		property Object^ BasicGeometry
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
		/// Returns the original Topology with a dictionary. 
		/// </summary>
		/// <param name="attributes">A dictionary</param>
		/// <returns>The original Topology with a dictionary</returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		Topology^ AddAttributesNoCopy(Dictionary<String^, Object^>^ attributes);

		/// <summary>
		/// Returns the contents (non-constituent members) of the input Topology.
		/// </summary>
		/// <returns name="Topology[]">A list of Topologies contained in the input Topology as non-constituent members</returns>
		property IEnumerable<Topology^>^ Contents
		{
			IEnumerable<Topology^>^ get();
		}

		/// <summary>
		/// Returns the Apertures of the input Topology.
		/// </summary>
		/// <returns name="Aperture[]">A list of Topologies contained in the input Topology as Apertures</returns>
		property IEnumerable<Aperture^>^ Apertures
		{
			IEnumerable<Aperture^>^ get();
		}

		/// <summary>
		/// Returns the sub-contents (contents of the constituent members) of a Topology.
		/// </summary>
		/// <returns name="Topology[]">A list of the sub-contents (contents of the constituent members) of a Topology</returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		property IEnumerable<Topology^>^ SubContents
		{
			IEnumerable<Topology^>^ get();
		}

		/// <summary>
		/// Returns the Topologies containing the input Topology as a content.
		/// </summary>
		/// <returns name="Context[]">A list of the non-constituent members containing the input Topology</returns>
		property IEnumerable<Context^>^ Contexts
		{
			IEnumerable<Context^>^ get();
		}

		/// <summary>
		/// Adds a Topology as a content (non-constituent member) to another Topology of a specific type. If the type filter is 0, the contents will be added to the parent topology. Otherwise, the contents will be added to the closest sub-topology which matches the filter.
		/// </summary>
		/// <param name="contentTopology">The Topology of the content</param>
		/// <param name="typeFilter">The type of the context Topology</param>
		/// <returns>The new Topology</returns>
#ifdef TOPOLOGIC_DYNAMO
		Topology^ AddContent(Topology^ contentTopology, [DefaultArgument("0")] int typeFilter);
#else
		Topology^ AddContent(Topology^ contentTopology, int typeFilter);
#endif

		/// <summary>
		/// Adds a list of Topologies as contents (non-constituent members) to another Topology of a specific type. If the type filter is 0, the contents will be added to the parent topology. Otherwise, the contents will be added to the closest sub-topology which matches the filter.
		/// </summary>
		/// <param name="contentTopologies">The Topologies of the content</param>
		/// <param name="typeFilter">The type of the context Topology</param>
		/// <returns>The new Topology</returns>
#ifdef TOPOLOGIC_DYNAMO
		Topology^ AddContents(IEnumerable<Topology^>^ contentTopologies, [DefaultArgument("0")] int typeFilter);
#else
		Topology^ AddContents(IEnumerable<Topology^>^ contentTopologies, int typeFilter);
#endif

		/// <summary>
		/// Removes a content (non-constituent member) from a Topology.
		/// </summary>
		/// <param name="contentTopology">The Topology of the content</param>
		/// <returns name="Topology">The new Topology</returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		Topology^ RemoveContent(Topology^ contentTopology);

		/// <summary>
		/// Removes contents (non-constituent members) from a Topology.
		/// </summary>
		/// <param name="contentTopologies">The Topologies of the contents</param>
		/// <returns name="Topology">The new Topology</returns>
		Topology^ RemoveContents(IEnumerable<Topology^>^ contentTopologies);

		/// <summary>
		/// Adds Apertures to a Topology.
		/// </summary>
		/// <param name="apertureTopologies">A list of Topologies associated to the apertures</param>
		/// <returns name="Topology">The new Topology</returns>
		Topology^ AddApertures(System::Collections::Generic::IEnumerable<Topology^>^ apertureTopologies);

		/// <summary>
		/// Returns the shared Topologies between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <param name="typeFilter">The type of the filtered Topologies</param>
		/// <returns name="Topology[]">A list of shared Topologies between the input Topology and another Topology</returns>
		IEnumerable<Topology^>^ SharedTopologies(
			Topology^ topology, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("255")] 
#endif
			int typeFilter);

		/// <summary>
		/// Performs the Difference operation between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Difference operation</returns>
		Topology^ Difference(
            Topology^ topology,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Imposes another Topology on the input Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Impose operation</returns>
		Topology^ Impose(
            Topology^ tool,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Imprints another Topology on the input Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Imprint operation</returns>
		Topology^ Imprint(
            Topology^ tool,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Performs the Intersection operation between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Intersection operation</returns>
		Topology^ Intersect(
            Topology^ topology,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Merges the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ Merge(
            Topology^ topology,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Merges the sub-topologies of the input Topology.
		/// </summary>
		/// <returns name="Topology">The result of the SelfMerge operation</returns>
		Topology^ SelfMerge();

		/// <summary>
		/// Slices the input Topology with another Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Slice operation</returns>
		Topology^ Slice(
            Topology^ tool,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Divides the input Topology with another Topology.
		/// </summary>
		/// <param name="tool">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Divide operation</returns>
		Topology^ Divide(
            Topology^ tool,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Unions the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the Union operation</returns>
		Topology^ Union(
            Topology^ topology,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Performs an XOR operation between the input Topology and another Topology.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <param name="transferDictionary">If True, transfer Dictionaries from every original constituent member to the resulting members generated from the original member</param>
		/// <returns name="Topology">The result of the XOR operation</returns>
		Topology^ XOR(
            Topology^ topology,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("false")] bool transferDictionary
#else
			bool transferDictionary
#endif
		);

		/// <summary>
		/// Returns the center of mass of any Topology.
		/// </summary>
		/// <param name="topology">A Topology</param>
		/// <returns name="Vertex">The center of mass of the given Topology</returns>
		property Vertex^ CenterOfMass
		{
			Vertex^ get();
		}

		/// <summary>
		/// Returns the centroid of any Topology.
		/// </summary>
		/// <param name="topology">A Topology</param>
		/// <returns name="Vertex">The centroid of the given Topology</returns>
		property Vertex^ Centroid
		{
			Vertex^ get();
		}

		/// <summary>
		/// Exports a Topology to a BRep file (.brep).
		/// </summary>
		/// <param name="filePath">The file to export the Topology to</param>
		/// <returns name="bool">True if the BRep file is succesffully created, otherwise false</returns>
		bool ExportToBRep(String^ filePath);

		/// <summary>
		/// Imports a Topology from a BRep file (.brep).
		/// </summary>
		/// <param name="filePath">The file to export the Topology to</param>
		/// <returns name="Topology">The loaded Topology</returns>
		static Topology^ ByImportedBRep(String^ filePath);

		/// <summary>
		/// Prints the topological information of a Topology.
		/// </summary>
		/// <returns name="String">The topological information of the input Topology</returns>
		String^ Analyze();

#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		property bool IsReversed
		{
			bool get();
		}

		/// <summary>
		/// Returns all sub-topologies (immediate constituent members) of a Topology.
		/// </summary>
		/// <returns name="Topology[]">A list of the sub-topologies (immediate constituent members) of a Topology</returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		property IEnumerable<Topology^>^ SubTopologies
		{
			IEnumerable<Topology^>^ get();
		}

		/// <summary>
		/// Returns the Shells constituent to the Topology.
		/// </summary>
		/// <returns name="Shell[]">A list of Shells constituent to the Topology</returns>
		property IEnumerable<Shell^>^ Shells
		{
			IEnumerable<Shell^>^ get();
		}

		/// <summary>
		/// Returns the Faces constituent to the Topology.
		/// </summary>
		/// <returns name="Face[]">A list of Faces constituent to the Topology</returns>
		property IEnumerable<Face^>^ Faces
		{
			IEnumerable<Face^>^ get();
		}

		/// <summary>
		/// Returns the Wires constituent to the Topology.
		/// </summary>
		/// <returns name="Wire[]">A list of Wires constituent to the Topology</returns>
		property IEnumerable<Wire^>^ Wires
		{
			IEnumerable<Wire^>^ get();
		}

		/// <summary>
		/// Returns the Edges constituent to the Topology.
		/// </summary>
		/// <returns name="Edge[]">A list of Edges constituent to the Topology</returns>
		property IEnumerable<Edge^>^ Edges
		{
			IEnumerable<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Vertices constituent to the Topology.
		/// </summary>
		/// <returns name="Vertex[]">A list of Vertices constituent to the Topology</returns>
		property IEnumerable<Vertex^>^ Vertices
		{
			IEnumerable<Vertex^>^ get();
		}

		/// <summary>
		/// Returns the Cells constituent to the Topology.
		/// </summary>
		/// <returns name="Cell[]">A list of Cells constituent to the Topology</returns>
		property IEnumerable<Cell^>^ Cells
		{
			IEnumerable<Cell^>^ get();
		}

		/// <summary>
		/// Returns the CellComplexes constituent to the Topology.
		/// </summary>
		/// <returns name="CellComplex[]">A list of CellComplexes constituent to the Topology</returns>
		property IEnumerable<CellComplex^>^ CellComplexes
		{
			IEnumerable<CellComplex^>^ get();
		}

		/// <summary>
		/// Checks if two Topologies are the same. NOTE: This is a pointer check, not a topological or a geometry one.
		/// </summary>
		/// <param name="topology">Another Topology</param>
		/// <returns>True if the Topologies are the same, otherwise false</returns>
		bool IsSame(Topology^ topology);

		/// <summary>
		/// Returns the sub-topology that is the closest to the selector and the simplest (i.e. has the lowest dimensionality).
		/// </summary>
		/// <param name="selector">A Topology</param>
		/// <returns>The closest and simplest sub-topology to the selector</returns>
		Topology^ ClosestSimplestSubshape(Topology^ selector);

		/// <summary>
		/// Returns the sub-topology that is the closest to the selector Vertex and of a specific type.
		/// </summary>
		/// <param name="selector">A Vertex</param>
		/// <param name="typeFilter">The type of the filtered Topologies</param>
		/// <returns>The closest sub-topology to the selector of the specified type</returns>
#ifdef TOPOLOGIC_DYNAMO
//#ifndef _DEBUG
//		[IsVisibleInDynamoLibrary(false)]
//#endif
		Topology^ SelectSubtopology(Vertex^ selector,
			[Autodesk::DesignScript::Runtime::DefaultArgument("255")] int typeFilter);
#else
		Topology^ SelectSubtopology(Vertex^ selector, int typeFilter);
#endif
		
		/// <summary>
		/// Copies a Topology and its contents.
		/// </summary>
		/// <returns>The copied Topology</returns>
		generic <class T>
		where T: Topology
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
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

		/// <summary>
		/// Filters a list of Topologies by type. This methods returns a list of Topologies of the type specified in the typefilter.
		/// </summary>
		/// <param name="topologies">A list of Topologies</param>
		/// <param name="typeFilter">The type of the filtered Topologies</param>
		/// <returns>The filtered opologies</returns>
		static IEnumerable<Topology^>^ Filter(IEnumerable<Topology^>^ topologies, int typeFilter);

#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		void RegisterFactory(const TopologicCore::Topology::Ptr& kpCoreTopology, Factories::TopologyFactory^ topologyFactory);

#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		void RegisterFactory(String^ rkGUID, Factories::TopologyFactory^ topologyFactory);

		/// <summary>
		/// Sets a dictionary for a Topology.
		/// </summary>
		/// <returns name="Topology">The Topology with the dictionary</returns>
		Topology^ SetDictionary(Dictionary<String^, Object^>^ dictionary);

		/// <summary>
		/// Sets a list of dictionaries for a Topology.
		/// </summary>
		/// <param name="selectors"></param>
		/// <param name="dictionaries">A list of dictionaries</param>
		/// <param name="typeFilter"></param>
		/// <returns name="Topology">The Topology with the dictionary</returns>
		Topology^ SetDictionaries(IEnumerable<Vertex^>^ selectors, IEnumerable<Dictionary<String^, Object^>^>^ dictionaries, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("255")]
#endif
		int typeFilter);

		/// <summary>
		/// Returns the dictionary of a Topology.
		/// </summary>
		/// <returns name="Dictionary">The dictionary</returns>
		property System::Collections::Generic::Dictionary<String^, Object^>^ Dictionary
		{
			System::Collections::Generic::Dictionary<String^, Object^>^ get();
		}

	public protected:
		static Topology^ ByCoreTopology(const std::shared_ptr<TopologicCore::Topology>& kpCoreTopology);

		Object^ CleanupGeometryOutput(IEnumerable<Object^>^ geometry);

	protected:
		Topology();
		virtual ~Topology();
	};
}