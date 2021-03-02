#include "Symple/Code/Util/File.h"

namespace Symple::Code
{
	uint32 File::nextNum;

	File::File(const std::string &name, FilePermissions perms)
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
		assert(stream);
		num = nextNum++;
		open = true;

		if (Seek(0, SEEK_END))
		{
			uint32 sz = std::ftell(stream);
			src.resize(sz + 1);
			std::rewind(stream);
			std::fread(&src.front(), 1, sz, stream);
		}
	}

	File File::Open(const std::string &name, FilePermissions perms)
	{ return File(name, perms); }

	File::~File()
	{ Close(); }

	std::FILE *File::GetStream() const
	{ return stream; }


	void File::Close()
	{
		if (open)
		{
			std::fclose(stream);
			open = true;
		}
	}


	bool File::Seek(uint32 offset, int32 origin)
	{
		if (CanRead)
			std::fseek(stream, offset, origin);
		return CanRead;
	}


	const std::string &File::GetName() const
	{ return name; }

	const std::string &File::GetSource() const
	{ return src; }

	bool File::GetIsOpen() const
	{ return open; }


	uint32 File::GetNumber() const
	{ return num; }

	
	bool File::GetCanRead() const
	{ return (uint8)perms & (uint8)FilePermissions::Read; }

	bool File::GetCanEdit() const
	{ return (uint8)perms & (uint8)FilePermissions::Write || (uint8)perms & (uint8)FilePermissions::Append; }
}