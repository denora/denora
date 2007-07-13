# Italian language file.
#
# Denora (c) 2004-2007 Denora Team
# Contact us at dev@denorastats.org
#
# Italian translation (c) 2005-2007 Hal9000 (hal9000@denorastats.org)
#
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id$
#
# When translating this file to another language, keep in mind that the
# order of parameters for sprintf() is fixed in the source code, so your
# messages need to take the same parameters in the same order as the
# English messages do.  (Obviously, this doesn't hold for the strftime()
# format lines immediately below.)  If you can't get a natural translation
# of a message without changing the order of the parameters, let us know
# (dev@denorastats.org) which message is causing a problem and I'll see
# what I can do.
#
# In help messages, "%S" (capital S, not lowercase) refers to the name of
# the service sending the message; for example, in StatServ help messages,
# "%S" is replaced by "StatServ" (or whatever it is renamed to in
# denora.conf).  The %S's do not count as sprintf() parameters, so they can be
# rearranged, removed, or added as necessary.
#
# Also in help messages, please try to limit line lengths to 60 characters
# of text (not including the leading tab).  This length was chosen because
# it does not cause line wrap under default settings on most current IRC
# clients.  Remember that format characters (control-B, control-_) are not
# included in that 60-character limit (since they don't show on the user's
# screen).  Also remember that format specifiers (%S, etc.) will expand
# when displayed, so remember to take this into account; you can assume
# that the length of a pseudoclient name (%S replacement) will be eight
# characters, so reduce the maximum line length by 6 for every %S on a
# line.
#
# Finally, remember to put a tab at the beginning of every line of text
# (including empty lines).  This has to be a tab, not spaces.

###########################################################################
#
# Name of this language
#
###########################################################################

# For languages other than English, this string should have the following
# format:
#     language-name-in-language (language-name-in-English)
# For example, "Español (Spanish)" or "Français (French)".

LANG_NAME
	Italiano (Italian)

###########################################################################
#
# General messages
#
###########################################################################

# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#	%a = weekday name (short)	%H = hour
#	%b = month name (short)		%M = minute
#	%d = day of month		%S = second
#	%Y = year			%Z = time zone

# This is used as the format string for strftime() for a date and time
# together.
STRFTIME_DATE_TIME_FORMAT
	%d %b %Y %H:%M:%S %Z
# This is used as the format string for strftime() for a date alone in long
# format (English: including weekday).
STRFTIME_LONG_DATE_FORMAT
	%a %d %b %Y
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
STRFTIME_SHORT_DATE_FORMAT
	%d %b %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	Dom
	Lun
	Mar
	Mer
	Gio
	Ven
	Sab
# %A
STRFTIME_DAYS_LONG
	Domenica
	Lunedì
	Martedì
	Mercoledì
	Giovedì
	Venerdì
	Sabato
# %b
STRFTIME_MONTHS_SHORT
	Gen
	Feb
	Mar
	Apr
	Mag
	Giu
	Lug
	Ago
	Set
	Ott
	Nov
	Dic
# %B
STRFTIME_MONTHS_LONG
	Gennaio
	Febbraio
	Marzo
	Aprile
	Maggio
	Giugno
	Luglio
	Agosto
	Settembre
	Ottobre
	Novembre
	Dicembre


# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Errore interno - impossibile elaborare la richiesta.
UNKNOWN_COMMAND
	Comando %s sconosciuto.
UNKNOWN_COMMAND_HELP
	Comando %s sconosciuto.  "/msg %s HELP" per aiuto.
SYNTAX_ERROR
	Sintassi: %s
MORE_INFO
	/msg %s HELP %s per maggiori informazioni.
NO_HELP_AVAILABLE
	Nessun aiuto disponibile per %s.

SERVICE_OFFLINE
	%s è al momento offline.

SQL_DISABLED
	Non puoi usare questo comando, il codice SQL non è attivo.

STATS_UPTIME_DHMS
	Stats in funzione da %d giorni, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	Stats in funzione da %d giorno, %02d:%02d:%02d
STATS_UPTIME_HMS
	Stats in funzione da %d ore, %d minuti, %d secondi
STATS_UPTIME_HM1S
	Stats in funzione da %d ore, %d minuti, %d secondo
STATS_UPTIME_H1MS
	Stats in funzione da %d ore, %d minuto, %d secondi
STATS_UPTIME_H1M1S
	Stats in funzione da %d ore, %d minuto, %d secondo
STATS_UPTIME_1HMS
	Stats in funzione da %d ore, %d minuti, %d secondi
STATS_UPTIME_1HM1S
	Stats in funzione da %d hour, %d minuti, %d secondo
STATS_UPTIME_1H1MS
	Stats in funzione da %d hour, %d minuto, %d secondi
STATS_UPTIME_1H1M1S
	Stats in funzione da %d hour, %d minuto, %d secondo
STATS_UPTIME_MS
	Stats in funzione da %d minuti, %d secondi
STATS_UPTIME_M1S
	Stats in funzione da %d minuti, %d secondo
STATS_UPTIME_1MS
	Stats in funzione da %d minuto, %d secondi
