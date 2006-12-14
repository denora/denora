# Spanish language file.
#
# Denora (c) 2004-2006 Denora Team
# Contact us at dev@nomadirc.net
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
# (dev@nomadirc.net) which message is causing a problem and I'll see
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
	Español (Spanish)

###########################################################################
#
# General messages
#
###########################################################################

# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#       %a = weekday name (short)       %H = hour
#       %b = month name (short)         %M = minute
#       %d = day of month               %S = second
#       %Y = year                       %Z = time zone

# This is used as the format string for strftime() for a date and time
# together.
STRFTIME_DATE_TIME_FORMAT
	%d %b %H:%M:%S %Y %Z
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
	Mie
	Jue
	Vie
	Sab
# %A
STRFTIME_DAYS_LONG
	Domingo
	Lunes
	Martes
	Miercoles
	Jueves
	Viernes
	Sabado
# %b
STRFTIME_MONTHS_SHORT
	Ene
	Feb
	Mar
	Abr
	May
	Jun
	Jul
	Ago
	Sep
	Oct
	Nov
	Dic
# %B
STRFTIME_MONTHS_LONG
	Enero
	Febrero
	Marzo
	Abril
	Mayo
	Junio
	Julio
	Agosto
	Septiembre
	Octubre
	Noviembre
	Diciembre

# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Error Interno - Imposible procesar peticion..
UNKNOWN_COMMAND
	Comando desconocido %s.
UNKNOWN_COMMAND_HELP
	Comando no conocido %s. "/msg %s HELP" para ayuda.
SYNTAX_ERROR
	Sintaxis: %s
MORE_INFO
	/msg %s HELP %s para mayor informacion.
NO_HELP_AVAILABLE
	No existe ayuda para %s.

SERVICE_OFFLINE
	%s esta momentaneamente offline.

SQL_DISABLED
	No puedes usar este comando, el codigo SQL esta deshabilitado.

STATS_UPTIME_DHMS
	Stats arriba %d dias, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	Stats arriba %d dia, %02d:%02d:%02d
STATS_UPTIME_HMS
	Stats arriba %d horas, %d minutos, %d segundos
STATS_UPTIME_HM1S
	Stats arriba %d horas, %d minutos, %d segundo
STATS_UPTIME_H1MS
	Stats arriba %d horas, %d minuto, %d segundos
STATS_UPTIME_H1M1S
	Stats arriba %d horas, %d minuto, %d segundo
STATS_UPTIME_1HMS
	Stats arriba %d hora, %d minutos, %d segundos
STATS_UPTIME_1HM1S
	Stats arriba %d hora, %d minutos, %d segundo
STATS_UPTIME_1H1MS
	Stats arriba %d hora, %d minuto, %d segundos
STATS_UPTIME_1H1M1S
	Stats arriba %d hora, %d minuto, %d segundo
STATS_UPTIME_MS
	Stats arriba %d minutos, %d segundos
STATS_UPTIME_M1S
	Stats arriba %d minutos, %d segundo
STATS_UPTIME_1MS
	Stats arriba %d minuto, %d segundos
STATS_UPTIME_1M1S
	Stats arriba %d minuto, %d segundo

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stats admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Acceso denegado.
PERMISSION_DENIED
	Permiso denegado.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Si
SAY_NO
	No

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN no definido; imposible partir nuevamente. Corre nuevamente el
	script \2Config\2 y recompila Denora para habilitar el comando RESTART.

# Module strings

STAT_MODULE_LOADED
	Modulo %s cargado
STAT_MODULE_UNLOADED
	Modulo %s descargado
STAT_MODULE_LOAD_FAIL
	Imposible cargar modulo %s
STAT_MODULE_REMOVE_FAIL
	Imposible quitar modulo %s
STAT_MODULE_LOAD_SYNTAX
	MODLOAD NombreArchivo
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD NombreArchivo
STAT_MODULE_NO_UNLOAD
	Este modulo no puede ser descargado.
STAT_MODULE_LIST_HEADER
	Lista actual de modulos:
STAT_MODULE_LIST
	Modulo: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	%d Modulos cargados.
STAT_MODULE_INFO_LIST
	Modulo: %s Version: %s Autor: %s cargado: %s
STAT_MODULE_CMD_LIST
	Entregando comando: /msg %s %s
STAT_MODULE_MSG_LIST
	Entregando control de IRCD para: %s
STAT_MODULE_NO_LIST
	No hay modulos cargados.
STAT_MODULE_NO_INFO
	No hay informacion disponible sobre el modulo %s
STAT_MODULE_INFO_SYNTAX
	MODINFO NombreArchivo
