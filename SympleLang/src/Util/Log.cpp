#include <pch.h>
#include "SympleCode/Util/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Symple
{
	namespace
	{
		Log _SympleLog("Symple");
	}

	Log* SympleLog = &_SympleLog;

	Log::Log(std::string_view name)
	{
		spdlog::set_pattern("[%n]%^<%l>%$: %v");
		mLogger = spdlog::stdout_color_mt(std::string(name));
		mLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> Log::GetLogger()
	{
		return mLogger;
	}
}