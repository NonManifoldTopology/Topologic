#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	public ref class SimulationResult
	{
	public:

		property List<Topologic::Cell^>^ Topology
		{
			List<Topologic::Cell^>^ get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="EPReportName"></param>
		/// <param name="EPReportForString"></param>
		/// <param name="EPTableName"></param>
		/// <param name="EPColumnName"></param>
		/// <param name="EPUnits"></param>
		/// <returns></returns>
		List<Topologic::Cell^>^ Query(String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="buildingCells"></param>
		/// <param name="alpha"></param>
		/// <returns></returns>
		static List<Modifiers::GeometryColor^>^ Display(List<Topologic::Cell^> buildingCells, int alpha);

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="cells"></param>
		/// <param name="oswPath">Used to deduce the SQL path</param>
		SimulationResult(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ osSpaces, String^ timestamp);
		~SimulationResult();

	protected:
		// The cells. The geometry is deduced from here.
		List<Topologic::Cell^>^ m_cells;

		// The SqlFile
		OpenStudio::SqlFile^ m_osSqlFile;

		OpenStudio::Model^ m_osModel;
		
		List<OpenStudio::Space^>^ m_osSpaces;
	};
}