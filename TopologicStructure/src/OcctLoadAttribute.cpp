#include <OcctLoadAttribute.h>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>

namespace TopologicStructure
{
	OcctLoadAttribute::OcctLoadAttribute()
		: m_xDirection(0.0)
		, m_yDirection(0.0)
		, m_zDirection(0.0)
		, m_magnitude(0.0)
	{
	}
	OcctLoadAttribute::OcctLoadAttribute(const double kXDirection, const double kYDirection, const double kZDirection,
		const double kMagnitude)
		: m_xDirection(kXDirection)
		, m_yDirection(kYDirection)
		, m_zDirection(kZDirection)
		, m_magnitude(kMagnitude)
	{
	}
	OcctLoadAttribute::~OcctLoadAttribute()
	{
	}
	const Standard_GUID & OcctLoadAttribute::GetID()
	{
		static Standard_GUID TDataStd_IntegerID("4833680d-42ac-4851-97c6-ecc085b0ab5e");
		return TDataStd_IntegerID;
	}
	const Standard_GUID & OcctLoadAttribute::ID() const
	{
		return GetID();
	}
	void OcctLoadAttribute::Restore(const Handle(TDF_Attribute)& kpOcctAttribute)
	{
		Handle(OcctLoadAttribute) pLoadAttribute = Handle(OcctLoadAttribute)::DownCast(kpOcctAttribute);
		m_xDirection = pLoadAttribute->m_xDirection;
		m_yDirection = pLoadAttribute->m_yDirection;
		m_zDirection = pLoadAttribute->m_zDirection;
		m_magnitude = pLoadAttribute->m_magnitude;
	}
	Handle(TDF_Attribute) OcctLoadAttribute::NewEmpty() const
	{
		return new OcctLoadAttribute();
	}
	void OcctLoadAttribute::Paste(const Handle(TDF_Attribute)& kpOcctAttribute, const Handle(TDF_RelocationTable)&) const
	{
		Handle(OcctLoadAttribute) pLoadAttribute = Handle(OcctLoadAttribute)::DownCast(kpOcctAttribute);
		pLoadAttribute->m_xDirection = m_xDirection;
		pLoadAttribute->m_yDirection = m_yDirection;
		pLoadAttribute->m_zDirection = m_zDirection;
		pLoadAttribute->m_magnitude = m_magnitude;
	}
	Handle(OcctLoadAttribute) OcctLoadAttribute::SetLoad(const TDF_Label & rkOcctLabel, const double kXDirection, const double kYDirection, const double kZDirection, const double kMagnitude)
	{
		Handle(OcctLoadAttribute) pLoadAttribute;
		if (!rkOcctLabel.FindAttribute(GetID(), pLoadAttribute)) {
			pLoadAttribute = new OcctLoadAttribute();
			rkOcctLabel.AddAttribute(pLoadAttribute);
		}

		pLoadAttribute->Backup();
		pLoadAttribute->m_xDirection = kXDirection;
		pLoadAttribute->m_yDirection = kYDirection;
		pLoadAttribute->m_zDirection = kZDirection;
		pLoadAttribute->m_magnitude = kMagnitude;

		return pLoadAttribute;
	}

	void OcctLoadAttribute::Get(double & rXDirection, double & rYDirection, double & rZDirection, double & rMagnitude) const
	{
		rXDirection = m_xDirection;
		rYDirection = m_zDirection;
		rZDirection = m_yDirection;
		rMagnitude = m_magnitude;
	}
}
