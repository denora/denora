
/* Database file handling routines.
 *
 * (c) 2004-2012 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * Based on code from Thilo Schulz.
 *
 *
 *
 */

#include "denora.h"

static int curday = 0;

/*************************************************************************/

int new_open_db_read(DenoraDBFile * dbptr, char **key, char **value)
{
	*key = malloc(MAXKEYLEN);
	*value = malloc(MAXVALLEN);

	if (!FileExists(dbptr->filename))
	{
		/* Exit before trying to open if the file is not there */
		return DB_READ_ERROR;
	}

	SET_SEGV_LOCATION();
	if ((dbptr->fptr = FileOpen(dbptr->filename, FILE_READ)) == NULL)
	{
		DenoraFree(*key);
		*key = NULL;
		DenoraFree(*value);
		*value = NULL;
		return DB_READ_ERROR;
	}
	SET_SEGV_LOCATION();
	dbptr->db_version =
	    fgetc(dbptr->fptr) << 24 | fgetc(dbptr->fptr) << 16 | fgetc(dbptr->
	            fptr)
	    << 8 | fgetc(dbptr->fptr);

	if (ferror(dbptr->fptr))
	{
		alog(LOG_DEBUG, "debug: Error reading version number on %s",
		     dbptr->filename);
		DenoraFree(*key);
		*key = NULL;
		DenoraFree(*value);
		*value = NULL;
		return DB_READ_ERROR;
	}
	else if (feof(dbptr->fptr))
	{
		alog(LOG_DEBUG,
		     "debug: Error reading version number on %s: End of file detected",
		     dbptr->filename);
		DenoraFree(*key);
		*key = NULL;
		DenoraFree(*value);
		*value = NULL;
		SET_SEGV_LOCATION();
		return DB_EOF_ERROR;
	}
	else if (dbptr->db_version < 1)
	{
		alog(LOG_DEBUG, "debug: Invalid version number (%d) on %s",
		     dbptr->db_version, dbptr->filename);
		DenoraFree(*key);
		*key = NULL;
		DenoraFree(*value);
		*value = NULL;
		return DB_VERSION_ERROR;
	}
	SET_SEGV_LOCATION();
	return DB_READ_SUCCESS;
}

/*************************************************************************/

int new_open_db_write(DenoraDBFile * dbptr)
{
	SET_SEGV_LOCATION();

	if (!(dbptr->fptr = FileOpen(dbptr->filename, FILE_WRITE)))
	{
		return DB_WRITE_ERROR;
	}
	SET_SEGV_LOCATION();

	if (fputc(dbptr->core_db_version >> 24 & 0xFF, dbptr->fptr) < 0 ||
	        fputc(dbptr->core_db_version >> 16 & 0xFF, dbptr->fptr) < 0 ||
	        fputc(dbptr->core_db_version >> 8 & 0xFF, dbptr->fptr) < 0 ||
	        fputc(dbptr->core_db_version & 0xFF, dbptr->fptr) < 0)
	{
		alog(LOG_DEBUG, "debug: Error writing version number on %s",
		     dbptr->filename);
		return DB_WRITE_ERROR;
	}
	SET_SEGV_LOCATION();

	return DB_WRITE_SUCCESS;
}

/*************************************************************************/

void new_close_db(FILE * fptr, char **key, char **value)
{
	SET_SEGV_LOCATION();

	if (key && *key)
	{
		DenoraFree(*key);
		*key = NULL;
	}
	SET_SEGV_LOCATION();

	if (value && *value)
	{
		DenoraFree(*value);
		*value = NULL;
	}
	SET_SEGV_LOCATION();

	if (fptr)
	{
		fclose(fptr);
	}
}

/*************************************************************************/

