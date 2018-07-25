#pragma once

#include <TopoDS_Shape.hxx>

#include <list>
#include <map>
#include <memory>

namespace TopologicCore
{
	class Topology;

	struct OcctShapeComparator {
		bool operator()(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2) const {
			int upperLimit = std::numeric_limits<int>::max();
			return rkOcctShape1.HashCode(upperLimit) < rkOcctShape2.HashCode(upperLimit);
		}
	};

	class ContentManager
	{
	public:
		typedef std::shared_ptr<ContentManager> Ptr;

	public:
		static ContentManager& GetInstance()
		{
			static ContentManager instance;
			return instance;
		}

		void Add(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Topology>& kpTopology);

		void Remove(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Topology>& kpTopology);

		bool Find(const TopoDS_Shape& rkOcctShape, std::list<std::shared_ptr<Topology>>& rContents);

		void ClearOne(const TopoDS_Shape& rkOcctShape);

		void ClearAll();

	protected:
		std::map<TopoDS_Shape, std::list<std::shared_ptr<Topology>>, OcctShapeComparator> m_occtShapeToContentsMap;
	};
}