#include <Model.h>

namespace TopologicEnergy
{
	Model ^ Model::ByOsmPathOswPath(String ^ osmFilePath, String ^ oswFilePath, OpenStudio::Model^ osModel, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces,
		String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits)
	{
		return gcnew Model(osmFilePath, oswFilePath, osModel, pBuildingCells, osSpaces, EPReportName, EPReportForString, EPTableName, EPColumnName, EPUnits);
	}

	Model::Model(String ^ osmFilePath, String ^ oswFilePath, OpenStudio::Model^ osModel, List<Topologic::Cell^>^ pBuildingCells, List<OpenStudio::Space^>^ osSpaces,
		String^ EPReportName, String^ EPReportForString, String^ EPTableName, String^ EPColumnName, String^ EPUnits)
		: m_osmFilePath(osmFilePath)
		, m_oswFilePath(oswFilePath)
		, m_osModel(osModel)
		, m_buildingCells(pBuildingCells)
		, m_osSpaces(osSpaces)
		, m_EPReportName(EPReportName)
		, m_EPReportForString(EPReportForString)
		, m_EPTableName(EPTableName)
		, m_EPColumnName(EPColumnName)
		, m_EPUnits(EPUnits)
	{

	}
}