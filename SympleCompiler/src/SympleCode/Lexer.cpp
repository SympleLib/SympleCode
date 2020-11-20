#include "SympleCode/Lexer.hpp"

#include "SympleCode/Token.hpp"

namespace Symple::Lexer
{
    static const char* sBeg = nullptr;
    static std::string_view sLex;

    bool IsDigit(char c)
    {
        return c >= '0' && c <= '9' || c == '.';
    }

    bool IsIdentifier(char c)
    {
        switch (c) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '_':
            return true;
        default:
            return false;
        }
    }

    bool IsSpace(char c)
    {
        switch (c)
        {
        case ' ':
        case '\r':
        case '\n':
        case '\t':
            return true;
        default:
            return false;
        }
    }

    char Peek()
    {
        return *sBeg;
    }

    char Get()
    {
        return *sBeg++;
    }

    void Lex(const char* beg, std::function<bool(const TokenInfo&)> fun)
    {
        sBeg = beg;
        while (fun(Next()));
    }

    TokenInfo Atom(Token token)
    {
        return { token, sBeg++, 1 };
    }

    TokenInfo Next()
    {
        while (IsSpace(Peek()))
            Get();

        switch (Peek()) {
        case '\0':
            return { Tokens::End, sBeg, 1 };
        default:
            return Atom(Tokens::Unexpected);
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            return Identifier();
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return Number();
        case '(':
            return Atom(Tokens::LeftParen);
        case ')':
            return Atom(Tokens::RightParen);
        case '[':
            return Atom(Tokens::LeftSquare);
        case ']':
            return Atom(Tokens::RightSquare);
        case '{':
            return Atom(Tokens::LeftCurly);
        case '}':
            return Atom(Tokens::RightCurly);
        case '<':
            return Atom(Tokens::LessThan);
        case '>':
            return Atom(Tokens::GreaterThan);
        case '=':
            return EqualsEqual();
        case '+':
            return Atom(Tokens::Plus);
        case '-':
            return Atom(Tokens::Minus);
        case '*':
            return Atom(Tokens::Asterisk);
        case '/':
            return Comment();
        case '#':
            return Atom(Tokens::Hash);
        case '.':
            return Atom(Tokens::Dot);
        case ',':
            return Atom(Tokens::Comma);
        case ':':
            return Atom(Tokens::Colon);
        case ';':
            return Atom(Tokens::Semicolon);
        case '\'':
            return Atom(Tokens::SingleQuote);
        case '"':
            return Atom(Tokens::DoubleQuote);
        case '|':
            return Atom(Tokens::Pipe);
        case '$':
            return Atom(Tokens::Dollar);
        case '@':
            return Atom(Tokens::At);
        case '!':
            return Atom(Tokens::Exclimation);
        }
    }

    TokenInfo Identifier()
    {
        const char* start = sBeg;
        Get();
        while (IsIdentifier(Peek()))
            Get();
        return { Tokens::Identifier, start, sBeg };
    }

    TokenInfo Number()
    {
        const char* start = sBeg;
        Get();
        while (IsDigit(Peek()))
            Get();
        return { Tokens::Number, start, sBeg };
    }

    TokenInfo Comment()
    {
        const char* start = sBeg;
        Get();
        if (Peek() == '/') {
            Get();
            start = sBeg;
            while (Peek() != '\0')
            {
                if (Get() == '\n')
                {
                    return { Tokens::Comment, start, std::distance(start, sBeg) - (size_t)1 };
                }
            }
            return { Tokens::Comment, start, std::distance(start, sBeg) - (size_t)1 };
        }
        else if (Peek() == '*')
        {
            Get();
            start = sBeg;
            while (Peek() != '\0')
            {
                if (Get() == '*' && Get() == '/')
                {
                    return { Tokens::Comment, start, std::distance(start, sBeg) - (size_t)4 };
                }
            }
            return { Tokens::Unexpected, start, 1 };
        }
        else
        {
            return { Tokens::Slash, start, 1 };
        }
    }

    TokenInfo EqualsEqual()
    {
        const char* start = sBeg;
        Get();
        if (Get() == '=')
            return { Tokens::EqualsEqual, start, sBeg };
        return Tokens::Equal;
    }
}