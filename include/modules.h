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

#ifndef MODULES_H
#define MODULES_H

/* Cross OS compatibility macros */
#ifdef _WIN32

typedef HMODULE deno_module_t;

#define deno_modopen(file)		LoadLibrary(file)
/* deno_moderr in modules.c */
#define deno_modsym(handle, symbol)	(void *)GetProcAddress(handle, symbol)
#define deno_modclose(handle)		FreeLibrary(handle) ? 0 : 1
#define deno_modclearerr()		SetLastError(0)
#define MODULE_EXT			".dll"

#else


typedef void *	deno_module_t;

#ifdef HAS_RTLD_LOCAL
#define deno_modopen(file)		dlopen(file, RTLD_LAZY|RTLD_LOCAL)
#else
#define deno_modopen(file) 		dlopen(file, RTLD_LAZY)
#endif

#define deno_moderr()			dlerror()
#define deno_modsym(handle, symbol)	dlsym(handle, DL_PREFIX symbol)
#define deno_modclose(handle)		dlclose(handle)
#define deno_modclearerr()		errno = 0
#define MODULE_EXT			".so"

#endif

#endif