STATS_UPTIME_1M1S
	Stats in funzione da %d minuto, %d secondo

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stats admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Accesso negato.
PERMISSION_DENIED
	Permesso negato.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Sì
SAY_NO
	No

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN non è definito; impossibile riavviare. Riesegui lo script \2configure\2 
	e ricompila Denora per abilitare il comando RESTART.

# Module strings

STAT_MODULE_LOADED
	Modulo %s caricato
STAT_MODULE_UNLOADED
	Modulo %s rimosso
STAT_MODULE_LOAD_FAIL
	Impossibile caricare il modulo %s
STAT_MODULE_REMOVE_FAIL
	Impossibile rimuovere il modulo %s
STAT_MODULE_LOAD_SYNTAX
	MODLOAD NomeFile
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD NomeFile
STAT_MODULE_NO_UNLOAD
	Questo modulo non può essere rimosso.
STAT_MODULE_LIST_HEADER
	Lista dei moduli attuali:
STAT_MODULE_LIST
	Modulo: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	%d moduli caricati.
STAT_MODULE_INFO_LIST
	Modulo: %s Versione: %s Autore: %s caricato: %s
STAT_MODULE_CMD_LIST
	Fornisce il comando: /msg %s %s
STAT_MODULE_MSG_LIST
	Fornisce il support IRCD per: %s
STAT_MODULE_NO_LIST
	Nessun modulo attualmente caricato
STAT_MODULE_NO_INFO
	Nessuna informazione disponibile sul modulo %s
STAT_MODULE_INFO_SYNTAX
	MODINFO NomeFile
MODULE_HELP_HEADER
	I seguenti comandi sono stati aggiunti da un modulo:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	%S comandi:
	    STATS       Mostra lo stato del network
	    LOGIN       Diventa un Amministratore Stats (Stats Admin).
	    LOGOUT      Esci dal tuo account admin.

STAT_HELP_STATS
	Sintassi: STATS
	
	Mostra il numero attuale di server, canali ed utenti
	connessi al network.

STAT_HELP_SHUTDOWN
	Sintassi: SHUTDOWN
	
	Termina le Stats salvando tutti i database.
	
	Limitato ai Stats Admin

