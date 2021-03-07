#include "Symple/Code/Util/File.h"

namespace Symple::Code
{
	uint32 File::s_NextNum;

	File::File(const std::string &name, FilePermissions perms)
		: m_Name(name), m_Perms(perms)
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

		m_Stream = std::fopen(name.c_str(), mode);
		assert(m_Stream);
		m_Num = s_NextNum++;
		m_Open = true;

		if (Seek(0, SEEK_END))
		{
			uint32 sz = std::ftell(m_Stream);
			m_Source.resize(sz + 1);
			std::rewind(m_Stream);
			std::fread(m_Source.data(), 1, sz, m_Stream);
		}
	}

	File File::Open(const std::string &name, FilePermissions perms)
	{ return File(name, perms); }

	File::~File()
	{ Close(); }

	std::FILE *File::GetStream()
	{ return m_Stream; }


	void File::Close()
	{
		if (m_Open)
		{
			std::fclose(m_Stream);
			m_Open = true;
		}
	}


	bool File::Seek(uint32 offset, int32 origin)
	{
		if (CanRead)
			std::fseek(m_Stream, offset, origin);
		return CanRead;
	}


	const std::string &File::GetName() const
	{ return m_Name; }

	const std::string &File::GetSource() const
	{ return m_Source; }

	bool File::GetIsOpen() const
	{ return m_Open; }


	uint32 File::GetNumber() const
	{ return m_Num; }

	
	bool File::GetCanRead() const
	{ return (uint8)m_Perms & (uint8)FilePermissions::Read; }

	bool File::GetCanEdit() const
	{ return (uint8)m_Perms & (uint8)FilePermissions::Write || (uint8)m_Perms & (uint8)FilePermissions::Append; }
}