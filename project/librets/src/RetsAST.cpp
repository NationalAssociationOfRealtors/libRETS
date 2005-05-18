#include "librets/RetsAST.h"

using namespace librets;
using std::string;
using antlr::RefToken;
using antlr::RefAST;

RetsAST::RetsAST(const RetsAST & other)
    : CommonAST(other), mLine(other.mLine), mColumn(other.mColumn)
{
}

RetsAST::RetsAST()
    : CommonAST(), mLine(0), mColumn(0)
{
}

RetsAST::~RetsAST()
{
}

int RetsAST::getLine() const
{
    // most of the time the line number is not set if the node is a
    // imaginary one. Usually this means it has a child. Refer to the
    // child line number. Of course this could be extended a bit.
    // based on an example by Peter Morling.
    if (mLine != 0)
    {
        return mLine;
    }
    
    if(getFirstChild())
    {
        return RefRetsAST(getFirstChild())->getLine();
    }
    
    return 0;
}

void RetsAST::setLine(int line)
{
    mLine = line;
}

int RetsAST::getColumn() const
{
    if (mColumn != 0)
    {
        return mColumn;
    }
    
    if(getFirstChild())
    {
        return RefRetsAST(getFirstChild())->getColumn();
    }
    
    return 0;
}

void RetsAST::setColumn(int column)
{
    mColumn = column;
}

void RetsAST::initialize(int t, const string & text)
{
    CommonAST::initialize(t, text);
    mLine = 0;
    mColumn = 0;
}

void RetsAST::initialize(RefToken t)
{
    CommonAST::initialize(t);
    mLine = t->getLine();
    mColumn = t->getColumn();
}

void RetsAST::initialize(RefRetsAST ast)
{
    CommonAST::initialize(RefAST(ast));
    mLine = ast->getLine();
    mColumn = ast->getColumn();
}

void RetsAST::addChild(RefRetsAST child)
{
    BaseAST::addChild(RefAST(child));
}

void RetsAST::setNextSibling(RefRetsAST sibling)
{
    BaseAST::setNextSibling(RefAST(sibling));
}

antlr::RefAST RetsAST::clone()
{
    return RefAST(new RetsAST(*this));
}

antlr::RefAST RetsAST::factory()
{
    return RefAST(RefRetsAST(new RetsAST()));
}
