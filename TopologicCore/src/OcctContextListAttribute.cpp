//#include <OcctContextListAttribute.h>
//
//#include <Standard_GUID.hxx>
//
//namespace TopologicCore
//{
//	OcctContextListAttribute::OcctContextListAttribute()
//	{
//	}
//
//	OcctContextListAttribute::~OcctContextListAttribute()
//	{
//		m_contexts.clear();
//	}
//
//	const Standard_GUID & OcctContextListAttribute::GetID()
//	{
//		static Standard_GUID TDataStd_IntegerID("876cd374-3338-4452-a9fe-c3ea9de599fb");
//		return TDataStd_IntegerID;
//	}
//
//	const Standard_GUID & OcctContextListAttribute::ID() const
//	{
//		return GetID();
//	}
//
//	void OcctContextListAttribute::Restore(const Handle(TDF_Attribute)& kpOcctAttribute)
//	{
//		Handle(OcctContextListAttribute) pContextsAttribute = Handle(OcctContextListAttribute)::DownCast(kpOcctAttribute);
//		m_contexts = pContextsAttribute->m_contexts;
//	}
//
//	Handle(TDF_Attribute) OcctContextListAttribute::NewEmpty() const
//	{
//		return new OcctContextListAttribute();
//	}
//
//	void OcctContextListAttribute::Paste(
//		const Handle(TDF_Attribute)& kpOcctAttribute,
//		const Handle(TDF_RelocationTable)& /*relocation table*/) const
//	{
//		Handle(OcctContextListAttribute) pRelationshipAttribute = Handle(OcctContextListAttribute)::DownCast(kpOcctAttribute);
//		pRelationshipAttribute->m_contexts = m_contexts;
//	}
//
//	Handle(OcctContextListAttribute) OcctContextListAttribute::Set(const TDF_Label & label)
//	{
//		Handle(OcctContextListAttribute) pOcctContextListAttribute;
//		if (!label.FindAttribute(OcctContextListAttribute::GetID(), pOcctContextListAttribute)) {
//			pOcctContextListAttribute = new OcctContextListAttribute();
//			label.AddAttribute(pOcctContextListAttribute);
//		}
//		return pOcctContextListAttribute;
//	}
//
//	void OcctContextListAttribute::RemoveContext(const TDF_Label& rkLabel)
//	{
//		m_contexts.remove(rkLabel);
//	}
//
//	void OcctContextListAttribute::AddContext(const TDF_Label& rkLabel)
//	{
//		std::list<TDF_Label>::const_iterator kContextIterator = std::find(m_contexts.begin(), m_contexts.end(), rkLabel);
//		if (kContextIterator == m_contexts.end()) // not here yet
//		{
//			m_contexts.push_back(rkLabel);
//		}
//	}
//}