# Dutch language file.
#
# Denora (c) 2004-2013 Denora Team
# Contact us at dev@denorastats.org
#
# Dutch translation (c) 2005 Pieter Bootms
# Email: geniusdex@anope.org
# 
# 01/04/2006 New Maintainer dotslasher
# 
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
# 
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
# For example, "Espa?ol (Spanish)" or "Fran?ais (French)".

LANG_NAME
	Nederlands (Dutch)

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
	%b %d %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	Zo
	Ma
	Di
	Wo
	Do
	Vr
	Za
# %A
STRFTIME_DAYS_LONG
	Zondag
	Maandag
	Dinsdag
	Woensdag
	Donderdag
	Vrijdag
	Zaterdag
# %b
STRFTIME_MONTHS_SHORT
	Jan
	Feb
	Mar
	Apr
	Mei
	Jun
	Jul
	Aug
	Sep
	Okt
	Nov
	Dec
# %B
STRFTIME_MONTHS_LONG
	Januari
	Februari
	Maart
	April
	Mei
	Juni
	Juli
	Augustus
	September
	Oktober
	November
	December


# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Interne fout - aanvraag kan niet worden afgehandeld.
UNKNOWN_COMMAND
	Onbekend commando %s.
UNKNOWN_COMMAND_HELP
	Onbekend commando %s.  "/msg %s HELP" voor hulp.
SYNTAX_ERROR
	Gebruik: %s
MORE_INFO
	/msg %s HELP %s voor meer informatie.
NO_HELP_AVAILABLE
	Geen hulp beschikbaar over %s.

SERVICE_OFFLINE
	%s is momentaal uitgeschakeld.

SQL_DISABLED
	Je kan dit commando niet gebruiken, SQL code is uitgeschakeld.

STATS_UPTIME_DHMS
	Stats staan %d dagen, %02d:%02d:%02d aan
STATS_UPTIME_1DHMS
	Stats staan %d dag, %02d:%02d:%02d aan
STATS_UPTIME_HMS
	Stats staan %d uur, %d minuten en %d seconden
STATS_UPTIME_HM1S
	Stats staan %d uur, %d minuten en %d seconde
STATS_UPTIME_H1MS
	Stats staan %d uur, %d minuut en %d seconden aan
STATS_UPTIME_H1M1S
	Stats staan %d uur, %d minuut en %d seconde aan
STATS_UPTIME_1HMS
	Stats staan %d uur, %d minuten en %d seconden aan
STATS_UPTIME_1HM1S
	Stats staan %d uur, %d minuten en %d seconde aan
STATS_UPTIME_1H1MS
	Stats staan %d uur, %d minuut en %d seconden aan
STATS_UPTIME_1H1M1S
	Stats staan %d uur, %d minuut en %d seconde aan
STATS_UPTIME_MS
	Stats staan %d minuten en %d seconden aan
STATS_UPTIME_M1S
	Stats staan %d minuten en %d seconde aan
STATS_UPTIME_1MS
	Stats staan %d minuut en %d seconden aan
STATS_UPTIME_1M1S
	Stats staan %d minuut en %d seconde aan

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stas admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Toegang geweigerd.
PERMISSION_DENIED
	Toegang geweigerd.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Ja
SAY_NO
	Nee

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN is niet gedefineerd; kan niet herstarten. Start het
	configure script opnieuw en hercompileer Denora om het RESTART
	commando in te schakelen.

# Module strings

STAT_MODULE_LOADED
	Module %s is geladen
STAT_MODULE_UNLOADED
	Module %s is verwijderd
STAT_MODULE_LOAD_FAIL
	Kan module %s niet laden
STAT_MODULE_REMOVE_FAIL
	Kan module %s niet verwijderen
STAT_MODULE_LOAD_SYNTAX
	MODLOAD bestandsnaam
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD bestandsnaam
STAT_MODULE_NO_UNLOAD
	Deze module kan niet worden verwijderd.
STAT_MODULE_LIST_HEADER
	Huidige module lijst:
STAT_MODULE_LIST
	Module: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	%d modules geladen.
STAT_MODULE_INFO_LIST
	Module: %s Versie: %s Auteur: %s Geladen: %s
STAT_MODULE_CMD_LIST
	Verschaft commando: /msg %s %s
STAT_MODULE_MSG_LIST
	Verschaft IRCD afhandeling voor: %s
STAT_MODULE_NO_LIST
	Er zijn geen modules geladen
STAT_MODULE_NO_INFO
	Er is geen informatie beschikbaar over module %s
STAT_MODULE_INFO_SYNTAX
	MODINFO bestandsnaam
MODULE_HELP_HEADER
	De volgende commando's zijn geladen door een module:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	%S commando's:
	    STATUS       Geeft de huidige netwekstatus weer
	    LOGIN       Maakt u een Stats beheerder.
	    LOGOUT      Geef uw beheerdersrechten op.

STAT_HELP_STATUS
	Gebruik: STATUS
	
	Laat het huidige aantal servers, kanalen, en gebruikers zien
	dat verbonden is met het netwerk.

