#include "SympleCode/Syntax/Node.h"

namespace Symple
{
	Node::Node(std::shared_ptr<Token> tok)
		: mToken(tok), mKind(Unknown)
	{}

	Node::Kind Node::GetKind()
	{ return mKind; }

	std::shared_ptr<Token> Node::GetToken()
	{ return mToken; }
}