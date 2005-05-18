#ifndef LIBRETS_RETS_AST_H
#define LIBRETS_RETS_AST_H

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

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