MODULE_HELP_HEADER
	Los siguientes comandos han sido cargados por un modulo:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	Comandos de %S:
	    STATS       Muestra el estado de la red
	    LOGIN       Te hace un administrador de Stats.
	    LOGOUT      Finaliza tu sesion de administrador.

STAT_HELP_STATS
	Sintaxis: STATS
	
	Muestra el numero actual de servidores, canales y usuarios
	conectados a la red.

STAT_HELP_SHUTDOWN
	Sintaxis: SHUTDOWN
	
	Termina el programa de Stats, guardando los cambios.
	
	Limitado a Stats Admin

STAT_HELP_RESTART
	Sintaxis: RESTART
	
	Reinicia a Denora, guardando los cambios.
	(i.e. salir e inmediatamente correr el ejecutable denuevo).

STAT_HELP_RELOAD
	Sintaxis: RELOAD
	
	Hace que Denora recargue el archivo de configuracion.

STAT_HELP_ADMIN_CMD
	Comandos disponibles solo para Stats Admins:
	    ADMIN       Add/Delete Stats Admins
	    CHANSTATS   Monitorea canales para conseguir estadisticas
	    ENVINFO     Muestra information del entorno
	    EXCLUDE     Excluye a canales, usuarios de las estadisticas
	    EXPORT      Exporta datos a XML
	    HTML        Fuerza la creacion del archivo HTML
	    MODLOAD     Carga un modulo
	    MODUNLOAD   Descarga un modulo
	    MODLIST     Lista los modulos cargados
	    MODINFO     Informacion sobre un modulo cargado
	    RESTART     Hace que Denora reinicie
	    RELOAD      Hace que Denora recargue el archivo de configuracion
	    SET         Configura opciones en tiempo de ejecucion.
	    SHUTDOWN    Termina el programa Stats guardando los cambios

STAT_HELP_MODLOAD
	Sintaxis: MODLOAD NombreArchivo

	Este comando carga el modulo llamado NombreArchivo del
	directorio de modulos.

	Limitado a Stats Admins.

STAT_HELP_MODUNLOAD
	Sintaxis: MODUNLOAD NombreArchivo

	Este comando descarga el modulo llamado NombreArchivo del
	directorio de modulos.

	Limitado a Stats Admins.

STAT_HELP_MODINFO
	Sintaxis: MODINFO NombreArchivo

	Este comando lista informacion sobre el modulo cargado pedido

	Limitado a Stats Admins.

STAT_HELP_MODLIST
	Sintaxis: MODLIST

	Lista todos los modulos actualmente cargados.

	Limitado a Stats Admins.

STAT_HELP_EXCLUDE
	Sintaxis: EXCLUDE {ADD|DEL} blanco
	
	Excluye al blanco dado de las Estadisticas
	
	Limitado a Stats Admins

STAT_HELP_EXPORT
	Sintaxis: EXPORT que 
	
	Exporta los datos pedidos a XML, donde que
	es una de las siguientes opciones:
	
	channels,users,tld,servers,stats,all
	
	Limitado a Stats Admins

STAT_HELP_HTML
	Sintaxis: HTML
	
	Fuerza la creacion del archivo HTML

STAT_HELP_USERDEBUG
	Sintaxis: USERDEBUG usuario 
	
	Devuelve informacion de Depuracion sobre el usuario dado

STAT_HELP_LOGIN
	Sintaxis: LOGIN nombre password
	
	Te hace un Stats Administrator.

STAT_HELP_LOGOUT
	Sintaxis: LOGOUT
	
	Sale de tu cuenta de administrador.

STAT_HELP_SET
	Syntax: SET OPTION [ON|OFF]

	Configura variadas opciones de ejecucion.

	Possible options are:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	Limitado a Stats Admins.

STAT_HELP_ENVINFO
	Sintaxis: ENVINFO
	
	Muestra informacion del entorno.
	
	Limitado a Stats Admins.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	Ahora eres un administrador de Estadisticas.

STAT_ADMIN_LOGGED_OUT
	Has salido de tu cuenta de administrador.

STAT_ADMIN_ALREADY_LOGGED
	Ya eres un Stats Administrator!

STAT_LOGIN_SYNTAX
	LOGIN nombre clave

STAT_RELOAD
	El archivo de configuracion de Stats ha sido recargado.

###########################################################################
#
# STATS messages
#
###########################################################################

STAT_STATS_SERVER_COUNT
	Estadistica de servidores para %s:
	
	Servidores actualmente conectados:  %d
	Maximo de servidores conectados:    %d

STAT_STATS_CHAN_COUNT
	Estadisticas de canales para %s:
	
	Canales actualmente usuados:  %d
	Maximo de canales usuados:    %d

STAT_STATS_USER_COUNT
	Estadisticas de usuarios para %s
	
	Usuarios actualmente conectados:  %d
	Maximo de usuarios conectados:    %d