STAT_HELP_SHUTDOWN
	Gebruik: SHUTDOWN
	
	Laat Stats alle data bestanden wegschrijven en daarna zichzelf
	afsluiten.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_RESTART
	Gebruik: RESTART
	
	Laat Stats opnieuw opstarten.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_RELOAD
	Gebruik: RELOAD
	
	Laat Stats het configuratiebestand opnieuw inlezen.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_ADMIN_CMD
	Commando's beschikbaar voor Stats beheerders:
	    ADMIN       Add/Delete Stats Admins
	    CHANSTATS   Kanalen in de gaten houden voor statistieken
	    ENVINFO     Geef omgevingsinformatie weer
	    EXCLUDE     Sluit gebruikers van chanstats
	    EXPORT      Exporteer data naar XML
	    HTML        Exporteer data naar een HTML bestand
	    MODLOAD     Laad een module
	    MODUNLOAD   Verwijder een module
	    MODLIST     Geef alle geladen modules weer
	    MODINFO     Geef informatie over een geladen module
	    RESTART     Start Denora opnieuw op
	    RELOAD      Lees het configuratiebestand opnieuw in
	    SET         Set various run time options.
	    SHUTDOWN    Sluit Denora af

STAT_HELP_MODLOAD
	Gebruik: MODLOAD bestandsnaam
	
	Dit commando zorgt ervoor dat de module genaamd bestandsnaam
	in de modules map geladen wordt.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_MODUNLOAD
	Gebruik: MODUNLOAD bestandsnaam
	
	Dit commando zorgt ervoor dat de module genaamd bestandsnaam
	verwijderd wordt uit het geheugen.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_MODINFO
	Gebruik: MODINFO bestandsnaam
	
	Dit command geeft informatie weer over de module genaamd
	bestandsnaam.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_MODLIST
	Gebruik: MODLIST
	
	Geef alle momenteel geladen modules weer.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_EXCLUDE
	Gebruik: EXCLUDE {ADD|DEL|LIST} doel
	
	Sluit het gegeven doel uit van de statistieken.
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_EXPORT
	Gebruik: EXPORT wat
	
	Exporteert de gevraagde data naar een XML bestand, waarbij
	wat een van de volgende opties is:
	
	channels,users,tld,servers,stats,all
	
	channels: kanalen
	users:    gebruikers
	tld:      domeinstatistieken
	server:   servers
	stats:    statistieken
	all:      alles
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_HTML
	Gebruik: HTML
	
	Forceer het maken van het HTML bestand.

STAT_HELP_USERDEBUG
	Gebruik: USERDEBUG gebruiker
	
	Geef foutopsporingsinformatie weer over de  gegeven gebruiker.

STAT_HELP_LOGIN
	Gebruik: LOGIN naam wachtwoord
	
	Maakt u een Stats beheerder.

STAT_HELP_LOGOUT
	Gebruik: LOGOUT
	
	Geef uw Stats beheerders rechten op. Om deze terug te krijgen,
	kunt u hierna weer LOGIN gebruiken.

STAT_HELP_SET
	Gebruik: SET OPTION [ON|OFF]
	
	Stel verscheidene opties in.

	Possible options are:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]
	
	Dit commando is gelimiteerd tot Stats beheerders.

STAT_HELP_ENVINFO
	Gebruik: ENVINFO
	
	Geef omgevingsinformatie weer.
	
	Dit commando is gelimiteerd tot Stats beheerders.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	U bent nu een Stats beheerder.

STAT_ADMIN_LOGGED_OUT
	U bent nu geen Stats beheerder meer.

STAT_ADMIN_ALREADY_LOGGED
	U bent al bekend als een Stats beheerder!

STAT_LOGIN_SYNTAX
	LOGIN naam wachtwoord

STAT_RELOAD
	Het configuratiebestand is opnieuw ingelezen.

###########################################################################
#
# STATUS messages
#
###########################################################################

STAT_STATUS_SERVER_COUNT
	Statistieken van servers voor %s:
	Servers momenteel verbonden: %d
	Record aantal verbonden servers: %d

STAT_STATUS_CHAN_COUNT
	Statistieken van kanalen voor %s:
	Kanalen momenteel in gebruik: %d
	Record aantal gemaakte kanalen: %d

STAT_STATUS_USER_COUNT
	Statistieken van gebruikers voor %s:
	Gebruikers momenteel verbonden: %d
	Record aantal verbonden gebruikers: %d

STAT_STATUS_SYNTAX
	STATUS [SERVERS|CHANNELS|USERS]

STAT_STATUS_PAGE
	For more stats visit %s

# SET responses
STAT_SET_SYNTAX
	SET optie waarde
STAT_SET_DEBUG_ON
	Stats zijn nu in foutopsporingsmode.
STAT_SET_DEBUG_OFF
	Stats zijn nu niet meer in foutopsporingsmode.
STAT_SET_DEBUG_LEVEL
	Stats zijn nu in foutopsporingsmode (niveau %d).
