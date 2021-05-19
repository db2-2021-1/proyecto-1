%{
#include <string.h>

#include "parser.h"
%}

%option caseless

%%
AND     { return AND;     }
BETWEEN { return BETWEEN; }
DELETE  { return DELETE;  }
FROM    { return FROM;    }
INSERT  { return INSERT;  }
INTO    { return INTO;    }
SELECT  { return SELECT;  }
TABLE   { return TABLE;   }
VALUES  { return VALUES;  }
WHERE   { return WHERE;   }

[a-z][a-z0-9_]* { return NAME; }

[ \n\r\t]+ {}
%%