STAT_STATS_SYNTAX
	STATS [SERVERS|CHANNELS|USERS]

STAT_STATS_PAGE
	For more stats visit %s

# SET responses
STAT_SET_SYNTAX
	SET opcion configuracion
STAT_SET_DEBUG_ON
	Stats ahora estan en modo debug. 
STAT_SET_DEBUG_OFF
	Stats ya no estan en modo debug. 
STAT_SET_DEBUG_LEVEL
	Stats ahora estan en modo debug (nivel %d).
STAT_SET_DEBUG_ERROR
	Opciones para DEBUG pueden ser ON, OFF, o un numero positivo.
STAT_SET_UNKNOWN_OPTION
	Opcion de SET desconocida %s.
STAT_SET_SQL_ON
	Codigo SQL sera usado.
STAT_SET_SQL_OFF
	Codigo SQL no sera usado.
STAT_SET_SQL_ERROR
	Opciones para SQL deben ser ON u OFF.
STAT_SET_SQL_ERROR_INIT
	Error durante el inicio de SQL, revisa tus logs para corregir.
STAT_SET_SQL_ERROR_DISABLED
	SQL no esta configurado para esta red. Por favor edita el archivo 
	de configuracion (denora.conf).
STAT_SET_LIST_OPTION_ON
	%s esta habilitado
STAT_SET_LIST_OPTION_OFF
	%s esta deshabilitado
STAT_SET_HTML_ON
	Salida de HTML sera usada.
STAT_SET_HTML_OFF
	Salida de HTML no sera usada.
STAT_SET_HTML_ERROR
	Opciones para HTML deben ser ON u OFF.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Channel Stats permite monitorear canales tal como los eggdrops
	y obtener informacion sobre la cantidad de letras, lineas, etc..
	
	CHANSTATS ADD canal
	CHANSTATS DEL canal
	CHANSTATS LIST
	CHANSTATS SET canal FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER usuario1 usuario2
	CHANSTATS RENAME usuario1 usuario2
	
	El comando CHANSTATS ADD agrega el canal dado a la lista de
	canales monitorizados.
	
	El comando CHANSTATS DEL elimina el canal dado de la lista de
	canales monitorizados.
	
	El comando CHANSTATS LIST muestra la lista de canales
	monitorizados de la red.
	
	El comando CHANSTATS SET activa los comandos de fantasia para el
	canal dado. Puedes hacer que StatServ los muestre usando mensajes
	privados normales al canal (ON), o usando noticias.
	
	El comando CHANSTATS SUMUSER transfiere todas las estadisticas desde
	usuario2 a usuario1 y elimina usuario2.
	
	The CHANSTATS RENAME command renames user1 into user2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SUMUSER|SET} [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	%s ahora sera monitoreado por estadisticas de canales.

STAT_CHANSTATS_CHAN_LARGENET
	LargeNet esta habilitado y no puedes agregar nuevos canales.

STAT_CHANSTATS_LOGCHAN
	LogChannel no puede ser agregado a estadisticas de canales

STAT_CHANSTATS_NOT_VALID_CHAN
	Haz ingresado un nombre de canal no valido

STAT_CHANSTATS_CHAN_ALREADY
	%s ya esta configurado para ser monitoreado por estadisticas de canales.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD canal

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL canal 

STAT_CHANSTATS_CHAN_DEL
	Eliminando %s del monitoreo de canales.

STAT_CHANSTATS_CHAN_NOTFOUND
	%s no aparece en la lista de canales monitoreados.

STATS_CHANSTATS_PART
	Ya no se monitoreara a %s

STATS_CHANSTATS_NOCHANNELS
	No hay canales siendo monitoreados.

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET canal FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	Comandos de Fantasia para %s estan ahora habilitados

STATS_CHANSTATS_FANTASY_OFF
	Comandos de Fantasia para %s estan ahora deshabilitados

STATS_CHANSTATS_FANTASY_NOTICE
	Salida del comando de Fantasia para %s sera ahora notificada al usuario 

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER usuario1 usuario2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME usuario1 usuario2

STATS_CHANSTATS_SUMUSER_DONE
	The data from %s has been transferred to %s

STATS_CHANSTATS_RENAMEUSER_DONE
	User %s has been renamed to %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} blanco

STAT_EXCLUDE_ADDED
	%s ahora esta siendo excluido de Stats

STAT_EXCLUDE_ALREADY
	%s ya esta siendo excluido de Stats

STAT_EXCLUDE_DELETED
	%s ha sido removido de la lista de exclusiones

STAT_EXCLUDE_NOT_FOUND
	%s no se encuentra en la lista de exclusiones

STAT_EXCLUDE_LIST_HEADER
	Lista de Excluidos