STAT_SET_DEBUG_ERROR
	De waarde voor DEBUG moet ON, OFF, of een positief nummer zijn.
STAT_SET_UNKNOWN_OPTION
	Onbekende SET optie %s.
STAT_SET_SQL_ON
	SQL code zal vanaf nu gebruikt worden.
STAT_SET_SQL_OFF
	SQL code zal vanaf nu niet meer gebruikt worden.
STAT_SET_SQL_ERROR
	De waarde voor SQL moet ON of OFF zijn.
STAT_SET_SQL_ERROR_INIT
	Er is een fout opgetreden tijdens het initialiseren van SQL; zie uw logfiles voor details.
STAT_SET_SQL_ERROR_DISABLED
	SQL is niet ingesteld voor dit netweork. Wijzig het configuratiebestand (denora.conf).
STAT_SET_LIST_OPTION_ON
	%s is ingeschakeld.
STAT_SET_LIST_OPTION_OFF
	%s is uitgschakeld.
STAT_SET_HTML_ON
	Er zal voortaan een HTML bestand worden gemaakt.
STAT_SET_HTML_OFF
	Er zal vanaf nu geen HTML bestand meer worden gemaakt.
STAT_SET_HTML_ERROR
	De waarde voor HTML moet ON of OFF zijn.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Kanaalstatistieken stellen u in staat om kanaal in de gaten te
	houden zoals een eggdrop en informatie te krijgen over het
	aantal tekens of regels, etc...
	
	CHANSTATS ADD kanaal
	CHANSTATS DEL kanaal
	CHANSTATS LIST
	CHANSTATS SET kanaal FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER gebruiker1 gebruiker2
	CHANSTATS RENAME gebruiker1 gebruiker2
	
	Het CHANSTATS ADD commando voegt het gegeven kanaal toe aan de
	in de gaten gehouden kanaallijst.
	
	Het CHANSTATS DEL commando verwijdert het gegeven kaanaal van de
	in de gaten gehouden kanaallijst.
	
	Het CHANSTATS LIST commando toont je de lijst van de in de gaten gehouden
	kanalen op het netwerk.
	
	Het CHANSTATS SET commando activeert de fantasy commandos voor het
	gegeven kanaal. Je kan StatServ ze laten zien door middel van gewone
	berichtgeving in het kanaal (ON), of door middel van persoonlijke notices (NOTICE).
	
	Het CHANSTATS SUMUSER commando draagt alle stats van gebruiker2
	naar gebruiker1 en verwijdert gebruiker2.
	
	The CHANSTATS RENAME command renames user1 into user2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SET|SUMUSER|RENAME} [KANAAL|GEBRUIKER|OPTIE]

STAT_CHANSTATS_CHAN_ADDED
	%s wordt nu in de gaten gehouden voor kanaalstatistieken.

STAT_CHANSTATS_CHAN_LARGENET
	LargeNet staat aan, je kan geen nieuwe kanalen toevoegen.

STAT_CHANSTATS_LOGCHAN
	LogChannel can not be added to channel stats

STAT_CHANSTATS_NOT_VALID_CHAN
	You have not entered a valid channel name

STAT_CHANSTATS_CHAN_ALREADY
	%s wordt al in de gaten gehouden voor kanaalstatistieken.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD kanaal

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL kanaal

STAT_CHANSTATS_CHAN_DEL
	Kanaal %s wordt niet langer in de gaten gehouden voor statistieken.

STAT_CHANSTATS_CHAN_NOTFOUND
	Kanaal %s wordt niet in de gaten gehouden voor statistieken.

STATS_CHANSTATS_PART
	%s wordt niet langer in de gaten gehouden.

STATS_CHANSTATS_NOCHANNELS
	Er worden geen kanalen in de gaten gehouden.

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET kanaal FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	Fantasie commando's voor %s zijn nu ingeschakeld.

STATS_CHANSTATS_FANTASY_OFF
	Fantasie commando's voor %s zijn nu uitgeschakeld.

STATS_CHANSTATS_FANTASY_NOTICE
	Fantasie commando's voor %s zullen hun resultaat nu als een notitie naar de gebruiker sturen.

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER gebruiker1 gebruiker2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME gebruiker newgebruiker

STATS_CHANSTATS_SUMUSER_DONE
	De gegevens van %s zijn nu overgezet naar %s

STATS_CHANSTATS_RENAMEUSER_DONE
	Gebruiker %s is hernoemd naar %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} doel

STAT_EXCLUDE_ADDED
	%s wordt nu uitgesloten van statistieken.

STAT_EXCLUDE_ALREADY
	%s is al uitgesloten van statistieken.

STAT_EXCLUDE_DELETED
	%s wordt niet meer uitgesloten van statistieken.

STAT_EXCLUDE_NOT_FOUND
	%s wordt niet uitgesloten van statistieken.

STAT_EXCLUDE_LIST_HEADER
	Uitzonderingen lijst

# EXPORT responses
STATS_EXPORT_CHANNELS
	Kanalen zijn geexporteerd naar %s.

