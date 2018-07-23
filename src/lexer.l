 /*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
%{
#include <stdlib.h>
#include "ast.h"
#include "parser.tab.h"
#include "special_vars.h"
void yyerror(struct _object **ast,char *s);
int yywrap(void);
unsigned int lang_mx;
%}

/*%option noyywrap*/

digit [0-9]
bindigit [0-1]
letter [a-zA-Z_]
hexletter [a-fA-F]
line_comment "//"
block_comment "/*"
shebang ^"#!"
identifier {letter}({letter}|{digit})*
qstring \"[^"\n]*["\n]
qstring2 \'[^'\n]*['\n]
ws [ \t\r]+
newline \n

%x xBLOCK_COMMENT
%x xSHEBANG
%x xSINGLE_LINE_COMMENT

%%

{digit}+	{
				yylval.int_val = atoi(yytext);
				//printf("%d\n",yylval.iValue);
				return INTEGER;
			}
			
{digit}+\.{digit}+ {yylval.float_val=(float)atof(yytext); return FLOAT;}

0x({digit}|{hexletter})+	{
				sscanf(yytext+2, "%X", &lang_mx);
				yylval.int_val=lang_mx;
				return INTEGER;
			}

0b({bindigit})+	{
        yylval.int_val = (int) strtol(yytext+2, NULL, 2);
				return INTEGER;
			}

"let"		return LET;
"func"	return FUNC;
"quit"		return QUIT;
"prn"		return PRN;
"if"		return IF;
"while"		return WHILE;
"else"		return ELSE;
"#t"		return TT;
"#f"		return NIL;
":t"		return TYPE;
"^^"		return POW;
"&&"		return AND;
"||"		return OR;
"=="		return EQ;
"!="		return NEQ;
"<="		return LE;
">="		return GE;
"^|^"		return b_xor;
"<<"		return shift_l;
">>"		return shift_r;
"<<<"		return rot_l;
">>>"		return rot_r;
"%%"		return REMINDER;
"$!"		return LAST_EVAL_VAL;
"(%"		return BEGIN_LISP_SYM;
"%)"		return END_LISP_SYM;
"->"		return APPLY;
".."		return BIND_SYM;
"!!"		return MAPPL_SYM;
"::"		return NAMESPACE_SYM;


[-()<>%+@*/#;,{}.$!"\[\]:\\='?~|&£^] {
				return *yytext;
			}


{identifier}	{ 
				//yylval.inVar = *yytext - 'a';
				//printf("Word %s\n",yytext);
				yylval.s_val=strdup(yytext);
				//printf("internal word=%s %s\n",yytext,yylval);
				return WORD;
			}
{qstring}	{ 
				yylval.s_val = strdup(yytext+1); 
				if (yylval.s_val[yyleng-2] != '"') 
				;//die("improperly terminated string"); 
				else 
				yylval.s_val[yyleng-2] = 0;
				return STRING;
			}
{line_comment}	{
			BEGIN(xSINGLE_LINE_COMMENT) ;
		}
<xSINGLE_LINE_COMMENT,xSHEBANG>[^\r\n]* {
	BEGIN(INITIAL) ;
}
{block_comment} {
	BEGIN(xBLOCK_COMMENT) ;
}
<xBLOCK_COMMENT>. ;
<xBLOCK_COMMENT>newline {
	++yylineno;
}
<xBLOCK_COMMENT>"*/" {
	BEGIN(INITIAL) ;
}
{shebang} {
	if(yylineno==1) BEGIN(xSHEBANG);
}
<<EOF>>		yyterminate();
{ws}		;	/* ignore whitespace */
{newline}	yylineno++;

.			{printf("%s ",yytext);yyerror(NULL,"Unknown character");}
%%
int yywrap(void) {
    return 1;
}