# EXPORT responses
STATS_EXPORT_CHANNELS
	Canales exportados a %s

STATS_EXPORT_USERS
	Usuarios exportados a %s

STATS_EXPORT_TLD
	Top Level Domain exportados a %s

STATS_EXPORT_SERVERS
	Servidores exportados a %s

STATS_EXPORT_STATS
	Estadisticas exportadas a %s

STATS_EXPORT_ALL
	Toda la informacion exportada a %s

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	%s no existe en la db de stats.

STATS_RENAMEUSER_EXISTING
	User %s already exists in the stats db.

STATS_HELP_CHANNEL_1
	Los siguientes comandos de fantasia estan disponibles

STATS_HELP_CHANNEL_2
	%stop10  :   Los top 10 usuarios del canal actual

STATS_HELP_CHANNEL_3
	%stop    :   Los top 3  usuarios del canal actual

STATS_HELP_CHANNEL_4
	%sgtop10 :   The top 10 usuarios de toda la red

STATS_HELP_CHANNEL_5
	%sgtop   :   The top 3  usuarios de toda la red

STATS_HELP_CHANNEL_6
	%sstats  :   Estadisticas del usuario para el canal

STATS_HELP_CHANNEL_7
	%sgstats :   Estadisticas del usuario para la red

STATS_HELP_CHANNEL_8
	%speak   :   Topes de usuario del canal actual

STATS_TOP10_CHANNEL
	Top 10 de %s

STATS_TOP_CHANNEL
	Top 3 de %s

STATS_TOP10_NETWORK
	Top 10 de la Red

STATS_TOP_NETWORK
	Top 3 de la Red

STATS_USER_NETWORK
	Estadisticas de red para %s

STATS_USER_CHANNEL
	Estadisticas de canal para %s en %s

STATS_MESSAGE
	%2i %-16s letras: %s, palabras: %s, lineas: %s, smileys: %s, actions: %s

STATS_MESSAGE_ONE
	letras: %s, palabras: %s, lineas: %s, smileys: %s, actions: %s

STATS_CHANNEL_PEAK
	Tope usuarios canal fue %d

# ENVINFO responses
STAT_ENVINFO_HEADER
	Informacion del entorno para Stats:
STAT_ENVINFO_VERSION
	Version de Stats:           %s
STAT_ENVINFO_PROTOCOL
	Protocolo IRCd:             %s
STAT_ENVINFO_UPLINK
	Enlace de Stats:            %s
STAT_ENVINFO_SQL
	Codigo SQL:                 %s
STAT_ENVINFO_HTML
	Codigo HTML:                %s
STAT_ENVINFO_DEBUG
	Depurando:                  %s
STAT_ENVINFO_MODULESLOADED
	Numero de modulos cargados: %d
STAT_ENVINFO_LANGUAGE
	Idioma de la red:           %s
STAT_ENVINFO_FOOTER
	Fin de la Informacion del Entorno

STATS_HTML_EXPORTED
	El archivo de estadisticas HTML ha sido generado

# HTML code
HTML_LANG
	<META HTTP-EQUIV="CHARSET" CONTENT="ISO-8859-1">
	<META HTTP-EQUIV="CONTENT-LANGUAGE" CONTENT="Spanish">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Estadisticas de Red:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<td>Usuarios actuales: </td> <td> %ld </td> <td>Maximo de usuarios: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Canales actuales: </td> <td> %ld </td> <td>Maximo de canales: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>Operadores actuales: </td> <td> %ld </td> <td>Maximo de operadores: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Servidores actuales: </td> <td> %ld </td> <td>Maximo de servidores: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Usuarios en Away: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Estadisticas de Red para %s

HTML_TLD_TITLE
	Estadisticas de Top Level Domain

HTML_TLD_HEADER
	<table><tr><th>Codigo de Pais</th><th>Pais</th><th>Conteo</th><th>Total</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Nombre del Servidor</th></tr>

HTML_SERVER_CONTENT
	<tr><td height="4">Servidor: </td><td height="4"><a href=#%s> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td height="4">Servidor: </td><td height="4"><a href=#%s> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) indica que el servidor esta en linea en el momento<br>(Click en el nombre de un Servidor para mas informacion)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>Estadisticas Diarias de la Red:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4"><center>(Todas las Estadisticas Diarias son reseteadas a la Medianoche)</center></td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Maximo de Usuarios Diarios: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Maximos de Canales Diarios:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">Maximo de Operadores Diarios: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Maximo de Servidores Diarios: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Version</th><th align=right>Conteo</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name=%s>Servidor:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Usuarios actuales:</td><td>%ld (%ld)</td><td>Max %ld en [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Operadores actuales:</td><td>%ld (%ld)</td><td>Max %ld en [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Ping actual</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Total de Usuarios conectados:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>Ping mas alto</td><td>%ld</td><td>en [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Splits de Servidores</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Ultima vez visto:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>Kills de IRCop</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Kills de Servidor</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Canal</th><th align=right>Miembros</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Canal</th><th align=right>Total de Ingresos</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) indica que el canal esta activo en este momento

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> Canales no bienvenidos

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Canal</th><th>Total de Kicks</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) indica que el canal esta activo en este momento

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Canal</th><th>Total de Topics</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) indica que el canal esta activo en este momento

