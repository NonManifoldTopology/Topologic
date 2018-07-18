//#include <OcctContentsAttribute.h>
//
//#include <Standard_GUID.hxx>
//
//namespace TopologicCore
//{
//	OcctContentsAttribute::OcctContentsAttribute()
//	{
//	}
//
//	OcctContentsAttribute::~OcctContentsAttribute()
//	{
//		m_contents.clear();
//	}
//
//	const Standard_GUID & OcctContentsAttribute::GetID()
//	{
//		static Standard_GUID TDataStd_IntegerID("da36a71c-3c3e-4c2c-9543-90e0e5fcda27");
//		return TDataStd_IntegerID;
//	}
//
//	const Standard_GUID & OcctContentsAttribute::ID() const
//	{
//		return GetID();
//	}
//
//	void OcctContentsAttribute::Restore(const Handle(TDF_Attribute)& kpOcctAttribute)
//	{
//		Handle(OcctContentsAttribute) pContentsAttribute = Handle(OcctContentsAttribute)::DownCast(kpOcctAttribute);
//		m_contents = pContentsAttribute->m_contents;
//	}
//
//	Handle(TDF_Attribute) OcctContentsAttribute::NewEmpty() const
//	{
//		return new OcctContentsAttribute();
//	}
//
//	void OcctContentsAttribute::Paste(
//		const Handle(TDF_Attribute)& kpOcctAttribute,
//		const Handle(TDF_RelocationTable)& /*relocation table*/) const
//	{
//		Handle(OcctContentsAttribute) pRelationshipAttribute = Handle(OcctContentsAttribute)::DownCast(kpOcctAttribute);
//		pRelationshipAttribute->m_contents = m_contents;
//	}
//
//	Handle(OcctContentsAttribute) OcctContentsAttribute::Set(const TDF_Label & label)
//	{
//		Handle(OcctContentsAttribute) pOcctContentsAttribute;
//		if (!label.FindAttribute(OcctContentsAttribute::GetID(), pOcctContentsAttribute)) {
//			pOcctContentsAttribute = new OcctContentsAttribute();
//			label.AddAttribute(pOcctContentsAttribute);
//		}
//		return pOcctContentsAttribute;
//	}
//
//	void OcctContentsAttribute::RemoveContent(const TDF_Label& rkLabel)
//	{
//		m_contents.remove(rkLabel);
//	}
//
//	void OcctContentsAttribute::AddContent(const TDF_Label& rkLabel)
//	{
//		std::list<TDF_Label>::const_iterator kContentIterator = std::find(m_contents.begin(), m_contents.end(), rkLabel);
//		if (kContentIterator == m_contents.end()) // not here yet
//		{
//			m_contents.push_back(rkLabel);
//		}
//	}
//}