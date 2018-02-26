#pragma once

#include <TopoLogicCore/include/Topology.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopoLogic
{
	/// <summary>
	/// The abstract base class for all topological entities in the library.
	/// </summary>
	public ref class Topology abstract
	{
	public:
		/// <summary>
		/// Returns the dimensionality of the topological entity.
		/// </summary>
		/// <returns name="int">The topological entity's dimentionality</returns>
		property int Dimensionality
		{
			int get();
		}

		/// <summary>
		/// Creates the host geometry counterpart of the topological entity. The mapping is as follows. 
		/// - Vertex: Point;
		/// - Edge: either a Curve or its subclasses, other than Polycurve or its subclasses; 
		/// - Wire: PolyCurve or Polygon; 
		/// - Face: Surface or NurbsSurface;
		/// - Shell: PolySurface; 
		/// - Cell: either a Solid or its subclasses; 
		/// - CellComplex: a list of Solids; 
		/// - Cluster: a list of topological entities without a constituent parent.
		/// </summary>
		/// <returns name="Geometry">The host geometry counterpart of the topological entity</returns>
		property Object^ Geometry
		{
			virtual Object^ get() abstract;
		}

		/// <summary>
		/// Returns the locked flag of this topological entity
		/// </summary>
		/// <returns name="bool">The locked flag of this topological entity</returns>
		property bool Locked
		{
			bool get();
		}

		/// <summary>
		/// Set the topology's locked flag
		/// </summary>
		/// <param name="value"></param>
		/// <returns name="Topology">The original topology with the new locked flag</returns>
		Topology^ SetLocked(bool value);

		/// <summary>
		/// Returns the topological entities containing the input topology as a non-constituent member
		/// </summary>
		/// <returns name="Topology[]">The topological entities containing the input topology as a non-constituent member</returns>
		List<Topology^>^ MemberOf();

		/// <summary>
		/// Returns the non-constituent members of the input topological entity.
		/// </summary>
		/// <returns name="Topology[]">The non-constituent members of the input topological entity</returns>
		List<Topology^>^ Members();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> {
			"ArgumentImagesInArguments",
				"ArgumentImagesInTools",
				"ToolImagesInArguments",
				"ToolImagesInTools",
				"GeometryArgumentImagesInArguments",
				"GeometryArgumentImagesInTools",
				"GeometryToolImagesInArguments",
				"GeometryToolImagesInTools"
		})]
		Dictionary<String^, Object^>^ BooleanImages(Topology^ topology);

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
		/// <returns name="bool">True if the BRep file is succesffully loaded, otherwise false</returns>
		bool LoadFromBRep(String^ path);

		/// <summary>
		/// Print the topological information of the input entity.
		/// </summary>
		/// <param name="topology">A topological entity</param>
		/// <returns name="String">The topological information of the input entity</returns>
		String^ Analyze();

	public protected:
		static Topology^ ByCoreTopology(TopoLogicCore::Topology * const kpCoreTopology);

		virtual TopoLogicCore::Topology* GetCoreTopology() = 0;

	protected:
		Topology();
		virtual ~Topology();
	};
}