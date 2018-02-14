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
#include <BOPAlgo_Splitter.hxx>
#include <BOPCol_ListOfShape.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Splitter.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_MapOfShape.hxx>
#include <BRepAlgo_Image.hxx>

#include <BRepBuilderAPI_MakeSolid.hxx>
#include <GEOMAlgo_Splitter.h>

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

	TopoDS_CompSolid Topology::MakeCompSolid(const TopoDS_Shape& rkOcctShape)
	{
		TopoDS_CompSolid occtCompSolid;
		BRep_Builder occtBuilder;
		occtBuilder.MakeCompSolid(occtCompSolid);

		TopExp_Explorer occtExplorer;
		TopTools_MapOfShape occtCells;
		for (occtExplorer.Init(rkOcctShape, TopAbs_SOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtCells.Contains(occtCurrent))
			{
				occtCells.Add(occtCurrent);
				try {
					occtBuilder.Add(occtCompSolid, occtCurrent);
				}
				catch (TopoDS_FrozenShape&)
				{
					throw std::exception("The cell is not free and cannot be modified.");
				}
				catch (TopoDS_UnCompatibleShapes&)
				{
					throw std::exception("The cell and face are not compatible.");
				}
			}
		}

		return occtCompSolid;
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
		return GetOcctShape()->Locked();
	}

	void Topology::Locked(const bool kLocked)
	{
		// TODO: Or locked?
		GetOcctShape()->Locked(kLocked);
	}

	Topology* Topology::Difference(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools, const bool kOutputCellComplex)
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
		if (kOutputCellComplex)
		{
			return ByOcctShape(MakeCompSolid(rkOcctCutShape));
		}

		return ByOcctShape(rkOcctCutShape);
	}

	Topology* Topology::Impose(
		const std::list<Topology*>& rkTopologyArguments, 
		const std::list<Topology*>& rkTopologyTools, 
		const bool kOutputCellComplex)
	{
		BOPCol_ListOfShape occtShapeArguments;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyArguments.begin();
			kTopologyIterator != rkTopologyArguments.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;

			// If cell complex, add the cells
			CellComplex* pCellComplex = dynamic_cast<CellComplex*>(pTopology);
			if (pCellComplex != nullptr)
			{
				std::list<Cell*> cells;
				pCellComplex->Cells(cells);
				for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
					kCellIterator != cells.end();
					kCellIterator++)
				{
					occtShapeArguments.Append(*(*kCellIterator)->GetOcctShape());
				}
			}else
			{
				occtShapeArguments.Append(*pTopology->GetOcctShape());
			}
		}

		BOPAlgo_Splitter occtSplitter;
		occtSplitter.SetArguments(occtShapeArguments);
		BOPCol_ListOfShape occtShapeTools;
		for (std::list<Topology*>::const_iterator kTopologyIterator = rkTopologyTools.begin();
			kTopologyIterator != rkTopologyTools.end();
			kTopologyIterator++)
		{
			Topology* pTopology = *kTopologyIterator;
			CellComplex* pCellComplex = dynamic_cast<CellComplex*>(pTopology);
			if (pCellComplex != nullptr)
			{
				std::list<Cell*> cells;
				pCellComplex->Cells(cells);
				for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
					kCellIterator != cells.end();
					kCellIterator++)
				{
					occtShapeTools.Append(*(*kCellIterator)->GetOcctShape());
				}
			}else
			{
				occtShapeTools.Append(*pTopology->GetOcctShape());
			}
		}

		occtSplitter.SetTools(occtShapeTools);
		// Split the arguments and tools
		occtSplitter.Perform();

		const TopoDS_Shape& rkOcctSplitShape = occtSplitter.Shape();
		const BOPCol_DataMapOfShapeListOfShape& rkImages = occtSplitter.Images();

		// Add only shapes that are NOT in the tools.
		BOPCol_ListOfShape occtMergeArguments;
		for (BOPCol_ListOfShape::const_iterator kArgumentIterator = occtShapeArguments.begin();
			kArgumentIterator != occtShapeArguments.end();
			kArgumentIterator++)
		{
			if (!occtMergeArguments.Contains(*kArgumentIterator))
			{
				const BOPCol_ListOfShape& rkArgumentImages = rkImages.Find(*kArgumentIterator);

				for (BOPCol_ListOfShape::const_iterator kArgumentImageIterator = rkArgumentImages.begin();
					kArgumentImageIterator != rkArgumentImages.end();
					kArgumentImageIterator++)
				{
					const TopoDS_Shape& rkImage = *kArgumentImageIterator;

					// Check the individual faces, later add them to create a shell
					bool hasElement = false;
					TopoDS_Builder occtBuilder;
					TopoDS_Shell occtImageShell;
					occtBuilder.MakeShell(occtImageShell);

					TopTools_MapOfShape occtArgumentImageFaces;
					TopExp_Explorer occtArgumentExplorer;
					for (occtArgumentExplorer.Init(rkImage, TopAbs_FACE); occtArgumentExplorer.More(); occtArgumentExplorer.Next())
					{
						const TopoDS_Shape& rkOcctCurrent = occtArgumentExplorer.Current();
						if (!occtArgumentImageFaces.Contains(rkOcctCurrent))
						{
							occtArgumentImageFaces.Add(rkOcctCurrent);
						}
					}

					for (TopTools_MapOfShape::const_iterator kOcctArgumentImageFaceIterator = occtArgumentImageFaces.cbegin();
						kOcctArgumentImageFaceIterator != occtArgumentImageFaces.cend();
						kOcctArgumentImageFaceIterator++)
					{
						const TopoDS_Shape& rkArgumentImageFace = *kOcctArgumentImageFaceIterator;

						bool isInTools = false;
						for (BOPCol_ListOfShape::const_iterator kToolIterator = occtShapeTools.begin();
							kToolIterator != occtShapeTools.end() && !isInTools;
							kToolIterator++)
						{
							const TopoDS_Shape& rkTool = *kToolIterator;

							const BOPCol_ListOfShape& rkToolImages = rkImages.Find(rkTool);

							// Is this image face part of any of this tool's faces?
							// If not, add to create a shell
							for (BOPCol_ListOfShape::const_iterator kToolImageIterator = rkToolImages.begin();
								kToolImageIterator != rkToolImages.end() && !isInTools;
								kToolImageIterator++)
							{
								const TopoDS_Shape& rkToolImage = *kToolImageIterator;
								TopTools_MapOfShape occtToolImageFaces;
								TopExp_Explorer occtToolExplorer;
								for (occtToolExplorer.Init(rkToolImage, TopAbs_FACE); occtToolExplorer.More(); occtToolExplorer.Next())
								{
									const TopoDS_Shape& rkOcctCurrent = occtToolExplorer.Current();
									if (!occtToolImageFaces.Contains(rkOcctCurrent))
									{
										occtToolImageFaces.Add(rkOcctCurrent);
									}
								}

								for (TopTools_MapOfShape::const_iterator kOcctToolImageFaceIterator = occtToolImageFaces.cbegin();
									kOcctToolImageFaceIterator != occtToolImageFaces.cend() && !isInTools;
									kOcctToolImageFaceIterator++)
								{
									if (kOcctToolImageFaceIterator->IsSame(rkArgumentImageFace))
									{
										isInTools = true;
									}
								}
							}
						}

						if (!isInTools)
						{
							occtMergeArguments.Append(*kArgumentImageIterator);

							occtBuilder.Add(occtImageShell, rkArgumentImageFace);
							hasElement = true;
						}
					}

					if (hasElement)
					{
						occtMergeArguments.Append(occtImageShell);
					}
				}
			}
		}

		// Merge the tools again
		occtMergeArguments.Append(occtShapeTools);

		if (occtMergeArguments.Size() == 1)
		{
			if (kOutputCellComplex)
			{
				return ByOcctShape(MakeCompSolid(*occtMergeArguments.begin()));
			}

			return ByOcctShape(*occtMergeArguments.begin());
		}

		BOPAlgo_Builder occtAlgoBuilder;
		occtAlgoBuilder.SetArguments(occtMergeArguments);
		occtAlgoBuilder.Perform();

		if (occtAlgoBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			occtAlgoBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		const TopoDS_Shape& rkOcctMergeShape = occtAlgoBuilder.Shape();

		if (kOutputCellComplex)
		{
			return ByOcctShape(MakeCompSolid(rkOcctMergeShape));
		}

		return ByOcctShape(rkOcctMergeShape);
	}

	Topology* Topology::Imprint(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools, const bool kOutputCellComplex)
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

	Topology* Topology::Intersection(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools, const bool kOutputCellComplex)
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
		if (kOutputCellComplex)
		{
			return ByOcctShape(MakeCompSolid(rkOcctCommonShape));
		}

		return ByOcctShape(rkOcctCommonShape);
	}

	Topology* Topology::Merge(const std::list<Topology*>& rkTopologyArguments, const bool kOutputCellComplex)
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

		if (kOutputCellComplex)
		{
			return ByOcctShape(MakeCompSolid(rkOcctMergeShape));
		}

		return ByOcctShape(rkOcctMergeShape);
	}

	Topology* Topology::Slice(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools, const bool kOutputCellComplex)
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

		if (kOutputCellComplex)
		{
			return ByOcctShape(MakeCompSolid(rkOcctSplitShape));
		}

		return ByOcctShape(rkOcctSplitShape);
	}

	Topology* Topology::Union(const std::list<Topology*>& rkTopologyArguments, const std::list<Topology*>& rkTopologyTools, const bool kOutputCellComplex)
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
		if (kOutputCellComplex)
		{
			return ByOcctShape(MakeCompSolid(rkFuseShape));
		}

		return ByOcctShape(rkFuseShape);
	}
}