#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace Topologic;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicEnergy
{
	enum FaceType
	{
		FACE_WALL,
		FACE_FLOOR,
		FACE_ROOFCEILING
	};

	ref class Model;
	ref class EnergySimulation;

	public ref class EnergyModel
	{
	public:

		/// <summary>
		/// Create a TopologicEnergy model from a Topologic shape.
		/// </summary>
		/// <param name="building"></param>
		/// <param name="shadingSurfaces"></param>
		/// <param name="floorLevels"></param>
		/// <param name="buildingName"></param>
		/// <param name="buildingType"></param>
		/// <param name="defaultSpaceType"></param>
		/// <param name="glazingRatio"></param>
		/// <param name="coolingTemp"></param>
		/// <param name="heatingTemp"></param>
		/// <param name="weatherFilePath">Path to a .epw file</param>
		/// <param name="designDayFilePath">Path to a .ddy file</param>
		/// <param name="openStudioTemplatePath">Path to a template .osm file</param>
		////// <param name="openStudioOutputPath">Path to an output .osm file. Timestamp will be added to the path.</param>
		/// <returns name="energyModel"></returns>
		static EnergyModel^ ByCellComplex(
			CellComplex^ building,
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] Cluster^ shadingSurfaces,
			List<double>^ floorLevels,
			[Autodesk::DesignScript::Runtime::DefaultArgument("Building")] String^ buildingName,
			[Autodesk::DesignScript::Runtime::DefaultArgument("Commercial")] String^ buildingType,
			[Autodesk::DesignScript::Runtime::DefaultArgument("ASHRAE 189.1-2009 ClimateZone 4-8 MediumOffice")] String^ defaultSpaceType,
			[Autodesk::DesignScript::Runtime::DefaultArgument("-1")] double glazingRatio,
			double coolingTemp,
			double heatingTemp,
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\GBR_London.Gatwick.037760_IWEC.epw")] String^ weatherFilePath,
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\GBR_London.Gatwick.037760_IWEC.ddy")] String^ designDayFilePath,
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\MinimalTemplate120.osm")] String^ openStudioTemplatePath
			);

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="energyModel"></param>
		/// <param name="openStudioExePath"></param>
		/// <param name="openStudioOutputDirectory"></param>
		/// <param name="run"></param>
		/// <returns name="EnergySimulation"></returns>
		static EnergySimulation^ Simulate(EnergyModel^ energyModel, String^ openStudioExePath, String ^ openStudioOutputDirectory, bool run);*/
		
		static void Export(EnergyModel^ energyModel, String^ openStudioOutputDirectory);

	public protected:
		static DSCore::Color^ GetColor(double ratio, int alpha);
		static double DoubleValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static String^ StringValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static int IntValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static void Export(EnergyModel^ energyModel, String^ openStudioOutputDirectory, String^% oswPath);

		property OpenStudio::Model^ OsModel {
			OpenStudio::Model^ get() { return m_osModel; }
		}

		property List<Topologic::Cell^>^ BuildingCells {
			List<Topologic::Cell^>^ get() { return m_buildingCells; }
		}

		property List<OpenStudio::Space^>^ OsSpaces {
			List<OpenStudio::Space^>^ get() { return m_osSpaces; }
		}

		property String^ BuildingName {
			String^ get();
		}


	private:
		EnergyModel(OpenStudio::Model^ osModel, OpenStudio::Building^ osBuilding, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces);
		~EnergyModel() {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="osModel"></param>
		/// <param name="idfPathName"></param>
		/// <returns name="bool"></returns>
		static bool CreateIdfFile(OpenStudio::Model^ osModel, String^ idfPathName);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="osModel"></param>
		/// <param name="osmPathName"></param>
		/// <returns name="bool"></returns>
		static bool SaveModel(OpenStudio::Model^ osModel, String^ osmPathName);

		static OpenStudio::Space^ AddSpace(
			int spaceNumber,
			Cell^ cell,
			CellComplex^ cellComplex,
			OpenStudio::Model^ osModel,
			Autodesk::DesignScript::Geometry::Vector^ upVector,
			double buildingHeight,
			List<double>^ floorLevels,
			double glazingRatio,
			double heatingTemp,
			double coolingTemp
		);

		static void AddShadingSurfaces(Cell^ buildingCell, OpenStudio::Model^ osModel);

		static void AddShadingSurfaces(Face^ buildingFace, OpenStudio::Model^ osModel, OpenStudio::ShadingSurfaceGroup^ osShadingGroup, int faceIndex);

		static OpenStudio::Surface^ AddSurface(
			int surfaceNumber,
			Face^ buildingFace,
			Cell^ buildingSpace,
			CellComplex^ cellComplex,
			OpenStudio::Point3dVector^ osFacePoints,
			OpenStudio::Space^ osSpace,
			OpenStudio::Model^ osModel,
			Autodesk::DesignScript::Geometry::Vector^ upVector,
			double glazingRatio);

		static OpenStudio::Model^ GetModelFromTemplate(String^ osmTemplatePath, String^ epwWeatherPath, String^ ddyPath);

		static List<Vertex^>^ ScaleFaceVertices(Face^ buildingFace, double scaleFactor);

		static Vertex^ GetFaceCentre(Face^ buildingFace);

		static OpenStudio::Point3dVector^ GetFacePoints(Face^ buildingFace);

		static bool IsUnderground(Face^ buildingFace);

		static FaceType CalculateFaceType(Face^ buildingFace, OpenStudio::Point3dVector^% facePoints, Cell^ buildingSpace, Autodesk::DesignScript::Geometry::Vector^ upVector);

		static int AdjacentCellCount(Face^ buildingFace);

		static int StoryNumber(
			Cell^ buildingCell,
			double buildingHeight,
			List<double>^ floorLevels
		);

		static OpenStudio::ThermalZone^ CreateThermalZone(OpenStudio::Model^ model, OpenStudio::Space^ space, double ceilingHeight, double heatingTemp, double coolingTemp);

		static OpenStudio::BuildingStory^ AddBuildingStory(OpenStudio::Model^ model, int floorNumber);

		static OpenStudio::SubSurface^ CreateSubSurface(List<Topologic::Vertex^>^ vertices, OpenStudio::Model^ osModel);

		static OpenStudio::Building^ ComputeBuilding(
			OpenStudio::Model^ osModel,
			String^ buildingName,
			String^ buildingType,
			double buildingHeight,
			int numFloors,
			String^ spaceType);


		static OpenStudio::DefaultScheduleSet^ getDefaultScheduleSet(OpenStudio::Model^ model);

		static OpenStudio::DefaultConstructionSet^ getDefaultConstructionSet(OpenStudio::Model^ model);

		static List<OpenStudio::BuildingStory^>^ CreateBuildingStories(OpenStudio::Model^ osModel, int numFloors);

		static OpenStudio::SqlFile^ CreateSqlFile(OpenStudio::Model^ osModel, String^ sqlFilePath);

		/*static List<Modifiers::GeometryColor^>^ AnalyzeSqlFile(OpenStudio::SqlFile^ osSqlFile, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ spaces, List<Cell^>^ buildingCells,
			String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits);*/

		static List<OpenStudio::BuildingStory^>^ buildingStories;
		static OpenStudio::DefaultConstructionSet^ defaultConstructionSet;
		static OpenStudio::DefaultScheduleSet^ defaultScheduleSet;

		static int numOfApertures;
		static int numOfAppliedApertures;

		List<Topologic::Cell^>^ m_buildingCells;
		List<OpenStudio::Space^>^ m_osSpaces;
		OpenStudio::Model^ m_osModel;
		OpenStudio::Building^ m_osBuilding;
	};
}