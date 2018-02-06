#include <Topology.h>
#include <Cluster.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Shell.h>
#include <Face.h>
#include <Wire.h>
#include <Edge.h>
#include <Vertex.h>

#include <BOPAlgo_Builder.hxx>
#include <BOPCol_ListOfShape.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Splitter.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>

namespace TopoLogicCore
{
	Topology::Topology(const int kDimensionality)
		: m_dimensionality(kDimensionality)
		, m_isLocked(false)
		, m_modelingPriority(5)
	{

	}

	Topology* Topology::ByOcctShape(const TopoDS_Shape& rkOcctShape)
	{
		TopAbs_ShapeEnum occtShapeType = rkOcctShape.ShapeType();
		switch (occtShapeType)
		{
		case TopAbs_COMPOUND: return new Cluster(new TopoDS_Compound(TopoDS::Compound(rkOcctShape)));
		case TopAbs_COMPSOLID: return new CellComplex(new TopoDS_CompSolid(TopoDS::CompSolid(rkOcctShape)));
		case TopAbs_SOLID: return new Cell(new TopoDS_Solid(TopoDS::Solid(rkOcctShape)));
		case TopAbs_SHELL: return new Shell(new TopoDS_Shell(TopoDS::Shell(rkOcctShape)));
		case TopAbs_FACE: return new Face(new TopoDS_Face(TopoDS::Face(rkOcctShape)));
		case TopAbs_WIRE: return new Wire(new TopoDS_Wire(TopoDS::Wire(rkOcctShape)));
		case TopAbs_EDGE: return new Edge(new TopoDS_Edge(TopoDS::Edge(rkOcctShape)));
		case TopAbs_VERTEX: return new Vertex(new TopoDS_Vertex(TopoDS::Vertex(rkOcctShape)));
		default:
			throw std::exception("Topology::ByOcctShape: unknown topology.");
		}
	}

	Topology::~Topology()
	{

		for (AttributeMapIterator attributeMapIterator = m_attributeMap.begin();
			attributeMapIterator != m_attributeMap.end();
			attributeMapIterator++)
		{
			delete attributeMapIterator->second;
		}
		m_attributeMap.clear();

		for (std::list<Topology*>::iterator memberIterator = m_members.begin();
			memberIterator != m_members.end();
			memberIterator++)
		{
			delete *memberIterator;
		}
		m_members.clear();


		for (std::list<Topology*>::iterator memberOfIterator = m_membersOf.begin();
			memberOfIterator != m_membersOf.end();
			memberOfIterator++)
		{
			delete *memberOfIterator;
		}
		m_membersOf.clear();
	}

	bool Topology::SaveToBrep(const std::string & rkPath) const
	{
		return BRepTools::Write(*GetOcctShape(), rkPath.c_str());;
	}

	bool Topology::LoadFromBrep(const std::string & rkPath) const
	{
		TopoDS_Shape occtShape;
		BRep_Builder occtBRepBuilder;
		return BRepTools::Read(occtShape, rkPath.c_str(), occtBRepBuilder);
	}

	std::string Topology::Analyze(Topology const * const kpkTopology)
	{
		ShapeAnalysis_ShapeContents occtShapeAnalysis;
		occtShapeAnalysis.Perform(*kpkTopology->GetOcctShape());

		std::string shapeType;
		switch (kpkTopology->GetOcctShape()->ShapeType())
		{
		case TopAbs_COMPOUND: shapeType = "Cluster"; break;
		case TopAbs_COMPSOLID: shapeType = "CellComplex"; break;
		case TopAbs_SOLID: shapeType = "Cell"; break;
		case TopAbs_SHELL: shapeType = "Shell"; break;
		case TopAbs_FACE: shapeType = "Face"; break;
		case TopAbs_WIRE: shapeType = "Wire"; break;
		case TopAbs_EDGE: shapeType = "Edge"; break;
		case TopAbs_VERTEX: shapeType = "Vertex"; break;
		default: shapeType = "invalid shape"; break;
		}

		std::stringstream resultStream;
		resultStream <<
			"The shape is a " << shapeType << "." << std::endl <<
			"Number of cells = " << occtShapeAnalysis.NbSharedSolids() << std::endl <<
			"Number of shells = " << occtShapeAnalysis.NbSharedShells() << std::endl <<
			"Number of faces = " << occtShapeAnalysis.NbSharedFaces() << std::endl <<
			"Number of wires = " << occtShapeAnalysis.NbSharedWires() << std::endl <<
			"Number of edges = " << occtShapeAnalysis.NbSharedEdges() << std::endl <<
			"Number of vertices = " << occtShapeAnalysis.NbSharedVertices() << std::endl;

		return resultStream.str();
	}

	bool Topology::Locked() const
	{
		// TODO: Or locked?
		return GetOcctShape()->Free();
	}

	void Topology::Locked(const bool kLocked)
	{
		// TODO: Or locked?
		GetOcctShape()->Free(kLocked);
	}

	Topology* Topology::Difference(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools)
	{
		BOPCol_ListOfShape occtShapeArguments;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyArguments.begin();
			kTopologyIterator != rkTopologyArguments.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeArguments.Append(*pTopology->GetOcctShape());
		}

