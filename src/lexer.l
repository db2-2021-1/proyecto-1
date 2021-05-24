%{
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "parser_def.h"
#include "tree.h"
%}

%option caseless
%option noinput
%option nounput

%%
AND     { return AND;     }
BETWEEN { return BETWEEN; }
COPY    { return COPY;    }
CREATE  { return CREATE;  }
CSV     { return CSV;     }
DELETE  { return DELETE;  }
FROM    { return FROM;    }
HEADER  { return HEADER;  }
INDEX   { return INDEX;   }
INSERT  { return INSERT;  }
INT     { return INT;     }
INTO    { return INTO;    }
IS      { return IS;      }
ON      { return ON;      }
SELECT  { return SELECT;  }
TABLE   { return TABLE;   }
USING   { return USING;   }
VALUES  { return VALUES;  }
VARCHAR { return VARCHAR; }
WHERE   { return WHERE;   }

\"(\\.|\"\"|[^"\n])*\" {
#if _POSIX_C_SOURCE >= 200809L
	yylval.strval = strndup(yytext+1, yyleng-2);
#else
	yylval.strval = malloc(yyleng - 1);
	memcpy(yylval.strval, yytext + 1, yyleng - 2);
	yylval.strval[yyleng - 2] = '\0';
#endif
	return STRING;
}
[a-z][a-z0-9_]*        { yylval.strval = strdup(yytext); return NAME;   }
-?[0-9]+               { yylval.intval = atoi(yytext); return INTNUM; }

[*(),;] { return yytext[0]; }

[ \n\r\t]+ {}
%%

void parse_init(const char* str)
{
	yy_scan_string(str);
}

void parse_free()
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
}
