/*
 *
 * (c) 2004-2014 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 *
 *
 */
#include "denora.h"

typedef struct enc_mod_
{
	char *(*encrypt)(const char *str);
	int (*is_crypt)(const char *str);
} EncryptMod;

EncryptMod encmod;

void initEncryptMod(void)
{
	encmod.encrypt = NULL;
	encmod.is_crypt = NULL;
}

/*************************************************************************/

void EncryptModSet_Handler(char *(*func) (const char *str))
{
	encmod.encrypt = func;
}

void EncryptModSet_IsCrypt(int (*func) (const char *str))
{
	encmod.is_crypt = func;
}

char *EncryptString(const char *str)
{
	return encmod.encrypt(str);
}


/*************************************************************************/

int is_crypted(const char *passwd)
{
	return encmod.is_crypt(passwd);
}

char *MakePassword(char *plaintext)
{
	return EncryptString((const char *) plaintext);
}

/*************************************************************************/

int ValidPassword(char *plaintext, char *checkvs)
{
	char *result;

	result = EncryptString((const char *) plaintext);
	if (!BadPtr(result) && !strcmp(result, checkvs))
	{
		free(result);
		return 1;
	}
	return 0;
}
