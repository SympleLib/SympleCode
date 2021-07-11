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
		const FilePermissions perms = FilePermissions::None;
		mutable std::FILE *stream = nullptr;
		String name;
		String source;
		bool open = false;
	public:
		File() = default;
		File(const String &name, FilePermissions);
		~File();

		static File Open(const String &name, FilePermissions);
		void Close();
		
		// Returns false if cannot read
		bool Seek(uint32 offset, int32 origin);

		std::FILE *GetStream();
		const String &GetName() const;
		const String &GetSource() const;
		bool IsOpen() const;

		bool CanRead() const;
		bool CanEdit() const;
	};
}