STAT_HELP_RESTART
	Sintassi: RESTART
	
	Riavvia le Stats.
	(ovvero l'eseguibile viene terminato ed immediatamente riavviato).

STAT_HELP_RELOAD
	Sintassi: RELOAD
	
	Ricarica il file di configurazione.

STAT_HELP_ADMIN_CMD
	Comandi disponibili unicamnete agli Stats Admins:
	    ADMIN       Aggiungi/Rimuovi Amministratori
	    CHANSTATS   Crea canali monitorati per Stats
	    ENVINFO     Visualizza informazioni sull'ambiente
	    EXCLUDE     Escludi utenti o canali da Stats
	    EXPORT      Esporta dati in XML
	    HTML        Forza la creazione del file HTML
	    MODLOAD     Carica un modulo
	    MODUNLOAD   Rimuove un modulo
	    MODLIST     Elenca i moduli caricati
	    MODINFO     Informazioni su un modulo caricato
	    RESTART     Riavvia Denora
	    RELOAD      Ricarica il file di configurazione
	    SET         Imposta varie opzioni di runtime
	    SHUTDOWN    Termina le Stats salvando i database

STAT_HELP_MODLOAD
	Sintassi: MODLOAD NomeFile

	Questo comando carica il modulo chiamato NomeFile dalla cartella
	modules.

	Limitato a Stats Admins.

STAT_HELP_MODUNLOAD
	Sintassi: MODUNLOAD NomeFile

	Questo comando rimuove il modulo chiamato NomeFile dalla cartella
	modules.

	Limitato a Stats Admins.

STAT_HELP_MODINFO
	Sintassi: MODINFO NomeFile

	Questo comando elenca informazioni sul modulo caricato specificato

	Limitato a Stats Admins.

STAT_HELP_MODLIST
	Sintassi: MODLIST

	Elenca tutti i moduli attualmente caricati

	Limitato a Stats Admins.

STAT_HELP_EXCLUDE
	Sintassi: EXCLUDE {ADD|DEL|LIST} oggetto
	
	Escludi l'oggetto specificato (nickname o canale) da Stats
	
	Limitato a Stats Admins

STAT_HELP_EXPORT
	Sintassi: EXPORT cosa 
	
	Esporta i dati richiesti in XML, dove
	cosa è una delle seguenti opzioni:
	
	channels,users,tld,servers,stats,all
	
	Limitato ai Stats Admins

STAT_HELP_HTML
	Sintassi: HTML
	
	Forza la generazione del file HTML

STAT_HELP_USERDEBUG
	Sintassi: USERDEBUG utente 
	
	Mostra informazioni di debug per l'utente indicato

STAT_HELP_LOGIN
	Sintassi: LOGIN name password
	
	Diventa un Amministratore Stats (Stats Admin).

STAT_HELP_LOGOUT
	Sintassi: LOGOUT
	
	Esci dal tuo account admin.

STAT_HELP_SET
	Sintassi: SET OPTION [ON|OFF]

	Imposta varie opzioni di run time.

	Opzioni possibili:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	Limitato ai Stats Admins.

STAT_HELP_ENVINFO
	Sintassi: ENVINFO
	
	Visualizza informazioni d'ambiente
	
	Limitato ai Stats Admins.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	Sei ora un Amministratore Stats (Stats Admin).

STAT_ADMIN_LOGGED_OUT
	Sei ora uscito dal tuo account admin.

STAT_ADMIN_ALREADY_LOGGED
	Sei già un Amministratore Stats (Stats Admin)!

STAT_LOGIN_SYNTAX
	LOGIN nome password

STAT_RELOAD
	Il file di configurazione di Stats è stato ricaricato.

###########################################################################
#
# STATS messages
#
###########################################################################

STAT_STATS_SERVER_COUNT
	Statistiche di server per %s:
	Server attualmente linkati:  %d
	Picco dei server connessi: %d

STAT_STATS_CHAN_COUNT
	Statistiche di canali per %s:
	Canali attualmente utilizzati:  %d
	Picco dei canali creati: %d

STAT_STATS_USER_COUNT
	Statistiche di utenti per %s
	Utenti attualmente connessi:  %d
	Picco di utenti connessi: %d

STAT_STATS_SYNTAX
	STATS [SERVERS|CHANNELS|USERS]

STAT_STATS_PAGE
	Per più statistiche visita %s

# SET responses
STAT_SET_SYNTAX
	SET opzione impostazione
STAT_SET_DEBUG_ON
	Modalità debug attivata.
STAT_SET_DEBUG_OFF
	Modalità debug disattivata.
STAT_SET_DEBUG_LEVEL
	Modalità debug attivata (livello %d).
STAT_SET_DEBUG_ERROR
	L'impostazione per DEBUG dev'essere ON, OFF, oppure un numero positivo.
STAT_SET_UNKNOWN_OPTION
	Opzione SET %s sconosciuta.
STAT_SET_SQL_ON
	Codice SQL attivato.
STAT_SET_SQL_OFF
	Codice SQL disattivato.
STAT_SET_SQL_ERROR
	L'impostazione per SQL dev'essere ON oppure OFF.
STAT_SET_SQL_ERROR_INIT
	Errore durante l'inizializzazione di SQL, leggi i log per ulteriori informazioni.
STAT_SET_SQL_ERROR_DISABLED
	SQL non è configurato per questo network. Modifica il file
	di configurazione (denora.conf).
STAT_SET_LIST_OPTION_ON
	%s è attivato
STAT_SET_LIST_OPTION_OFF
	%s è disattivato
STAT_SET_HTML_ON
	L'output HTML è attivato.
STAT_SET_HTML_OFF
	L'output HTML è disattivato.
STAT_SET_HTML_ERROR
	L'impostazione per HTML dev'essere ON oppure OFF.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Le Statistiche di Canale ti permettono di monitorare canali ed ottenere
	informazioni statistiche quali numero di lettere, conteggio linee ecc...
	
	CHANSTATS ADD CHANNEL
	CHANSTATS DEL CHANNEL
	CHANSTATS LIST
	CHANSTATS SET FANTASY [ON|OFF|NOTICE]
	CHANSTATS SUMUSER utente1 utente2
	CHANSTATS RENAME utente1 utente2
	
	Il comando CHANSTATS ADD aggiunge il canale specificato alla lista dei canali
	da monitorare.
	
	Il comando CHANSTATS DEL rimuove il canale specificato dalla lista dei canali
	monitorati.
	
	Il comando CHANSTATS LIST mostra la lista dei canali monitorati sul network.
	
	Il comando CHANSTATS SET abilita i comandi fantasy per il canale specificato.
	Puoi impostare StatServ di mostrarli mediante dei normali messaggi sul canale
	(ON), oppure mediante notices.
	
	Il comando CHANSTATS SUMUSER trasferisce tutte le statistiche dall'utente2
	all'utente1 e cancella l'utente2.

	Il comando CHANSTATS RENAME rinomina l'utente1 in utente2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS [ADD|DEL|LIST|SET|SUMUSER|RENAME] [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	%s è ora monitorato per le statistiche di canale.

STAT_CHANSTATS_CHAN_LARGENET
	Non puoi aggiungere nuovi canali poichè l'opzione LargeNet è attivata.

STAT_CHANSTATS_LOGCHAN
	Il canale assegnato a LogChannel non può essere aggiunto alle statistiche di canale

STAT_CHANSTATS_NOT_VALID_CHAN
	Non hai digitato un nome di canale valido

STAT_CHANSTATS_CHAN_ALREADY
	%s è già monitorato per le statistiche di canale.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD CHAN

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL CHAN

STAT_CHANSTATS_CHAN_DEL
	Rimozione di %s dal monitoraggio delle statistiche di canale.

STAT_CHANSTATS_CHAN_NOTFOUND
	%s non corrisponde ai canali monitorati.

STATS_CHANSTATS_PART
	Non monitoro più %s

STATS_CHANSTATS_NOCHANNELS
	Nessun canale monitorato in lista

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET FANTASY [ON|OFF|NOTICE]

STATS_CHANSTATS_FANTASY_ON
	I comandi Fantasy per %s sono ora attivati

STATS_CHANSTATS_FANTASY_OFF
	I comandi Fantasy per %s sono ora disattivati

STATS_CHANSTATS_FANTASY_NOTICE
	Le risposte ai comandi fantasy per %s vengono ora mandate in notice all'utente

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER utente1 utente2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME utente1 utente2

STATS_CHANSTATS_SUMUSER_DONE
	I dati di %s sono stati trasferiti a %s

STATS_CHANSTATS_RENAMEUSER_DONE
	Utente %s rinominato in %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	Sintassi: EXCLUDE {ADD|DEL} oggetto

STAT_EXCLUDE_ADDED
	%s viene ora escluso da Stats

STAT_EXCLUDE_ALREADY
	%s è già escluso da Stats

STAT_EXCLUDE_DELETED
	%s è stato rimosso dalla lista exclude

STAT_EXCLUDE_NOT_FOUND
	%s non si trova nella lista exclude

STAT_EXCLUDE_LIST_HEADER
	Lista Exclude

# EXPORT responses
STATS_EXPORT_CHANNELS
	Esportato canali in %s

STATS_EXPORT_USERS
	Esportato utenti in %s

STATS_EXPORT_TLD
	Esportato top level domain in %s

STATS_EXPORT_SERVERS
	Esportato servers in %s

STATS_EXPORT_STATS
	Esportato stats in %s

STATS_EXPORT_ALL
	Esportato tutte le informazioni in %s

STATS_EXPORT_SYNTAX
	EXPORT [CHANNELS|USERS|TLD|SERVERS|STATS|ALL]

STATS_SUMUSER_NOTFOUND
	%s non esiste nel db delle stats.

STATS_RENAMEUSER_EXISTING
	%s esiste digià nel db delle stats.

STATS_HELP_CHANNEL_1
	I seguenti comandi fantasy sono disponibili

STATS_HELP_CHANNEL_2
	%stop10  :   La top 10 degli utenti del canale attuale

STATS_HELP_CHANNEL_3
	%stop    :   La top 3 degli utenti del canale attuale

STATS_HELP_CHANNEL_4
	%sgtop10 :   La top 10 degli utenti dell'intero network

STATS_HELP_CHANNEL_5
	%sgtop   :   La top 3 degli utenti dell'intero network

STATS_HELP_CHANNEL_6
	%sstats  :   Statistiche sull'utente per il canale

STATS_HELP_CHANNEL_7
	%sgstats :   Statistiche sull'utente per il network

STATS_HELP_CHANNEL_8
	%speak   :   Picco d'utenza del canale attuale

STATS_TOP10_CHANNEL
	Top 10 di %s

STATS_TOP_CHANNEL
	Top 3 di %s

STATS_TOP10_NETWORK
	Top 10 del network

STATS_TOP_NETWORK
	Top 3 del network

STATS_USER_NETWORK
	Statistiche del Network per %s

STATS_USER_CHANNEL
	Statistiche del Canale per %s su %s

STATS_MESSAGE
	%2i %-16s lettere: %s, parole: %s, linee: %s, faccine: %s, azioni: %s

STATS_MESSAGE_ONE
	lettere: %s, parole: %s, linee: %s, faccine: %s, azioni: %s

STATS_CHANNEL_PEAK
	Il picco d'utenza è stato di %d utenti

# ENVINFO responses
STAT_ENVINFO_HEADER
	Informazioni d'ambiente per Stats:
STAT_ENVINFO_VERSION
	Versione Stats:            %s
STAT_ENVINFO_PROTOCOL
	Protocollo IRCd:           %s
STAT_ENVINFO_UPLINK
	Uplink Stats:              %s
STAT_ENVINFO_SQL
	Codice SQL:                %s
STAT_ENVINFO_HTML
	Codice HTML:               %s
STAT_ENVINFO_DEBUG
	Debugging:                 %s
STAT_ENVINFO_MODULESLOADED
	Numero di moduli caricati: %d
STAT_ENVINFO_LANGUAGE
	Lingua del network:        %s
STAT_ENVINFO_FOOTER
	Fine delle informazioni d'ambiente

STATS_HTML_EXPORTED
	Il file HTML di Stats è stato generato

# HTML code

HTML_LANG
	<META HTTP-EQUIV="CHARSET" CONTENT="ISO-8859-1">
	<META HTTP-EQUIV="CONTENT-LANGUAGE" CONTENT="Italian">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Statistiche del Network:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<td>Utenti attuali: </td> <td> %ld </td> <td>Picco utenti: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Canali attuali: </td> <td> %ld </td> <td>Picco canali: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>Opers attuali: </td> <td> %ld </td> <td>Picco opers: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Server attuali: </td> <td> %ld </td> <td>Picco server: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Utenti impostati assenti: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Statistiche del network per %s

HTML_TLD_TITLE
	Statistiche dei domini top level

HTML_TLD_HEADER
	<table><tr><th>Codice Paese</th><th>Paese</th><th>Conteggio</th><th>Totale</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Nome server</th></tr>

HTML_SERVER_CONTENT
	<tr><td height="4">Server: </td><td height="4"><a href=#%s> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td height="4">Server: </td><td height="4"><a href=#%s> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) indica che il server è online al momento<br>(Clicca sul nome di un server per maggiori informazioni)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>Statistiche giornaliere del network:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4"><center>(Tutte le statistiche gioranliere vengono azzerate a mezzanotte)</center></td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Picco utenti giornaliero: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Picco canali giornaliero:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">Picco opers giornaliero: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Picco server giornaliero: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Versione</th><th align=right>Conteggio</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name=%s>Server:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Utenti attuali:</td><td>%ld (%ld)</td><td>Max %ld il [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Opers attuali:</td><td>%ld (%ld)</td><td>Max %ld il [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Ping attuale</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Totale utenti connessi:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>Ping più elevato</td><td>%ld</td><td>il [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Split dei server</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Visto per l'ultima volta:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>IrcOp Kills</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Server Kills</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Canale</th><th align=right>Utenti</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Canale</th><th align=right>Totale entrate</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) indica che il canale è attivo al momento

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> Canali non benvenuti

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Canale</th><th>Kick totali</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) indica che il canale è attivo al momento

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Canale</th><th>Topic totali</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) indica che il canale è attivo al momento

