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
		static Dictionary<String^, Object^>^ MemberOf(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Members(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyArguments"></param>
		/// <param name="topologyTools"></param>
		/// <param name="outputCellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Difference(List<Topology^> topologyArguments, List<Topology^> topologyTools, bool outputCellComplex);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyArguments"></param>
		/// <param name="topologyTools"></param>
		/// <param name="outputCellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Impose(List<Topology^> topologyArguments, List<Topology^> topologyTools, bool outputCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyArguments"></param>
		/// <param name="topologyTools"></param>
		/// <param name="outputCellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Imprint(List<Topology^> topologyArguments, List<Topology^> topologyTools, bool outputCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyArguments"></param>
		/// <param name="topologyTools"></param>
		/// <param name="outputCellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Intersection(List<Topology^> topologyArguments, List<Topology^> topologyTools, bool outputCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyList"></param>
		/// <param name="outputCellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Merge(List<Topology^>^ topologyList, bool outputCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyArguments"></param>
		/// <param name="topologyTools"></param>
		/// <param name="outputCellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Slice(List<Topology^> topologyArguments, List<Topology^> topologyTools, bool outputCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyArguments"></param>
		/// <param name="topologyTools"></param>
		/// <param name="outputCellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Union(List<Topology^> topologyArguments, List<Topology^> topologyTools, bool outputCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="path"></param>
		/// <returns></returns>
		static bool SaveToBRep(Topology^ topology, String^ path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="path"></param>
		/// <returns></returns>
		static bool LoadFromBRep(Topology^ topology, String^ path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		static String^ Analyze(Topology^ topology);

	public protected:
		static Topology^ ByCoreTopology(TopoLogicCore::Topology * const kpCoreTopology);

		virtual TopoLogicCore::Topology* GetCoreTopology() = 0;

	protected:
		Topology();
		virtual ~Topology();
	};
}