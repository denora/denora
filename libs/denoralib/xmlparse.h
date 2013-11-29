char *myStrGetToken(const char *str, const char dilim, int token_number);
char *myStrGetOnlyToken(const char *str, const char dilim, int token_number);
char *myStrGetTokenRemainder(const char *str, const char dilim, int token_number);
char *myStrSubString(const char *src, int start, int end);
#define BUFSIZE 4096
char **alloc_array(int x);
char *strnrepl(char *s, int size, const char *old, const char *new);
int myNumToken(const char *str, const char dilim);

