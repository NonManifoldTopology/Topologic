#include <OcctContentsAttribute.h>

#include <Standard_GUID.hxx>

namespace TopoLogicCore
{
	OcctContentsAttribute::OcctContentsAttribute()
	{
	}

	OcctContentsAttribute::~OcctContentsAttribute()
	{
		m_contents.clear();
	}

	const Standard_GUID & OcctContentsAttribute::GetID()
	{
		static Standard_GUID TDataStd_IntegerID("da36a71c-3c3e-4c2c-9543-90e0e5fcda27");
		return TDataStd_IntegerID;
	}

	const Standard_GUID & OcctContentsAttribute::ID() const
	{
		return GetID();
	}

	void OcctContentsAttribute::Restore(const Handle(TDF_Attribute)& kpOcctAttribute)
	{
		Handle(OcctContentsAttribute) pContentsAttribute = Handle(OcctContentsAttribute)::DownCast(kpOcctAttribute);
		m_contents = pContentsAttribute->m_contents;
	}

	Handle(TDF_Attribute) OcctContentsAttribute::NewEmpty() const
	{
		return new OcctContentsAttribute();
	}

	void OcctContentsAttribute::Paste(
		const Handle(TDF_Attribute)& kpOcctAttribute,
		const Handle(TDF_RelocationTable)& /*relocation table*/) const
	{
		Handle(OcctContentsAttribute) pRelationshipAttribute = Handle(OcctContentsAttribute)::DownCast(kpOcctAttribute);
		pRelationshipAttribute->m_contents = m_contents;
	}

	void OcctContentsAttribute::RemoveContent(const std::shared_ptr<Topology>& kpTopology)
	{
		m_contents.remove(kpTopology);
	}

	void OcctContentsAttribute::AddContent(const std::shared_ptr<Topology>& kpTopology)
	{
		std::list<std::shared_ptr<Topology>>::const_iterator kContentIterator = std::find(m_contents.begin(), m_contents.end(), kpTopology);
		if (kContentIterator == m_contents.end()) // not here yet
		{
			m_contents.push_back(kpTopology);
		}
	}
}