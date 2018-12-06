#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	ref class EnergyModel;

	public ref class Simulation
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
		/// <param name="EPColumnName"></param>
		/// <param name="EPUnits"></param>
		/// <returns name="Data"></returns>
		Dictionary<String^, Dictionary<String^, Object^>^>^ Query(String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="energyModel"></param>
		/// <param name="data"></param>
		/// <returns name="GeometryColor[]"></returns>
		static List<Modifiers::GeometryColor^>^ Display(EnergyModel^ energyModel, Dictionary<String^, Dictionary<String^, Object^>^>^ data);

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="cells"></param>
		/// <param name="oswPath">Used to deduce the SQL path</param>
		Simulation(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ osSpaces, String^ timestamp);
		~Simulation();

	protected:
		// The cells. The geometry is deduced from here.
		List<Topologic::Cell^>^ m_cells;

		// The SqlFile
		OpenStudio::SqlFile^ m_osSqlFile;

		OpenStudio::Model^ m_osModel;
		
		List<OpenStudio::Space^>^ m_osSpaces;
	};
}