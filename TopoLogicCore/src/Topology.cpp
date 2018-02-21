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
#include <BOPAlgo_CellsBuilder.hxx>
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
#include <BOPAlgo_MakerVolume.hxx>

#include <BRepBuilderAPI_MakeSolid.hxx>

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

	std::string Topology::Analyze()
	{
		ShapeAnalysis_ShapeContents occtShapeAnalysis;
		occtShapeAnalysis.Perform(*GetOcctShape());

		std::string shapeType;
		switch (GetOcctShape()->ShapeType())
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

		// No method is provided in ShapeAnalysis_ShapeContents to compute the number of CompSolids.
		// Do this manually.
		int numberCompSolids = 0;
		TopExp_Explorer occtExplorer;
		BOPCol_ListOfShape occtCompSolids;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_COMPSOLID); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& rkOcctCurrent = occtExplorer.Current();
			if (!occtCompSolids.Contains(rkOcctCurrent))
			{
				occtCompSolids.Append(rkOcctCurrent);
				numberCompSolids++;
			}
		}
		std::stringstream resultStream;
		resultStream <<
			"The shape is a " << shapeType << "." << std::endl <<
			"Number of cell complexes = " << numberCompSolids << std::endl <<
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

		BOPAlgo_CellsBuilder occtCellsBuilder;
		BooleanImages(kpkOtherTopology, occtCellsBuilder, occtArgumentImagesInArguments, occtArgumentImagesInTools, occtToolsImagesInArguments, occtToolsImagesInTools);

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
		BOPCol_ListOfShape& kOcctArgumentImagesInArguments, 
		BOPCol_ListOfShape& kOcctArgumentImagesInTools, 
		BOPCol_ListOfShape& kOcctToolsImagesInArguments, 
		BOPCol_ListOfShape& kOcctToolsImagesInTools)
	{
		// 1. Prepare the OCCT arguments and tools
		BOPCol_ListOfShape occtShapeArguments;
		BOPCol_ListOfShape occtCellsBuildersArguments;
		BOPCol_ListOfShape occtShapeTools;

		occtShapeArguments.Append(*GetOcctShape());
		occtCellsBuildersArguments.Append(*GetOcctShape());

		occtShapeTools.Append(*kpkOtherTopology->GetOcctShape());
		occtCellsBuildersArguments.Append(*kpkOtherTopology->GetOcctShape());

		rOcctCellsBuilder.SetArguments(occtCellsBuildersArguments);
		// Split the arguments and tools
		rOcctCellsBuilder.Perform();

		//const TopoDS_Shape& rkOcctSplitShape = occtSplitter.Shape();
		const TopoDS_Shape& rMergeResult = rOcctCellsBuilder.Shape();

		//const BOPCol_DataMapOfShapeListOfShape& rkImages = occtSplitter.Images();
		const BOPCol_DataMapOfShapeListOfShape& rkImages = rOcctCellsBuilder.Images();

		const TopoDS_Shape& rkParts = rOcctCellsBuilder.GetAllParts();
		Topology* pTopologyParts = Topology::ByOcctShape(rkParts);
		std::string strParts = pTopologyParts->Analyze();

		// 3. Classify the images: exclusively from argument, exclusively from tools, or shared.
		BOPCol_ListOfShape occtExclusivelyArgumentImages;
		BOPCol_ListOfShape occtExclusivelyToolImages;
		BOPCol_ListOfShape occtSharedImages;

		for (BOPCol_ListOfShape::const_iterator kArgumentIterator = occtShapeArguments.begin();
			kArgumentIterator != occtShapeArguments.end();
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
				for (BOPCol_ListOfShape::const_iterator kToolIterator = occtShapeTools.begin();
					kToolIterator != occtShapeTools.end() && !isArgumentImageInTools;
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
					occtSharedImages.Append(rkArgumentImage);
				}
				else
				{
					occtExclusivelyArgumentImages.Append(rkArgumentImage);
				}
			}
		}

		for (BOPCol_ListOfShape::const_iterator kToolIterator = occtShapeTools.begin();
			kToolIterator != occtShapeTools.end();
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
				for (BOPCol_ListOfShape::const_iterator kArgumentIterator = occtShapeArguments.begin();
					kArgumentIterator != occtShapeArguments.end() && !isToolInArguments;
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
					occtExclusivelyToolImages.Append(rkToolImage);
				}

				// The shared images have been taken care of; no need to check them.
			}
		}

		// 4. Get the faces
		BOPCol_ListOfShape occtExclusivelyArgumentImageFaces;
		BOPCol_ListOfShape occtExclusivelyToolImageFaces;
		BOPCol_ListOfShape occtSharedImageFaces;

		for (BOPCol_ListOfShape::const_iterator kArgumentImageIterator = occtExclusivelyArgumentImages.begin();
			kArgumentImageIterator != occtExclusivelyArgumentImages.end();
			kArgumentImageIterator++)
		{
			TopExp_Explorer occtArgumentImageExplorer;
			for (occtArgumentImageExplorer.Init(*kArgumentImageIterator, TopAbs_FACE); occtArgumentImageExplorer.More(); occtArgumentImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtArgumentImageExplorer.Current();
				if (!occtExclusivelyArgumentImageFaces.Contains(rkOcctCurrent))
				{
					occtExclusivelyArgumentImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		for (BOPCol_ListOfShape::const_iterator kToolImageIterator = occtExclusivelyToolImages.begin();
			kToolImageIterator != occtExclusivelyToolImages.end();
			kToolImageIterator++)
		{
			TopExp_Explorer occtToolImageExplorer;
			for (occtToolImageExplorer.Init(*kToolImageIterator, TopAbs_FACE); occtToolImageExplorer.More(); occtToolImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtToolImageExplorer.Current();
				if (!occtExclusivelyToolImageFaces.Contains(rkOcctCurrent))
				{
					occtExclusivelyToolImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		for (BOPCol_ListOfShape::const_iterator kSharedImageIterator = occtSharedImages.begin();
			kSharedImageIterator != occtSharedImages.end();
			kSharedImageIterator++)
		{
			TopExp_Explorer occtSharedImageExplorer;
			for (occtSharedImageExplorer.Init(*kSharedImageIterator, TopAbs_FACE); occtSharedImageExplorer.More(); occtSharedImageExplorer.Next())
			{
				const TopoDS_Shape& rkOcctCurrent = occtSharedImageExplorer.Current();
				if (!occtSharedImageFaces.Contains(rkOcctCurrent))
				{
					occtSharedImageFaces.Append(rkOcctCurrent);
				}
			}
		}

		// 5. Identify the argument faces in a tool by doing IsSame Comparison
		for (BOPCol_ListOfShape::const_iterator kArgumentImageFaceIterator = occtExclusivelyArgumentImageFaces.cbegin();
			kArgumentImageFaceIterator != occtExclusivelyArgumentImageFaces.cend();
			kArgumentImageFaceIterator++)
		{
			bool isInSharedImage = false;
			for (BOPCol_ListOfShape::const_iterator kSharedImageFaceIterator = occtSharedImageFaces.cbegin();
				kSharedImageFaceIterator != occtSharedImageFaces.cend() && !isInSharedImage;
				kSharedImageFaceIterator++)
			{
				if (kArgumentImageFaceIterator->IsSame(*kSharedImageFaceIterator))
				{
					isInSharedImage = true;
				}
			}

			if (isInSharedImage)
			{
				// part of tools in the arguments
				kOcctToolsImagesInArguments.Append(*kArgumentImageFaceIterator);
			}
			else
			{
				kOcctArgumentImagesInArguments.Append(*kArgumentImageFaceIterator);
			}
		}

		// 6. Identify the tool faces in an image by doing IsSame Comparison
		for (BOPCol_ListOfShape::const_iterator kToolImageFaceIterator = occtExclusivelyToolImageFaces.cbegin();
			kToolImageFaceIterator != occtExclusivelyToolImageFaces.cend();
			kToolImageFaceIterator++)
		{
			bool isInSharedImage = false;
			for (BOPCol_ListOfShape::const_iterator kSharedImageFaceIterator = occtSharedImageFaces.cbegin();
				kSharedImageFaceIterator != occtSharedImageFaces.cend() && !isInSharedImage;
				kSharedImageFaceIterator++)
			{
				if (kToolImageFaceIterator->IsSame(*kSharedImageFaceIterator))
				{
					isInSharedImage = true;
				}
			}

			if (isInSharedImage)
			{
				// part of arguments in the tools
				kOcctArgumentImagesInTools.Append(*kToolImageFaceIterator);
			}
			else
			{
				kOcctToolsImagesInTools.Append(*kToolImageFaceIterator);
			}
		}
	}

	Topology* Topology::BooleanOperation(
		Topology const * const kpkOtherTopology,
		const BooleanFlag kBooleanFlag)
	{
		BOPCol_ListOfShape occtCellsBuildersArguments;
		occtCellsBuildersArguments.Append(*GetOcctShape());
		occtCellsBuildersArguments.Append(*kpkOtherTopology->GetOcctShape());
		
		BOPAlgo_CellsBuilder occtCellsBuilder;
		occtCellsBuilder.SetArguments(occtCellsBuildersArguments);

		// Split the arguments and tools
		occtCellsBuilder.Perform();

		BOPCol_ListOfShape occtListToTake;
		BOPCol_ListOfShape occtListToAvoid;

		if (kBooleanFlag == BOOLEAN_UNION)
		{
			occtListToTake.Append(*GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);

			occtListToTake.Clear();
			occtListToTake.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}
		else if (kBooleanFlag == BOOLEAN_DIFFERENCE)
		{
			occtListToTake.Append(*GetOcctShape());
			occtListToAvoid.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}
		else if (kBooleanFlag == BOOLEAN_INTERSECTION)
		{
			occtListToTake.Append(*GetOcctShape());
			occtListToTake.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}else if(kBooleanFlag == BOOLEAN_MERGE)
		{
			occtListToTake.Append(*GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);

			occtListToTake.Clear();
			occtListToTake.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}
		else if (kBooleanFlag == BOOLEAN_SLICE)
		{
			// same as imprint
			occtListToTake.Append(*GetOcctShape());
			occtListToTake.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);

			occtListToTake.Clear();
			occtListToTake.Append(*GetOcctShape());
			occtListToAvoid.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}
		else if (kBooleanFlag == BOOLEAN_IMPOSE)
		{
			occtListToTake.Append(*GetOcctShape());
			occtListToAvoid.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
			
			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid, 1, true);
		}
		else if (kBooleanFlag == BOOLEAN_IMPRINT)
		{
			occtListToTake.Append(*GetOcctShape());
			occtListToTake.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);

			occtListToTake.Clear();
			occtListToTake.Append(*GetOcctShape());
			occtListToAvoid.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}
		else // BOOLEAN_XOR
		{
			occtListToTake.Append(*GetOcctShape());
			occtListToAvoid.Append(*kpkOtherTopology->GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);

			occtListToTake.Clear();
			occtListToAvoid.Clear();
			occtListToTake.Append(*kpkOtherTopology->GetOcctShape());
			occtListToAvoid.Append(*GetOcctShape());
			occtCellsBuilder.AddToResult(occtListToTake, occtListToAvoid);
		}

		if (occtCellsBuilder.HasErrors())
		{
			std::ostringstream errorStream;
			occtCellsBuilder.DumpErrors(errorStream);
			throw std::exception(errorStream.str().c_str());
		}

		const TopoDS_Shape& rkOcctMergeShape = occtCellsBuilder.Shape();
		Topology* pTopology = Topology::ByOcctShape(rkOcctMergeShape);
		std::string str = pTopology->Analyze();

		return ByOcctShape(rkOcctMergeShape);
	}

	Topology* Topology::Difference(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_DIFFERENCE);
	}

	Topology* Topology::Impose(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_IMPOSE);
	}

	Topology* Topology::Imprint(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_IMPRINT);
	}

	Topology* Topology::Intersection(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_INTERSECTION);
	}

	Topology* Topology::Merge(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_MERGE);
	}

	Topology* Topology::Slice(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_SLICE);
	}

	Topology* Topology::Union(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_UNION);
	}

	Topology* Topology::XOR(Topology const * const kpkOtherTopology)
	{
		return BooleanOperation(kpkOtherTopology, BOOLEAN_XOR);
	}
}