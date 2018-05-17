#pragma once

#include <TDF_Data.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_ListOfShape.hxx>

#include <memory>
#include <list>

namespace TopologicCore
{
	enum TopologyRelationshipType
	{
		REL_CONSTITUENT = 0,
		REL_APERTURE = 1,
		REL_OTHER_NON_CONSTITUENT = 2
	};

	class Topology;
	/// <summary>
	/// This class manages the OCAF framework in OCCT
	/// </summary>
	class LabelManager
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		static LabelManager& GetInstance()
		{
			static LabelManager instance;
			return instance;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctLabel"></param>
		void AddLabelToRoot(TDF_Label& rOcctLabel);

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctLabel"></param>
		/// <param name="rParentLabel"></param>
		/// <param name="kRelationshipType"></param>*/
		//static void AddLabelToParent(TDF_Label& rOcctLabel, TDF_Label& rParentLabel, const TopologyRelationshipType kRelationshipType);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rOcctLabel"></param>
		static void AddShapeToLabel(const TopoDS_Shape& rkOcctShape, TDF_Label& rOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctLabel"></param>
		static void AddCounterToLabel(TDF_Label& rOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctLabel"></param>
		static void AddContentsContextsToLabel(TDF_Label& rOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctLabel"></param>
		void DecreaseCounter(TDF_Label& rOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctLabel"></param>
		bool AddGeneratedMembersToLabel(TDF_Label& rOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctParentLabel"></param>
		/// <param name="rkTopologyPairs"></param>
		/// <returns></returns>
		bool AddModifiedMembers(TDF_Label& rOcctParentLabel, const std::list<std::pair<std::shared_ptr<Topology>, std::shared_ptr<Topology>>>& rkTopologyPairs);

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="rTopTopologies"></param>*/
		//void GetTopShapes(std::list<std::shared_ptr<Topology>>& rTopTopologies);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkLabel1"></param>
		/// <param name="rLabel2"></param>
		static void CopyAttributes(const TDF_Label& rkLabel1, TDF_Label& rLabel2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		/// <param name="rOcctChildLabel"></param>
		/// <returns></returns>
		static bool FindLabelByShape(const TopoDS_Shape& rkOcctShape, TDF_Label& rOcctChildLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctLabel"></param>
		/// <returns></returns>
		bool HasLabel(const TDF_Label& rOcctLabel) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TDF_Label GetRoot() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctContentLabel"></param>
		/// <param name="rkOcctContextLabel"></param>
		void AddContent(const TDF_Label& rkOcctContentLabel, const TDF_Label& rkOcctContextLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctLabel"></param>
		/// <param name="rContents"></param>
		static void Contents(const TDF_Label& rkOcctLabel, std::list<std::shared_ptr<Topology>>& rContents);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctLabel"></param>
		/// <returns></returns>
		static TopoDS_Shape GetShape(const TDF_Label& rkOcctLabel);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctLabel"></param>
		/// <param name="rkID"></param>
		/// <returns></returns>
		static TDF_Attribute* FindAttribute(const TDF_Label& rkOcctLabel, const Standard_GUID& rkID);

	protected:
		LabelManager();
		~LabelManager();

		Handle(TDF_Data) m_pOcctDocument;
	};
}