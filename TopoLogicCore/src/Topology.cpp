#include <Topology.h>
#include <Cluster.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Shell.h>
#include <Face.h>
#include <Wire.h>
#include <Edge.h>
#include <Vertex.h>

#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Face.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>

#include <BOPAlgo_Builder.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BOPCol_ListOfShape.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_MapOfShape.hxx>

#include <ShapeFix_Shape.hxx>

#include <array>
#include <algorithm>

namespace TopoLogicCore
{
	void AddOcctListShapeToAnotherList(const BOPCol_ListOfShape& rkAList, BOPCol_ListOfShape& rAnotherList)
	{
		for (BOPCol_ListOfShape::const_iterator kIterator = rkAList.begin();
			kIterator != rkAList.end();
			kIterator++)
		{
			rAnotherList.Append(*kIterator);
		}
	}

	Topology::Topology(const int kDimensionality)
		: m_dimensionality(kDimensionality)
	{

	}

	Topology* Topology::ByOcctShape(const TopoDS_Shape& rkOcctShape)
	{
		TopAbs_ShapeEnum occtShapeType = rkOcctShape.ShapeType();
		switch (occtShapeType)
		{
		case TopAbs_COMPOUND: return new Cluster(TopoDS::Compound(rkOcctShape));
		case TopAbs_COMPSOLID: return new CellComplex(TopoDS::CompSolid(rkOcctShape));
		case TopAbs_SOLID: return new Cell(TopoDS::Solid(rkOcctShape));
		case TopAbs_SHELL: return new Shell(TopoDS::Shell(rkOcctShape));
		case TopAbs_FACE: return new Face(TopoDS::Face(rkOcctShape));
		case TopAbs_WIRE: return new Wire(TopoDS::Wire(rkOcctShape));
		case TopAbs_EDGE: return new Edge(TopoDS::Edge(rkOcctShape));
		case TopAbs_VERTEX: return new Vertex(TopoDS::Vertex(rkOcctShape));
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
	}

	Topology* Topology::ByGeometry(Handle(Geom_Geometry) pGeometry)
	{
		return nullptr;
	}

	Topology* Topology::ByContext(Context const * const kpkContext)
	{
		return nullptr;
	}

	Topology* Topology::ByVertexIndex(const std::list<std::array<double, 3>>& rkVertexCoordinates, const std::list<std::list<int>>& rkVertexIndices)
	{
		return nullptr;
	}

	Topology* Topology::ByVertexIndex(const std::list<Vertex*>& rkVertices, const std::list<std::list<int>>& rkVertexIndices)
	{
		return nullptr;
	}

	Topology* Topology::AddContent(Topology * const kpTopology)
	{
		if(std::find(m_contents.begin(), m_contents.end(), kpTopology) != m_contents.end())
		{
			m_contents.push_back(kpTopology);
		}
		return this;
	}

	Topology* Topology::RemoveContent(Topology * const kpTopology)
	{
		m_contents.remove(kpTopology);
		return this;
	}

	Topology* Topology::AddContext(Context * const kpContext)
	{
		if (std::find(m_contexts.begin(), m_contexts.end(), kpContext) != m_contexts.end())
		{
			m_contexts.push_back(kpContext);
		}
		return this;
	}

	Topology* Topology::RemoveContext(Context * const kpContext)
	{
		m_contexts.remove(kpContext);
		return this;
	}

	void Topology::SharedTopologies(Topology * const kpTopology, std::list<Topology*>& rkSharedTopologies) const
	{
	}

	void Topology::PathsTo(Topology * const kpTopology, const int kMaxLevels, const int kMaxPaths, std::list<std::list<TopologicalQuery*>>& rkPaths) const
	{
	}

	bool Topology::SaveToBrep(const std::string & rkPath) const
	{
		return BRepTools::Write(*GetOcctShape(), rkPath.c_str());;
	}

	Topology* Topology::LoadFromBrep(const std::string & rkPath)
	{
		TopoDS_Shape occtShape;
		BRep_Builder occtBRepBuilder;
		bool returnValue = BRepTools::Read(occtShape, rkPath.c_str(), occtBRepBuilder);

		return Topology::ByOcctShape(occtShape);
	}

	std::string Topology::Analyze(const TopoDS_Shape& rkShape, const int kLevel)
	{
		BOPCol_ListOfShape occtImmediateMembers;
		ImmediateMembers(rkShape, occtImmediateMembers);

		std::array<std::string, 8> occtShapeNameSingular;
		occtShapeNameSingular[0] = "a cluster";
		occtShapeNameSingular[1] = "a cellComplex";
		occtShapeNameSingular[2] = "a cell";
		occtShapeNameSingular[3] = "a shell";
		occtShapeNameSingular[4] = "a face";
		occtShapeNameSingular[5] = "a wire";
		occtShapeNameSingular[6] = "an edge";
		occtShapeNameSingular[7] = "a vertex";

		std::array<std::string, 8> occtShapeNamePlural;
		occtShapeNamePlural[0] = "clusters";
		occtShapeNamePlural[1] = "cellComplexes";
		occtShapeNamePlural[2] = "cells";
		occtShapeNamePlural[3] = "shells";
		occtShapeNamePlural[4] = "faces";
		occtShapeNamePlural[5] = "wires";
		occtShapeNamePlural[6] = "edges";
		occtShapeNamePlural[7] = "vertices";

		TopAbs_ShapeEnum occtShapeType = rkShape.ShapeType();
		std::stringstream ssCurrentIndent;
		for (int i = 0; i < kLevel; ++i)
		{
			ssCurrentIndent << "  ";
		}
		std::string currentIndent = ssCurrentIndent.str();
		int numberOfSubentities[8] = { 0,0,0,0,0,0,0,0 };
		for (BOPCol_ListOfShape::const_iterator kMemberIterator = occtImmediateMembers.begin();
			kMemberIterator != occtImmediateMembers.end();
			kMemberIterator++)
		{
			const TopoDS_Shape& rkMemberTopology = *kMemberIterator;
			TopAbs_ShapeEnum occtShapeMemberType = rkMemberTopology.ShapeType();
			numberOfSubentities[occtShapeMemberType]++;
		}

		std::stringstream ssCurrentResult;

		// For the topmost level only, print the overall subentities result
		if (kLevel == 0)
		{
			ShapeAnalysis_ShapeContents occtShapeAnalysis;
			occtShapeAnalysis.Perform(rkShape);

			// No method is provided in ShapeAnalysis_ShapeContents to compute the number of CompSolids.
			// Do this manually.
			int numberCompSolids = 0;
			TopExp_Explorer occtExplorer;
			BOPCol_ListOfShape occtCompSolids;
			for (occtExplorer.Init(rkShape, TopAbs_COMPSOLID); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtExplorer.Current();
				if (!occtCompSolids.Contains(rkOcctCurrent))
				{
					occtCompSolids.Append(rkOcctCurrent);
					numberCompSolids++;
				}
			}
			ssCurrentResult <<
				"OVERALL ANALYSIS" << std::endl <<
				"================" << std::endl <<
				"The shape is " << occtShapeNameSingular[occtShapeType] << "." << std::endl <<
				"Number of cell complexes = " << numberCompSolids << std::endl <<
				"Number of cells = " << occtShapeAnalysis.NbSharedSolids() << std::endl <<
				"Number of shells = " << occtShapeAnalysis.NbSharedShells() << std::endl <<
				"Number of faces = " << occtShapeAnalysis.NbSharedFaces() << std::endl <<
				"Number of wires = " << occtShapeAnalysis.NbSharedWires() << std::endl <<
				"Number of edges = " << occtShapeAnalysis.NbSharedEdges() << std::endl <<
				"Number of vertices = " << occtShapeAnalysis.NbSharedVertices() << std::endl <<
				std::endl << std::endl <<
				"INDIVIDUAL ANALYSIS" << std::endl <<
				"================" << std::endl;
		}

		ssCurrentResult << currentIndent << "The shape is " << occtShapeNameSingular[occtShapeType] << "." << std::endl;

		for (int i = occtShapeType + 1; i < 8; ++i)
		{
			if(numberOfSubentities[i] > 0)
			{
				ssCurrentResult << currentIndent << "Number of " << occtShapeNamePlural[i] << " = " << numberOfSubentities[i] << std::endl;
			}
		}
		ssCurrentResult << currentIndent << "================" << std::endl;

		for (BOPCol_ListOfShape::const_iterator kMemberIterator = occtImmediateMembers.begin();
			kMemberIterator != occtImmediateMembers.end();
			kMemberIterator++)
		{
			const TopoDS_Shape& rkMemberTopology = *kMemberIterator;
			std::string strMemberAnalyze = Analyze(rkMemberTopology, kLevel + 1);
			ssCurrentResult << strMemberAnalyze;
		}

		return ssCurrentResult.str();
	}

	std::string Topology::Analyze()
	{
		return Analyze(*GetOcctShape(), 0);
	}

	void Topology::BooleanImages(
		Topology const * const kpkOtherTopology,
		std::list<Topology*>& kArgumentImagesInArguments, 
		std::list<Topology*>& kArgumentImagesInTools, 
		std::list<Topology*>& kToolsImagesInArguments, 
		std::list<Topology*>& kToolsImagesInTools)
	{
		BOPCol_ListOfShape occtArgumentImagesInArguments;
		BOPCol_ListOfShape occtArgumentImagesInTools;
		BOPCol_ListOfShape occtToolsImagesInArguments;
		BOPCol_ListOfShape occtToolsImagesInTools;
		BOPCol_ListOfShape occtExclusivelyArgumentImages;
		BOPCol_ListOfShape occtExclusivelyToolImages;
		BOPCol_ListOfShape occtSharedImages;
		BOPCol_ListOfShape occtExclusivelyArgumentImageFaces;
		BOPCol_ListOfShape occtExclusivelyToolImageFaces;
		BOPCol_ListOfShape occtSharedImageFaces;
;
		BOPAlgo_CellsBuilder occtCellsBuilder;
		BooleanImages(
			kpkOtherTopology,
			occtCellsBuilder,
			occtExclusivelyArgumentImages,
			occtExclusivelyToolImages,
			occtSharedImages,
			occtExclusivelyArgumentImageFaces,
			occtExclusivelyToolImageFaces,
			occtSharedImageFaces,
			occtArgumentImagesInArguments, 
			occtArgumentImagesInTools, 
			occtToolsImagesInArguments, 
			occtToolsImagesInTools);

		for (BOPCol_ListOfShape::const_iterator kImageIterator = occtArgumentImagesInArguments.begin();
			kImageIterator != occtArgumentImagesInArguments.end();
			kImageIterator++)
		{
			kArgumentImagesInArguments.push_back(Topology::ByOcctShape(*kImageIterator));
		}
		for (BOPCol_ListOfShape::const_iterator kImageIterator = occtArgumentImagesInTools.begin();
			kImageIterator != occtArgumentImagesInTools.end();
			kImageIterator++)
		{
			kArgumentImagesInTools.push_back(Topology::ByOcctShape(*kImageIterator));
		}
		for (BOPCol_ListOfShape::const_iterator kImageIterator = occtToolsImagesInArguments.begin();
			kImageIterator != occtToolsImagesInArguments.end();
			kImageIterator++)
		{
			kToolsImagesInArguments.push_back(Topology::ByOcctShape(*kImageIterator));
		}
		for (BOPCol_ListOfShape::const_iterator kImageIterator = occtToolsImagesInTools.begin();
			kImageIterator != occtToolsImagesInTools.end();
			kImageIterator++)
		{
			kToolsImagesInTools.push_back(Topology::ByOcctShape(*kImageIterator));
		}
	}

	void Topology::BooleanImages(
		Topology const * const kpkOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctExclusivelyArgumentImages,
		BOPCol_ListOfShape& rOcctExclusivelyToolImages,
		BOPCol_ListOfShape& rOcctSharedImages,
		BOPCol_ListOfShape& rOcctExclusivelyArgumentImageFaces,
		BOPCol_ListOfShape& rOcctExclusivelyToolImageFaces,
		BOPCol_ListOfShape& rOcctSharedImageFaces,
		BOPCol_ListOfShape& rOcctArgumentImagesInArguments, 
		BOPCol_ListOfShape& rOcctArgumentImagesInTools, 
		BOPCol_ListOfShape& rOcctToolsImagesInArguments, 
		BOPCol_ListOfShape& rOcctToolsImagesInTools)
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		if (GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex* pCellComplex = TopologicalQuery::Downcast<CellComplex>(this);
			std::list<Cell*> cells;
			pCellComplex->Cells(cells);
			for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
				kCellIterator != cells.end();
				kCellIterator++)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(*(*kCellIterator)->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsA.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(*GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsA.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		if (kpkOtherTopology->GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex const * const kpkCellComplex = TopologicalQuery::Downcast<CellComplex const>(kpkOtherTopology);
			std::list<Cell*> cells;
			kpkCellComplex->Cells(cells);
			for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
				kCellIterator != cells.end();
				kCellIterator++)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(*(*kCellIterator)->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsB.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(*kpkOtherTopology->GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsB.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		rOcctCellsBuilder.SetArguments(occtCellsBuildersArguments);

		// Split the arguments and tools
		rOcctCellsBuilder.Perform();

		if (rOcctCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			rOcctCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
		const TopoDS_Shape& rMergeResult = rOcctCellsBuilder.Shape();

		const BOPCol_DataMapOfShapeListOfShape& rkImages = rOcctCellsBuilder.Images();
		int size = rkImages.Size();

		const TopoDS_Shape& rkParts = rOcctCellsBuilder.GetAllParts();
		Topology* pTopologyParts = Topology::ByOcctShape(rkParts);
		std::string strParts = pTopologyParts->Analyze();

		// 3. Classify the images: exclusively from argument, exclusively from tools, or shared.
		for (BOPCol_ListOfShape::const_iterator kArgumentIterator = occtCellsBuildersOperandsA.begin();
			kArgumentIterator != occtCellsBuildersOperandsA.end();
			kArgumentIterator++)
		{
			const BOPCol_ListOfShape& rkArgumentImages = rkImages.Find(*kArgumentIterator);

			for (BOPCol_ListOfShape::const_iterator kArgumentImageIterator = rkArgumentImages.begin();
				kArgumentImageIterator != rkArgumentImages.end();
				kArgumentImageIterator++)
			{
				const TopoDS_Shape& rkArgumentImage = *kArgumentImageIterator;

				// Is this in tool too?
				bool isArgumentImageInTools = false;
				for (BOPCol_ListOfShape::const_iterator kToolIterator = occtCellsBuildersOperandsB.begin();
					kToolIterator != occtCellsBuildersOperandsB.end() && !isArgumentImageInTools;
					kToolIterator++)
				{
					const BOPCol_ListOfShape& rkToolImages = rkImages.Find(*kToolIterator);

					for (BOPCol_ListOfShape::const_iterator kToolImageIterator = rkToolImages.begin();
						kToolImageIterator != rkToolImages.end() && !isArgumentImageInTools;
						kToolImageIterator++)
					{
						if (kArgumentImageIterator->IsSame(*kToolImageIterator))
						{
							isArgumentImageInTools = true;
						}
					}
				}

				if (isArgumentImageInTools)
				{
					rOcctSharedImages.Append(rkArgumentImage);
				}
				else
				{
					rOcctExclusivelyArgumentImages.Append(rkArgumentImage);
				}
			}
		}

		for (BOPCol_ListOfShape::const_iterator kToolIterator = occtCellsBuildersOperandsB.begin();
			kToolIterator != occtCellsBuildersOperandsB.end();
			kToolIterator++)
		{
			const BOPCol_ListOfShape& rkToolImages = rkImages.Find(*kToolIterator);

			for (BOPCol_ListOfShape::const_iterator kToolImageIterator = rkToolImages.begin();
				kToolImageIterator != rkToolImages.end();
				kToolImageIterator++)
			{
				const TopoDS_Shape& rkToolImage = *kToolImageIterator;

				// Is this in tool too?
				bool isToolInArguments = false;
				for (BOPCol_ListOfShape::const_iterator kArgumentIterator = occtCellsBuildersOperandsA.begin();
					kArgumentIterator != occtCellsBuildersOperandsA.end() && !isToolInArguments;
					kArgumentIterator++)
				{
					const BOPCol_ListOfShape& rkArgumentImages = rkImages.Find(*kArgumentIterator);

					for (BOPCol_ListOfShape::const_iterator kArgumentImageIterator = rkArgumentImages.begin();
						kArgumentImageIterator != rkArgumentImages.end() && !isToolInArguments;
						kArgumentImageIterator++)
					{
						if (kArgumentImageIterator->IsSame(*kToolImageIterator))
						{
							isToolInArguments = true;
						}
					}
				}

				if (!isToolInArguments)
				{
					rOcctExclusivelyToolImages.Append(rkToolImage);
				}

				// The shared images have been taken care of; no need to check them.
			}
		}

		// 4. Get the faces

		for (BOPCol_ListOfShape::const_iterator kArgumentImageIterator = rOcctExclusivelyArgumentImages.begin();
			kArgumentImageIterator != rOcctExclusivelyArgumentImages.end();
			kArgumentImageIterator++)
		{
			TopExp_Explorer occtArgumentImageExplorer;
			for (occtArgumentImageExplorer.Init(*kArgumentImageIterator, TopAbs_FACE); occtArgumentImageExplorer.More(); occtArgumentImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtArgumentImageExplorer.Current();
				if (!rOcctExclusivelyArgumentImageFaces.Contains(rkOcctCurrent))
				{
					rOcctExclusivelyArgumentImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		for (BOPCol_ListOfShape::const_iterator kToolImageIterator = rOcctExclusivelyToolImages.begin();
			kToolImageIterator != rOcctExclusivelyToolImages.end();
			kToolImageIterator++)
		{
			TopExp_Explorer occtToolImageExplorer;
			for (occtToolImageExplorer.Init(*kToolImageIterator, TopAbs_FACE); occtToolImageExplorer.More(); occtToolImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtToolImageExplorer.Current();
				if (!rOcctExclusivelyToolImageFaces.Contains(rkOcctCurrent))
				{
					rOcctExclusivelyToolImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		for (BOPCol_ListOfShape::const_iterator kSharedImageIterator = rOcctSharedImages.begin();
			kSharedImageIterator != rOcctSharedImages.end();
			kSharedImageIterator++)
		{
			TopExp_Explorer occtSharedImageExplorer;
			for (occtSharedImageExplorer.Init(*kSharedImageIterator, TopAbs_FACE); occtSharedImageExplorer.More(); occtSharedImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtSharedImageExplorer.Current();
				if (!rOcctSharedImageFaces.Contains(rkOcctCurrent))
				{
					rOcctSharedImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		// 5. Identify the argument faces in a tool by doing IsSame Comparison
		for (BOPCol_ListOfShape::const_iterator kArgumentImageFaceIterator = rOcctExclusivelyArgumentImageFaces.cbegin();
			kArgumentImageFaceIterator != rOcctExclusivelyArgumentImageFaces.cend();
			kArgumentImageFaceIterator++)
		{
			bool isInSharedImage = false;
			for (BOPCol_ListOfShape::const_iterator kSharedImageFaceIterator = rOcctSharedImageFaces.cbegin();
				kSharedImageFaceIterator != rOcctSharedImageFaces.cend() && !isInSharedImage;
				kSharedImageFaceIterator++)
			{
				if (kArgumentImageFaceIterator->IsSame(*kSharedImageFaceIterator))
				{
					isInSharedImage = true;
					//rOcctSharedImageFaces.Remove(*kSharedImageFaceIterator);
				}
			}

			if (isInSharedImage)
			{
				// part of tools in the arguments
				rOcctToolsImagesInArguments.Append(*kArgumentImageFaceIterator);
			}
			else
			{
				rOcctArgumentImagesInArguments.Append(*kArgumentImageFaceIterator);
			}
		}

		// 6. Identify the tool faces in an image by doing IsSame Comparison
		for (BOPCol_ListOfShape::const_iterator kToolImageFaceIterator = rOcctExclusivelyToolImageFaces.cbegin();
			kToolImageFaceIterator != rOcctExclusivelyToolImageFaces.cend();
			kToolImageFaceIterator++)
		{
			bool isInSharedImage = false;
			for (BOPCol_ListOfShape::const_iterator kSharedImageFaceIterator = rOcctSharedImageFaces.cbegin();
				kSharedImageFaceIterator != rOcctSharedImageFaces.cend() && !isInSharedImage;
				kSharedImageFaceIterator++)
			{
				if (kToolImageFaceIterator->IsSame(*kSharedImageFaceIterator))
				{
					isInSharedImage = true;
					//rOcctSharedImageFaces.Remove(*kSharedImageFaceIterator);
				}
			}

			if (isInSharedImage)
			{
				// part of arguments in the tools
				rOcctArgumentImagesInTools.Append(*kToolImageFaceIterator);
			}
			else
			{
				rOcctToolsImagesInTools.Append(*kToolImageFaceIterator);
			}
		}

		// 7. Identify faces inside the shared images that are inside the arguments or tools.
		for (BOPCol_ListOfShape::const_iterator kSharedImageFaceIterator = rOcctSharedImageFaces.cbegin();
			kSharedImageFaceIterator != rOcctSharedImageFaces.cend();
			kSharedImageFaceIterator++)
		{
			bool isInTools = false;
			for (BOPCol_ListOfShape::const_iterator kToolImageFaceIterator = rOcctExclusivelyToolImageFaces.cbegin();
				kToolImageFaceIterator != rOcctExclusivelyToolImageFaces.cend() && !isInTools;
				kToolImageFaceIterator++)
			{
				if (kToolImageFaceIterator->IsSame(*kSharedImageFaceIterator))
				{
					isInTools = true;
					rOcctArgumentImagesInTools.Append(*kSharedImageFaceIterator);
				}
			}

			// If not in the arguments, then it is in the tools.
			if (!isInTools)
			{
				rOcctToolsImagesInArguments.Append(*kSharedImageFaceIterator);
			}
		}
	}

	void Topology::BooleanParts(
		Topology const * const kpkOtherTopology,
		std::list<Topology*>& rSpaceBetween_A_A_and_B_A,
		std::list<Topology*>& rSpaceBetween_B_A_and_A_B,
		std::list<Topology*>& rSpaceBetween_A_B_and_B_B)
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		if (GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex* pCellComplex = TopologicalQuery::Downcast<CellComplex>(this);
			std::list<Cell*> cells;
			pCellComplex->Cells(cells);
			for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
				kCellIterator != cells.end();
				kCellIterator++)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(*(*kCellIterator)->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsA.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(*GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsA.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		if (kpkOtherTopology->GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex const * const kpkCellComplex = TopologicalQuery::Downcast<CellComplex const>(kpkOtherTopology);
			std::list<Cell*> cells;
			kpkCellComplex->Cells(cells);
			for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
				kCellIterator != cells.end();
				kCellIterator++)
			{
				Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
				sfs->Init(*(*kCellIterator)->GetOcctShape());
				sfs->SetPrecision(Precision::Confusion());
				sfs->Perform();
				occtCellsBuildersOperandsB.Append(sfs->Shape());
				occtCellsBuildersArguments.Append(sfs->Shape());
			}
		}
		else
		{
			Handle(ShapeFix_Shape) sfs = new ShapeFix_Shape();
			sfs->Init(*kpkOtherTopology->GetOcctShape());
			sfs->SetPrecision(Precision::Confusion());
			sfs->Perform();
			occtCellsBuildersOperandsB.Append(sfs->Shape());
			occtCellsBuildersArguments.Append(sfs->Shape());
		}

		BOPAlgo_CellsBuilder occtCellsBuilder;
		occtCellsBuilder.SetArguments(occtCellsBuildersArguments);

		// Split the arguments and tools
		occtCellsBuilder.Perform();

		if (occtCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
		const TopoDS_Shape& rMergeResult = occtCellsBuilder.Shape();

		const BOPCol_DataMapOfShapeListOfShape& rkImages = occtCellsBuilder.Images();


		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		// A_A and B_A
		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsA.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsA.end();
			kOcctShapeIterator++)
		{
			occtCellsBuilder.RemoveAllFromResult();
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);

			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtListToAvoid.Append(*kOcctShapeIteratorB);
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			occtCellsBuilder.MakeContainers();
			const TopoDS_Shape& occtBooleanResult = occtCellsBuilder.Shape();
			Topology* pTopology = Topology::ByOcctShape(occtBooleanResult);
			rSpaceBetween_A_A_and_B_A.push_back(pTopology);
		}

		// B_A and A_B
		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsA.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsA.end();
			kOcctShapeIterator++)
		{
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtCellsBuilder.RemoveAllFromResult();
				occtListToTake.Clear();
				occtListToAvoid.Clear();
				occtListToTake.Append(*kOcctShapeIterator);
				occtListToTake.Append(*kOcctShapeIteratorB);
				occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
				occtCellsBuilder.MakeContainers();
				const TopoDS_Shape& occtBooleanResult = occtCellsBuilder.Shape();
				Topology* pTopology = Topology::ByOcctShape(occtBooleanResult);
				rSpaceBetween_B_A_and_A_B.push_back(pTopology);
			}
		}

		// A_B and B_B
		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
			kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
			kOcctShapeIteratorB++)
		{
			occtCellsBuilder.RemoveAllFromResult();
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIteratorB);

			for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsA.begin();
				kOcctShapeIterator != occtCellsBuildersOperandsA.end();
				kOcctShapeIterator++)
			{
				occtListToAvoid.Append(*kOcctShapeIterator);
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			occtCellsBuilder.MakeContainers();
			const TopoDS_Shape& occtBooleanResult = occtCellsBuilder.Shape();
			Topology* pTopology = Topology::ByOcctShape(occtBooleanResult);
			rSpaceBetween_A_B_and_B_B.push_back(pTopology);
		}
	}

	void Topology::BooleanOperation(
		Topology const * const kpkOtherTopology,
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsB)
	{
		// Buffer lists are currently provided for cell complexes to be processed per cells.
		AddBooleanOperands(kpkOtherTopology, rOcctCellsBuilder, rOcctCellsBuildersOperandsA, rOcctCellsBuildersOperandsB);

		// Split the arguments and tools
		rOcctCellsBuilder.Perform();

		if (rOcctCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			rOcctCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
	}

	Topology* Topology::GetBooleanResult(BOPAlgo_CellsBuilder& rOcctCellsBuilder)
	{
		rOcctCellsBuilder.MakeContainers();
		const TopoDS_Shape& occtBooleanResult = rOcctCellsBuilder.Shape();
		Topology* pTopology = Topology::ByOcctShape(occtBooleanResult);
		return pTopology;
	}

	Topology* Topology::Difference(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorA = occtCellsBuildersOperandsA.begin();
			kOcctShapeIteratorA != occtCellsBuildersOperandsA.end();
			kOcctShapeIteratorA++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIteratorA);
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtListToAvoid.Append(*kOcctShapeIteratorB);
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(occtCellsBuilder);
	}

	Topology* Topology::Impose(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		// Get part of A not in B
		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorA = occtCellsBuildersOperandsA.begin();
			kOcctShapeIteratorA != occtCellsBuildersOperandsA.end();
			kOcctShapeIteratorA++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIteratorA);
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtListToAvoid.Append(*kOcctShapeIteratorB);
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		// Add B
		int i = 1;
		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsB.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsB.end();
			kOcctShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, i++, true);
		}

		return GetBooleanResult(occtCellsBuilder);
	}

	Topology* Topology::Imprint(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;
		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorA = occtCellsBuildersOperandsA.begin();
			kOcctShapeIteratorA != occtCellsBuildersOperandsA.end();
			kOcctShapeIteratorA++)
		{
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtListToTake.Clear();
				occtListToAvoid.Clear();
				occtListToTake.Append(*kOcctShapeIteratorA);
				occtListToTake.Append(*kOcctShapeIteratorB);
				occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			}
		}

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsA.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsA.end();
			kOcctShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);
			occtListToAvoid.Append(occtCellsBuildersOperandsB);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(occtCellsBuilder);
	}

	Topology* Topology::Intersection(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorA = occtCellsBuildersOperandsA.begin();
			kOcctShapeIteratorA != occtCellsBuildersOperandsA.end();
			kOcctShapeIteratorA++)
		{
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtListToTake.Clear();
				occtListToAvoid.Clear();
				occtListToTake.Append(*kOcctShapeIteratorA);
				occtListToTake.Append(*kOcctShapeIteratorB);
				occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			}
		}

		return GetBooleanResult(occtCellsBuilder);
	}

	Topology* Topology::Merge(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsA.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsA.end();
			kOcctShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsB.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsB.end();
			kOcctShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(occtCellsBuilder);
	}

	Topology* Topology::Slice(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorA = occtCellsBuildersOperandsA.begin();
			kOcctShapeIteratorA != occtCellsBuildersOperandsA.end();
			kOcctShapeIteratorA++)
		{
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtListToTake.Clear();
				occtListToAvoid.Clear();
				occtListToTake.Append(*kOcctShapeIteratorA);
				occtListToTake.Append(*kOcctShapeIteratorB);
				occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			}
		}

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsA.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsA.end();
			kOcctShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);
			occtListToAvoid.Append(occtCellsBuildersOperandsB);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}
		return GetBooleanResult(occtCellsBuilder);
	}

	void Topology::AddUnionInternalStructure(const TopoDS_Shape& rkOcctShape, BOPCol_ListOfShape& rUnionArguments)
	{
		TopAbs_ShapeEnum occtShapeType = rkOcctShape.ShapeType();
		Topology* pTopology = Topology::ByOcctShape(rkOcctShape);
		std::list<Face*> faces;
		// Cell complex -> faces not part of the envelope
		// Cell -> inner shells
		// Shell --> inner wires of the faces
		// Face --> inner wires
		// Wire --> n/a
		// Edge --> n/a
		// Vertex --> n/a
		if (occtShapeType == TopAbs_COMPOUND)
		{
			Cluster* pCluster = Topology::Downcast<Cluster>(pTopology);
			std::list<Topology*> immediateMembers;
			pCluster->ImmediateMembers(immediateMembers);
			for (std::list<Topology*>::const_iterator kIterator = immediateMembers.begin();
				kIterator != immediateMembers.end();
				kIterator++)
			{
				Topology* pTopology = *kIterator;
				AddUnionInternalStructure(*pTopology->GetOcctShape(), rUnionArguments);
			}
		} else if (occtShapeType == TopAbs_COMPSOLID)
		{
			CellComplex* pCellComplex = Topology::Downcast<CellComplex>(pTopology);
			pCellComplex->InnerBoundaries(faces);
			for (std::list<Face*>::iterator kFaceIterator = faces.begin();
				kFaceIterator != faces.end();
				kFaceIterator++)
			{
				Face* pInternalFace = *kFaceIterator;
				rUnionArguments.Append(*pInternalFace->GetOcctShape());
			}
		}
		else if (occtShapeType == TopAbs_SOLID)
		{
			Cell* pCell = Topology::Downcast<Cell>(pTopology);
			std::list<Shell*> shells;
			pCell->InnerBoundaries(shells);
			for (std::list<Shell*>::iterator kShellIterator = shells.begin();
				kShellIterator != shells.end();
				kShellIterator++)
			{
				Shell* pInternalShell = *kShellIterator;
				rUnionArguments.Append(*pInternalShell->GetOcctShape());
			}
		}
		else if (occtShapeType == TopAbs_SHELL)
		{
			TopExp_Explorer occtShellExplorer;
			for (occtShellExplorer.Init(rkOcctShape, TopAbs_FACE); occtShellExplorer.More(); occtShellExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrentFace = occtShellExplorer.Current();
				TopoDS_Wire occtOuterWire = BRepTools::OuterWire(TopoDS::Face(rkOcctCurrentFace));

				TopExp_Explorer occtFaceExplorer;
				for (occtFaceExplorer.Init(rkOcctShape, TopAbs_WIRE); occtFaceExplorer.More(); occtFaceExplorer.Next())
				{
					const TopoDS_Shape& rkOcctCurrentFace = occtFaceExplorer.Current();
					if (!rUnionArguments.Contains(rkOcctCurrentFace) && !rkOcctCurrentFace.IsSame(occtOuterWire))
					{
						rUnionArguments.Append(rkOcctCurrentFace);
					}
				}
			}
		}
		else if (occtShapeType == TopAbs_FACE)
		{
			TopoDS_Wire occtOuterWire = BRepTools::OuterWire(TopoDS::Face(rkOcctShape));

			TopExp_Explorer occtExplorer;
			for (occtExplorer.Init(rkOcctShape, TopAbs_WIRE); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& occtCurrent = occtExplorer.Current();
				if (!rUnionArguments.Contains(occtCurrent) && !occtCurrent.IsSame(occtOuterWire))
				{
					rUnionArguments.Append(occtCurrent);
				}
			}
		}
	}

	void Topology::AddBooleanOperands(
		Topology const * const kpkOtherTopology, 
		BOPAlgo_CellsBuilder& rOcctCellsBuilder,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsA,
		BOPCol_ListOfShape& rOcctCellsBuildersOperandsB)
	{
		// Buffer lists are currently provided for cell complexes to be processed per cells.
		BOPCol_ListOfShape occtCellsBuildersArguments;
		if (GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex* pCellComplex = TopologicalQuery::Downcast<CellComplex>(this);
			std::list<Cell*> cells;
			pCellComplex->Cells(cells);
			for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
				kCellIterator != cells.end();
				kCellIterator++)
			{
				rOcctCellsBuildersOperandsA.Append(*(*kCellIterator)->GetOcctShape());
				occtCellsBuildersArguments.Append(*(*kCellIterator)->GetOcctShape());
			}
		}
		else
		{
			rOcctCellsBuildersOperandsA.Append(*GetOcctShape());
			occtCellsBuildersArguments.Append(*GetOcctShape());
		}

		if (kpkOtherTopology->GetType() == TOPOLOGY_CELLCOMPLEX)
		{
			CellComplex const * const kpkCellComplex = TopologicalQuery::Downcast<CellComplex const>(kpkOtherTopology);
			std::list<Cell*> cells;
			kpkCellComplex->Cells(cells);
			for (std::list<Cell*>::const_iterator kCellIterator = cells.begin();
				kCellIterator != cells.end();
				kCellIterator++)
			{
				rOcctCellsBuildersOperandsB.Append(*(*kCellIterator)->GetOcctShape());
				occtCellsBuildersArguments.Append(*(*kCellIterator)->GetOcctShape());
			}
		}
		else
		{
			rOcctCellsBuildersOperandsB.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuildersArguments.Append(*kpkOtherTopology->GetOcctShape());
		}

		rOcctCellsBuilder.SetArguments(occtCellsBuildersArguments);
	}

	Topology* Topology::Union(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsA.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsA.end();
			kOcctShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIterator = occtCellsBuildersOperandsB.begin();
			kOcctShapeIterator != occtCellsBuildersOperandsB.end();
			kOcctShapeIterator++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIterator);
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}

		Topology* pTopology = GetBooleanResult(occtCellsBuilder);

		BOPAlgo_CellsBuilder occtCellsBuilder2;
		BOPCol_ListOfShape occtCellsBuildersArguments;
		occtCellsBuildersArguments.Append(*pTopology->GetOcctShape());

		// Get the internal boundaries
		AddUnionInternalStructure(*GetOcctShape(), occtCellsBuildersArguments);
		AddUnionInternalStructure(*kpkOtherTopology->GetOcctShape(), occtCellsBuildersArguments);
		
		if (occtCellsBuildersArguments.Size() < 2)
		{
			return Topology::ByOcctShape(occtCellsBuildersArguments.First());
		}

		occtCellsBuilder2.SetArguments(occtCellsBuildersArguments);
		occtCellsBuilder2.Perform();

		if (occtCellsBuilder2.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder2.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}
		occtCellsBuilder2.AddAllToResult();
		TopoDS_Shape occtUnionResult = occtCellsBuilder2.Shape();
		Topology* pUnionTopology = Topology::ByOcctShape(occtUnionResult);
		return pUnionTopology;
	}

	Topology* Topology::XOR(Topology const * const kpkOtherTopology)
	{
		BOPCol_ListOfShape occtCellsBuildersOperandsA;
		BOPCol_ListOfShape occtCellsBuildersOperandsB;
		BOPAlgo_CellsBuilder occtCellsBuilder;

		BooleanOperation(kpkOtherTopology, occtCellsBuilder, occtCellsBuildersOperandsA, occtCellsBuildersOperandsB);

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;
		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorA = occtCellsBuildersOperandsA.begin();
			kOcctShapeIteratorA != occtCellsBuildersOperandsA.end();
			kOcctShapeIteratorA++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIteratorA);
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
				kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
				kOcctShapeIteratorB++)
			{
				occtListToAvoid.Append(*kOcctShapeIteratorB);
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorB = occtCellsBuildersOperandsB.begin();
			kOcctShapeIteratorB != occtCellsBuildersOperandsB.end();
			kOcctShapeIteratorB++)
		{
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kOcctShapeIteratorB);
			for (BOPCol_ListOfShape::const_iterator kOcctShapeIteratorA = occtCellsBuildersOperandsA.begin();
				kOcctShapeIteratorA != occtCellsBuildersOperandsA.end();
				kOcctShapeIteratorA++)
			{
				occtListToAvoid.Append(*kOcctShapeIteratorA);
			}
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		return GetBooleanResult(occtCellsBuilder);
	}

	void Topology::ImmediateMembers(const TopoDS_Shape& rkShape, BOPCol_ListOfShape& rImmediateMembers)
	{
		TopAbs_ShapeEnum occtShapeEnum[8] =
		{
			TopAbs_COMPOUND,
			TopAbs_COMPSOLID,
			TopAbs_SOLID,
			TopAbs_SHELL,
			TopAbs_FACE,
			TopAbs_WIRE,
			TopAbs_EDGE,
			TopAbs_VERTEX
		};

		TopAbs_ShapeEnum occtShapeType = rkShape.ShapeType();
		for (int typeIndex = occtShapeType; typeIndex < 8; ++typeIndex)
		{
			// Get all children of this type
			TopExp_Explorer occtExplorer;
			for (occtExplorer.Init(rkShape, occtShapeEnum[typeIndex]); occtExplorer.More(); occtExplorer.Next())
			{
				bool hasAnotherParent = false;
				const TopoDS_Shape& occtCurrent = occtExplorer.Current();
				if (occtCurrent.IsSame(rkShape))
				{
					continue;
				}

				// Check if there is any parent other than this topology.
				// Stop checking if any other parent is found.
				for (int parentTypeIndex = occtShapeType; parentTypeIndex < typeIndex && !hasAnotherParent; ++parentTypeIndex)
				{
					TopTools_IndexedDataMapOfShapeListOfShape occtParentsMap;
					TopExp::MapShapesAndUniqueAncestors(rkShape, occtShapeEnum[typeIndex], occtShapeEnum[parentTypeIndex], occtParentsMap);

					const TopTools_ListOfShape& rkOcctParents = occtParentsMap.FindFromKey(occtCurrent);

					for (TopTools_ListOfShape::const_iterator kParentIterator = rkOcctParents.cbegin();
						kParentIterator != rkOcctParents.cend();
						kParentIterator++)
					{
						const TopoDS_Shape& rkOcctParent = *kParentIterator;
						if (rkOcctParent.ShapeType() != occtShapeType ||
							(rkOcctParent.ShapeType() == occtShapeType && !rkOcctParent.IsSame(rkShape)))
						{
							hasAnotherParent = true;
						}
					}
				}

				if (!hasAnotherParent)
				{
					rImmediateMembers.Append(occtCurrent);
				}
			}
		}
	}

	void Topology::ImmediateMembers(std::list<Topology*>& rImmediateMembers) const
	{
		BOPCol_ListOfShape occtListMembers;
		Topology::ImmediateMembers(*GetOcctShape(), occtListMembers);
		for (BOPCol_ListOfShape::const_iterator kIterator = occtListMembers.begin();
			kIterator != occtListMembers.end();
			kIterator++)
		{
			rImmediateMembers.push_back(Topology::ByOcctShape(*kIterator));
		}
	}
}