int new_read_db_entry(char **key, char **value, FILE * fptr)
{
	char *string = *key;
	int character;
	int i = 0;

	**key = '\0';
	**value = '\0';
	SET_SEGV_LOCATION();

	while (1)
	{
		if ((character = fgetc(fptr)) == EOF)   /* a problem occurred reading the file */
		{
			if (ferror(fptr))
			{
				return DB_READ_ERROR;   /* error! */
			}
			return DB_EOF_ERROR;        /* end of file */
		}
		else if (character == BLOCKEND)         /* END OF BLOCK */
		{
			return DB_READ_BLOCKEND;
		}
		else if (character == VALUEEND)         /* END OF VALUE */
		{
			string[i] = '\0';   /* end of value */
			return DB_READ_SUCCESS;
		}
		else if (character == SEPARATOR)        /* END OF KEY */
		{
			string[i] = '\0';   /* end of key */
			string = *value;    /* beginning of value */
			i = 0;              /* start with the first character of our value */
		}
		else
		{
			if ((i == (MAXKEYLEN - 1)) && (string == *key))     /* max key length reached, continuing with value */
			{
				string[i] = '\0';       /* end of key */
				string = *value;        /* beginning of value */
				i = 0;          /* start with the first character of our value */
			}
			else if ((i == (MAXVALLEN - 1)) && (string == *value))      /* max value length reached, returning */
			{
				string[i] = '\0';
				return DB_READ_SUCCESS;
			}
			else
			{
				string[i] = character;  /* read string (key or value) */
				i++;
			}
		}
	}
}

/*************************************************************************/

int new_write_db_entry(const char *key, DenoraDBFile * dbptr,
                       const char *fmt, ...)
{
	char string[MAXKEYLEN + MAXVALLEN + 2], value[MAXVALLEN];   /* safety byte :P */
	va_list ap;
	unsigned int length;

	if (!dbptr)
	{
		return DB_WRITE_ERROR;
	}
	SET_SEGV_LOCATION();

	va_start(ap, fmt);
	ircvsnprintf(value, MAXVALLEN, fmt, ap);
	va_end(ap);

	if (!stricmp(value, "(null)"))
	{
		return DB_WRITE_NOVAL;
	}
	SET_SEGV_LOCATION();
	ircsnprintf(string, MAXKEYLEN + MAXVALLEN + 1, "%s%c%s", key,
	            SEPARATOR, value);
	length = strlen(string);
	string[length] = VALUEEND;
	length++;
	if (fwrite(string, 1, length, dbptr->fptr) < length)
	{
		alog(LOG_DEBUG, "debug: Error writing to %s", dbptr->filename);
		new_close_db(dbptr->fptr, NULL, NULL);
		alog(LOG_DEBUG, "debug: Restoring backup.");
		remove(dbptr->filename);
		rename(dbptr->temp_name, dbptr->filename);
		SET_SEGV_LOCATION();
		free(dbptr);
		dbptr = NULL;
		return DB_WRITE_ERROR;
	}
	SET_SEGV_LOCATION();
	return DB_WRITE_SUCCESS;
}

/*************************************************************************/

int new_write_db_endofblock(DenoraDBFile * dbptr)
{
	if (!dbptr)
	{
		return DB_WRITE_ERROR;
	}
	SET_SEGV_LOCATION();

	if (fputc(BLOCKEND, dbptr->fptr) == EOF)
	{
		alog(LOG_DEBUG, "debug: Error writing to %s", dbptr->filename);
		new_close_db(dbptr->fptr, NULL, NULL);
		return DB_WRITE_ERROR;
	}
	SET_SEGV_LOCATION();

	return DB_WRITE_SUCCESS;
}

/*************************************************************************/

