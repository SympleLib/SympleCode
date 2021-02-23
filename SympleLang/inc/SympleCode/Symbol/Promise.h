#pragma once

#include "SympleCode/Memory.h"

namespace Symple
{
	template<typename T, typename P>
	class Promise
	{
	private:
		P mPrompt;
		T mObject;
		bool mBroken = true;
	public:
		Promise(P& prompt)
			: mPrompt(prompt) {}

		P& GetPrompt()
		{ return mPrompt; }

		T& GetObject()
		{ return mObject; }

		void Complete(T& obj)
		{
			mObject = obj;
			mBroken = false;
		}

		bool IsBroken()
		{ return mBroken; }
	};
}