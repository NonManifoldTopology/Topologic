#include <OcctContextsAttribute.h>

#include <Standard_GUID.hxx>

namespace TopoLogicCore
{
	OcctContextsAttribute::OcctContextsAttribute()
	{
	}

	OcctContextsAttribute::~OcctContextsAttribute()
	{
		m_contexts.clear();
	}

	const Standard_GUID & OcctContextsAttribute::GetID()
	{
		static Standard_GUID TDataStd_IntegerID("876cd374-3338-4452-a9fe-c3ea9de599fb");
		return TDataStd_IntegerID;
	}

	const Standard_GUID & OcctContextsAttribute::ID() const
	{
		return GetID();
	}

	void OcctContextsAttribute::Restore(const Handle(TDF_Attribute)& kpOcctAttribute)
	{
		Handle(OcctContextsAttribute) pContextsAttribute = Handle(OcctContextsAttribute)::DownCast(kpOcctAttribute);
		m_contexts = pContextsAttribute->m_contexts;
	}

	Handle(TDF_Attribute) OcctContextsAttribute::NewEmpty() const
	{
		return new OcctContextsAttribute();
	}

	void OcctContextsAttribute::Paste(
		const Handle(TDF_Attribute)& kpOcctAttribute,
		const Handle(TDF_RelocationTable)& /*relocation table*/) const
	{
		Handle(OcctContextsAttribute) pRelationshipAttribute = Handle(OcctContextsAttribute)::DownCast(kpOcctAttribute);
		pRelationshipAttribute->m_contexts = m_contexts;
	}

	Handle(OcctContextsAttribute) OcctContextsAttribute::Set(const TDF_Label & label)
	{
		Handle(OcctContextsAttribute) pOcctContextsAttribute;
		if (!label.FindAttribute(OcctContextsAttribute::GetID(), pOcctContextsAttribute)) {
			pOcctContextsAttribute = new OcctContextsAttribute();
			label.AddAttribute(pOcctContextsAttribute);
		}
		return pOcctContextsAttribute;
	}

	void OcctContextsAttribute::RemoveContext(const TDF_Label& rkLabel)
	{
		m_contexts.remove(rkLabel);
	}

	void OcctContextsAttribute::AddContext(const TDF_Label& rkLabel)
	{
		std::list<TDF_Label>::const_iterator kContextIterator = std::find(m_contexts.begin(), m_contexts.end(), rkLabel);
		if (kContextIterator == m_contexts.end()) // not here yet
		{
			m_contexts.push_back(rkLabel);
		}
	}
}