STATS_EXPORT_USERS
	Gebruikers zijn geexporteerd naar %s.

STATS_EXPORT_TLD
	Domeininformatie is geexporteerd naar %s.

STATS_EXPORT_SERVERS
	Servers zijn geexporteerd naar %s.

STATS_EXPORT_STATS
	Statistieken zijn geexporteerd naar %s.

STATS_EXPORT_ALL
	Alle informatie is geexporteerd naar %s.

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	%s bestaat niet in de statistieken data.

STATS_RENAMEUSER_EXISTING
	User %s already exists in the stats db.

STATS_HELP_CHANNEL_1
	De volgende fantasie commando's zijn beschikbaar:

STATS_HELP_CHANNEL_2
	%stop10  :   De top 10 gebruikers van het huidige kanaal

STATS_HELP_CHANNEL_3
	%stop    :   De top 3 gebruikers van het huidige kanaal

STATS_HELP_CHANNEL_4
	%sgtop10 :   De top 10 gebruikers van het hele netwerk

STATS_HELP_CHANNEL_5
	%sgtop   :   De top 3 gebruikers van het hele netwerk

STATS_HELP_CHANNEL_6
	%sstats  :   Gebruikerstatistieken voor het kanaal

STATS_HELP_CHANNEL_7
	%sgstats :   Gebruikerstatistieken voor het netwerk

STATS_HELP_CHANNEL_8
	%speak   :   Het maximum aantal gebruikers van het huidige kanaal

STATS_TOP10_CHANNEL
	Top 10 van %s

STATS_TOP_CHANNEL
	Top 3 van %s

STATS_TOP10_NETWORK
	Top 10 van het netwerk

STATS_TOP_NETWORK
	Top 3 van het netwerk

STATS_USER_NETWORK
	Netwerkstatistieken voor %s

STATS_USER_CHANNEL
	Kanaalstatistieken voor %s in %s

STATS_MESSAGE
	%2i %-16s letters: %s, woorden: %s, regels: %s, smileys: %s, acties: %s

STATS_MESSAGE_ONE
	letters: %s, woorden: %s, regels: %s, smileys: %s, acties: %s

STATS_CHANNEL_PEAK
	Kanaal gebruikers piek was %d

# ENVINFO responses
STAT_ENVINFO_HEADER
	Omgevingsinformatie voor Stats:
STAT_ENVINFO_VERSION
	Stats versie:             %s
STAT_ENVINFO_PROTOCOL
	IRCd protocol:            %s
STAT_ENVINFO_UPLINK
	Stats uplink:             %s
STAT_ENVINFO_SQL
	SQL code:                 %s
STAT_ENVINFO_HTML
	HTML code:                %s
STAT_ENVINFO_DEBUG
	Foutopsporing:            %s
STAT_ENVINFO_MODULESLOADED
	Aantal geladen modules:   %d
STAT_ENVINFO_LANGUAGE
	Netwerktaal:              %s
STAT_ENVINFO_FOOTER
	Einde van de omgevingsinformatie

STATS_HTML_EXPORTED
	Stats HTML bestand is gegenereerd

# HTML code
HTML_LANG
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
	<meta http-equiv="Content-Language" content="Dutch">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Netwerk Statistieken:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<tr><td>Huidig aantal gebruikers: </td> <td> %ld </td> <td>Record aantal gebruikers: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Huidig aantal kanalen: </td> <td> %ld </td> <td>Record aantal kanalen: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>Huidig aantal opers: </td> <td> %ld </td> <td>Record aantal opers: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Huidig aantal servers: </td> <td> %ld </td> <td>Record aantal servers: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Aantal afwezige gebruikers: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Netwerk Statistieken voor %s

HTML_TLD_TITLE
	Domeinstatistieken

HTML_TLD_HEADER
	<table><tr><th>Landcode</th><th>Land</th><th>Aantal</th><th>Totaal</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Servernaam</th></tr>

HTML_SERVER_CONTENT
	<tr><td>Server: </td><td><a href="#%s"> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td>Server: </td><td><a href="#%s"> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) geeft aan dat een server momenteel verbonden is<br>(Klik op een servernaam voor meer informatie)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>Dagelijkse netwerkstatistieken:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4">(Alle dagelijkse statistieken worden om middernacht bijgesteld naar 0)</td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Record aantal gebruikers vandaag: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Record aantal kanalen vandaag: </td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">Record aantal opers vandaag: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Record aantal servers vandaag: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Versie</th><th align=right>Aantal</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name="%s"></a>Server:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Huidige aantal gebruikers:</td><td>%ld (%ld)</td><td>Record: %ld op [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Huidige aantal opers:</td><td>%ld (%ld)</td><td>Rexord: %ld op [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Huidige ping:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Totaal aantal verbonden gebruikers:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>Hoogste ping:</td><td>%ld</td><td>op [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Server splitsingen</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Voor het laatst gezien:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>IrcOp Kills</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Server Kills</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Kanaal</th><th align=right>Leden</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Kanaal</th><th align=right>Totaal aantal binnenkomsten</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) geeft aan dat kanaal op dit moment actief is

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top 10</b> onvriendelijke kanalen

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Kanaal</th><th>Totaal aantal keer dat iemand het kanaal uit geschopt is</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) geeft aan dat het kanaal momenteel actief is

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Kanaal</th><th>Totaal aantal onderwerpen</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) geeft aan dat het kanaal actief is op dit moment

