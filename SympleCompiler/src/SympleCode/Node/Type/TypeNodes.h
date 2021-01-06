#pragma once

namespace Symple
{
	class TypeNodes
	{
	public:
		template<typename T>
		bool Is() const
		{
			return dynamic_cast<const T*>(this);
		}

		template<typename T>
		T* Cast()
		{
			return dynamic_cast<T*>(this);
		}

		template<typename T>
		const T* Cast() const
		{
			return dynamic_cast<const T*>(this);
		}
	};
}