void fill_db_ptr(DenoraDBFile * dbptr, int version, int core_version,
                 char *service, char *filename)
{
	char buffer[PATH_MAX];
	char buf[BUFSIZE];
	char tempbuf[BUFSIZE];

	int failgetcwd = 0;
	*buffer = '\0';
	*buf = '\0';
	*tempbuf = '\0';


#ifdef _WIN32
	if (_getcwd(buffer, PATH_MAX) == NULL)
	{
#else
	if (getcwd(buffer, PATH_MAX) == NULL)
	{
#endif
		alog(LOG_DEBUG, "debug: Unable to set Current working directory");
		failgetcwd = 1;
	}

	dbptr->db_version = version;
	dbptr->core_db_version = core_version;
	SET_SEGV_LOCATION();
	if (!BadPtr(service))
	{
		dbptr->service = sstrdup(service);
	}
	else
	{
		dbptr->service = sstrdup("");
	}

#ifndef _WIN32
	if (failgetcwd)
	{
		/* path failed just get the file name in there */
		dbptr->filename = sstrdup(filename);
		ircsnprintf(tempbuf, BUFSIZE, "%s.temp", filename);
	}
	else
	{
		ircsnprintf(buf, BUFSIZE, "%s/%s", buffer, filename);
		dbptr->filename = sstrdup(buf);
		ircsnprintf(tempbuf, BUFSIZE, "%s.temp", buf);
	}
	dbptr->temp_name = sstrdup(tempbuf);
#else
	if (failgetcwd)
	{
		/* path failed just get the file name in there */
		dbptr->filename = sstrdup(filename);
		ircsnprintf(tempbuf, BUFSIZE, "%s.temp", filename);
	}
	else
	{
		ircsnprintf(buf, BUFSIZE, "%s\\%s", buffer, filename);
		dbptr->filename = sstrdup(buf);
		ircsnprintf(tempbuf, BUFSIZE, "%s.temp", buf);
	}
	dbptr->temp_name = sstrdup(tempbuf);
#endif
	return;
}

/*************************************************************************/

/**
 * Renames a database
 *
 * @param name Database to name
 * @param ext Extention
 * @return void
 */
static void rename_database(char *name, char *ext)
{

	char destpath[PATH_MAX];

	ircsnprintf(destpath, sizeof(destpath), "backups/%s.%s", name, ext);
	if (rename(name, destpath) != 0)
	{
		alog(LOG_NORMAL, "Backup of %s failed.", name);
		denora_cmd_global(s_StatServ, "WARNING! Backup of %s failed.",
		                  name);
	}
}

/*************************************************************************/

/**
 * Removes old databases
 *
 * @return void
 */
static void remove_backups(void)
{

	char ext[9];
	char path[PATH_MAX];

	time_t t;
	struct tm tm;

	time(&t);
	t -= (60 * 60 * 24 * KeepBackupsFor);
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	tm = *localtime(&t);
#endif
	strftime(ext, sizeof(ext), "%Y%m%d", &tm);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", ChannelDB, ext);
	unlink(path);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", ctcpDB, ext);
	unlink(path);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", ServerDB, ext);
	unlink(path);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", ChannelStatsDB, ext);
	unlink(path);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", TLDDB, ext);
	unlink(path);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", excludeDB, ext);
	unlink(path);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", AdminDB, ext);
	unlink(path);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", statsDB, ext);
	unlink(path);

}

/*************************************************************************/

/**
 * Handles database backups.
 *
 * @return void
 */
void backup_databases(void)
{

	time_t t;
	struct tm tm;

	if (!KeepBackups)
	{
		return;
	}

	time(&t);
	tm = *localtime(&t);

	if (!curday)
	{
		curday = tm.tm_yday;
		return;
	}

	if (curday != tm.tm_yday)
	{

		char ext[9];

		send_event(EVENT_DB_BACKUP, 1, EVENT_START);
		alog(LOG_DEBUG, "Backing up databases");

		remove_backups();

		curday = tm.tm_yday;
		strftime(ext, sizeof(ext), "%Y%m%d", &tm);

		rename_database(ChannelDB, ext);
		rename_database(ctcpDB, ext);
		rename_database(ServerDB, ext);
		rename_database(ChannelStatsDB, ext);
		rename_database(TLDDB, ext);
		rename_database(excludeDB, ext);
		rename_database(AdminDB, ext);
		rename_database(statsDB, ext);
		send_event(EVENT_DB_BACKUP, 1, EVENT_STOP);
	}
}

/*************************************************************************/

void ModuleDatabaseBackup(char *dbname)
{

	time_t t;
	struct tm tm;

	if (!KeepBackups)
	{
		return;
	}

	time(&t);
	tm = *localtime(&t);

	if (!curday)
	{
		curday = tm.tm_yday;
		return;
	}

	if (curday != tm.tm_yday)
	{

		char ext[9];

		alog(LOG_DEBUG, "Backing up %s", dbname);

		ModuleRemoveBackups(dbname);

		curday = tm.tm_yday;
		strftime(ext, sizeof(ext), "%Y%m%d", &tm);

		rename_database(dbname, ext);
	}
}

/*************************************************************************/

void ModuleRemoveBackups(char *dbname)
{
	char ext[9];
	char path[PATH_MAX];

	time_t t;
	struct tm tm;

	time(&t);
	t -= (60 * 60 * 24 * KeepBackupsFor);
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	tm = *localtime(&t);
#endif
	strftime(ext, sizeof(ext), "%Y%m%d", &tm);

	ircsnprintf(path, sizeof(path), "backups/%s.%s", dbname, ext);
	unlink(path);
}

/*************************************************************************/