HTML_TOP10_TOPIC_TITLE
	Top10 Topic Wissels Kanalen

HTML_CUR_NETSTATS_TITLE
	Huidige Netwerk Statistieken

HTML_DAILY_TITLE
	Dagelijkse Netwerk Statistieken

HTML_TOP10_ONLINE_CHANS_TITLE
	Top10 Online Kanalen

HTML_SERVERLIST_TITLE
	Server Lijst

HTML_TOP10_CHANEVER_TITLE
	Top10 Kanalen Ooit

HTML_SERVERDETAILS_TITLE
	Server Details

HTML_TOP10_CLIENT_TITLE
	Top10 Client Versies

HTML_NETMAP_TITLE
	Network Map

HTML_VERSION
	Powered by <a href="http://www.denorastats.org/">Denora</a> version %s %s

HTML_MOST_UNWELCOME_TITLE
	Meest Onwelkome Kanalen

HTML_POPULAR_TITLE
	Populairste Kanalen

HTML_MOST_TOPIC_TITLE
	Meeste Topic Kanalen

HTML_NETSTATS_TITLE
	Netwerk Statistieken

NOW_STATS_ADMIN
	%s (%s) is nu een Stats beheerder.

STATS_AUTH_FAILED
	Identificatie mislukt voor %s.

STATS_ADMIN_LOGOUT
	%s is niet langer een Stats beheerder.

SHUTDOWN_SAVING_DB
	Bijwerken data bestanden bij afsluiten, wacht aub.

CANNOT_OPEN_LOG
	Waarschuwing: kan log bestand niet openen: %s

GLOBAL_LOAD_MOD
	%s heeft module %s geladen.

GLOBAL_UNLOAD_MOD
	%s heeft module %s verwijderd.

GLOBAL_FATAL_ERROR
	FATALE FOUT! %s: %s

CONFIG_PORT_ERROR
	Poortnummers moeten tussen 1025 en 65535 liggen

CONFIG_HOSTNAME_ERROR
	Missende hostname (hostnaam) in het connect blok

CONFIG_PROTOCOL_ERROR
	Missende IRCDModule (protocol) in het connect block

CONFIG_PORT_ERROR_NOTDEF
	Missende port (poort) in het connect block

CONFIG_PASSWD_ERROR
	Missende passwd (wachtwoord) in het connect block

CONFIG_PING_TO_LOW
	Ping frequentie moet minimaal 2 minuten zijn

CONFIG_SQLPING_TO_LOW
	SQL Ping Frequency must be 2 minutes or greater

CONFIG_UPTIME_TO_LOW
	Uptime frequentie moet minimaal 2 minuten zijn

CONFIG_DELAYMOD_T0MANY
	Te veel late modules (maximaal 128)

CONFIG_TABLE_USER_ERROR
	Missende users (gebruikers) in tables blok

CONFIG_TABLE_CHANBANS_ERROR
	Missende chanbans (kanaalverbanningen) in tables blok

CONFIG_TABLE_CHANEXCEPT_ERROR
	Missende chanexcept (kanaaluitzonderingen) in tables blok

CONFIG_TABLE_ISON_ERROR
	Missende ison (isop) in tables blok

CONFIG_TABLE_SERVER_ERROR
	Missende server (server) in tables blok

CONFIG_TABLE_GLINE_ERROR
	Missende gline (gline) in tables block

CONFIG_INVALID_LANG
	Ongeldig taal nummer

CONFIG_ID_HOST_ERROR
	Missende user/host (gebruiker/host) in identity blok

CONFIG_ID_LANG_ERROR
	Missende language (taal) in identity blok

CONFIG_ID_USER_ERROR
	Missende user (gebruiker) in identity blok

CONFIG_ID_DESC_ERROR
	Missende desc (beschrijving) in identity blok

CONFIG_ID_NAME_ERROR
	Missende name (naam) in identity blok

CONFIG_ERROR_POSTIVE_VALUE
	Een positieve waarde wordt verwacht

CONFIG_SQL_USER_ERROR
	Missende user (gebruiker) in sql blok

CONFIG_SQL_NAME_ERROR
	Missende name (naam) in sql blok

CONFIG_ADMIN_NAME_ERROR
	Missende name (naam) in admin blok

CONFIG_ADMIN_HOST_ERROR
	Missende hostname (hostnaam) in admin blok

CONFIG_ADMIN_PASS_ERROR
	Missende passwd (wachtwoord) in admin blok

CONFIG_TIMEOUT_PING_ERROR
	Missende pingfreq (ping frequentie) in timeout blok

CONFIG_TIMEOUT_SQL_ERROR
	Lacking sqlfreq definition in timeout block

