%{
#include <string.h>

#include "parser.h"
#include "parser_def.h"
#include "tree.h"
%}

%option caseless

%%
AND     { return AND;     }
BETWEEN { return BETWEEN; }
CREATE  { return CREATE;  }
DELETE  { return DELETE;  }
FROM    { return FROM;    }
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
	yylval.strval = strndup(yytext+1, strlen(yytext)-2);
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
