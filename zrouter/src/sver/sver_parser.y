%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sver.h"

BEGIN_NAMESPACE_ADS
static sver * Sver = NULL;
void sver_parser_set_sver(sver *x) { Sver = x; }
END_NAMESPACE_ADS

USING_NAMESPACE_ADS;

extern int sver_parser_lex(void);
extern void sver_parser_error(char *s);
extern void sver_parser_unrec();
extern int sver_parser_lineno;
extern char * sver_parser_text;

static char sver_text1[4096];
static char sver_text2[4096];

%}

%token ID DOTID CONID OPEN CLOSE CURLY CLOSE_CURLY COMMA SEMI MODULE ENDMODULE 
%token INPUT INOUT OUTPUT WIRE BUSRANGE ASSIGN EQ

%%

top	: stmt_list
	;

stmt_list	: stmt
		| stmt_list stmt
		;

stmt	:	MODULE module_name OPEN mod_pin_list CLOSE SEMI { }
	|       MODULE module_name OPEN CLOSE SEMI { }
	|       MODULE module_name SEMI { }
	|	ENDMODULE { Sver->stop_module(); }
	|	INPUT tok_list SEMI
	|	INOUT tok_list SEMI
	|	OUTPUT tok_list SEMI
	|	WIRE tok_list SEMI
	|	ID { strcpy(sver_text1,sver_parser_text); } rest1 SEMI
	|	ASSIGN tok_list SEMI
	;

rest1	:	{ sver_parser_unrec(); }
	|	ID { strcpy(sver_text2,sver_parser_text); } rest2
	;

rest2	:	OPEN { Sver->start_inst(sver_text2,sver_text1); } 
			ipin_list CLOSE { Sver->stop_inst(); }
	|	ID tok_list { sver_parser_unrec(); }
	|	{ sver_parser_unrec(); }
	;

module_name	: ID { Sver->start_module(sver_parser_text); }

mod_pin_list	: mod_pin
		| mod_pin_list COMMA mod_pin
		;

mod_pin		: ID { Sver->add_module_pin(sver_parser_text); }

ipin_list	:
		| ne_ipin_list
		;

ne_ipin_list	: ipin
		| ne_ipin_list COMMA ipin
		;

id_list		: ID
		| CONID
		| id_list COMMA ID
		| id_list COMMA CONID
		;

ipin		: DOTID { strcpy(sver_text1,sver_parser_text); } OPEN
                   ipinval
                    CLOSE
		;

ipinval		: {}
		| ID { Sver->add_inst_pin(sver_text1,sver_parser_text); }
		| CONID {}
		| CURLY id_list CLOSE_CURLY {}
		;
		

tok_list	: 
		| tok_list one_tok
		;

one_tok	: ID
        | BUSRANGE
	| OPEN
	| CLOSE
	| COMMA
	| EQ
	;

%%