CONFIG_TIMEOUT_UPTIME_ERROR
	Missende uptimefreq (uptime frequentie) in timeout blok

CONFIG_TIMEOUT_HTML_ERROR
	Missende html (HTML) in timeout blok

CONFIG_TIMEOUT_UPDATE_ERROR
	Missende update (bijwerken) in timeout blok

CONFIG_TIMEOUT_WARNING_ERROR
	Missende warning (waarschuwing) in timeout blok

CONFIG_TIMEOUT_READ_ERROR
	Missende read (lees) in timeout blok

CONFIG_INVALID_TIME
	Dit is geen geldig tijdstip

CONFIG_HTML_UPDATE_TIME
	HTML timeout moet minstens 2 minuten zijn

CONFIG_NETINFO_NAME_ERROR
	Missende name (naam) in netinfo blok

CONFIG_FILENAME_PID_ERROR
	Missende pid (pid) in files blok

CONFIG_FILENAME_MOTD_ERROR
	Missende motd (welkomstbericht) in files blok

CONFIG_FILENAME_CHANDB_ERROR
	Missende ChannelDB (kanaaldatabestand) in files blok

CONFIG_FILENAME_CTCP_ERROR
	Missende ctcpDB (ctcp databestand) in files blok

CONFIG_FILENAME_SERVER_ERROR
	Missende ServerDB (serverdatabestand) in files blok

CONFIG_FILENAME_CHANSTATS_ERROR
	Missende ChannelStatsDB (kanaalstatistiekendatabestand) in files blok

CONFIG_FILENAME_TLD_ERROR
	Missend TLDDB (domeindatabestand) in files blok

CONFIG_FILENAME_EXCLUDE_ERROR
	Missende EXCLUDEDB (uitsluitingsdatabestand) in files blok

CONFIG_SS_DESC_ERROR
	Missende real (echte naam) in statserv blok

CONFIG_SS_NAME_ERROR
	Missende nick (bijnaam) in statserv blok

CONFIG_SS_TOLONG
	Bijnaam is te lang

CONFIG_SS_ALIASDESC_ERROR
	Missende aliasreal (echte alias) in statserv blok

ALOG_FAILED_SQL_CONNECT
	Er is een fout opgetreden tijdens het verbinden met SQL: %s

ALOG_SQL_ENABLED
	MySQL is ingeschakeld.

ALOG_SQL_DISABLED
	MySQL is uitgeschakeld.

ALOG_SQL_NOTSET
	MySQL Fout: Stel alle vereiste configuratie opties in.

ALOG_HTML_ERR_WRITE
	Fout: kan niet naar bestand schrijven: %s

ALOG_DB_ERROR
	WAARSCHUWING! Databestand leesfout in %s

ALOG_DEBUG_DB_OK
	foutopsporing: %s succesvol gelezen

ALOG_DEBUG_UNKNOW_CMODE
	foutopsporing: onbekende kanaalmode %c

ALOG_DEBUG_LAST_LINE
	foutopsporing: laatste regel is %s

ALOG_DEBUG_REPORT_AT
	foutopsporing: rapporteer deze in de bugtracker op http://bugs.anope.org

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	kanaal: join ontvangen voor niet-bestaande gebruiker %s op kanaal %s

ALOG_DEBUG_CHANGE_MODE_CHAN
	foutopsporing: Modes voor %s worden veranderd in %s

ALOG_DEBUG_BAD_CHAN_MODE
	foutopsporing: Misvormde mode gedetecteerd in %s.

ALOG_DEBUG_MODE_NO_PARAM
	kanaal: mode %c%c zonder parameter (?) voor kanaal %s

ALOG_DEBUG_FINDCHAN_BAD
	Fout: findchan() aangeroepen met NULL waardes

ALOG_DEBUG_FINDSTATCHAN_BAD
	Fout: findstatschan() aangeroepen met NULL waardes

ALOG_DEBUG_SJOIN_NONEXISTANT
	gebruiker: SJOIN voor onbekende gebruiker %s op %s

ALOG_DEBUG_JOIN_NONEXISTANT
	gebruiker: JOIN voor onbekende gebruiker %s op %s

ALOG_DEBUG_JOIN
	foutopsporing: %s komt %s binnen

ALOG_DEBUG_KICK_NONEXISTANT
	gebruiker: KICK voor onbekende gebruiker %s op %s: %s

ALOG_DEBUG_KICKED
	foutopsporing: %s wordt uit %s geschopt

ALOG_DEBUG_PART_NONEXISTANT
	gebruiker: PART van onbekende gebruiker %s: %s

ALOG_DEBUG_PART
	foutopsporing: %s verlaat %s

ALOG_DEBUG_PART_BUG
	gebruiker: FOUT verlaten %s: kanaal gevonden maar c->chan is NULL

ALOG_NON_TS_MODE
	TSMODE is ingeschakeld maar MODE heeft geen geldige TS

ALOG_DEBUG_NO_FIND_CHAN
	foutopsporing: kan kanaal %s niet vinden

