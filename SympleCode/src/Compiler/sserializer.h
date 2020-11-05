#pragma once

#include <vector>

#include <ostream>

namespace scp
{
	class serializer
	{
	private:
		std::vector<char> mBuffer;
		size_t mUnreadSize;
	public:
		serializer() {}
		serializer(const std::vector<char>& buffer) : mUnreadSize(buffer.size())
		{
			mBuffer = buffer;
		}

		inline friend std::ostream& operator <<(std::ostream& os, const serializer& serializer)
		{
			os << serializer.mUnreadSize << " Bytes [";
			if (serializer.mUnreadSize > 0)
				os << (short)serializer.mBuffer[serializer.mBuffer.size() - serializer.mUnreadSize];
			for (size_t i = serializer.mBuffer.size() - serializer.mUnreadSize + 1; i < serializer.mUnreadSize; i++)
				os << ", " << (short)serializer.mBuffer[i];
			os << "]";

			return os;
		}

		template<typename T>
		inline serializer& operator <<(const T& data)
		{
			for (unsigned int i = 0; i < sizeof(data); i++)
			{
				mBuffer.push_back(((const char*)&data)[i]);

				mUnreadSize++;
			}

			return *this;
		}

		template<typename T>
		inline serializer& operator >>(T& data)
		{
			if (!(mUnreadSize >= sizeof(data)))
				return *this;

			unsigned char* ptr = (unsigned char*)&data;
			for (unsigned int i = 0; i < sizeof(data); i++)
			{
				ptr[sizeof(data) - 1 - i] = mBuffer[mUnreadSize - 1];

				mUnreadSize--;
			}

			return *this;
		}

		inline void Serialize(const std::string& filepath) const
		{
			FILE* fp;
			fopen_s(&fp, filepath.c_str(), "wb");
			fwrite(mBuffer.data(), sizeof(char), mBuffer.size(), fp);
			fflush(fp);
			fclose(fp);
		}

		inline static serializer DeSerialize(const std::string& filepath)
		{
			FILE* fp;
			fopen_s(&fp, filepath.c_str(), "rb");

			fseek(fp, 0L, SEEK_END);
			std::vector<char> buffer(ftell(fp));

			rewind(fp);

			fread(buffer.data(), buffer.size(), 1, fp);
			fclose(fp);

			return serializer(buffer);
		}

		inline std::vector<char> GetBuffer() const
		{
			return mBuffer;
		}

		inline std::vector<char>& GetBuffer()
		{
			return mBuffer;
		}

		inline size_t GetSize()
		{
			return mBuffer.size();
		}

		inline size_t GetUnreadSize()
		{
			return mUnreadSize;
		}
	};

	inline serializer DeSerialize(const std::string& filepath)
	{
		FILE* fp;
		fopen_s(&fp, filepath.c_str(), "rb");

		fseek(fp, 0L, SEEK_END);
		std::vector<char> buffer(ftell(fp));

		rewind(fp);

		fread(buffer.data(), buffer.size(), 1, fp);
		fclose(fp);

		return serializer(buffer);
	}
}