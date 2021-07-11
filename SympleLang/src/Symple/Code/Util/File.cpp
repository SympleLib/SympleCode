#include "Symple/Code/Util/File.h"

namespace Symple::Code
{
	File::File(const String &name, FilePermissions perms)
		: name(name), perms(perms)
	{
		const char *mode;
		switch (perms)
		{
		case FilePermissions::Read:
			mode = "rb";
			break;
		case FilePermissions::Write:
			mode = "wb";
			break;
		case FilePermissions::Append:
			mode = "ab";
			break;
		case FilePermissions::ReadWrite:
			mode = "wb+";
			break;
		case FilePermissions::ReadAppend:
			mode = "ab+";
			break;

		default:
			mode = nullptr;
			std::abort();
			break;
		}

		stream = std::fopen(name.c_str(), mode);
		if (!stream)
			abort();
		open = true;

		if (Seek(0, SEEK_END) && CanRead())
		{
			uint32 sz = std::ftell(stream);
			source.resize(sz + 1);
			std::rewind(stream);
			std::fread(source.data(), 1, sz, stream);
		}
	}

	File File::Open(const String &name, FilePermissions perms)
	{ return File(name, perms); }

	File::~File()
	{ Close(); }

	std::FILE *File::GetStream()
	{ return stream; }


	void File::Close()
	{
		if (open)
		{
			std::fclose(stream);
			stream = nullptr;
			open = false;
		}
	}


	bool File::Seek(uint32 offset, int32 origin)
	{ return !std::fseek(stream, offset, origin); }


	const String &File::GetName() const
	{ return name; }

	const String &File::GetSource() const
	{ return source; }

	bool File::IsOpen() const
	{ return open; }

	
	bool File::CanRead() const
	{ return (uint8)perms & (uint8)FilePermissions::Read; }

	bool File::CanEdit() const
	{ return (uint8)perms & (uint8)FilePermissions::Write || (uint8)perms & (uint8)FilePermissions::Append; }
}