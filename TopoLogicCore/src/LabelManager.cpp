#include "LabelManager.h"
#include <Topology.h>
#include <OcctRelationshipAttribute.h>
#include <OcctCounterAttribute.h>

#include <TDF_AttributeIterator.hxx>
#include <TDF_CopyLabel.hxx>
#include <TNaming_Builder.hxx>
#include <TDF_RelocationTable.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	void LabelManager::AddLabelToRoot(TDF_Label& rOcctLabel)
	{
		const TDF_Label kRootLabel = m_pOcctDocument->Root();
		rOcctLabel = TDF_TagSource::NewChild(kRootLabel);
	}

	void LabelManager::AddLabelToParent(TDF_Label& rOcctLabel, TDF_Label& rParentLabel, const TopologyRelationshipType kRelationshipType)
	{
		rOcctLabel = TDF_TagSource::NewChild(rParentLabel);
		OcctRelationshipAttribute::Set(rOcctLabel, REL_CONSTITUENT);
	}

	void LabelManager::AddShapeToLabel(const TopoDS_Shape& rkOcctShape, TDF_Label & rOcctLabel)
	{
		TNaming_Builder occtAttributeBuilder(rOcctLabel);
		occtAttributeBuilder.Generated(rkOcctShape);
	}

	void LabelManager::AddCounterToLabel(TDF_Label & rOcctLabel)
	{
		OcctCounterAttribute::Set(rOcctLabel);
	}

	void LabelManager::DecreaseCounter(TDF_Label& rOcctLabel)
	{
		if (rOcctLabel.IsNull())
		{
			return;
		}

		// TDF_Label cannot be removed. Simply delete all attributes if the counter is 0.
		Handle(OcctCounterAttribute) pOcctCounterAttribute;
		bool hasCounter = rOcctLabel.FindAttribute(OcctCounterAttribute::GetID(), pOcctCounterAttribute);
		if (!hasCounter)
		{
			//assert(false);
		}else
		{
			pOcctCounterAttribute->DecreaseCounter(rOcctLabel);
			if(pOcctCounterAttribute->Get() == 0 && 
				rOcctLabel.Depth() == 1) // first level
			{
				rOcctLabel.ForgetAllAttributes();
			}
		}
	}

	bool LabelManager::AddGeneratedMembersToLabel(TDF_Label& rOcctLabel)
	{
		Handle(TNaming_NamedShape) occtShapeAttribute;
		bool result = rOcctLabel.FindAttribute(TNaming_NamedShape::GetID(), occtShapeAttribute);
		if (!result)
		{
			return false;
		}
		const TopoDS_Shape& rkOcctShape = occtShapeAttribute->Get();
		// Store the children
		for (int i = ((int)rkOcctShape.ShapeType()) + 1; i < (int)TopAbs_SHAPE; ++i)
		{
			TopAbs_ShapeEnum occtShapeEnum = (TopAbs_ShapeEnum)i;
			TopTools_MapOfShape occtMembers;
			Topology::DownwardNavigation(rkOcctShape, occtShapeEnum, occtMembers);
			for (TopTools_MapIteratorOfMapOfShape occtMembersIterator(occtMembers);
				occtMembersIterator.More();
				occtMembersIterator.Next())
			{
				TDF_Label rOcctChildLabel;
				AddLabelToParent(rOcctChildLabel, rOcctLabel, REL_CONSTITUENT);
				TNaming_Builder occtChildAttributeBuilder(rOcctChildLabel);
				occtChildAttributeBuilder.Generated(occtMembersIterator.Value());
			}
		}

		return true;
	}

	bool LabelManager::AddModifiedMembers(TDF_Label& rOcctParentLabel, const std::list<std::pair<std::shared_ptr<Topology>, std::shared_ptr<Topology>>>& rkTopologyPairs)
	{
		if (rOcctParentLabel.IsNull())
		{
			return false;
		}

		for(const std::pair<std::shared_ptr<Topology>, std::shared_ptr<Topology>>& rkTopologyPair : rkTopologyPairs)
		{
			// Create a child label from the modified shape
			TDF_Label occtModifiedChildLabel;
			AddLabelToParent(occtModifiedChildLabel, rOcctParentLabel, REL_CONSTITUENT);
			TNaming_Builder occtModifiedChildAttributeBuilder(occtModifiedChildLabel);

			const TopoDS_Shape& rkOcctOriginalShape = rkTopologyPair.first->GetOcctShape();
			const TopoDS_Shape& rkOcctModifiedShape = rkTopologyPair.second->GetOcctShape();

			if(rkOcctOriginalShape.IsSame(rkOcctModifiedShape))
			{
				occtModifiedChildAttributeBuilder.Generated(rkOcctModifiedShape);
			}
			else
			{
				occtModifiedChildAttributeBuilder.Modify(rkOcctOriginalShape, rkOcctModifiedShape);
			}
		
			// If the original shape has attributes, copy them to the new one.
			const TDF_Label& rkOcctOriginalLabel = rkTopologyPair.first->GetOcctLabel();
			CopyAttributes(rkOcctOriginalLabel, occtModifiedChildLabel);
		}
		return true;
	}

	void LabelManager::GetTopShapes(std::list<std::shared_ptr<Topology>>& rTopTopologies)
	{
		TopTools_ListOfShape occtShapes;
		const TDF_Label kRootLabel = m_pOcctDocument->Root();
		for (TDF_ChildIterator occtChildLabelIterator(kRootLabel);
			occtChildLabelIterator.More();
			occtChildLabelIterator.Next())
		{
			const TDF_Label& rkOcctTopLabel = occtChildLabelIterator.Value();
			Handle(TNaming_NamedShape) occtShapeAttribute;
			bool result = rkOcctTopLabel.FindAttribute(TNaming_NamedShape::GetID(), occtShapeAttribute);
			if (result)
			{
				std::shared_ptr<Topology> pTopology = Topology::ByOcctShape(occtShapeAttribute->Get(), rkOcctTopLabel);
				pTopology->SetOcctLabel(rkOcctTopLabel);
				rTopTopologies.push_back(pTopology);
			}
		}
	}

	bool LabelManager::FindChildLabelByShape(const TopoDS_Shape& rkOcctShape, const TDF_Label& rkOcctParentLabel, TDF_Label& rOcctChildLabel)
	{
		// Check the parent first.
		Handle(TNaming_NamedShape) occtShapeAttribute;
		bool hasShapeAttribute = rkOcctParentLabel.FindAttribute(TNaming_NamedShape::GetID(), occtShapeAttribute);
		if (hasShapeAttribute)
		{
			const TopoDS_Shape& rkOcctParentShape = occtShapeAttribute->Get();
			// If the shape attribute is equal to rkOcctShape, return it.
			if (rkOcctParentShape.IsSame(rkOcctShape))
			{
				rOcctChildLabel = rkOcctParentLabel;
				return true;
			}
		}

		// Iterate through the children of rkOcctLabel.
		for (TDF_ChildIterator occtChildLabelIterator(rkOcctParentLabel);
			occtChildLabelIterator.More();
			occtChildLabelIterator.Next())
		{
			Handle(TNaming_NamedShape) occtShapeAttribute;
			const TDF_Label& rkOcctChildLabel = occtChildLabelIterator.Value();
			bool hasShapeAttribute = rkOcctChildLabel.FindAttribute(TNaming_NamedShape::GetID(), occtShapeAttribute);
			if (!hasShapeAttribute)
			{
				continue;
			}
			const TopoDS_Shape& rkOcctChildShape = occtShapeAttribute->Get();
			// If the shape attribute is equal to rkOcctShape, return it.
			if (rkOcctChildShape.IsSame(rkOcctShape))
			{
				rOcctChildLabel = rkOcctChildLabel;
				return true;
			}
		}
		return false;
	}

	bool LabelManager::HasLabel(const TDF_Label & rOcctLabel) const
	{
		const TDF_Label kRootLabel = m_pOcctDocument->Root();
		TDF_Label kChildLabel = kRootLabel.FindChild(rOcctLabel.Tag(), false);
		if(kChildLabel.IsNull() || kChildLabel.IsDifferent(rOcctLabel))
		{
			return false;
		}
		return true;
	}

	LabelManager::LabelManager()
	: m_pOcctDocument(new TDF_Data())
	{

	}

	LabelManager::~LabelManager()
	{
		m_pOcctDocument->Destroy();
	}

	void LabelManager::CopyAttributes(const TDF_Label& rkLabel1, TDF_Label& rLabel2)
	{
		// Forget existing attributes in this label and its children
		rLabel2.ForgetAllAttributes();

		// Copy the label, i.e. transfer attributes from rkLabel1 to rkLabel2.
		// The following attributes are excluded.
		// - Counter, because it is only valid for each label, and a label may be represented by different Topologic shapes.
		// - Shape, because a modified shape from the shape in rkLabel1 is being stored here.
		// TDF_CopyLabel cannot be used because it will copy all attributes.
		for (TDF_AttributeIterator occtAttributeIterator(rkLabel1); occtAttributeIterator.More(); occtAttributeIterator.Next())
		{
			const Handle(TDF_Attribute)& kpOcctAttribute = occtAttributeIterator.Value();
			if (!Handle(OcctCounterAttribute)::DownCast(kpOcctAttribute).IsNull())
			{
				AddCounterToLabel(rLabel2);
				continue;
			}
			
			Handle(TNaming_NamedShape) pOcctShape = Handle(TNaming_NamedShape)::DownCast(kpOcctAttribute);
			if (!pOcctShape.IsNull())
			{
				AddShapeToLabel(pOcctShape->Get(), rLabel2);
				continue;
			}

			Handle(TDF_Attribute) pOcctCopyAttribute = kpOcctAttribute->NewEmpty();
			// Copy the attribute
			kpOcctAttribute->Paste(pOcctCopyAttribute, new TDF_RelocationTable());
			// Add the attribute to rkLabel2
			try{
				rLabel2.AddAttribute(pOcctCopyAttribute);
			}catch(Standard_DomainError e)
			{ 
				std::string strException(e.GetMessageString());
			}
		}

		// Add the children based on attribute information
		//for (TDF_AttributeIterator occtAttributeIterator(rLabel2); occtAttributeIterator.More(); occtAttributeIterator.Next())
		//{
		//	Handle(TDF_Attribute) pOcctAttribute = occtAttributeIterator.Value();
		//	Handle(TDF_TagSource) pOcctTagSource = Handle(TDF_TagSource)::DownCast(pOcctAttribute);
		//	if (pOcctTagSource.IsNull())
		//	{
		//		continue;
		//	}

		//	const TDF_Label& rkOriginalLabelChild = pOcctTagSource->Label();

		//	TDF_Label label2Child;// = TDF_TagSource::NewChild(rkLabel2);
		//	AddLabelToParent(label2Child, rLabel2);
		//	TDF_CopyLabel occtCopyLabelChild(rkOriginalLabelChild, label2Child);
		//	occtCopyLabelChild.Perform();
		//}
	}
}