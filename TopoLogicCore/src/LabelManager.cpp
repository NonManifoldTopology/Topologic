#include "LabelManager.h"
#include <Topology.h>
#include <OcctRelationshipAttribute.h>
#include <OcctCounterAttribute.h>
#include <OcctContextListAttribute.h>
#include <OcctContentsAttribute.h>

#include <TDF_AttributeIterator.hxx>
#include <TDF_CopyLabel.hxx>
#include <TNaming_Builder.hxx>
#include <TDF_RelocationTable.hxx>

#include <assert.h>

namespace TopologicCore
{
	void LabelManager::AddLabelToRoot(TDF_Label& rOcctLabel)
	{
		const TDF_Label kRootLabel = m_pOcctDocument->Root();
		rOcctLabel = TDF_TagSource::NewChild(kRootLabel);
	}

	/*void LabelManager::AddLabelToParent(TDF_Label& rOcctLabel, TDF_Label& rParentLabel, const TopologyRelationshipType kRelationshipType)
	{
		rOcctLabel = TDF_TagSource::NewChild(rParentLabel);
		OcctRelationshipAttribute::Set(rOcctLabel, REL_CONSTITUENT);
	}*/

	void LabelManager::AddShapeToLabel(const TopoDS_Shape& rkOcctShape, TDF_Label & rOcctLabel)
	{
		TNaming_Builder occtAttributeBuilder(rOcctLabel);
		occtAttributeBuilder.Generated(rkOcctShape);
	}

	void LabelManager::AddCounterToLabel(TDF_Label & rOcctLabel)
	{
		OcctCounterAttribute::Set(rOcctLabel);
	}

	void LabelManager::AddContentsContextsToLabel(TDF_Label & rOcctLabel)
	{
		OcctContextListAttribute::Set(rOcctLabel);
		OcctContentsAttribute::Set(rOcctLabel);
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
			TopTools_ListOfShape occtMembers;
			Topology::DownwardNavigation(rkOcctShape, occtShapeEnum, occtMembers);
			for (TopTools_ListIteratorOfListOfShape occtMembersIterator(occtMembers);
				occtMembersIterator.More();
				occtMembersIterator.Next())
			{
				TDF_Label rOcctChildLabel;
				AddLabelToRoot(rOcctChildLabel);
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
			AddLabelToRoot(occtModifiedChildLabel);
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

	/*void LabelManager::GetTopShapes(std::list<std::shared_ptr<Topology>>& rTopTopologies)
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
	}*/

	bool LabelManager::FindLabelByShape(const TopoDS_Shape& rkOcctShape, TDF_Label& rOcctChildLabel)
	{
		// Check the parent first.
		Handle(TNaming_NamedShape) occtShapeAttribute;
		TDF_Label occtRootLabel = GetInstance().GetRoot();
		bool hasShapeAttribute = occtRootLabel.FindAttribute(TNaming_NamedShape::GetID(), occtShapeAttribute);

		// Iterate through the children of rkOcctLabel.
		for (TDF_ChildIterator occtChildLabelIterator(occtRootLabel, true);
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

	TDF_Label LabelManager::GetRoot() const
	{
		const TDF_Label kRootLabel = m_pOcctDocument->Root();
		return kRootLabel;
	}

	void LabelManager::AddContent(const TDF_Label & rkOcctContentLabel, const TDF_Label & rkOcctContextLabel)
	{
		{
			Handle(OcctContextListAttribute) pOcctContextListAttribute; // of the content label
			bool hasContextsAttribute = rkOcctContentLabel.FindAttribute(OcctContextListAttribute::GetID(), pOcctContextListAttribute);
			if (!hasContextsAttribute)
			{
				assert(false);
			}

			// Remove the content label from the old context labels
			std::list<TDF_Label>& rkContexts = pOcctContextListAttribute->GetContexts();
			for (const TDF_Label& rkOcctContextLabel : rkContexts)
			{
				Handle(OcctContentsAttribute) pOcctContentsAttribute; // of the context label
				bool hasContentsAttribute = rkOcctContextLabel.FindAttribute(OcctContentsAttribute::GetID(), pOcctContentsAttribute);
				if (!hasContentsAttribute)
				{
					assert(false);
				}
				else
				{
					pOcctContentsAttribute->RemoveContent(rkOcctContentLabel);
				}
			}
			rkContexts.clear();

			// Add the new context to the content
			pOcctContextListAttribute->AddContext(rkOcctContextLabel);
		}

		{
			Handle(OcctContentsAttribute) pOcctContentsAttribute; // of the context label
			bool hasContentsAttribute = rkOcctContextLabel.FindAttribute(OcctContentsAttribute::GetID(), pOcctContentsAttribute);
			if (!hasContentsAttribute)
			{
				assert(false);
			}
			// Add this content to the new context
			pOcctContentsAttribute->AddContent(rkOcctContentLabel);
		}
	}

	void LabelManager::Contents(const TDF_Label & rkOcctLabel, std::list<std::shared_ptr<Topology>>& rContents)
	{
		Handle(OcctContentsAttribute) pOcctContentsAttribute; // of the context label
		bool hasContentsAttribute = rkOcctLabel.FindAttribute(OcctContentsAttribute::GetID(), pOcctContentsAttribute);
		if (!hasContentsAttribute)
		{
			assert(false);
		}

		const std::list<TDF_Label>& rkOcctContentLabels = pOcctContentsAttribute->GetContents();
		for(const TDF_Label& rkOcctContentLabel : rkOcctContentLabels)
		{
			rContents.push_back(Topology::ByOcctShape(GetShape(rkOcctContentLabel), rkOcctContentLabel));
		}
	}

	TopoDS_Shape LabelManager::GetShape(const TDF_Label & rkOcctLabel)
	{
		Handle(TNaming_NamedShape) pOcctShape; // of the context label
		bool hasContentsAttribute = rkOcctLabel.FindAttribute(TNaming_NamedShape::GetID(), pOcctShape);
		if (!pOcctShape.IsNull())
		{
			return pOcctShape->Get();
		}
		else
		{
			assert(false);
			throw std::exception("No shape");
		}
	}

	TDF_Attribute* LabelManager::FindAttribute(const TDF_Label & rkOcctLabel, const Standard_GUID& rkID)
	{
		Handle(TDF_Attribute) pOcctAttribute;
		rkOcctLabel.FindAttribute(rkID, pOcctAttribute);
		return pOcctAttribute.get();
	}

	LabelManager::LabelManager()
	: m_pOcctDocument(new TDF_Data())
	{

	}

	LabelManager::~LabelManager()
	{
		// m_pOcctDocument is automatically destroyed.
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