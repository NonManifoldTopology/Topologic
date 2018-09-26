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

		List<Modifiers::GeometryColor^>^ GeometryQuery(String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits);

		List<double>^ ValueQuery(String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits);

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