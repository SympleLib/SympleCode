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

		template<typename... Args>
		void Trace(std::string_view fmt, Args&&... args)
		{
			mLogger->trace(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Debug(std::string_view fmt, Args&&... args)
		{
			mLogger->debug(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Info(std::string_view fmt, Args&&... args)
		{
			mLogger->info(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Warn(std::string_view fmt, Args&&... args)
		{
			mLogger->warn(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Critical(std::string_view fmt, Args&&... args)
		{
			mLogger->critical(fmt, std::forward<Args>(args)...);
		}

		Ref<spdlog::logger> GetLogger();
	};

	extern Log* SympleLog;
}