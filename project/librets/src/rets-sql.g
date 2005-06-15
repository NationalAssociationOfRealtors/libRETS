header "post_include_hpp"
{
#include "librets/RetsAST.h"
}

options
{
	language="Cpp";
    namespace = "librets";
    namespaceStd = "std";
    namespaceAntlr = "antlr";
}

class RetsSqlParser extends Parser;

options
{
    exportVocab = RetsSql;
    k = 3;
    ASTLabelType = "RefRetsAST";
    buildAST = true;
    defaultErrorHandler = false;
}

tokens
{
    SELECT = "select"; FROM = "from"; WHERE = "where";
    OR = "or"; AND = "and"; NOT = "not";
    COLUMNS; COLUMN; QUERY_ELEMENT;
}

{
  public: antlr::RefToken getTableName() { return mTableName; };
  private: antlr::RefToken mTableName;
}

sql_statements
    : (sql_statement)* EOF
    ;

sql_statement
    : sql_command (SEMI!)?
    ;

sql_command
    : select_statement
    ;

select_statement
    :! SELECT^ c:column_names
        FROM! t:table_name
        WHERE! w:where_condition
        { #select_statement = #([SELECT], t_AST, c_AST, w_AST); }
    ;

column_names
    : STAR { #column_names = #([COLUMNS]); }
    | column_name (COMMA! column_name)*
        { #column_names = #([COLUMNS], #column_names); }
    ;

column_name!
    : id:ID         { #column_name = #([COLUMN], #[ID, ""], #id);  }
    | table:ID DOT col:ID
        { #column_name = #([COLUMN], #table, #col); }
    ;

table_name
    : id:ID { mTableName = id; }
    ;

where_condition
    : and_clause (OR^ and_clause)*
    ;

and_clause
    : not_clause (AND^ not_clause)*
    ;

not_clause
    : NOT^ query_element
    | query_element
    ;

query_element
    : LPAREN! where_condition RPAREN!
    | numeric_condition
        { #query_element = #([QUERY_ELEMENT], #query_element); }

    ;

numeric_condition
    : column_name (EQ^ | LESS^ | LTE^ | GREATER^ | GTE^) field_value
    ;

field_value
    : ID
    | INT
    | STRING
    ;


class RetsSqlLexer extends Lexer;

options
{
    k = 2;
    testLiterals = false;
    caseSensitiveLiterals = false;
    exportVocab = RetsSql;
}

WS_ :	(' '
	|	'\t' { tab(); }
	|	'\n' { newline(); }
	|	'\r')
		{ $setType(antlr::Token::SKIP); }
	;

LPAREN  : '(' ;
RPAREN  : ')' ;
STAR    : '*' ;
PLUS    : '+' ;
EQ      : '='  ;
GREATER: ">" ;
GTE     : ">=" ;
LESS    : "<"  ;
LTE  options { paraphrase = "<="; }             : "<=" ;
SEMI options { paraphrase = "a semicolon"; }    : ';' ;
COMMA options { paraphrase = ","; }             : ',' ;
DOT options { paraphrase = "."; }               : '.' ;

COMMENT
    : "--" (~('\n'|'\r'))* ('\n'|'\r'('\n')?)?
        {
            $setType(antlr::Token::SKIP);
            newline();
        }
    ;
  
protected
DIGIT
	:	'0'..'9'
	;

protected
ALPHA
    : ('a'..'z' | 'A'..'Z')
    ;

INT
    : (DIGIT)+
	;

ID
options
{
    paraphrase = "an identifier";
	testLiterals = true;
}
	:	(ALPHA | '_') (ALPHA | DIGIT | '_' | ':')*
	;

STRING
    : '\''! (~'\'')* '\''!
    ;
