//#include <OcctRelationshipAttribute.h>
//
//#include <Standard_GUID.hxx>
//
//namespace TopologicCore
//{
//	OcctRelationshipAttribute::OcctRelationshipAttribute()
//		: m_relationshipType(REL_OTHER_NON_CONSTITUENT)
//	{
//	}
//
//	OcctRelationshipAttribute::~OcctRelationshipAttribute()
//	{
//	}
//
//	const Standard_GUID & OcctRelationshipAttribute::GetID()
//	{
//		static Standard_GUID TDataStd_IntegerID("c9de9b8f-1fa4-46ce-b397-23d75c756291");
//		return TDataStd_IntegerID;
//	}
//
//	const Standard_GUID & OcctRelationshipAttribute::ID() const
//	{
//		return GetID();
//	}
//
//	void OcctRelationshipAttribute::Restore(const Handle(TDF_Attribute)& kpOcctAttribute)
//	{
//		Handle(OcctRelationshipAttribute) pRelationshipAttribute = Handle(OcctRelationshipAttribute)::DownCast(kpOcctAttribute);
//		m_relationshipType = pRelationshipAttribute->m_relationshipType;
//	}
//
//	Handle(TDF_Attribute) OcctRelationshipAttribute::NewEmpty() const
//	{
//		return new OcctRelationshipAttribute();
//	}
//	
//	void OcctRelationshipAttribute::Paste(
//		const Handle(TDF_Attribute)& kpOcctAttribute,
//		const Handle(TDF_RelocationTable)& /*relocation table*/) const
//	{
//		Handle(OcctRelationshipAttribute) pRelationshipAttribute = Handle(OcctRelationshipAttribute)::DownCast(kpOcctAttribute);
//		pRelationshipAttribute->m_relationshipType = m_relationshipType;
//	}
//
//	Handle(OcctRelationshipAttribute) OcctRelationshipAttribute::Set(const TDF_Label& rkOcctLabel, const TopologyRelationshipType rkRelationshipType)
//	{
//		Handle(OcctRelationshipAttribute) pRelationshipAttribute;
//		if (!rkOcctLabel.FindAttribute(GetID(), pRelationshipAttribute)) {
//			pRelationshipAttribute = new OcctRelationshipAttribute();
//			rkOcctLabel.AddAttribute(pRelationshipAttribute);
//		}
//
//		if (pRelationshipAttribute->m_relationshipType != rkRelationshipType)
//		{
//			pRelationshipAttribute->Backup();
//			pRelationshipAttribute->m_relationshipType = rkRelationshipType;
//		}
//
//		return pRelationshipAttribute;
//	}
//
//	TopologyRelationshipType OcctRelationshipAttribute::Get() const
//	{ 
//		return m_relationshipType; 
//	}
//}