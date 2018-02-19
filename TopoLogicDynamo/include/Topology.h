#pragma once

#include <TopoLogicCore/include/Topology.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopoLogic
{
	public ref class Topology abstract
	{
	public:
		property int Dimensionality
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			int get();
		}

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() abstract;
		}

		property bool Locked
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			bool get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		Topology^ SetLocked(bool value);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ MemberOf();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Members();

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
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Difference(Topology^ topology);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Impose(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Imprint(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Intersection(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Merge(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Slice(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ Union(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		Dictionary<String^, Object^>^ XOR(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		bool SaveToBRep(String^ path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		bool LoadFromBRep(String^ path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		String^ Analyze();

	public protected:
		static Topology^ ByCoreTopology(TopoLogicCore::Topology * const kpCoreTopology);

		virtual TopoLogicCore::Topology* GetCoreTopology() = 0;

	protected:
		Topology();
		virtual ~Topology();
	};
}