#pragma once

#include "Utilities.h"

#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>

#include <list>

namespace TopologicCore
{
	class Topology;

	class GlobalCluster
	{
	public:
		typedef std::shared_ptr<GlobalCluster> Ptr;

	public:
		TOPOLOGIC_API static GlobalCluster& GetInstance()
		{
			static GlobalCluster instance;
			return instance;
		}

		TOPOLOGIC_API GlobalCluster();
		TOPOLOGIC_API ~GlobalCluster();

		TOPOLOGIC_API void AddTopology(const std::shared_ptr<Topology>& rkTopology);

		TOPOLOGIC_API void AddTopology(const TopoDS_Shape& rkOcctShape);

		void RemoveTopology(const std::shared_ptr<Topology>& rkTopology);

		void RemoveTopology(const TopoDS_Shape& rkOcctShape);

		void Clear();

		const TopoDS_Compound& GetOcctCompound() const;

		TopoDS_Compound& GetOcctCompound();

		TOPOLOGIC_API void SubTopologies(std::list<std::shared_ptr<Topology>>& rSubTopologies) const;

	protected:
		TopoDS_Compound m_occtCompound;
		TopoDS_Builder m_occtBuilder;
	};
}