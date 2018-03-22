#pragma once

#include "Utilities.h"
#include "Topology.h"

#include <list>
#include <memory>

namespace TopoLogicCore
{
	class AperturePath
	{
	public:
		AperturePath(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2)
			: m_kOcctShape1(rkOcctShape1)
			, m_kOcctShape2(rkOcctShape2)
		{

		}

		~AperturePath()
		{

		}

		bool operator==(const AperturePath& rkAnotherPath) const;

		const TopoDS_Shape& GetTopology1() const { return m_kOcctShape1; }
		const TopoDS_Shape& GetTopology2() const { return m_kOcctShape2; }

	protected:
		const TopoDS_Shape m_kOcctShape1;
		const TopoDS_Shape m_kOcctShape2;
	};

}