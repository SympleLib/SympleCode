#pragma once

#include "Symple/Code/Common.h"

namespace Symple::Code
{
	enum class SYC_API FilePermissions: uint8
	{
		None,
		Read   = 1 << 0,
		Write  = 1 << 1,
		Append = 1 << 2,

		ReadWrite = Read | Write,
		ReadAppend = Read | Append,
	};

	class SYC_API File
	{
	private:
		const FilePermissions m_Perms = FilePermissions::None;
		mutable std::FILE *m_Stream = nullptr;
		std::string m_Name;
		std::string m_Source;
		bool m_Open = false;

		uint32 m_Num = -1;

		static uint32 s_NextNum;
	private:
	public:
		File() = default;
		File(const std::string &name, FilePermissions);
		~File();

		static File Open(const std::string &name, FilePermissions);
		void Close();
		
		// Returns false if cannot read
		bool Seek(uint32 offset, int32 origin);

		std::FILE *GetStream();
		const std::string &GetName() const;
		const std::string &GetSource() const;
		bool GetIsOpen() const;

		uint32 GetNumber() const;

		bool GetCanRead() const;
		bool GetCanEdit() const;

		SY_PROPERTY_GET(GetStream) mutable std::FILE *Stream;
		SY_PROPERTY_GET(GetName) const std::string &Name;
		SY_PROPERTY_GET(GetSource) const std::string &Source;
		SY_PROPERTY_GET(GetIsOpen) bool IsOpen;

		SY_PROPERTY_GET(GetNumber) uint32 Number;

		SY_PROPERTY_GET(GetCanRead) bool CanRead;
		SY_PROPERTY_GET(GetCanEdit) bool CanEdit;
	};
}