HTML_TOP10_TOPIC_TITLE
	Top10 Canali Cambio Topic

HTML_CUR_NETSTATS_TITLE
	Statistiche del network attuali

HTML_DAILY_TITLE
	Statistiche giornaliere del network

HTML_TOP10_ONLINE_CHANS_TITLE
	Top10 canali online

HTML_SERVERLIST_TITLE
	Lista server

HTML_TOP10_CHANEVER_TITLE
	Top10 assoluta canali

HTML_SERVERDETAILS_TITLE
	Dettagli server

HTML_TOP10_CLIENT_TITLE
	Top10 versioni client

HTML_NETMAP_TITLE
	Mappa network

HTML_VERSION
	Versione %s %s

HTML_MOST_UNWELCOME_TITLE
	Canali meno benvenuti

HTML_POPULAR_TITLE
	Canali popolari

HTML_MOST_TOPIC_TITLE
	Canali da più topic

HTML_NETSTATS_TITLE
	Statistiche del network

NOW_STATS_ADMIN
	%s (%s) è ora un Amministratore Stats (Stats Admin).

STATS_AUTH_FAILED
	Autentificazione fallita per %s

STATS_ADMIN_LOGOUT
	%s non è più un Amministratore Stats (Stats Admin).

SHUTDOWN_SAVING_DB
	Aggiornamento database in chiusura, prego attendere.

