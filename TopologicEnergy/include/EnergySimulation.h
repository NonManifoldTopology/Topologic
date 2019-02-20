#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	ref class EnergyModel;

	public ref class EnergySimulation
	{
	public:

		property List<Topologic::Cell^>^ Topology
		{
			List<Topologic::Cell^>^ get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="energyModel"></param>
		/// <param name="openStudioExePath"></param>
		/// <param name="openStudioOutputDirectory"></param>
		/// <param name="run"></param>
		/// <returns name="EnergySimulation"></returns>
		static EnergySimulation^ ByEnergyModel(EnergyModel^ energyModel, String^ openStudioExePath, String ^ openStudioOutputDirectory, bool run);

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="cells"></param>
		/// <param name="oswPath">Used to deduce the SQL path</param>
		EnergySimulation(List<Topologic::Cell^>^ cells, System::String^ oswPath, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ osSpaces);
		~EnergySimulation();

		property OpenStudio::SqlFile^ OsSqlFile
		{
			OpenStudio::SqlFile^ get()
			{
				return m_osSqlFile;
			}
		}

		property List<OpenStudio::Space^>^ OsSpaces
		{
			List<OpenStudio::Space^>^ get()
			{
				return m_osSpaces;
			}
		}

	protected:
		// The cells. The geometry is deduced from here.
		List<Topologic::Cell^>^ m_cells;

		// The SqlFile
		OpenStudio::SqlFile^ m_osSqlFile;

		OpenStudio::Model^ m_osModel;
		
		List<OpenStudio::Space^>^ m_osSpaces;
	};
}