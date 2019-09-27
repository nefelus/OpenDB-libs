%{
#include <stdlib.h>
#include "sver.h"
#include "sver_parser.h"
#ifdef WIN32
#include <io.h>
#endif

/*int sver_parser_lineno; */

%}

%option noyywrap

newline    \n
whitespace [ \t\r\f\b]+
identifier [\\a-zA-Z_$][\/\.a-zA-Z_0-9$\[\]]*
dotid      \.[\\a-zA-Z_$][\/\.a-zA-Z_0-9$\[\]]*
trail_comment    \/\/[^\n\r]*
start_com   \/\*
end_com     \*\/
busrange   \[[0-9]+:[0-9]+\]
conid      1\'b[0-9]
other      [^ \t\r\f\b\n]+

%s COMMENT

%%

{newline}     { ++sver_parser_lineno; }
{whitespace}  { }
<INITIAL>{start_com}        { BEGIN COMMENT; }
<INITIAL>{trail_comment}    { }
<INITIAL>"("           { return OPEN; }
<INITIAL>")"           { return CLOSE; }
<INITIAL>"{"           { return CURLY; }
<INITIAL>"}"           { return CLOSE_CURLY; }
<INITIAL>","           { return COMMA; }
<INITIAL>";"           { return SEMI; }
<INITIAL>"="           { return EQ; }
<INITIAL>assign        { return ASSIGN; }
<INITIAL>module        { return MODULE; }
<INITIAL>endmodule     { return ENDMODULE; }
<INITIAL>input         { return INPUT; }
<INITIAL>inout         { return INOUT; }
<INITIAL>output        { return OUTPUT; }
<INITIAL>wire          { return WIRE; }
<INITIAL>supply1       { return WIRE; }
<INITIAL>{busrange}    { return BUSRANGE; }
<INITIAL>{identifier}  { return ID; }
<INITIAL>{dotid}       { return DOTID; }
<INITIAL>{conid}       { return CONID; }

<COMMENT>{end_com}     { BEGIN INITIAL; }
<COMMENT>{other}       { }

%%
