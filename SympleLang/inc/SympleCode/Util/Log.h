#pragma once

#include <spdlog/spdlog.h>

namespace Symple
{
	class Log
	{
	private:
		Ref<spdlog::logger> mLogger;
	public:
		Log(std::string_view name);

		Ref<spdlog::logger> GetLogger();
	};

	extern Log* SympleLog;
}