HTML_TOP10_TOPIC_TITLE
	Top10 Cambios de Topic de Canal

HTML_CUR_NETSTATS_TITLE
	Estadisticas Actuales de la Red

HTML_DAILY_TITLE
	Estadisticas Diarias de la Red

HTML_TOP10_ONLINE_CHANS_TITLE
	Top10 Canales en Linea

HTML_SERVERLIST_TITLE
	Lista de Servidores

HTML_TOP10_CHANEVER_TITLE
	Top10 Canales de Todos los Tiempos

HTML_SERVERDETAILS_TITLE
	Detalles de Servidor

HTML_TOP10_CLIENT_TITLE
	Top10 Versiones de Clientes

HTML_NETMAP_TITLE
	Mapa de la Red

HTML_VERSION
	Version es %s %s

HTML_MOST_UNWELCOME_TITLE
	Canales peor recibidos

HTML_POPULAR_TITLE
	Canales Populares

HTML_MOST_TOPIC_TITLE
	Canales con mas topics

HTML_NETSTATS_TITLE
	Estadisticas de Red

NOW_STATS_ADMIN
	%s (%s) es ahora un administrador de Stats.

STATS_AUTH_FAILED
	Autentificacion fallida para %s

STATS_ADMIN_LOGOUT
	%s ya no es un administrador de Stats.

SHUTDOWN_SAVING_DB
	Actualizando bases de datos al apagar, por favor espere.

CANNOT_OPEN_LOG
	Advertencia: no puedo abrir el archivo de registro: %s

GLOBAL_LOAD_MOD
	%s cargo el modulo %s

GLOBAL_UNLOAD_MOD
	%s descargo el modulo %s

GLOBAL_FATAL_ERROR
	ERROR FATAL! %s: %s

CONFIG_PORT_ERROR
	El numero para los puertos debe estar en el rango 1025..65535

CONFIG_HOSTNAME_ERROR
	Falta definir el hostname en el bloque connect

CONFIG_PROTOCOL_ERROR
	Falta definir protocol (protocolo IRCd) en el bloque connect

CONFIG_PORT_ERROR_NOTDEF
	Falta definir port en el bloque connect

CONFIG_PASSWD_ERROR
	Falta definir passwd en el bloque connect

CONFIG_PING_TO_LOW
	Ping Frequency debe ser de 2 minutos o mas

CONFIG_SQLPING_TO_LOW
	La frecuencia de SQL Ping debe ser de 2 minutos o mas

CONFIG_UPTIME_TO_LOW
	La frecuencia de Uptime debe ser de 2 minutos o mas

CONFIG_DELAYMOD_T0MANY
	Excesivos delayed modules (max 128)

CONFIG_TABLE_USER_ERROR
	Falta definir users en el bloque tables

CONFIG_TABLE_CHANBANS_ERROR
	Falta definir chanbans en el bloque tables

CONFIG_TABLE_CHANEXCEPT_ERROR
	Falta definir chanexcept en el bloque tables

CONFIG_TABLE_ISON_ERROR
	Falta definir ison en el bloque tables

CONFIG_TABLE_SERVER_ERROR
	Falta definir server en el bloque tables

CONFIG_TABLE_GLINE_ERROR
	Falta definir gline en el bloque tables

CONFIG_INVALID_LANG
	Numero de idioma invalido

CONFIG_ID_HOST_ERROR
	Falta definir user (host) en el bloque identity

CONFIG_ID_LANG_ERROR
	Falta definir language en el bloque identity

CONFIG_ID_USER_ERROR
	Falta definir user en el bloque identity

CONFIG_ID_DESC_ERROR
	Falta definir desc en el bloque identity

CONFIG_ID_NAME_ERROR
	Falta definir name en el bloque identity

CONFIG_ERROR_POSTIVE_VALUE
	Se esperaba un valor positivo

CONFIG_SQL_USER_ERROR
	Falta definir user en el bloque sql

CONFIG_SQL_NAME_ERROR
	Falta definir name en el bloque sql

CONFIG_ADMIN_NAME_ERROR
	Falta definir name en el bloque admin

CONFIG_ADMIN_HOST_ERROR
	Falta definir hostname en el bloque admin