CANNOT_OPEN_LOG
	Attenzione: non posso aprire il logfile: %s

GLOBAL_LOAD_MOD
	%s caricato il modulo %s

GLOBAL_UNLOAD_MOD
	%s rimosso il modulo %s

GLOBAL_FATAL_ERROR
	ERRORE FATALE! %s: %s

CONFIG_PORT_ERROR
	I numeri delle porte devono essere tra 1025..65535

CONFIG_HOSTNAME_ERROR
	Errore nella definizione di hostname nel blocco connect

CONFIG_PROTOCOL_ERROR
	Errore nella definizione di IRCDModule (protocol) nel blocco connect

CONFIG_PORT_ERROR_NOTDEF
	Errore nella definizione di port nel blocco connect

CONFIG_PASSWD_ERROR
	Errore nella definizione di passwd nel blocco connect

CONFIG_PING_TO_LOW
	La frequenza dei ping dev'essere maggiore o uguale a 2 minuti

CONFIG_SQLPING_TO_LOW
	La frequenza del ping SQL dev'essere maggiore o uguale a 2 minuti

CONFIG_UPTIME_TO_LOW
	La frequenza di Uptime dev'essere maggiore o uguale a 2 minuti

CONFIG_DELAYMOD_T0MANY
	Troppi moduli ritardati (max 128)

CONFIG_TABLE_USER_ERROR
	Errore nella definizione di users nel blocco tables

CONFIG_TABLE_CHANBANS_ERROR
	Errore nella definizione di chanbans nel blocco tables

CONFIG_TABLE_CHANEXCEPT_ERROR
	Errore nella definizione di chanexcept nel blocco tables

CONFIG_TABLE_ISON_ERROR
	Errore nella definizione di ison nel blocco tables

CONFIG_TABLE_SERVER_ERROR
	Errore nella definizione di server nel blocco tables

CONFIG_TABLE_GLINE_ERROR
	Errore nella definizione di gline nel blocco tables

CONFIG_INVALID_LANG
	Numero della lingua non valido

CONFIG_ID_HOST_ERROR
	Errore nella definizione di user (host) nel blocco identity

CONFIG_ID_LANG_ERROR
	Errore nella definizione di language nel blocco identity

CONFIG_ID_USER_ERROR
	Errore nella definizione di user nel blocco identity

CONFIG_ID_DESC_ERROR
	Errore nella definizione di desc nel blocco identity

