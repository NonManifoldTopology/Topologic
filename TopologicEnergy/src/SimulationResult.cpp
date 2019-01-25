#include "SimulationResult.h"
#include "EnergySimulation.h"
#include "EnergyModel.h"

using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Linq;

namespace TopologicEnergy
{

	SimulationResult^ SimulationResult::ByEnergySimulation(EnergySimulation^ energySimulation, String ^ EPReportName, String ^ EPReportForString, String ^ EPTableName, String ^ EPColumnName, String ^ EPUnits)
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

			// Map the outputVariable to a ratio between 0 and 1
			/*double ratio = (outputVariable - minValue) / (maxValue - minValue);
			DSCore::Color^ color = EnergyModel::GetColor(ratio);*/

			Dictionary<String^, Object^>^ attributes = gcnew Dictionary<String^, Object^>();
			attributes->Add("Value", outputVariable);
			/*attributes->Add("r", (long long int)color->Red);
			attributes->Add("g", (long long int)color->Green);
			attributes->Add("b", (long long int)color->Blue);
			delete color;*/
			data->Add(spaceName, attributes);
		}

		return gcnew SimulationResult(data);
	}

	List<Modifiers::GeometryColor^>^ SimulationResult::Display(EnergySimulation ^ energySimulation, List<DSCore::Color^>^ colors)
	{
		if (energySimulation->Topology->Count != colors->Count)
		{
			throw gcnew Exception("The number of colors does not match the number of cells.");
		}

		List<Modifiers::GeometryColor^>^ dynamoGeometryColors = gcnew List<Modifiers::GeometryColor^>();
		List<Modifiers::GeometryColor^>^ dynamoApertures = gcnew List<Modifiers::GeometryColor^>();
		for(int i = 0; i < colors->Count; ++i)
		{
			Cell^ cell = energySimulation->Topology[i];
			DSCore::Color^ color = colors[i];
			//if (!dictionary->ContainsKey("Name"))
			//{
			//	throw gcnew Exception("A cell does not have a name.");
			//}
			//Object^ name = dictionary["Name"];

			//String^ strName = nullptr;
			//try
			//{
			//	strName = safe_cast<String^>(name);
			//}
			//catch (...)
			//{
			//	throw gcnew Exception("A cell's name is not a string.");
			//}

			//DSCore::Color^ color = nullptr;// DSCore::Color::ByARGB(alpha, (int)r, (int)g, (int)b);
			//bool hasKey = data->ContainsKey(strName);
			//if (hasKey)
			//{
			//	Dictionary<String^, Object^>^ attributes = data[strName];
			//	long long int r = (long long int)attributes["r"];
			//	long long int g = (long long int)attributes["g"];
			//	long long int b = (long long int)attributes["b"];
			//	color = DSCore::Color::ByARGB(alpha, (int)r, (int)g, (int)b);
			//}
			//else
			//{
			//	color = DSCore::Color::ByARGB(alpha, 128, 128, 128);
			//}

			{
				bool hasGeometry = false;
				System::Object^ cellGeometry = cell->Geometry_;
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
				System::Object^ contentGeometry = subcontent->Geometry_;
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

	List<List<Object^>^>^ SimulationResult::Legend(Nullable<double> minDomain, Nullable<double> maxDomain)
	{
		/// Value, color --> Normalized value, color
		List<double>^ values = Values;

		double minValue = (double)Enumerable::Min(values);
		double maxValue = (double)Enumerable::Max(values);
		double deltaValue = maxValue - minValue;

		List<List<Object^>^>^ output = gcnew List<List<Object^>^>();

		List<DSCore::Color^>^ colors = ARGB(minDomain, maxDomain, 50);
		List<Object^>^ colorsAsObjects = gcnew List<Object^>();
		for each (DSCore::Color^ color in colors)
		{
			colorsAsObjects->Add(color);
		}
		output->Add(colorsAsObjects);
		if (deltaValue > 0.00001)
		{
			List<Object^>^ normalizedValues = gcnew List<Object^>();
			for each(Object^ value in values)
			{
				double normalizedValue = ((double)value - minValue) / deltaValue;
				normalizedValues->Add(normalizedValue);
			}
			output->Add(normalizedValues);
		}
		else
		{
			List<Object^>^ valuesAsObjects = gcnew List<Object^>();
			for each(Object^ value in values)
			{
				valuesAsObjects->Add(value);
			}
			output->Add(valuesAsObjects);
		}
		return output;
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

	List<DSCore::Color^>^ SimulationResult::ARGB(Nullable<double> minDomain, Nullable<double> maxDomain, int alpha)
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

		List<DSCore::Color^>^ colorList = gcnew List<DSCore::Color^>();
		List<double>^ values = Values;
		for each(double value in values)
		{
			double ratio = (value - finalMinDomain) / deltaFinalDomain;
			DSCore::Color^ color = EnergyModel::GetColor(ratio, alpha);
			colorList->Add(color);
		}

		return colorList;
	}

	//List<Modifiers::GeometryColor^>^ SimulationResult::Display(EnergyModel ^ energyModel, Dictionary<String^, Dictionary<String^, Object^>^>^ data, int alpha)
	//{
	//	List<Modifiers::GeometryColor^>^ dynamoGeometryColors = gcnew List<Modifiers::GeometryColor^>();
	//	List<Modifiers::GeometryColor^>^ dynamoApertures = gcnew List<Modifiers::GeometryColor^>();
	//	for each(Cell^ cell in energyModel->BuildingCells)
	//	{
	//		Dictionary<String^, Object^>^ dictionary = cell->Dictionary;
	//		if (!dictionary->ContainsKey("Name"))
	//		{
	//			throw gcnew Exception("A cell does not have a name.");
	//		}
	//		Object^ name = dictionary["Name"];

	//		String^ strName = nullptr;
	//		try
	//		{
	//			strName = safe_cast<String^>(name);
	//		}
	//		catch (...)
	//		{
	//			throw gcnew Exception("A cell's name is not a string.");
	//		}

	//		DSCore::Color^ color = nullptr;// DSCore::Color::ByARGB(alpha, (int)r, (int)g, (int)b);
	//		bool hasKey = data->ContainsKey(strName);
	//		if (hasKey)
	//		{
	//			Dictionary<String^, Object^>^ attributes = data[strName];
	//			long long int r = (long long int)attributes["r"];
	//			long long int g = (long long int)attributes["g"];
	//			long long int b = (long long int)attributes["b"];
	//			color = DSCore::Color::ByARGB(alpha, (int)r, (int)g, (int)b);
	//		}
	//		else
	//		{
	//			color = DSCore::Color::ByARGB(alpha, 128, 128, 128);
	//		}

	//		{
	//			bool hasGeometry = false;
	//			System::Object^ cellGeometry = cell->Geometry_;
	//			Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(cellGeometry);
	//			if (dynamoGeometry != nullptr)
	//			{
	//				Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, color);
	//				dynamoGeometryColors->Add(dynamoGeometryColor);
	//				hasGeometry = true;
	//			}

	//			if (!hasGeometry)
	//			{
	//				// 2. Try a list of Dynamo geometries
	//				List<Object^>^ listOfObjects = dynamic_cast<List<Object^>^>(cellGeometry);
	//				if (listOfObjects != nullptr)
	//				{
	//					for each(Object^ object in listOfObjects)
	//					{
	//						Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(object);
	//						if (dynamoGeometry != nullptr)
	//						{
	//							Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoGeometry, color);
	//							delete object;
	//							dynamoGeometryColors->Add(dynamoGeometryColor);
	//						}
	//					}
	//				}
	//			}
	//		}

	//		DSCore::Color^ contentColor = DSCore::Color::ByARGB(255, 128, 128, 128);
	//		List<Topologic::Topology^>^ subcontents = cell->SubContents;
	//		for each(Topologic::Topology^ subcontent in subcontents)
	//		{
	//			bool hasContentGeometry = false;
	//			System::Object^ contentGeometry = subcontent->Geometry_;
	//			Autodesk::DesignScript::Geometry::Geometry^ dynamoContentGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(contentGeometry);
	//			if (dynamoContentGeometry != nullptr)
	//			{
	//				Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoContentGeometry, contentColor);
	//				dynamoApertures->Add(dynamoGeometryColor);
	//				hasContentGeometry = true;
	//			}

	//			if (!hasContentGeometry)
	//			{
	//				// 2. Try a list of Dynamo geometries
	//				List<Object^>^ listOfObjects = dynamic_cast<List<Object^>^>(contentGeometry);
	//				if (listOfObjects != nullptr)
	//				{
	//					for each(Object^ object in listOfObjects)
	//					{
	//						Autodesk::DesignScript::Geometry::Geometry^ dynamoGeometry = dynamic_cast<Autodesk::DesignScript::Geometry::Geometry^>(object);
	//						if (dynamoContentGeometry != nullptr)
	//						{
	//							Modifiers::GeometryColor^ dynamoGeometryColor = Modifiers::GeometryColor::ByGeometryColor(dynamoContentGeometry, contentColor);
	//							delete object;
	//							dynamoApertures->Add(dynamoGeometryColor);
	//						}
	//					}
	//				}
	//			}
	//		}

	//	}
	//	dynamoGeometryColors->AddRange(dynamoApertures);
	//	return dynamoGeometryColors;
	//}

	SimulationResult::SimulationResult(Dictionary<String^, Dictionary<String^, Object^>^>^ data)
		: m_data(data)
	{

	}

	SimulationResult::~SimulationResult()
	{

	}
}