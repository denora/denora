#include "denora.h"

char *plaintext(const char *plaintext);
int is_plaintext(const char *plaintext);

int DenoraInit(int argc, char **argv)
{
	moduleAddAuthor("Denora");
	moduleAddVersion("2.0");
	moduleSetType(ENCMOD);

	EncryptModSet_Handler(plaintext);
	EncryptModSet_IsCrypt(is_plaintext);
	return MOD_CONT;
}


/**
 * Unload the module
 **/
void DenoraFini(void)
{

}


char *plaintext(const char *plaintext)
{
	return (char *) plaintext;
}

int is_plaintext(const char *plaintext)
{
	return 1;
}