CONFIG_ID_NAME_ERROR
	Errore nella definizione di name nel blocco identity

CONFIG_ERROR_POSTIVE_VALUE
	È atteso un valore positivo

CONFIG_SQL_USER_ERROR
	Errore nella definizione di user nel blocco sql

CONFIG_SQL_NAME_ERROR
	Errore nella definizione di name nel blocco sql

CONFIG_ADMIN_NAME_ERROR
	Errore nella definizione di name nel blocco admin

CONFIG_ADMIN_HOST_ERROR
	Errore nella definizione di hostname nel blocco admin

CONFIG_ADMIN_PASS_ERROR
	Errore nella definizione di passwd nel blocco admin

CONFIG_TIMEOUT_PING_ERROR
	Errore nella definizione di pingfreq nel blocco timeout

CONFIG_TIMEOUT_SQL_ERROR
	Errore nella definizione di sqlfreq nel blocco timeout

CONFIG_TIMEOUT_UPTIME_ERROR
	Errore nella definizione di uptimefreq nel blocco timeout

CONFIG_TIMEOUT_HTML_ERROR
	Errore nella definizione di html nel blocco timeout

CONFIG_TIMEOUT_UPDATE_ERROR
	Errore nella definizione di update nel blocco timeout

CONFIG_TIMEOUT_WARNING_ERROR
	Errore nella definizione di warning nel blocco timeout

CONFIG_TIMEOUT_READ_ERROR
	Errore nella definizione di read nel blocco timeout

CONFIG_INVALID_TIME
	Questo non è un valore valido per un orario

CONFIG_HTML_UPDATE_TIME
	Il time out per l'HTML dev'essere maggiore o uguale a 2 minuti

CONFIG_NETINFO_NAME_ERROR
	Errore nella definizione di name nel blocco netinfo

CONFIG_FILENAME_PID_ERROR
	Errore nella definizione di pid nel blocco files

CONFIG_FILENAME_MOTD_ERROR
	Errore nella definizione di motd nel blocco files

CONFIG_FILENAME_CHANDB_ERROR
	Errore nella definizione di ChannelDB nel blocco files

CONFIG_FILENAME_CTCP_ERROR
	Errore nella definizione di ctcpDB nel blocco files

CONFIG_FILENAME_SERVER_ERROR
	Errore nella definizione di ServerDB nel blocco files

CONFIG_FILENAME_CHANSTATS_ERROR
	Errore nella definizione di ChannelStatsDB nel blocco files

CONFIG_FILENAME_TLD_ERROR
	Errore nella definizione di TLDDB nel blocco files

CONFIG_FILENAME_EXCLUDE_ERROR
	Errore nella definizione di EXCLUDEDB nel blocco files

CONFIG_SS_DESC_ERROR
	Errore nella definizione di real nel blocco statserv

CONFIG_SS_NAME_ERROR
	Errore nella definizione di nick nel blocco statserv

CONFIG_SS_TOLONG
	Nickname troppo lungo

CONFIG_SS_ALIASDESC_ERROR
	Errore nella definizione di aliasreal nel blocco statserv

ALOG_FAILED_SQL_CONNECT
	Impossibile collegarsi al database : %s

ALOG_SQL_ENABLED
	%s: è stato attivato.

ALOG_SQL_DISABLED
	MySQL: è stato disattivato.

ALOG_SQL_NOTSET
	Errore MySQL: Imposta tutte le opzioni di configurazione richieste.

ALOG_HTML_ERR_WRITE
	Errore: impossibile accedere al file per la scrittura: %s

ALOG_DB_ERROR
	ATTENZIONE! DB_READ_ERROR in %s

ALOG_DEBUG_DB_OK
	debug: %s letto con successo

ALOG_DEBUG_UNKNOW_CMODE
	debug: modo canale %c sconosciuto

ALOG_DEBUG_LAST_LINE
	debug: l'ultima linea è %s

ALOG_DEBUG_REPORT_AT
	debug: riporta queste al bugtracker, http://bugtracker.nomadirc.net

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Canale: ricevuto join per l'utente %s non esistente sul canale dall'ID %d

ALOG_DEBUG_CHANGE_MODE_CHAN
	debug: Cambio modo per %s in %s

AL0G_DEBUG_BAD_CHAN_MODE
	Debug: Modo malformato scovato su %s.

ALOG_DEBUG_MODE_NO_PARAM
	canale: modo %c%c senza parametro (?) per il canale %s

ALOG_DEBUG_FINDCHAN_BAD
	Errore: findchan() chiamato con valori NULL

ALOG_DEBUG_FINDSTATCHAN_BAD
	Errore: findstatschan() chiamato con valori NULL

ALOG_DEBUG_SJOIN_NONEXISTANT
	utente: SJOIN per l'utente non esistente %s su %s

ALOG_DEBUG_JOIN_NONEXISTANT
	utente: JOIN per l'utente non esistente %s su %s

ALOG_DEBUG_JOIN
	debug: %s entra in %s

ALOG_DEBUG_KICK_NONEXISTANT
	utente: KICK per l'utente non esistente %s su %s: %s

