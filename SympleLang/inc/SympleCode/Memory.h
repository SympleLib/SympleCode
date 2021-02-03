#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#if __SY_DEBUG
#define __SY_ASSERT(cond, msg) if (!(cond)) spdlog::critical("(Assertion Failed) '{}'@{}: {}", __FILE__, __LINE__, #msg)
#else
#define __SY_ASSERT(cond, msg)
#endif

namespace Symple
{
	using std::shared_ptr;
	using std::make_shared;

	using std::unique_ptr;
	using std::make_unique;

	using std::dynamic_pointer_cast;
}