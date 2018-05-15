#include <LoadCluster.h>
#include <Load.h>
#include <Context.h>

namespace TopologicStructure
{
	LoadCluster^ LoadCluster::ByLoads(List<Load^>^ loads)
	{
		return gcnew LoadCluster(loads, nullptr, loads->Count, 1);
	}

	LoadCluster^ LoadCluster::ByEdge(Topologic::Edge^ edge, Autodesk::DesignScript::Geometry::Vector^ vector, int rows, double uScale, double uShift)
	{
		if (rows < 1)
		{
			throw gcnew Exception("rows must be larger than zero."); // correct?
		}
		if (uScale <= 0.0)
		{
			throw gcnew Exception("uScale must be larger than zero."); // correct?
		}

		List<Load^>^ pLoads = gcnew List<Load^>();
		double minU = uShift;
		double maxU = minU + uScale;
		if (rows == 1)
		{
			double halfU = (minU + maxU) / 2.0;
			Load^ pLoad = Load::ByEdge(edge, halfU, vector);
			pLoads->Add(pLoad);
		}else
		{
			double du = (maxU - minU) / (double)(rows - 1);
			for (int i = 0; i < rows; ++i)
			{
				double u = minU + i * du;
				Load^ pLoad = Load::ByEdge(edge, u, vector);
				pLoads->Add(pLoad);
			}
		}

		//What to fill u?
		TopologicStructure::Context^ pContext = TopologicStructure::Context::Create(edge, 0.0, 0.0, 0.0, uScale, 0.0, 0.0, uShift, 0.0, 0.0);

		return gcnew LoadCluster(pLoads, pContext, rows, 1);
	}

	LoadCluster^ LoadCluster::ByFace(Topologic::Face ^ face, Autodesk::DesignScript::Geometry::Vector ^ vector, int rows, int columns, double uScale, double vScale, double uShift, double vShift)
	{
		if (rows < 1)
		{
			throw gcnew Exception("rows must be larger than zero."); // correct?
		}
		if (columns < 1)
		{
			throw gcnew Exception("columns must be larger than zero."); // correct?
		}
		if (uScale <= 0.0)
		{
			throw gcnew Exception("uScale must be larger than zero."); // correct?
		}
		if (vScale <= 0.0)
		{
			throw gcnew Exception("vScale must be larger than zero."); // correct?
		}


		List<Load^>^ pLoads = gcnew List<Load^>();
		double minU = uShift;
		double maxU = minU + uScale;
		double minV = vShift;
		double maxV = minV + vScale;

		List<double>^ uList = gcnew List<double>();
		if (rows == 1)
		{
			double halfU = (minU + maxU) / 2.0;
			uList->Add(halfU);
		}
		else
		{
			double du = (maxU - minU) / (double)(rows - 1);
			for (int i = 0; i < rows; ++i)
			{
				double u = minU + i * du;
				uList->Add(u);
			}
		}

		List<double>^ vList = gcnew List<double>();
		if (columns == 1)
		{
			double halfV = (minV + maxV) / 2.0;
			vList->Add(halfV);
		}
		else
		{
			double dv = (maxV - minV) / (double)(columns - 1);
			for (int i = 0; i < columns; ++i)
			{
				double v = minV + i * dv;
				vList->Add(v);
			}
		}

		for each(double u in uList)
		{
			for each (double v in vList)
			{
				Load^ pLoad = Load::ByFace(face, u, v, vector);
				pLoads->Add(pLoad);
			}
		}
		//What to fill u and v?
		TopologicStructure::Context^ pContext = TopologicStructure::Context::Create(face, 0.0, 0.0, 0.0, uScale, vScale, 0.0, uShift, vShift, 0.0);

		return gcnew LoadCluster(pLoads, pContext, rows, columns);
	}

	LoadCluster::LoadCluster(List<Load^>^ loads, TopologicStructure::Context^ context, int rows, int columns)
		: Topologic::Cluster()
		, m_pLoads(loads)
		, m_pContext(context)
		, m_rows(rows > 0? rows : throw gcnew Exception("rows must be larger than zero."))
		, m_columns(columns > 0 ? columns : throw gcnew Exception("columns must be larger than zero."))
	{

	}

	LoadCluster::~LoadCluster()
	{

	}


	Object^ LoadCluster::Geometry::get()
	{
		List<Object^>^ pLoadClusterGeometries = gcnew List<Object^>();
		for each(Load^ pLoad in m_pLoads)
		{
			pLoadClusterGeometries->Add(pLoad->Geometry);
		}
		return pLoadClusterGeometries;
	}
	/*List<Load^>^ LoadCluster::Loads::get()
	{ 
		List<Topology^>^ pChildTopologies = ImmediateMembers();
		List<Load^>^ pChildLoads = gcnew List<Load^>();
		for each(Topology^ pChildTopology in pChildTopologies)
		{
			Load^ pChildLoad = dynamic_cast<Load^>(pChildTopology);
			if (pChildLoad != nullptr)
			{
				pChildLoads->Add(pChildLoad);
			}
		}
		return pChildLoads;
	}*/
}