ALOG_DEBUG_KICKED
	debug: kickaggio di %s da %s

ALOG_DEBUG_PART_NONEXISTANT
	utente: PART dall'utente non esistente %s: %s

ALOG_DEBUG_PART
	debug: %s lascia %s

ALOG_DEBUG_PART_BUG
	utente: BUG parting %s: canale trovato ma c->chan NULL

ALOG_NON_TS_MODE
	TSMODE attivato ma MODE ha un TS non valido

ALOG_DEBUG_NO_FIND_CHAN
	debug: canale %s introvabile

ALOG_DEBUG_TOPIC_TS
	debug: TOPIC TS codificato %s convertito in %d

ALOG_TOPIC_NONEXISTANT
	canale: TOPIC %s per il canale non esistente %s

ALOG_ADD_BAN_BAD
	debug: add_ban chiamato con valori NULL

ALOG_ADD_BAN_ADDED
	debug: Aggiunto ban %s al canale %s

ALOG_ADD_EXCEPTION_ADDED
	debug: Aggiunta eccezione %s al canale %s

ALOG_ADD_INVITE_ADDED
	debug: Aggiunto invito %s al canale %s

ALOG_CREATE_CHAN
	debug: Creazione canale %s

ALOG_DELETE_CHAN
	debug: Rimozione canale %s

ALOG_BAN_FREE_ERROR
	canale: BUG freeing %s: bans[%d] è NULL!

ALOG_EXCEPTION_FREE_ERROR
	BUG freeing %s: excepts[%d] è NULL!

ALOG_INVITE_FREE_ERROR
	canale: BUG freeing %s: invite[%d] è NULL!

ALOG_BAN_DEL_CHAN
	debug: Rimosso ban %s dal canale %s

ALOG_EXCEPTION_DEL_CHAN
	debug: Rimossa eccezione %s dal canale %s

ALOG_INVITE_DEL_CHAN
	debug: Rimosso invito %s dal canale %s

ALOG_DEBUG_FLOOD_MODE
	debug: Modalità flood per il canale %s impostata a %s

ALOG_NO_FLOOD
	nessun'impostazione flood

ALOG_KEY_SET_TO
	debug: Chiave del canale %s impostata a %s

ALOG_NO_KEY
	nessuna chiave

ALOG_LIMIT_SET_TO
	debug: Limite del canale %s impostato a %u

ALOG_REDIRECT_SET_TO
	debug: Deviazione del canale %s impostata a %s

ALOG_NO_REDIRECT
	nessuna deviazione

ALOG_DEBUG_BAD_FINDSGROUP
	Errore: findsgroup() chiamato con valori NULL

ALOG_LARGENET_WARNING_1
	Hai scelto di avviare Denora coi minor dati possibili

ALOG_LARGENET_WARNING_2
	salvati in SQL, dovresti utilizzare questa opzione unicamente se ti trovi su di un

ALOG_LARGENET_WARNING_3
	network molto grande composto da 100,000 utenti o più, o se hai un database sql remoto

CONFIG_ERROR_BASE
	Errore di configurazione: %s vicino alla linea %d di %s

CONFIG_UNKNOWN_FILE
	sconosciuto

CONFIG_ERROR_OPENING
	Impossibile aprire il file di configurazione %s

ALOG_DEBUG_RESOLVED_HOST
	debug: %s risolto in %s

ALOG_DEBUG_BAD_SSTRDUP
	debug: sstrdup() chiamato con NULL-arg

ALOG_RESET_DAILY
	azzeramento statistiche giornaliere

ALOG_RESETTING_SUNDAY
	azzeramento statistiche settimanali (di domenica)

ALOG_RESETTING_MONDAY
	azzeramento statistiche settimanali (di lunedì)

ALOG_RESETTING_TUESDAY
	azzeramento statistiche settimanali (di martedì)

ALOG_RESETTING_WEDNESDAY
	azzeramento statistiche settimanali (di mercoledì)

ALOG_RESETTING_THURSDAY
	azzeramento statistiche settimanali (di giovedì)

ALOG_RESETTING_FRIDAY
	azzeramento statistiche settimanali (di venerdì)

ALOG_RESETTING_SATURDAY
	azzeramento statistiche settimanali (di sabato)

ALOG_RESETTING_MONTHLY
	azzeramento statistiche mensili

ALOG_TRY_TO_DISPLAY
	debug: cercando di mostrare il messaggio %s

ALOG_DONE_DISPLAY
	debug: mostrato il messaggio %s

ALOG_UKNOWN_USER_MODE
	debug: modo sconosciuto : usermode %c (in %s)

ALOG_SWHOIS_ERROR
	SWHOIS impostato per utente non esistente %s.

ALOG_VHOST_FOR
	debug: %s cambia il proprio vhost in %s

ALOG_REALNAME_FOR
	debug: %s cambia il proprio realname in %s

ALOG_USERNAME_FOR
	debug: %s cambia il proprio username in %s

ALOG_UMODE_NONEXISTANT
	utente: MODE %s per il nick non esistente %s: %s

ALOG_UNKNWON_MSG
	debug: messaggio sconosciuto dal server (%s)

