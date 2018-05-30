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

	public ref class EnergyUtility
	{
	public:
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
		/// <param name="osmTemplatePath"></param>
		/// <param name="epwWeatherPath"></param>
		/// <param name="ddyPath"></param>
		/// <param name="osmOutputPath"></param>
		/// <param name="buildingCellComplex">The building as a cell complex</param>
		/// <param name="buildingName"></param>
		/// <param name="buildingType"></param>
		/// <param name="spaceType"></param>
		/// <param name="heatingTemp"></param>
		/// <param name="coolingTemp"></param>
		/// <param name="buildingHeight"></param>
		/// <param name="floorLevels"></param>
		/// <param name="numFloors"></param>
		/// <param name="glazingRatio"></param>
		/// <param name="contextBuildings"></param>
		/// <returns name="OpenStudio.Model"></returns>
		static OpenStudio::Model^ BuildOsModel(
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")] List<Cell^>^ contextBuildings,
			CellComplex^ buildingCellComplex,
			[Autodesk::DesignScript::Runtime::DefaultArgument("Commercial")] String^ buildingType,
			[Autodesk::DesignScript::Runtime::DefaultArgument("Default Building")] String^ buildingName,
			String^ spaceType,
			double buildingHeight,
			int numFloors,
			List<double>^ floorLevels,
			double glazingRatio,
			String^ epwWeatherPath,
			String^ ddyPath,
			String^ osmTemplatePath,
			String^ osmOutputPath,
			double coolingTemp,
			double heatingTemp);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="strOsmPath"></param>
		/// <param name="epwPathName"></param>
		/// <param name="oswPathName"></param>
		/// <param name="openStudioExePath"></param>
		static void PerformEnergyAnalysis(String^ strOsmPath, String^ epwPathName, String^ oswPathName, String^ openStudioExePath);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="osModel"></param>
		/// <param name="osmPathName"></param>
		/// <returns name="bool"></returns>
		static bool SaveModel(OpenStudio::Model^ osModel, String^ osmPathName);

		static OpenStudio::Model^ GetModelFromTemplate(String^ osmTemplatePath, String^ epwWeatherPath, String^ ddyPath);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="face"></param>
		/// <param name="apertureDesign"></param>
		/// <param name="numEdgeSamples"></param>
		/// <returns name="Face"></returns>
		static Face^ ApplyAperture(Face^ face, Face^ apertureDesign, int numEdgeSamples);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="face"></param>
		/// <param name="apertureDesign"></param>
		/// <param name="numEdgeSamples"></param>
		/// <returns name="Face"></returns>
		static Face^ ApplyApertureV2(Face^ face, Face^ apertureDesign, int numEdgeSamples);

	private:
		EnergyUtility() {}

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


		static List<OpenStudio::BuildingStory^>^ buildingStories;
		static OpenStudio::DefaultConstructionSet^ defaultConstructionSet;
		static OpenStudio::DefaultScheduleSet^ defaultScheduleSet;

		static int numOfApertures;
		static int numOfAppliedApertures;
		static int subsurfaceCounter;
	};
}