#pragma once

#include <map>

namespace Symple
{
	struct Register
	{
		enum TypeType
		{
			Ax,
			Cx,
			Dx,
			Bx,
			Sp,
			Bp,
			Si,
			Di,

			Last = Di,
		} Type;

		const char* ToString(int size) const
		{
			switch (Type)
			{
			case Ax:
				switch (size)
				{
				case 1:
					return "%al";
				case 2:
					return "%ax";
				case 4:
					return "%eax";
				}
				
				return nullptr;
			case Cx:
				switch (size)
				{
				case 1:
					return "%cl";
				case 2:
					return "%cx";
				case 4:
					return "%ecx";
				}

				return nullptr;
			case Dx:
				switch (size)
				{
				case 1:
					return "%dl";
				case 2:
					return "%dx";
				case 4:
					return "%edx";
				}

				return nullptr;
			case Bx:
				switch (size)
				{
				case 1:
					return "%bl";
				case 2:
					return "%bx";
				case 4:
					return "%ebx";
				}

				return nullptr;
			case Sp:
				switch (size)
				{
				case 1:
					return "%spl";
				case 2:
					return "%sp";
				case 4:
					return "%esp";
				}

				return nullptr;
			case Bp:
				switch (size)
				{
				case 1:
					return "%bpl";
				case 2:
					return "%bp";
				case 4:
					return "%ebp";
				}

				return nullptr;
			case Si:
				switch (size)
				{
				case 1:
					return "%sil";
				case 2:
					return "%si";
				case 4:
					return "%esi";
				}

				return nullptr;
			case Di:
				switch (size)
				{
				case 1:
					return "%dil";
				case 2:
					return "%di";
				case 4:
					return "%edi";
				}

				return nullptr;
			}

			return nullptr;
		}
	};

	class RegisterManager
	{
	private:
		static std::map<Register::TypeType, Register*> mActiveRegisters;

		static Register::TypeType FreeRegister();
	public:
		static Register* Alloc();
		static Register* Reserve(Register::TypeType type);
		static void Free(Register* reg);
	};
}