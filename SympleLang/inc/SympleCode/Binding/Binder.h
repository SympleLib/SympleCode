#pragma once

#include <ctype.h>

#include "SympleCode/DiagnosticBag.h"

#include "SympleCode/Syntax/ExpressionSyntax.h"

#include "SympleCode/Binding/Node.h"

#include "SympleCode/Binding/BoundMember.h"
#include "SympleCode/Binding/BoundFunction.h"

#include "SympleCode/Binding/BoundExpression.h"
#include "SympleCode/Binding/BoundErrorExpression.h"
#include "SympleCode/Binding/BoundUnaryExpression.h"
#include "SympleCode/Binding/BoundBinaryExpression.h"
#include "SympleCode/Binding/BoundLiteralExpression.h"

namespace Symple::Binding
{
	class Binder
	{
	private:
		shared_ptr<DiagnosticBag> mDiagnosticBag = make_shared<DiagnosticBag>();

		shared_ptr<BoundExpression> BindExpressionInternal(shared_ptr<Syntax::ExpressionSyntax>);
	public:
		shared_ptr<Node> Bind(shared_ptr<Syntax::Node>);

		shared_ptr<BoundMember> BindMember(shared_ptr<Syntax::MemberSyntax>);
		shared_ptr<BoundFunction> BindFunction(shared_ptr<Syntax::FunctionDeclarationSyntax>);

		shared_ptr<Type> BindType(shared_ptr<Syntax::TypeSyntax>);

		shared_ptr<BoundExpression> BindExpression(shared_ptr<Syntax::ExpressionSyntax>);
		shared_ptr<BoundUnaryExpression> BindUnaryExpression(shared_ptr<Syntax::UnaryExpressionSyntax>);
		shared_ptr<BoundBinaryExpression> BindBinaryExpression(shared_ptr<Syntax::BinaryExpressionSyntax>);
		shared_ptr<BoundLiteralExpression> BindLiteralExpression(shared_ptr<Syntax::LiteralExpressionSyntax>);

		shared_ptr<DiagnosticBag> GetDiagnosticBag();
	};
}