#include "denora.h"

char *my_crypt(const char *plaintext);
int is_crypted(const char *passwd);

int DenoraInit(int argc, char **argv)
{
	moduleAddAuthor("Denora");
	moduleAddVersion("2.0");
	moduleSetType(ENCMOD);

	EncryptModSet_Handler(my_crypt);
	EncryptModSet_IsCrypt(is_crypted);

	return MOD_CONT;
}


/**
 * Unload the module
 **/
void DenoraFini(void)
{

}


char *my_crypt(const char *plaintext)
{
#if defined(HAVE_CRYPT_H) && defined(HAVE_CRYPT)
	unsigned long seed[2];
	char salt[] = "$1$........";
	const char *const seedchars =
	    "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char *password;
	int i;

	/* Generate a (not very) random seed.
	   You should do it better than this... */
	seed[0] = time(NULL);
	seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

	/* Turn it into printable characters from `seedchars'. */
	for (i = 0; i < 8; i++)
		salt[3 + i] = seedchars[(seed[i / 5] >> (i % 5) * 6) & 0x3f];

	/* Read in the user's password and encrypt it. */
	return crypt(plaintext, (char *) salt);
#endif
}


int is_crypted(const char *passwd)
{
#if defined(HAVE_CRYPT_H) && defined(HAVE_CRYPT)
	/* Check if the string matches $1$........$...................... */
	if (strlen(passwd) == 34 && strncmp("$1$",passwd, 3) == 0 && passwd[11] == '$')
	{
		return 1;
	}
#endif
	return 0;
}