		BOPCol_ListOfShape occtShapeTools;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyTools.begin();
			kTopologyIterator != rkTopologyTools.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeTools.Append(*pTopology->GetOcctShape());
		}

		// use BRepAlgoAPI_Common 
		BRepAlgoAPI_Cut occtCut;
		occtCut.SetArguments(occtShapeArguments);
		occtCut.SetTools(occtShapeTools);
		occtCut.Build();

		if (occtCut.HasErrors())
		{
			std::ostringstream errorStream;
			occtCut.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		const TopoDS_Shape& rkOcctCutShape = occtCut.Shape();
		return ByOcctShape(rkOcctCutShape);
	}

	Topology* Topology::Impose(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools)
	{
		// Impose: C = (A diff B) merge B
		Topology* pDifferenceTopology = Difference(rkTopologyArguments, rkTopologyArguments);

		std::list<Topology*> mergeTopologyList;
		mergeTopologyList.push_back(pDifferenceTopology);
		mergeTopologyList.insert(mergeTopologyList.end(), rkTopologyTools.begin(), rkTopologyTools.end());
		Topology* pMergeTopology = Merge(mergeTopologyList);

		mergeTopologyList.clear();
		delete pDifferenceTopology;

		return pMergeTopology;
	}

	Topology* Topology::Imprint(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools)
	{
		// Imprint: C = (A diff B) merge (A intersection B)
		Topology* pDifferenceTopology = Difference(rkTopologyArguments, rkTopologyArguments);

		Topology* pIntersectionTopology = Intersection(rkTopologyArguments, rkTopologyArguments);

		std::list<Topology*> mergeTopologyList;
		mergeTopologyList.push_back(pDifferenceTopology);
		mergeTopologyList.push_back(pIntersectionTopology);
		Topology* pMergeTopology = Merge(mergeTopologyList);

		mergeTopologyList.clear();
		delete pDifferenceTopology;
		delete pIntersectionTopology;

		return pMergeTopology;
	}

	Topology* Topology::Intersection(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools)
	{
		BOPCol_ListOfShape occtShapeArguments;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyArguments.begin();
			kTopologyIterator != rkTopologyArguments.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeArguments.Append(*pTopology->GetOcctShape());
		}

		BOPCol_ListOfShape occtShapeTools;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyTools.begin();
			kTopologyIterator != rkTopologyTools.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeTools.Append(*pTopology->GetOcctShape());
		}

		// use BRepAlgoAPI_Common 
		BRepAlgoAPI_Common occtCommon;
		occtCommon.SetArguments(occtShapeArguments);
		occtCommon.SetTools(occtShapeTools);
		occtCommon.Build();

		if (occtCommon.HasErrors())
		{
			std::ostringstream errorStream;
			occtCommon.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		const TopoDS_Shape& rkOcctCommonShape = occtCommon.Shape();
		return ByOcctShape(rkOcctCommonShape);
	}

	Topology* Topology::Merge(const std::list<Topology*>& rkTopologyArguments)
	{
		BOPCol_ListOfShape occtShapeArguments;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyArguments.begin();
			kTopologyIterator != rkTopologyArguments.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeArguments.Append(*pTopology->GetOcctShape());
		}

		// BOPAlgo_Builder implements OCCT's general fuse algorithm
		BOPAlgo_Builder occtAlgoBuilder;
		occtAlgoBuilder.SetArguments(occtShapeArguments);
		occtAlgoBuilder.Perform();

		if (occtAlgoBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			occtAlgoBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		const TopoDS_Shape& rkOcctMergeShape= occtAlgoBuilder.Shape();
		return ByOcctShape(rkOcctMergeShape);
	}

	Topology* Topology::Slice(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools)
	{
		BOPCol_ListOfShape occtShapeArguments;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyArguments.begin();
			kTopologyIterator != rkTopologyArguments.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeArguments.Append(*pTopology->GetOcctShape());
		}

		BOPCol_ListOfShape occtShapeTools;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyTools.begin();
			kTopologyIterator != rkTopologyTools.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeTools.Append(*pTopology->GetOcctShape());
		}

		// use BOPAlgo_Splitter
		// TODO: Or BOPAlgo_MakerVolume? But only creates solids
		BRepAlgoAPI_Splitter occtSplitter;
		occtSplitter.SetArguments(occtShapeArguments);
		occtSplitter.SetTools(occtShapeTools);
		occtSplitter.Build();

		if (occtSplitter.HasErrors())
		{
			std::ostringstream errorStream;
			occtSplitter.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		const TopoDS_Shape& rkOcctSplitShape = occtSplitter.Shape();
		return ByOcctShape(rkOcctSplitShape);
	}

	Topology* Topology::Union(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools)
	{
		BOPCol_ListOfShape occtShapeArguments;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyArguments.begin();
			kTopologyIterator != rkTopologyArguments.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeArguments.Append(*pTopology->GetOcctShape());
		}

		BOPCol_ListOfShape occtShapeTools;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyTools.begin();
			kTopologyIterator != rkTopologyTools.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			occtShapeTools.Append(*pTopology->GetOcctShape());
		}

		// use BRepAlgoAPI_Fuse 
		BRepAlgoAPI_Fuse occtFuse;
		occtFuse.SetArguments(occtShapeArguments);
		occtFuse.SetTools(occtShapeTools);
		occtFuse.Build();

		if (occtFuse.HasErrors())
		{
			std::ostringstream errorStream;
			occtFuse.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		const TopoDS_Shape& rkFuseShape = occtFuse.Shape();
		return ByOcctShape(rkFuseShape);
	}
}