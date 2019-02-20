#pragma once

using namespace System;

namespace Topologic {
	public ref class About
	{
	public:
		static String^ Version();

	private:
		About() {}
		~About() {}
	};
}