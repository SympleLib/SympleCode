#include "SympleCode/Tree/BinaryOperators.hpp"

BinaryOperator::BinaryOperator(const Branch& lvalue, const Branch& rvalue)
	: mLvalue(lvalue), mRvalue(rvalue)
{
	CreateBranch();
}

const Branch& BinaryOperator::GetLValue() const
{ return mLvalue; }

const Branch& BinaryOperator::GetRValue() const
{ return mRvalue; }

const Branch& BinaryOperator::GetBranch() const
{ return mBranch; }

void BinaryOperator::CreateBranch()
{
	mBranch.Label = GetName();
	mBranch.PushBranch("L Value", mLvalue);
	mBranch.PushBranch("R Value", mRvalue);
}

AddOperator::AddOperator(const Branch& lvalue, const Branch& rvalue)
	: BinaryOperator(lvalue, rvalue) {}

std::string AddOperator::GetName() const
{
	return "Bin Op. +";
}

SubOperator::SubOperator(const Branch& lvalue, const Branch& rvalue)
	: BinaryOperator(lvalue, rvalue) {}

std::string SubOperator::GetName() const
{
	return "Bin Op. -";
}