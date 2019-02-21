#include "Utilities.h"

namespace TopologicCore {
	TopAbs_ShapeEnum OcctTypeFromTopologicType(const TopologyType kTopologyType)
	{
		switch (kTopologyType)
		{
		case TOPOLOGY_VERTEX: return TopAbs_VERTEX;
		case TOPOLOGY_EDGE: return TopAbs_EDGE;
		case TOPOLOGY_WIRE: return TopAbs_WIRE;
		case TOPOLOGY_FACE: return TopAbs_FACE;
		case TOPOLOGY_SHELL: return TopAbs_SHELL;
		case TOPOLOGY_CELL: return TopAbs_SOLID;
		case TOPOLOGY_CELLCOMPLEX: return TopAbs_COMPSOLID;
		case TOPOLOGY_CLUSTER: return TopAbs_COMPOUND;
		default: throw std::exception("No OCCT counterpart is found from this topology.");
		}
	}
}