ALOG_TRYING_LOAD_MOD
	cercando di caricare [%s]

ALOG_STATUS
	stato: [%d]

ALOG_TRYING_UNLOAD_MOD
	cercando di rimuovere [%s]

RESTARTING
	Riavvio in corso

RESTART_FAILED
	Riavvio non riuscito

SAVING_FFF
	debug: Salvando informazioni FFF sul disco

ALOG_SIGPIPE_WARNING
	Denora eseguirà un backtrace e continuerà a lavorare, ma potrebbe diventare instabile

ALOG_NO_STATS_BIN
	Tentativo di riavvio non riuscito--STATS_BIN non definito (riesegui configure)

ALOG_ERR_REHASH_NO_MEM
	Errore durante il ricaricamento del file di configurazione, nessuna memoria disponibile!

ALOG_ERR_REHASH
	Errore nella lettura del file di configurazione (Ricevuto SIGHUP)

BACKTRACE_LINES
	Backtrace: riporta le seguenti linee

BACKTRACE_NOT_HERE
	Backtrace: non disponibile su questa piattaforma - usa GDB

ALOG_DEBUG_SERVINTRO
	debug: Server introdotto (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: Server introdotto (%s) da %s

ALOG_DEBUG_PINGTIME
	debug: Tempo ping da %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Errore: findserver() chiamato con valori NULL

ALOG_ERR_FINDSERVERSTATS
	Errore: findserverstats() chiamato con valori NULL

ALOG_ERR_FINDSERVERSUID
	Errore: findserver_suid() chiamato con valori NULL

ALOG_ERR_DEL_SERVSTATS
	Errore: delete_serverstats() chiamato con valori NULL

ALOG_DEL_SERVSTATS_FOR
	debug: delete_serverstats() chiamato per %s

ALOG_DEL_SERVSTATS_DONE
	debug: delete_serverstats() completato

ALOG_ERR_DEL_SERVER
	Errore: delete_server() chiamato con valori NULL

ALOG_DEL_SERVER_FOR
	debug: delete_server() chiamato per %s

ALOG_DEL_SERVER_NOQUIT
	debug: delete_server() rimosso tutti gli utenti per NOQUIT

ALOG_DEL_SERVER_DONE
	debug: delete_server() completato

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: sql_do_server_version ricevuto formato versione sconosciuto

ALOG_MOD_TRY_LOAD
	caricamento di [%s]

ALOG_MOD_STATUS
	stato: [%d][%s]

ALOG_MOD_ERROR
	Errore modulo: %s

ALOG_MOD_BE_ONLY_ONE
	Non puoi caricare 2 moduli di protocollo.

ALOG_MOD_UNLOAD_SELF
	debug: %s ha richiesto la rimozione...

ALOG_CMD_DISPLAY_START
	debug: mostrando il comando %s

ALOG_CMD_DISPLAY_END
	debug: comando %s mostrato

ALOG_CMD_DISPLAY_LIST
	debug: mostrando lista comandi per %s

ALOG_MSG_DISPLAY_START
	debug: mostrando messaggio %s

ALOG_MSG_DISPLAY_END
	debug: messaggio %s mostrato

ALOG_MSG_DISPLAY_LIST
	debug: mostrando lista messaggi per %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: aggiunto modulo CallBack: [%s] verrà eseguito il %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST per utente inesistente %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME per utente inesistente %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME per utente inesistente %s

STAT_ADMIN_ALREADY
	%s è già uno Stats Admin

STAT_ADMIN_CREATED
	%s è ora uno Stats Admin

STAT_ADMIN_DELETED
	%s è stato rimosso da Stats Admin

STAT_ADMIN_NOTADMIN
	%s non è uno Stats Admin

STAT_ADMIN_SHOW
	Utente: %s Maschera: %s ID Lingua: %d

STAT_HELP_ADMIN
	L'aggiunta o la rimoione di Stats Admin non influenza gli
	admin aggiunti nel file di configurazione.	
	
	ADMIN ADD utente password [hostmask] [numero-lingua]
	ADMIN DEL utente
	ADMIN SETPASS utente nuova-password
	ADMIN SHOW utente
	ADMIN LIST
	
	Il comando ADMIN ADD aggiunge l'utente specificato a Stats Admin
	
	Il comando ADMIN DEL rimuove l'utente specificato da Stats Admin
	
	Il comando ADMIN SETPASS permette di cambiare la password
	all'admin specificato
	
	Il comando ADMIN SHOW permette di visualizzare  le informazioni
	sull'admin specificato
	
	Il comando ADMIN LIST mostra una lista di tutti gli amministratori
	configurati in questo momento
	
	Limitato a Stats Admin

STAT_ADMIN_SYNTAX
	ADMIN [ADD|DEL|LIST] user pass [host] [langnum]

STAT_ADMIN_DELETE_CONFIG
	Impossibile rimuovere %s perchè si trova nel file di configurazione

STAT_CHGPASS_OK
	La password di %s è stata modificata

STAT_CHGPASS_CONFIG
	Impossibile modificare password presenti nel file di configurazione
