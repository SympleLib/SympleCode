#pragma once

#include "Symple/Code/Type/TypeBase.h"
#include "Symple/Code/Util/Memory.h"

namespace Symple::Code
{
	class SYC_API Type: public Printable
	{
	private:
		TypeBaseRef m_Base;
		uint32 m_PtrCount;

		std::string m_MangledName, m_Code;
	public:
		Type(TypeBaseRef, uint32 pointerCount = 0);
		GlobalRef<Type> Ptr() const;
		GlobalRef<Type> Deref() const;

		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		static const GlobalRef<const Type> Default;

		bool Is(TypeBaseRef) const;
		bool Is(GlobalRef<const Type>) const;
		template<typename... Args>
		bool Is(GlobalRef<const Type> type, Args&&... types) const
		{ return Is(type) || Is(std::forward(types)...); }

		TypeBaseRef GetBase() const;
		uint32 GetPointerCount() const;
		bool GetIsStruct() const;
		bool GetIsFloat() const;
		bool GetIs86() const;
		uint32 GetSize() const;

		const std::string &GetMangledName() const;
		const std::string &GetCode() const;

		SY_PROPERTY_GET(GetBase) TypeBaseRef Base;
		SY_PROPERTY_GET(GetPointerCount) uint32 PointerCount;
		SY_PROPERTY_GET(GetIsStruct) bool IsStruct;
		SY_PROPERTY_GET(GetIsFloat) bool IsFloat;
		SY_PROPERTY_GET(GetIs86) bool Is86;
		SY_PROPERTY_GET(GetSize) uint32 Size;

		SY_PROPERTY_GET(GetMangledName) const std::string &MangledName;
		SY_PROPERTY_GET(GetCode) const std::string &Code;
	};
}