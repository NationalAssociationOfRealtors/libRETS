/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */
#ifndef LIBRETS_RETS_AST_H
#define LIBRETS_RETS_AST_H
/** 
 * @file RetsAST.h
 * (Internal) Contains the RetsAST class that inherits from antlr.
 */
 /// @cond MAINTAINER
#include <antlr/CommonAST.hpp>

namespace librets {

class RetsAST;

typedef antlr::ASTRefCount<RetsAST> RefRetsAST;

/**
 * Custom AST class that adds line and column numbers to the AST
 * nodes.  Filenames will take more work since you'll need a custom
 * token class as well (one that contains the filename)
 */
class RetsAST : public antlr::CommonAST
{
  public:

    /**
     * Copy constructor.
     *
     * @param other Other object
     */
    RetsAST(const RetsAST & other);

    /**
     * Default constructor.
     */
    RetsAST();

    virtual ~RetsAST();

    /**
     * Returns the line number of the node (or try to derive it from the
     * child node).
     *
     * @return The line number
     */
    virtual int getLine() const;

    virtual void setLine(int line);

    /**
     * Returns the column number of the node (or try to derive it from
     * the child node).
     *
     * @return The column number
     */
    virtual int getColumn( void ) const;

    virtual void setColumn(int column);

    /*
     * The initialize methods are called by the tree building
     * constructs depending on which version is called the line number
     * is filled in.  e.g. a bit depending on how the node is
     * constructed it will have the line number filled in or not
     * (imaginary nodes!).
     */
    virtual void initialize(int t, const std::string & text);

    virtual void initialize(antlr::RefToken t);

    virtual void initialize(RefRetsAST ast);

    // for convenience will also work without
    void addChild(RefRetsAST child);

    // for convenience will also work without
    void setNextSibling(RefRetsAST sibling);

    // provide a clone of the node (no sibling/child pointers are copied)
    virtual antlr::RefAST clone();

    static antlr::RefAST factory();

  private:
    int mLine;
    int mColumn;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