CONFIG_ADMIN_PASS_ERROR
	Falta definir passwd en el bloque admin

CONFIG_TIMEOUT_PING_ERROR
	Falta definir pingfreq en el bloque timeout

CONFIG_TIMEOUT_SQL_ERROR
	Falta definir sqlfreq en el bloque timeout

CONFIG_TIMEOUT_UPTIME_ERROR
	Falta definir uptimefreq en el bloque timeout

CONFIG_TIMEOUT_HTML_ERROR
	Falta definir html en el bloque timeout

CONFIG_TIMEOUT_UPDATE_ERROR
	Falta definir update en el bloque timeout

CONFIG_TIMEOUT_WARNING_ERROR
	Falta definir warning en el bloque timeout

CONFIG_TIMEOUT_READ_ERROR
	Falta definir read en el bloque timeout

CONFIG_INVALID_TIME
	No es un valor valido para tiempo

CONFIG_HTML_UPDATE_TIME
	El Time Out de HTML debe ser 2 minutos o mas

CONFIG_NETINFO_NAME_ERROR
	Falta definir name en el bloque netinfo

CONFIG_FILENAME_PID_ERROR
	Falta definir pid en el bloque files

CONFIG_FILENAME_MOTD_ERROR
	Falta definir motd en el bloque files

CONFIG_FILENAME_CHANDB_ERROR
	Falta definir ChannelDB en el bloque files

CONFIG_FILENAME_CTCP_ERROR
	Falta definir ctcpDB en el bloque files

CONFIG_FILENAME_SERVER_ERROR
	Falta definir ServerDB en el bloque files

CONFIG_FILENAME_CHANSTATS_ERROR
	Falta definir ChannelStatsDB en el bloque files

CONFIG_FILENAME_TLD_ERROR
	Falta definir TLDDB en el bloque files

CONFIG_FILENAME_EXCLUDE_ERROR
	Falta definir EXCLUDEDB en el bloque files

CONFIG_SS_DESC_ERROR
	Falta definir real en el bloque statserv

CONFIG_SS_NAME_ERROR
	Falta definir nick en el bloque statserv

CONFIG_SS_TOLONG
	Nickname demasiado largo

CONFIG_SS_ALIASDESC_ERROR
	Falta definir aliasreal en el bloque statserv

ALOG_FAILED_SQL_CONNECT
	Falla al conectar a la base de datos: %s

ALOG_SQL_ENABLED
	MySQL: ha sido habilitado.

ALOG_SQL_DISABLED
	MySQL: ha sido deshabilitado.

ALOG_SQL_NOTSET
	MySQL Error: Completa todas las opciones de configuracion requeridas.

ALOG_HTML_ERR_WRITE
	Error: no se puede acceder el archivo para escritura: %s

ALOG_DB_ERROR
	ADVERTENCIA! DB_READ_ERROR en %s

ALOG_DEBUG_DB_OK
	debug: %s leido satisfactoriamente

ALOG_DEBUG_UNKNOW_CMODE
	debug: modo de canal desconocido %c

ALOG_DEBUG_LAST_LINE
	debug: ultima linea es %s

ALOG_DEBUG_REPORT_AT
	debug: reporta esto en el bugtracker en http://bugtracker.nomadirc.net

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Channel: se recibio join para el usuario no-existente %s el ID del canal %d

ALOG_DEBUG_CHANGE_MODE_CHAN
	debug: Cambiando modos de %s a %s

AL0G_DEBUG_BAD_CHAN_MODE
	Debug: Modo malformado detectado en %s.

ALOG_DEBUG_MODE_NO_PARAM
	channel: modo %c%c sin parametro (?) para el canal %s

ALOG_DEBUG_FINDCHAN_BAD
	Error: findchan() se llamo con argumentos NULOS

ALOG_DEBUG_FINDSTATCHAN_BAD
	Error: findstatschan() se llamo con argumentos NULOS

ALOG_DEBUG_SJOIN_NONEXISTANT
	user: SJOIN para el usuario no-existente %s en %s

ALOG_DEBUG_JOIN_NONEXISTANT
	user: JOIN para el usuario no-existente %s en %s

ALOG_DEBUG_JOIN
	debug: %s entra a %s

ALOG_DEBUG_KICK_NONEXISTANT
	user: KICK para el usuario no-existente %s en %s: %s

ALOG_DEBUG_KICKED
	debug: expulsando %s de %s

ALOG_DEBUG_PART_NONEXISTANT
	user: PART para el usuario no-existente %s: %s

ALOG_DEBUG_PART
	debug: %s sale de %s

ALOG_DEBUG_PART_BUG
	user: BUG saliendo de %s: entrada del canal encontrada pero c->chan NULL

