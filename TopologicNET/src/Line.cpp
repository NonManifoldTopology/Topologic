#include "Line.h"

namespace Topologic
{
	Line::Line(const std::shared_ptr<TopologicCore::Line>& kpCoreLine)
		: m_pCoreLine(kpCoreLine != nullptr ? new TopologicCore::Line::Ptr(kpCoreLine) : throw gcnew Exception("A null line was created."))
	{

	}

	Line::~Line()
	{

	}
}