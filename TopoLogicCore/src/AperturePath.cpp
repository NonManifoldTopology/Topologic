#include "AperturePath.h"

#include <TopExp_Explorer.hxx>

namespace TopoLogicCore
{
	bool DoesShape1ContainShape2(const TopoDS_Shape& rkShape1, const TopoDS_Shape& rkShape2)
	{
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(rkShape1, rkShape2.ShapeType()); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (occtCurrent.IsSame(rkShape2))
			{
				return true;
			}
		}

		return false;
	}

	bool AperturePath::operator==(const AperturePath& rkAnotherPath) const
	{
		return DoesShape1ContainShape2(rkAnotherPath.GetTopology1(), GetTopology1()) &&
			   DoesShape1ContainShape2(rkAnotherPath.GetTopology2(), GetTopology2());
	}
}