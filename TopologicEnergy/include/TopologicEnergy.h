#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace Topologic;

namespace TopologicEnergy
{
	enum FaceType
	{
		FACE_WALL,
		FACE_FLOOR,
		FACE_ROOFCEILING
	};

	ref class Model;
	ref class SimulationResult;

	public ref class TopologicEnergy
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
		/// <param name="openStudioOutputPath">Path to an output .osm file. Timestamp will be added to the path.</param>
		/// <returns></returns>
		static Model^ CreateEnergyModel(
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
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\MinimalTemplate120.osm")] String^ openStudioTemplatePath,
			[Autodesk::DesignScript::Runtime::DefaultArgument(".\\TopologicEnergy-files\\output.osm")] String^ openStudioOutputPath
			);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="model"></param>
		/// <param name="openStudioExePath"></param>
		/// <returns></returns>
		//static List<Modifiers::GeometryColor^>^ PerformEnergyAnalysis(Model^ model, String^ openStudioExePath);
		static SimulationResult^ PerformEnergyAnalysis(Model^ model, String^ openStudioExePath);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="face"></param>
		/// <param name="apertureDesign"></param>
		/// <param name="numEdgeSamples"></param>
		/// <returns name="Face"></returns>
		static Face^ ApplyAperture(Face^ face, Face^ apertureDesign, int numEdgeSamples);

	public protected:
		static DSCore::Color^ GetColor(double ratio);
		static double DoubleValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static String^ StringValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);

		static int IntValueFromQuery(OpenStudio::SqlFile^ sqlFile, String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPRowName, String^ EPUnits);


	private:
		TopologicEnergy() {}

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

		static int AdjacentCellCount(Face^ buildingFace, CellComplex^ cellComplex);

		static int StoryNumber(
			Cell^ buildingCell,
			double buildingHeight,
			List<double>^ floorLevels
		);

		static OpenStudio::ThermalZone^ CreateThermalZone(OpenStudio::Model^ model, OpenStudio::Space^ space, double ceilingHeight, double heatingTemp, double coolingTemp);

		static OpenStudio::BuildingStory^ AddBuildingStory(OpenStudio::Model^ model, int floorNumber);

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

		static List<Modifiers::GeometryColor^>^ AnalyzeSqlFile(OpenStudio::SqlFile^ osSqlFile, OpenStudio::Model^ osModel, List<OpenStudio::Space^>^ spaces, List<Cell^>^ buildingCells,
			String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits);
			//double minValue, double maxValue);

		static List<OpenStudio::BuildingStory^>^ buildingStories;
		static OpenStudio::DefaultConstructionSet^ defaultConstructionSet;
		static OpenStudio::DefaultScheduleSet^ defaultScheduleSet;

		static int numOfApertures;
		static int numOfAppliedApertures;
		static int subsurfaceCounter;
	};
}