#include "AperturePath.h"

#include <TopExp_Explorer.hxx>

namespace TopologicCore
{
	bool DoesShape1ContainShape2(const TopoDS_Shape& rkShape1, const TopoDS_Shape& rkShape2)
	{
		// An empty shape means a void.
		if (rkShape1.IsNull() || rkShape2.IsNull())
		{
			if (rkShape1.IsNull() && rkShape2.IsNull()) // If both are null, return true.
			{
				return true;
			}

			return false; // If only one is null, return false.
		}

		// If both are not null, do downward navigation.
		TopTools_MapOfShape occtMembers;
		Topology::DownwardNavigation(rkShape1, rkShape2.ShapeType(), occtMembers);

		for (TopTools_MapIteratorOfMapOfShape occtMemberIterator(occtMembers); 
			occtMemberIterator.More(); 
			occtMemberIterator.Next())
		{
			const TopoDS_Shape& occtCurrent = occtMemberIterator.Value();
			// If this shape is the same as shape2, return true
			if (occtCurrent.IsSame(rkShape2))
			{
				return true;
			}
		}

		// Shape 2 is not found, return false
		return false;
	}

	bool AperturePath::operator==(const AperturePath& rkAnotherPath) const
	{
		// Check if the shapes in the other path CONTAIN the shapes in this path
		return DoesShape1ContainShape2(rkAnotherPath.GetTopology1(), GetTopology1()) &&
			   DoesShape1ContainShape2(rkAnotherPath.GetTopology2(), GetTopology2());
	}
}