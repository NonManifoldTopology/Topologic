#include "SimulationResult.h"
#include "EnergySimulation.h"
#include "EnergyModel.h"

using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Linq;

namespace TopologicEnergy
{

	Object^ SimulationResult::ByEnergySimulation(EnergySimulation^ energySimulation, String ^ EPReportName, String ^ EPReportForString, String ^ EPTableName, String ^ EPColumnName, String ^ EPUnits)
	{
		OpenStudio::OptionalDouble^ totalSE = energySimulation->OsSqlFile->totalSiteEnergy();

		double totalSEGJ = totalSE->__float__();
		double totalSEkwh = totalSEGJ * 277.8;
		OpenStudio::OptionalDouble^ totalEU = energySimulation->OsSqlFile->electricityTotalEndUses();
		double totalEUGJ = totalEU->__float__();
		double totalEUkwh = totalEUGJ * 277.8;
		OpenStudio::OptionalString^ spaceNameTemp = energySimulation->OsSpaces[0]->name();
		
		String^ spaceName = spaceNameTemp->get();
		String^ EPRowName = spaceName + "_THERMAL_ZONE";

		//If min and max values are not specified then calculate them from the data
		double maxValue = 0.0;
		try {
			maxValue = EnergyModel::DoubleValueFromQuery(energySimulation->OsSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
		}
		catch (...)
		{
			throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
		}
		double minValue = maxValue;
		for each(OpenStudio::Space^ space in energySimulation->OsSpaces)
		{
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double aValue = 10000.0;
			try {
				aValue = EnergyModel::DoubleValueFromQuery(energySimulation->OsSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
			}
			if (aValue > maxValue)
			{
				maxValue = aValue;
			}
			if (aValue < minValue)
			{
				minValue = aValue;
			}
		}

		if (minValue <= maxValue && maxValue - minValue < 0.0001)
		{
			minValue = maxValue - (maxValue * 0.0001);
		}


		// Create a map: space name -> cell
		Dictionary<String^, Dictionary<String^, Object^>^>^ data = gcnew Dictionary<String^, Dictionary<String^, Object^>^>();

		//STEP 2: Find the cell that matches the space and set its colour.
		int i = 0;
		for each(OpenStudio::Space^ space in energySimulation->OsSpaces)
		{
			++i;
			OpenStudio::OptionalString^ osSpaceName = space->name();
			String^ spaceName = osSpaceName->get();
			EPRowName = spaceName + "_THERMAL_ZONE";
			double outputVariable = 0.0;
			try {
				outputVariable = EnergyModel::DoubleValueFromQuery(energySimulation->OsSqlFile, EPReportName, EPReportForString, EPTableName, EPColumnName, EPRowName, EPUnits);
			}
			catch (...)
			{
				throw gcnew Exception("Fails to execute SQL query. There is an incorrect argument.");
			}

			Dictionary<String^, Object^>^ attributes = gcnew Dictionary<String^, Object^>();
			attributes->Add("Value", outputVariable);
			attributes->Add("Unit", EPUnits);
			data->Add(spaceName, attributes);
		}

		return gcnew SimulationResult(data);
	}

	List<Modifiers::GeometryColor^>^ SimulationResult::Display(EnergyModel^ energyModel, List<DSCore::Color^>^ colors)
	{
		if (energyModel->Topology->Count != colors->Count)
		{
			throw gcnew Exception("The number of colors does not match the number of cells.");
		}

		List<Modifiers::GeometryColor^>^ dynamoGeometryColors = gcnew List<Modifiers::GeometryColor^>();
		List<Modifiers::GeometryColor^>^ dynamoApertures = gcnew List<Modifiers::GeometryColor^>();
		for(int i = 0; i < colors->Count; ++i)
		{
			Cell^ cell = energyModel->Topology[i];
			DSCore::Color^ color = colors[i];

			{
				bool hasGeometry = false;
				System::Object^ cellGeometry = cell->BasicGeometry;
				Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(cellGeometry);
				if (dynamoGeometry != nullptr)
				{
					Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, color);
					dynamoGeometryColors->Add(dynamoGeometryColor);
					hasGeometry = true;
				}

				if (!hasGeometry)
				{
					// 2. Try a list of Dynamo geometries
					List<Object^>^ listOfObjects = dynamic_cast<List<Object^>^>(cellGeometry);
					if (listOfObjects != nullptr)
					{
						for each(Object^ object in listOfObjects)
						{
							Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(object);
							if (dynamoGeometry != nullptr)
							{
								Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, color);
								delete object;
								dynamoGeometryColors->Add(dynamoGeometryColor);
							}
						}
					}
				}
			}

			DSCore::Color^ contentColor = DSCore::Color::ByARGB(255, 128, 128, 128);
			List<Topologic::Topology^>^ subcontents = cell->SubContents;
			for each(Topologic::Topology^ subcontent in subcontents)
			{
				bool hasContentGeometry = false;
				System::Object^ contentGeometry = subcontent->BasicGeometry;
				Autodesk::DesignScript::Geometry::Geometry^ dynamoContentGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(contentGeometry);
				if (dynamoContentGeometry != nullptr)
				{
					Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoContentGeometry, contentColor);
					dynamoApertures->Add(dynamoGeometryColor);
					hasContentGeometry = true;
				}

				if (!hasContentGeometry)
				{
					// 2. Try a list of Dynamo geometries
					List<Object^>^ listOfObjects = dynamic_cast<List<Object^>^>(contentGeometry);
					if (listOfObjects != nullptr)
					{
						for each(Object^ object in listOfObjects)
						{
							Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(object);
							if (dynamoContentGeometry != nullptr)
							{
								Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoContentGeometry, contentColor);
								delete object;
								dynamoApertures->Add(dynamoGeometryColor);
							}
						}
					}
				}
			}
		}

		dynamoGeometryColors->AddRange(dynamoApertures);
		return dynamoGeometryColors;
	}

	List<List<int>^>^ SimulationResult::LegendRGB(Nullable<double> minDomain, Nullable<double> maxDomain, int count)
	{
		List<List<int>^>^ colors = gcnew List<List<int>^>();
		double finalMinDomain = 0.0;
		double finalMaxDomain = 0.0;
		List<double>^ ratios = LegendRatios(minDomain, maxDomain, count, finalMinDomain, finalMaxDomain);
		for each(double ratio in ratios)
		{
			List<int>^ color = EnergyModel::GetColor(ratio);
			colors->Add(color);
		}

		return colors;

		//if (count < 2)
		//{
		//	throw gcnew Exception("The number of steps must be more than 2.");
		//}

		//List<double>^ domain = Domain;
		//double finalMinDomain = Enumerable::Min(domain);
		//double finalMaxDomain = Enumerable::Max(domain);

		//if (minDomain.HasValue)
		//{
		//	finalMinDomain = minDomain.Value;
		//}
		//if (maxDomain.HasValue)
		//{
		//	finalMaxDomain = maxDomain.Value;
		//}

		//double deltaFinalDomain = finalMaxDomain - finalMinDomain;
		//if (deltaFinalDomain < 0.00001)
		//{
		//	throw gcnew Exception("The domain is too small. Please provide a larger interval.");
		//}

		//List<List<int>^>^ colors = gcnew List<List<int>^>();
		//List<Object^>^ valuesAsObjects = gcnew List<Object^>();
		//double countMinusOne = (double)count;
		//for (int i = 0; i < count; ++i)
		//{
		//	double doubleI = (double)i;	// 0..10
		//	double ratio = doubleI / countMinusOne; // 0..1
		//	double value = finalMinDomain + deltaFinalDomain * ratio;
		//	valuesAsObjects->Add(value);
		//	List<int>^ color = EnergyModel::GetColor(ratio);
		//	colors->Add(color);
		//}

		///*List<List<Object^>^>^ output = gcnew List<List<Object^>^>();
		//output->Add(colorsAsObjects);
		//output->Add(valuesAsObjects);*/

		//return colors;
	}

	List<double>^ SimulationResult::LegendValues(Nullable<double> minDomain, Nullable<double> maxDomain, int count)
	{
		List<double>^ values = gcnew List<double>();
		double finalMinDomain = 0.0;
		double finalMaxDomain = 0.0;
		List<double>^ ratios = LegendRatios(minDomain, maxDomain, count, finalMinDomain, finalMaxDomain);
		double deltaFinalDomain = finalMaxDomain - finalMinDomain; // Any problem with this should have been caught in LegendRatios
		for each(double ratio in ratios)
		{
			double value = finalMinDomain + deltaFinalDomain * ratio;
			values->Add(value);
		}

		return values;

		//if (count < 2)
		//{
		//	throw gcnew Exception("The number of steps must be more than 2.");
		//}

		//List<double>^ domain = Domain;
		//double finalMinDomain = Enumerable::Min(domain);
		//double finalMaxDomain = Enumerable::Max(domain);

		//if (minDomain.HasValue)
		//{
		//	finalMinDomain = minDomain.Value;
		//}
		//if (maxDomain.HasValue)
		//{
		//	finalMaxDomain = maxDomain.Value;
		//}

		//double deltaFinalDomain = finalMaxDomain - finalMinDomain;
		//if (deltaFinalDomain < 0.00001)
		//{
		//	throw gcnew Exception("The domain is too small. Please provide a larger interval.");
		//}

		//List<List<int>^>^ colors = gcnew List<List<int>^>();
		//List<double>^ values = gcnew List<double>();
		//double countMinusOne = (double)count;
		//for (int i = 0; i < count; ++i)
		//{
		//	double doubleI = (double)i;	// 0..10
		//	double ratio = doubleI / countMinusOne; // 0..1
		//	double value = finalMinDomain + deltaFinalDomain * ratio;
		//	values->Add(value);
		//	List<int>^ color = EnergyModel::GetColor(ratio);
		//	colors->Add(color);
		//}

		//return values;
	}

	List<String^>^ SimulationResult::Names::get()
	{
		return gcnew List<String^>(m_data->Keys);
	}

	List<double>^ SimulationResult::Values::get()
	{
		List<double>^ values = gcnew List<double>();
		for each(KeyValuePair<String^, Dictionary<String^, Object^>^> pair in m_data)
		{
			Dictionary<String^, Object^>^ dictionary = pair.Value;
			if (dictionary->ContainsKey("Value"))
			{
				try {
					values->Add((double)dictionary["Value"]);
				}
				catch (...)
				{
					values->Add(-1);
				}
			}
			else
			{
				values->Add(-1);
			}
		}
		return values;
	}

	List<double>^ SimulationResult::Domain::get()
	{
		List<double>^ values = Values;

		if (values->Count == 0)
		{
			return nullptr;
		}

		List<double>^ domain = gcnew List<double>();
		domain->Add(Enumerable::Min(values));
		domain->Add(Enumerable::Max(values));
		return domain;
	}

	List<List<int>^>^ SimulationResult::RGB(Nullable<double> minDomain, Nullable<double> maxDomain)
	{
		List<double>^ domain = Domain;
		double finalMinDomain = Enumerable::Min(domain);
		double finalMaxDomain = Enumerable::Max(domain);

		if(minDomain.HasValue)
		{
			finalMinDomain = minDomain.Value;
		}
		if (maxDomain.HasValue)
		{
			finalMaxDomain = maxDomain.Value;
		}

		double deltaFinalDomain = finalMaxDomain - finalMinDomain;
		if (deltaFinalDomain < 0.00001)
		{
			throw gcnew Exception("The domain is too small. Please increase it.");
		}

		//List<DSCore::Color^>^ colorList = gcnew List<DSCore::Color^>();
		List<List<int>^>^ colorList = gcnew List<List<int>^>();
		List<double>^ values = Values;
		for each(double value in values)
		{
			double ratio = (value - finalMinDomain) / deltaFinalDomain;
			List<int>^ rgb = gcnew List<int>();
			rgb = EnergyModel::GetColor(ratio);
			//DSCore::Color^ color = EnergyModel::GetColor(ratio, alpha);
			colorList->Add(rgb);
		}

		return colorList;
	}

	SimulationResult::SimulationResult(Dictionary<String^, Dictionary<String^, Object^>^>^ data)
		: m_data(data)
	{

	}

	SimulationResult::~SimulationResult()
	{

	}

	List<double>^ SimulationResult::LegendRatios(Nullable<double> minDomain, Nullable<double> maxDomain, int count, double% finalMinDomain, double& finalMaxDomain)
	{
		if (count < 2)
		{
			throw gcnew Exception("The number of steps must be more than 2.");
		}

		List<double>^ domain = Domain;
		finalMinDomain = Enumerable::Min(domain);
		finalMaxDomain = Enumerable::Max(domain);

		if (minDomain.HasValue)
		{
			finalMinDomain = minDomain.Value;
		}
		if (maxDomain.HasValue)
		{
			finalMaxDomain = maxDomain.Value;
		}

		double deltaFinalDomain = finalMaxDomain - finalMinDomain;
		if (deltaFinalDomain < 0.00001)
		{
			throw gcnew Exception("The domain is too small. Please provide a larger interval.");
		}

		List<double>^ ratios = gcnew List<double>();
		double countMinusOne = (double)(count-1);
		for (int i = 0; i < count; ++i)
		{
			double doubleI = (double)i;	// 0..10
			double ratio = doubleI / countMinusOne; // 0..1
			ratios->Add(ratio);
		}
		return ratios;
	}
}