ALOG_NON_TS_MODE
	TSMODE habilitado pero MODE no tiene un TS valido

ALOG_DEBUG_NO_FIND_CHAN
	debug: no se puede encontrar el canal %s

ALOG_DEBUG_TOPIC_TS
	debug: TOPIC TS codificado %s convertido a %d

ALOG_TOPIC_NONEXISTANT
	channel: TOPIC %s para el canal no-existente %s

ALOG_ADD_BAN_BAD
	debug: add_ban llamado con valores NULOS

ALOG_ADD_BAN_ADDED
	debug: Ban %s agregado al canal %s

ALOG_ADD_EXCEPTION_ADDED
	debug: Excepcion %s agregada al canal %s

ALOG_ADD_INVITE_ADDED
	debug: Invitacion %s agregada al canal %s

ALOG_CREATE_CHAN
	debug: Creando canal %s

ALOG_DELETE_CHAN
	debug: Eliminando canal %s

ALOG_BAN_FREE_ERROR
	channel: BUG liberando %s: bans[%d] es NULO!

ALOG_EXCEPTION_FREE_ERROR
	BUG liberando %s: excepts[%d] es NULO!

ALOG_INVITE_FREE_ERROR
	channel: BUG liberando %s: invite[%d] es NULO!

ALOG_BAN_DEL_CHAN
	debug: Eliminado ban %s del canal %s

ALOG_EXCEPTION_DEL_CHAN
	debug: Excepcion %s eliminada del canal %s

ALOG_INVITE_DEL_CHAN
	debug: Invitacion %s eliminada del canal %s

ALOG_DEBUG_FLOOD_MODE
	debug: Modo de flood para el canal %s cambiada a %s

ALOG_NO_FLOOD
	sin opciones de flood

ALOG_KEY_SET_TO
	debug: Clave para el canal %s cambiada a %s

ALOG_NO_KEY
	sin clave

ALOG_LIMIT_SET_TO
	debug: Limite del canal %s cambiado a %u

ALOG_REDIRECT_SET_TO
	debug: Redireccion del canal %s cambiada a %s

ALOG_NO_REDIRECT
	sin redireccion

ALOG_DEBUG_BAD_FINDSGROUP
	Error: findsgroup() llamado con argumentos NULOS

ALOG_LARGENET_WARNING_1
	Has seleccionado que Denora se ejecute con la menor cantidad de datos

ALOG_LARGENET_WARNING_2
	guardados en SQL, deberias elegir esta opcion si estas en una

ALOG_LARGENET_WARNING_3
	red muy grande de 100,000+ usuarios o tienes acceso a una base de datos remota

CONFIG_ERROR_BASE
	Conf Error: %s cerca de la linea %d de %s

CONFIG_UNKNOWN_FILE
	desconocido

CONFIG_ERROR_OPENING
	Imposible abrir el archivo de configuracion %s

ALOG_DEBUG_RESOLVED_HOST
	debug: Resuelto %s a %s

ALOG_DEBUG_BAD_SSTRDUP
	debug: sstrdup() llamado con argumentos NULOS

ALOG_RESET_DAILY
	reseteando las estadisticas diarias

ALOG_RESETTING_SUNDAY
	reseteando las estadisticas semanales (en Domingo)

ALOG_RESETTING_MONDAY
	reseteando las estadisticas semanales (en Lunes)

ALOG_RESETTING_TUESDAY
	reseteando las estadisticas semanales (en Martes)

ALOG_RESETTING_WEDNESDAY
	reseteando las estadisticas semanales (en Miercoles)

ALOG_RESETTING_THURSDAY
	reseteando las estadisticas semanales (en Jueves)

ALOG_RESETTING_FRIDAY
	reseteando las estadisticas semanales (en Viernes)

ALOG_RESETTING_SATURDAY
	reseteando las estadisticas semanales (en Sabado)

ALOG_RESETTING_MONTHLY
	reseteando estadisticas mensuales

ALOG_TRY_TO_DISPLAY
	debug: intentando mostrar mensaje %s

ALOG_DONE_DISPLAY
	debug: se ha mostrado el mensaje %s

ALOG_UKNOWN_USER_MODE
	debug: modo desconocido: usermode %c (en %s)

ALOG_SWHOIS_ERROR
	SWHOIS puesto para un usuario no-existente %s.

ALOG_VHOST_FOR
	debug: %s cambia su vhost a %s

ALOG_REALNAME_FOR
	debug: %s cambia su realname a %s

ALOG_USERNAME_FOR
	debug: %s cambia su username a %s

ALOG_UMODE_NONEXISTANT
	user: MODE %s para nick no-existente %s: %s

ALOG_UNKNWON_MSG
	debug: mensaje del servidor desconocido (%s)

