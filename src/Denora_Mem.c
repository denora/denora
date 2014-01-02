/* Memory Functions
 *
 *
 * (c) 2014 Denora Dev Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 */
#include "denora.h"

/*************************************************************************/

char ***DenoraCallocArray2D(int x, int y) 
{
	int i;
	char ***a;
	a = calloc(x, sizeof(char **));

	for(i = 0; i != x; i++) 
	{
		a[i] = calloc(y, sizeof(char *));
	}
	return a;
}

/*************************************************************************/

char **DenoraCallocArray(int x) 
{
	int i;
	char **a;
	a = calloc(x, sizeof(char **));

	return a;
}

