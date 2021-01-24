#include "SympleCode/Syntax/Node.h"

namespace Symple
{
	Node::Node(Kind kind, std::shared_ptr<Token> tok)
		: mKind(kind), mToken(tok)
	{}

	Node::Node(std::shared_ptr<Token> tok)
		: mToken(tok), mKind(Unknown)
	{}


	void Node::Print(FILE* fs)
	{

	}

	void Node::Print(std::ostream os)
	{
		
	}


	Node::Kind Node::GetKind()
	{ return mKind; }

	std::shared_ptr<Token> Node::GetToken()
	{ return mToken; }
}