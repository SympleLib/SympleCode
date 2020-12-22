#include "SympleCode/Emit/Emitter.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <memory>

#define Write(fmt, ...) ((void)fprintf_s(mFile, fmt "\n", __VA_ARGS__))
#define WriteLiteral(fmt, ...) ((void)fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__))

#define RegSp ("%esp")
#define RegBp ("%ebp")

#define FORMAT__MAX 128

namespace Symple
{
	Emitter::Emitter(Diagnostics* diagnostics, const char* path)
		: mDiagnostics(diagnostics), mPath(path), mStackPos(0), mDataPos(0), mDeclaredVariables()
	{
		while (OpenFile());
		while (OpenLiteralFile());
	}

	Emitter::~Emitter()
	{
		rewind(mLiteralFile);

		char c;
		while ((c = fgetc(mLiteralFile)) != EOF)
			fputc(c, mFile);

		fclose(mLiteralFile);
		fclose(mFile);
	}

	char Emitter::Mod(int size)
	{
		if (size <= 1)
			return 'b';
		if (size <= 2)
			return 'w';
		if (size <= 4)
			return 'l';
		return ' ';
	}

	char* Emitter::RegAx(int size)
	{
		if (size <= 1)
			return "%al";
		if (size <= 2)
			return "%ax";
		if (size <= 4)
			return "%eax";
		return "%eax";
	}

	char* Emitter::RegDx(int size)
	{
		if (size <= 1)
			return "%dl";
		if (size <= 2)
			return "%dx";
		if (size <= 4)
			return "%edx";
		return "%eax";
	}

	char* Emitter::Format(char* fmt, ...)
	{
		char* out = new char[FORMAT__MAX];

		va_list vl;
		va_start(vl, fmt);
		vsprintf_s(out, FORMAT__MAX, fmt, vl);
		va_end(vl);

		return out;
	}

	void Emitter::Emit(const CompilationUnitNode* unit)
	{
		for (const MemberNode* member : unit->GetMembers())
			EmitMember(member);
	}

	char* Emitter::EmitMember(const MemberNode* member)
	{
		switch (member->GetKind())
		{
		case Node::Kind::FunctionHint:
			return EmitFunctionDeclaration(member->Cast<FunctionDeclarationNode>());
		}
	}

	#pragma region File Stuff
	bool Emitter::OpenFile()
	{
		errno_t err = fopen_s(&mFile, mPath, "w");
		if (err && !mFile)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[56];
				sprintf_s(realMsg, "[!]: Error opening temp file: %.25s", msg);
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, realMsg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
			else
			{
				char msg[37] = "[!]: Unknown error opening temp file";
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, msg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool Emitter::OpenLiteralFile()
	{
		errno_t err = tmpfile_s(&mLiteralFile);
		if (err && !mLiteralFile)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[56];
				sprintf_s(realMsg, "[!]: Error opening temp file: %.25s", msg);
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, realMsg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
			else
			{
				char msg[37] = "[!]: Unknown error opening temp file";
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, msg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
		}
		else
		{
			return false;
		}

		return true;
	}
	#pragma endregion
}