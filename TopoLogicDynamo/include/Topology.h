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
		property int Dimensionality
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="int">The shape's dimentionality</returns>
			int get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Geometry">The host geometry counter part of the topological shape</returns>
		property Object^ Geometry
		{
			virtual Object^ get() abstract;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="bool">The locked flag of this shape</returns>
		property bool Locked
		{
			bool get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <returns name="Topology">The original topology with the new locked flag</returns>
		Topology^ SetLocked(bool value);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Topology[]">The topological shapes containing the input shape as a non-constituent member.</returns>
		List<Topology^>^ MemberOf();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Topology[]">The non-constituent members of the input topological shape.</returns>
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
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Difference operation</returns>
		Topology^ Difference(Topology^ topology);
		
		/// <summary>
		/// Impose the second topology to the first one.
		/// </summary>
		/// <param name="topology">Another topology</param>
		/// <returns name="Topology">The result of the Impose operation</returns>
		Topology^ Impose(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Imprint operation</returns>
		Topology^ Imprint(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Intersection operation</returns>
		Topology^ Intersection(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Merge operation</returns>
		Topology^ Merge(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Slice operation</returns>
		Topology^ Slice(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the Union operation</returns>
		Topology^ Union(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Topology">The result of the XOR operation</returns>
		Topology^ XOR(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <returns name="bool">True if the BRep file is succesffully created, otherwise false</returns>
		bool SaveToBRep(String^ path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <returns name="bool">True if the BRep file is succesffully loaded, otherwise false</returns>
		bool LoadFromBRep(String^ path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="String">The topological information of the input shape</returns>
		String^ Analyze();

	public protected:
		static Topology^ ByCoreTopology(TopoLogicCore::Topology * const kpCoreTopology);

		virtual TopoLogicCore::Topology* GetCoreTopology() = 0;

	protected:
		Topology();
		virtual ~Topology();
	};
}