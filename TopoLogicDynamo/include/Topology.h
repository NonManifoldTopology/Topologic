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
		/// <param name="topology1"></param>
		/// <param name="topology2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Difference(Topology^ topology1, Topology^ topology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology1"></param>
		/// <param name="topology2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Impose(Topology^ topology1, Topology^ topology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology1"></param>
		/// <param name="topology2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Imprint(Topology^ topology1, Topology^ topology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology1"></param>
		/// <param name="topology2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Intersection(Topology^ topology1, Topology^ topology2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topologyList"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Merge(List<Topology^>^ topologyList);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <param name="topologyList"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Slice(Topology^ topology, List<Topology^>^ topologyList);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology1"></param>
		/// <param name="topology2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "Topology", "Geometry" })]
		static Dictionary<String^, Object^>^ Union(List<Topology^>^ topologyList);

	public protected:
		virtual TopoLogicCore::Topology* GetCoreTopology() = 0;

	protected:
		Topology();
		virtual ~Topology();
	};
}