ALOG_TRYING_LOAD_MOD
	intentando cargar [%s]

ALOG_STATUS
	estado: [%d]

ALOG_TRYING_UNLOAD_MOD
	intentando descargar [%s]

RESTARTING
	Reiniciando

RESTART_FAILED
	Reinicio fallido

SAVING_FFF
	debug: Guardando informacion FFF al disco

ALOG_SIGPIPE_WARNING
	Denora ejecutara un backtrace, y continuara, sin embargo Denora podria volverse inestable

ALOG_NO_STATS_BIN
	Intento de reinicio fallido -- STATS_BIN no definido (configure denuevo)

ALOG_ERR_REHASH_NO_MEM
	Error durante el refresco del archivo de configuracion, sin memoria!

ALOG_ERR_REHASH
	Error leyendo el archivo de configuracion (SIGHUP recibido)

BACKTRACE_LINES
	Backtrace: reporta las siguientes lineas

BACKTRACE_NOT_HERE
	Backtrace: no disponible en esta plataforma - usa GDB

ALOG_DEBUG_SERVINTRO
	debug: Servidor introducido (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: Servidor introducido (%s) desde %s

ALOG_DEBUG_PINGTIME
	debug: Tiempo Ping desde %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Error: findserver() llamado con argumentos NULOS

ALOG_ERR_FINDSERVERSTATS
	Error: findserverstats() llamado con argumentos NULOS

ALOG_ERR_FINDSERVERSUID
	Error: findserver_suid() llamado con argumentos NULOS

ALOG_ERR_DEL_SERVSTATS
	Error: delete_serverstats() llamado con argumentos NULOS

ALOG_DEL_SERVSTATS_FOR
	debug: delete_serverstats() llamado por %s

ALOG_DEL_SERVSTATS_DONE
	debug: delete_serverstats() completado

ALOG_ERR_DEL_SERVER
	Error: delete_server() llamado con argumentos NULOS

ALOG_DEL_SERVER_FOR
	debug: delete_server() llamado por %s

ALOG_DEL_SERVER_NOQUIT
	debug: delete_server() todos los usuarios limpiados por NOQUIT

ALOG_DEL_SERVER_DONE
	debug: delete_server() completado

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: sql_do_server_version recibido con formato desconocido de version

ALOG_MOD_TRY_LOAD
	tratando de cargar [%s]

ALOG_MOD_STATUS
	estado: [%d][%s]

ALOG_MOD_ERROR
	Error modulo: %s

ALOG_MOD_BE_ONLY_ONE
	No puedes tener 2 modulos de protocolo cargados.

ALOG_MOD_UNLOAD_SELF
	debug: %s solicita descarga...

ALOG_CMD_DISPLAY_START
	debug: tratando de mostrar comando %s

ALOG_CMD_DISPLAY_END
	debug: hecho mostrando comando %s

ALOG_CMD_DISPLAY_LIST
	debug: Mostrando lista comandos para %s

ALOG_MSG_DISPLAY_START
	debug: tratando de mostrar mensaje %s

ALOG_MSG_DISPLAY_END
	debug: hecho mostrando mensaje %s

ALOG_MSG_DISPLAY_LIST
	debug: Mostrando lista mensajes para %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: Agregado CallBack modulo: [%s] debe ser ejecutado en %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST para inexistente usuario %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME para inexistente usuario %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME para inexistente usuario %s

STAT_ADMIN_ALREADY
	%s ya es un admin de Stats

STAT_ADMIN_CREATED
	%s is ahora un Admin de Stats

STAT_ADMIN_DELETED
	%s ha sido removido de los Admins de Stats

STAT_ADMIN_NOTADMIN
	%s no es un Admin de Stats

STAT_HELP_ADMIN
	Sintaxis: ADMIN [add|del] user pass [host] [langnum]
	
	Agrega o elimina un administrador de Stats. No afecta a los admins 
	que estan en el archivo de configuracion.

	Limitado a Stats Admin

STAT_ADMIN_SYNTAX
	ADMIN {ADD|DEL} USER [PASS|HOST|langnum]

STAT_ADMIN_DELETE_CONFIG
	No puedo eliminar a %s porque esta en el archivo de configuracion.

STAT_CHGPASS_OK
	La clave para %s ha sido cambiada.

STAT_CHGPASS_CONFIG
	No puedo cambiar la clave para admins que estan en el archivo de
	configuracion.

STAT_CHGPASS_SYNTAX
	CHGPASS ADMINNAME NEWPASS

STAT_HELP_CHGPASS
	Sintaxis: CHGPASS user pass
	
	Cambia la clave para Stats Admin en tiempo de ejecucion (al vuelo).

	Limitado a Stats Admin