ALOG_DEBUG_TOPIC_TS
	foutopsporing: versleutelde TOPIC TS %s is geconverteerd naar %d

ALOG_TOPIC_NONEXISTANT
	kanaal: TOPIC %s voor onbekend kanaal %s

ALOG_ADD_BAN_BAD
	foutopsporing: add_ban() aangeroepen met NULL waardes

ALOG_ADD_BAN_ADDED
	foutopsporing: Verbanning %s toegevoegd aan kanaal %s

ALOG_ADD_EXCEPTION_ADDED
	foutopsporing: Uitzondering %s toegevoegd aan kanaal %s

ALOG_ADD_INVITE_ADDED
	foutopsporing: Uitnodiging %s toegevoegd aan kanaal %s

ALOG_CREATE_CHAN
	foutopsporing: Kanaal %s wordt aangemaakt

ALOG_DELETE_CHAN
	foutopsporing: Kanaal %s wordt verwijderd

ALOG_BAN_FREE_ERROR
	kanaal: FOUT opschonen %s: bans[%d] is NULL!

ALOG_EXCEPTION_FREE_ERROR
	FOUT opschonen %s: excepts[%d] is NULL!

ALOG_INVITE_FREE_ERROR
	kanaal: FOUT opschonen %s: invite[%d] is NULL!

ALOG_BAN_DEL_CHAN
	foutopsporing: Verbanning %s verwijderd van kanaal %s

ALOG_EXCEPTION_DEL_CHAN
	foutopsporing: Uitzondering %s verwijderd van kanaal %s

ALOG_INVITE_DEL_CHAN
	foutopsporing: Uitnodiging %s verwijderd van kanaal %s

ALOG_DEBUG_FLOOD_MODE
	foutopsporing: Vloedmode voor kanaal %s is ingesteld op %s

ALOG_NO_FLOOD
	geen vloedinstellingen

ALOG_KEY_SET_TO
	foutopsporing: Sleutel van kanaal %s is ingesteld op %s

ALOG_NO_KEY
	geen sleutel

ALOG_LIMIT_SET_TO
	foutopsporing: Limiet van kanaal %s is ingesteld op %u

ALOG_REDIRECT_SET_TO
	foutopsporing: Doorsturing van kanaal %s is ingesteld op %s

ALOG_NO_REDIRECT
	geen doorsturing

ALOG_DEBUG_BAD_FINDSGROUP
	Fout: findsgroup() aangeroepen met NULL waardes

ALOG_LARGENET_WARNING_1
	U heeft aangegeven Denora te draaien met de minste hoeveelheid aan data

ALOG_LARGENET_WARNING_2
	opgeslagen in SQL. Deze optie moet alleen gebruikt worden op een erg groot

ALOG_LARGENET_WARNING_3
	netwerk met tenminste 100.000 gebruikers of een niet-lokale SQL server.

CONFIG_ERROR_BASE
	Configuratie Fout: %s in de buurt van regel %d van %s

CONFIG_UNKNOWN_FILE
	onbekend

CONFIG_ERROR_OPENING
	Onmogelijk om configuratiebestand %s te openen

ALOG_DEBUG_RESOLVED_HOST
	foutopsporing: %s herleid tot %s

ALOG_DEBUG_BAD_SSTRDUP
	foutopsporing: sstrdup() aangeroepen met NULL waarde

ALOG_RESET_DAILY
	dagelijkste statistieken worden op nul gezet

ALOG_RESETTING_WEEKLY
	wekelijkse statistieken worden op nul gezet

ALOG_RESETTING_MONTHLY
	maandelijkste statistieken worden op nul gezet

ALOG_TRY_TO_DISPLAY
	foutopsporing: gepoogd tot het weergeven van bericht %s

ALOG_DONE_DISPLAY
	foutopsporing: klaar met weergeven van bericht %s

ALOG_UKNOWN_USER_MODE
	foutopsporing: onbekende mode : gebruikersmode %c (in %s)

ALOG_SWHOIS_ERROR
	SWHOIS gezet voor onbekende gebruiker %s.

ALOG_VHOST_FOR
	foutopsporing: %s verandert zijn/haar vhost in %s

ALOG_REALNAME_FOR
	foutopsporing: %s verandert zijn/haar echte naam in %s

ALOG_USERNAME_FOR
	foutopsporing: %s verandert zijn/haar gebruikersnaam in %s

ALOG_UMODE_NONEXISTANT
	gebruiker: MODE %s voor onbekende gebruiker %s: %s

ALOG_UNKNWON_MSG
	foutopsporing: onbekend bericht van de server (%s)

ALOG_TRYING_LOAD_MOD
	proberen om [%s] te laden

ALOG_STATUS
	status: [%d]

ALOG_TRYING_UNLOAD_MOD
	proberen om [%s] te verwijderen

RESTARTING
	Bezig met opnieuw starten...

RESTART_FAILED
	Opnieuw starten is mislukt

SAVING_FFF
	foutopsporing: Bezig met opslaan van FFF informatie naar de harde schijf

