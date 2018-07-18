//#include <OcctCounterAttribute.h>
//
//#include <TDF_Label.hxx>
//
//#include <assert.h>
//
//namespace TopologicCore
//{
//	OcctCounterAttribute::OcctCounterAttribute()
//		: m_counter(0)
//	{
//	}
//
//	OcctCounterAttribute::~OcctCounterAttribute()
//	{
//	}
//
//	const Standard_GUID & OcctCounterAttribute::GetID()
//	{
//		static Standard_GUID ID("822850de-a56d-4c1e-a483-21f1e494bc5e");
//		return ID;
//	}
//
//	const Standard_GUID & OcctCounterAttribute::ID() const
//	{
//		return GetID();
//	}
//
//	void OcctCounterAttribute::Restore(const Handle(TDF_Attribute)& kpOcctAttribute)
//	{
//		Handle(OcctCounterAttribute) pCounterAttribute = Handle(OcctCounterAttribute)::DownCast(kpOcctAttribute);
//		m_counter = pCounterAttribute->m_counter;
//	}
//
//	Handle(TDF_Attribute) OcctCounterAttribute::NewEmpty() const
//	{
//		return new OcctCounterAttribute();
//	}
//
//	void OcctCounterAttribute::Paste(
//		const Handle(TDF_Attribute)& kpOcctAttribute,
//		const Handle(TDF_RelocationTable)& /*relocation table*/) const
//	{
//		Handle(OcctCounterAttribute) pCounterAttribute = Handle(OcctCounterAttribute)::DownCast(kpOcctAttribute);
//		pCounterAttribute->m_counter = m_counter;
//	}
//
//	Handle(OcctCounterAttribute) OcctCounterAttribute::IncreaseCounter(const TDF_Label& rkOcctLabel)
//	{
//		Handle(OcctCounterAttribute) pCounterAttribute;
//		if (!rkOcctLabel.FindAttribute(GetID(), pCounterAttribute)) {
//			pCounterAttribute = new OcctCounterAttribute();
//			rkOcctLabel.AddAttribute(pCounterAttribute);
//		}
//
//		pCounterAttribute->Backup();
//		pCounterAttribute->m_counter++;
//
//		return pCounterAttribute;
//	}
//
//	Handle(OcctCounterAttribute) OcctCounterAttribute::DecreaseCounter(const TDF_Label& rkOcctLabel)
//	{
//		Handle(OcctCounterAttribute) pCounterAttribute;
//		if (!rkOcctLabel.FindAttribute(GetID(), pCounterAttribute)) {
//			assert(false);
//			/*pCounterAttribute = new OcctCounterAttribute();
//			rkOcctLabel.AddAttribute(pCounterAttribute);*/
//		}
//
//		pCounterAttribute->Backup();
//		pCounterAttribute->m_counter--;
//
//		return pCounterAttribute;
//	}
//
//	Handle(OcctCounterAttribute) OcctCounterAttribute::Set(const TDF_Label & label)
//	{
//		Handle(OcctCounterAttribute) pOcctCounterAttribute;
//		if (!label.FindAttribute(OcctCounterAttribute::GetID(), pOcctCounterAttribute)) {
//			pOcctCounterAttribute = new OcctCounterAttribute();
//			label.AddAttribute(pOcctCounterAttribute);
//		}
//		return pOcctCounterAttribute;
//	}
//
//	int OcctCounterAttribute::Get() const
//	{
//		return m_counter;
//	}
//}