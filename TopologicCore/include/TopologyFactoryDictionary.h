#pragma once
//
//#include "Utilities.h"
//
#include <TopAbs_ShapeEnum.hxx>
//
//#include <list>
#include <map>
#include <memory>
#include <string>

namespace TopologicCore
{
	class TopologyFactory;

	class TopologyFactoryDictionary
	{
	public:
		typedef std::shared_ptr<TopologyFactoryDictionary> Ptr;

	public:
		static TopologyFactoryDictionary& GetInstance()
		{
			static TopologyFactoryDictionary instance;
			return instance;
		}

		void Add(const std::string& rkGuid, const std::shared_ptr<TopologyFactory>& kpTopologyFactory);

		bool Find(const std::string& rkGuid, std::shared_ptr<TopologyFactory>& rTopologyFactory);

		static std::shared_ptr<TopologyFactory> GetDefaultFactory(const TopAbs_ShapeEnum kOcctType);

	protected:
		std::map<std::string, std::shared_ptr<TopologyFactory>> m_topologyFactoryMap;
	};
}