ALOG_SIGPIPE_WARNING
	Denora zal een tracering uitvoeren en doorgaan, maar Denora kan hierdoor onstabiel worden

ALOG_NO_STATS_BIN
	Herstart poging mislukt -- STATS_BIN niet geconfigureerd (herstart configure)

ALOG_ERR_REHASH_NO_MEM
	Er is een fout opgetreden tijdens het herlanden van het configuratiebestand: het geheugen is vol!

ALOG_ERR_REHASH
	Fout tijdenshet lezen van het configuratiebestand (SIGHUP ontvangen)

BACKTRACE_LINES
	Tracering: rapporteer de volgende regels

BACKTRACE_NOT_HERE
	Tracering: niet beschikbaar op dit platform - gebruik GDB

ALOG_DEBUG_SERVINTRO
	debug: Server geintroduceerd (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: Server geintroduceerd (%s) van %s

ALOG_DEBUG_PINGTIME
	debug: Ping Tijd van %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Error: findserver() aangeroepen met NULL waardes

ALOG_ERR_FINDSERVERSTATS
	Error: findserverstats() aangeroepen met NULL waardes

ALOG_ERR_FINDSERVERSUID
	Error: findserver_suid() aangeroepen met NULL waardes

ALOG_ERR_DEL_SERVSTATS
	Error: delete_serverstats() aangeroepen met NULL waardes

ALOG_DEL_SERVSTATS_FOR
	debug: delete_serverstats() aangeroepen voor %s

ALOG_DEL_SERVSTATS_DONE
	debug: delete_serverstats() voltooid

ALOG_ERR_DEL_SERVER
	Error: delete_server() called with NULL values

ALOG_DEL_SERVER_FOR
	debug: delete_server() called for %s

ALOG_DEL_SERVER_NOQUIT
	debug: delete_server() cleared all users for NOQUIT

ALOG_DEL_SERVER_DONE
	debug: delete_server() voltooid

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: sql_do_server_version onbekend versie formaat ontvangen

ALOG_MOD_TRY_LOAD
	proberen om [%s] te laden

ALOG_MOD_STATUS
	status: [%d][%s]

ALOG_MOD_ERROR
	Module fout: %s

ALOG_MOD_BE_ONLY_ONE
	Je kan geen 2 protocol modules laden.

ALOG_MOD_UNLOAD_SELF
	debug: %s requested unload...

ALOG_CMD_DISPLAY_START
	debug: proberen om commando %s te tonen

ALOG_CMD_DISPLAY_END
	debug: klaar met het tonen van commando %s

ALOG_CMD_DISPLAY_LIST
	debug: tonen van de commando lijst voor %s

ALOG_MSG_DISPLAY_START
	debug: proberend om bericht %s te tonen 

ALOG_MSG_DISPLAY_END
	debug: klaar met bericht %s te tonen

ALOG_MSG_DISPLAY_LIST
	debug: tonen van berichtenlijst voor %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: module CallBack toegevoegd: [%s] door uitvoering op %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST voor niet existerende gebruiker %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME voor niet existerende gebruiker %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME voor niet existerende gebruiker %s

STAT_ADMIN_ALREADY
	%s is reeds een stats admin

STAT_ADMIN_CREATED
	%s is nu een Stats Admin

STAT_ADMIN_DELETED
	%s is verwijdert worden van de Stats Admin

STAT_ADMIN_NOTADMIN
	%s is geen Stats Admin

STAT_ADMIN_SHOW
	Gebruiker: %s Hostmask: %s Taal ID: %d

STAT_HELP_ADMIN
	Het toevoegen of verwijderen van Admins heeft geen effect
	op beheerders die in het config bestand toegevoegd zijn.
	
	ADMIN ADD gebruikersnaam wachtwoord [hostmask] [taalid]
	ADMIN DEL gebruikersnaam
	ADMIN SETPASS gebruikersnaam nieuw-wachtwoord
	ADMIN SHOW gebruikersnaam
	ADMIN LIST
	
	Het ADMIN ADD commando voegt een gespecificeerde gebruiker toe.
	
	Het ADMIN DEL commando verwijdert een gespecificeerde gebruiker.
	
	Het ADMIN SETPASS commando stelt je in staat om het wachtwoord te
	wijzigen van een bepaalde admin.
	
	Het ADMIN SHOW commando geeft je meer informatie betreffende een
	administrator.
	
	Het ADMIN LIST commando geeft een lijst weer van alle momenteel
	in het systeem aanwezige beheerders weer.
	
	Vereist: Stats Admin

STAT_ADMIN_SYNTAX
	ADMIN [ADD|DEL|LIST] gebruiker wachtwoord [host] [taalnum]

STAT_ADMIN_DELETE_CONFIG
	Kan %s niet verwijderen. Deze is in het configuratie bestand aanwezig.

STAT_CHGPASS_OK
	Wachtwoord van %s gewijzigd.

STAT_CHGPASS_CONFIG
	Kan wachtwoorden van in het configuratie bestand aanwezige beheerder niet wijzigen.
