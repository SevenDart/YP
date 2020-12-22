%option main
%%
[a-z]       {
            int x = (yytext[0] - 'a' + 1) % 26;
            yytext[0] = x + 'a';
            printf("%c", yytext[0]);
            }
[A-Z]       {
            int x = (yytext[0] - 'A' + 1) % 26;
            yytext[0] = x + 'A';
            printf("%c", yytext[0]);
            }
%%