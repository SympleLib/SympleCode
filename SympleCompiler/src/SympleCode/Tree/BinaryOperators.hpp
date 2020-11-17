#pragma once

#include "SympleCode/Tree/Tree.hpp"

class BinaryOperator
{
protected:
	const Branch& mLvalue, mRvalue;
	Branch mBranch;
public:
	BinaryOperator(const Branch& lvalue, const Branch& rvalue);

	const Branch& GetLValue() const;
	const Branch& GetRValue() const;

	virtual std::string GetName() const = 0;
	const Branch& GetBranch() const;
protected:
	virtual void CreateBranch();
};

class AddOperator : public BinaryOperator
{
public:
	AddOperator(const Branch& lvalue, const Branch& rvalue);

	std::string GetName() const override;
};

class SubOperator : public BinaryOperator
{
public:
	SubOperator(const Branch& lvalue, const Branch& rvalue);

	std::string GetName() const override;
};