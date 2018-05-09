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

	void OcctContextsAttribute::RemoveContext(const std::shared_ptr<Context>& kpContext)
	{
		m_contexts.remove(kpContext);
	}

	void OcctContextsAttribute::AddContext(const std::shared_ptr<Context>& kpContext)
	{
		std::list<std::shared_ptr<Context>>::const_iterator kContextIterator = std::find(m_contexts.begin(), m_contexts.end(), kpContext);
		if (kContextIterator == m_contexts.end()) // not here yet
		{
			m_contexts.push_back(kpContext);
		}
	}
}