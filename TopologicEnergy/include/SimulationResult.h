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

using namespace System;
using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	ref class EnergyModel;
	ref class EnergySimulation;

	public ref class SimulationResult
	{
	public:

		/// <summary>
		/// Retrieves the simulation result from an energy simulation object.
		/// </summary>
		/// <param name="energySimulation">An energy simulation object</param>
		/// <param name="EPReportName">An EPReportName label</param>
		/// <param name="EPReportForString">An EPReportForString label</param>
		/// <param name="EPTableName">An EPTableName label</param>
		/// <param name="EPColumnName">An EPColumnName label</param>
		/// <param name="EPUnits">An EPUnits label</param>
		/// <returns name="SimulationResult">A simulation result object</returns>
		static SimulationResult^ ByEnergySimulation(EnergySimulation^ energySimulation, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits);

		/// <summary>
		/// Draws an EnergyModel using a list of colors
		/// </summary>
		/// <param name="energyModel">An EnergyModel</param>
		/// <param name="colors">A list of colors</param>
		/// <returns name="GeometryColor[]">A list of colored geometries</returns>
		static IEnumerable<Modifiers::GeometryColor^>^ Display(EnergyModel^ energyModel, IEnumerable<DSCore::Color^>^ colors);

		/// <summary>
		/// Outputs the color range of the data. The first element is the colors, the second element is the normalised position within the spectrum. The colors are sorted by the values.
		/// </summary>
		/// <param name="minDomain">An optional minimum domain</param>
		/// <param name="maxDomain">An optional maximum domain</param>
		/// <param name="count">The number of sampled colors</param>
		/// <returns name="int[][]">The color range</returns>
		IEnumerable<IEnumerable<int>^>^ LegendRGB(
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> minDomain,
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> maxDomain,
			[Autodesk::DesignScript::Runtime::DefaultArgument("10")] int count);

		/// <summary>
		/// Outputs the value range of the data.
		/// </summary>
		/// <param name="minDomain">An optional minimum domain</param>
		/// <param name="maxDomain">An optional maximum domain</param>
		/// <param name="count">The number of sampled values</param>
		/// <returns name="double[]">The sampled values</returns>
		IEnumerable<double>^ LegendValues(
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> minDomain,
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> maxDomain,
			[Autodesk::DesignScript::Runtime::DefaultArgument("10")] int count);

		/// <summary>
		/// Returns the names of the data.
		/// </summary>
		property IEnumerable<String^>^ Names
		{
			IEnumerable<String^>^ get();
		}

		/// <summary>
		/// Returns the values of the data.
		/// </summary>
		property IEnumerable<double>^ Values
		{
			IEnumerable<double>^ get();
		}

		/// <summary>
		/// Returns the domain of the data, represented by the minimum and maximum values.
		/// </summary>
		property IEnumerable<double>^ Domain
		{
			IEnumerable<double>^ get();
		}

		/// <summary>
		/// Returns the colors associated to the values, with an optional input domain.
		/// </summary>
		/// <param name="minDomain">The</param>
		/// <param name="maxDomain"></param>
		/// <returns name="int[][]"></returns>
		IEnumerable<IEnumerable<int>^>^ RGB(
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> minDomain, 
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Nullable<double> maxDomain);

	public protected:
		/// <summary>
		/// 
		/// </summary>
		SimulationResult(Dictionary<String^, Dictionary<String^, Object^>^>^ data);
		~SimulationResult();

		IEnumerable<double>^ LegendRatios(
			Nullable<double> minDomain,
			Nullable<double> maxDomain,
			int count,
			double% finalMinDomain,
			double& finalMaxDomain);

	protected:
		Dictionary<String^, Dictionary<String^, Object^>^>^ m_data;
	};
}