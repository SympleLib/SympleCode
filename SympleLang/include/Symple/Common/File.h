#pragma once

#include "Symple/Common/Common.h"

namespace Symple
{
	enum class SY_API FilePermissions: uint8
	{
		Read   = 1 << 0,
		Write  = 1 << 1,
		Append = 1 << 2,

		ReadWrite = Read | Write,
		ReadAppend = Read | Append,
	};

	class SY_API File
	{
	private:
		const FilePermissions perms;
		mutable std::FILE *stream;
		std::string name;
		std::string src;
		bool open;

		uint32 num;

		static uint32 nextNum;
	private:
		std::FILE *GetStream() const;
	public:
		File(const std::string &name, FilePermissions);
		~File();

		static File Open(const std::string &name, FilePermissions);
		void Close();
		
		// Returns false if cannot read
		bool Seek(uint32 offset, int32 origin);

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