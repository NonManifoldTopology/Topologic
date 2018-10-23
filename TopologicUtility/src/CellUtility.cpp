#include "CellUtility.h"

#include <TopologicCore/include/Cell.h>
#include <TopologicCore/include/Face.h>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepGProp.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <GProp_GProps.hxx>
#include <Message_ProgressIndicator.hxx>
#include <ShapeFix_Solid.hxx>
#include <TopoDS.hxx>

namespace TopologicUtility
{
	TopologicCore::Cell::Ptr CellUtility::ByVerticesFaceIndices(const std::vector<TopologicCore::Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkFaceIndices)
	{
		if (rkVertices.empty())
		{
			throw std::exception("No vertex is passed.");
		}

		std::list<TopologicCore::Face::Ptr> faces;
		for (const std::list<int>& rkVertexIndices : rkFaceIndices)
		{
			BRepBuilderAPI_MakeWire occtMakeWire;

			std::list<int>::const_iterator kSecondFromLastVertexIterator = --rkVertexIndices.end();
			for (std::list<int>::const_iterator kVertexIterator = rkVertexIndices.begin();
				kVertexIterator != kSecondFromLastVertexIterator;
				kVertexIterator++)
			{
				int vertexIndex = *kVertexIterator;

				std::list<int>::const_iterator kNextVertexIterator = kVertexIterator;
				kNextVertexIterator++;
				int nextVertexIndex = *kNextVertexIterator;

				occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
					rkVertices[vertexIndex]->GetOcctVertex(),
					rkVertices[nextVertexIndex]->GetOcctVertex())
				);
			}
			occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
				rkVertices[*--rkVertexIndices.end()]->GetOcctVertex(),
				rkVertices[*rkVertexIndices.begin()]->GetOcctVertex())
			);

			const TopoDS_Wire& rkOcctWire = occtMakeWire.Wire();
			BRepBuilderAPI_MakeFace occtMakeFace(rkOcctWire);
			faces.push_back(std::make_shared<TopologicCore::Face>(occtMakeFace));
		}
		TopologicCore::Cell::Ptr pCell = TopologicCore::Cell::ByFaces(faces);

		return pCell;
	}


	TopologicCore::Cell::Ptr CellUtility::ByLoft(const std::list<TopologicCore::Wire::Ptr>& rkWires)
	{
		BRepOffsetAPI_ThruSections occtLoft(true);
		for (const TopologicCore::Wire::Ptr& kpWire : rkWires)
		{
			occtLoft.AddWire(kpWire->GetOcctWire());
		};
		try {
			occtLoft.Build();
		}
		catch (...)
		{
			throw std::exception("Loft error");
		}
		return std::make_shared<TopologicCore::Cell>(TopoDS::Solid(occtLoft.Shape()));
	}

	double CellUtility::Volume(const TopologicCore::Cell::Ptr & kpCell)
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(kpCell->GetOcctShape(), occtShapeProperties);
		return occtShapeProperties.Mass();
	}

	bool CellUtility::DoesContain(const TopologicCore::Cell::Ptr & kpCell, const TopologicCore::Vertex::Ptr & kpVertex)
	{
		ShapeFix_Solid occtSolidFix(kpCell->GetOcctSolid());
		occtSolidFix.Perform();
		BRepClass3d_SolidClassifier occtSolidClassifier(occtSolidFix.Solid(), kpVertex->Point()->Pnt(), Precision::Confusion());
		TopAbs_State occtState = occtSolidClassifier.State();
		return (occtState == TopAbs_IN || occtState == TopAbs_ON);
	}

}