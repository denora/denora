# Brazil Portugu�s language file.
#
# Denora (c) 2004-2012 Denora Team
# Contact us at dev@denorastats.org
#
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
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
# For example, "Espa�ol (Spanish)" or "Fran�ais (French)".

LANG_NAME
	English

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
	%b %d %H:%M:%S %Y %Z
# This is used as the format string for strftime() for a date alone in long
# format (English: including weekday).
STRFTIME_LONG_DATE_FORMAT
	%a %b %d %Y
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
	Dom
	Seg
	Ter
	Qua
	Qui
	Sex
	S�b
# %A
STRFTIME_DAYS_LONG
	Domingo
	Segunda
	Ter�a
	Quarta
	Quinta
	Sexta
	S�bado
# %b
STRFTIME_MONTHS_SHORT
	Jan
	Fev
	Mar
	Abr
	Mai
	Jun
	Jul
	Ago
	Set
	Out
	Nov
	Dez
# %B
STRFTIME_MONTHS_LONG
	Janeiro
	Fevereiro
	Mar�o
	Abril
	Maio
	Junho
	Julho
	Agosto
	Setembro
	Outubro
	Novembro
	Dezembro


# This is used in StatServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Erro interno - n�o foi poss�vel processar a solicita��o.
UNKNOWN_COMMAND
	Comando desconhecido %s.
UNKNOWN_COMMAND_HELP
	Comando desconhecido %s.  "/msg %s HELP" para maiores informa��es.
SYNTAX_ERROR
	Sintaxe: %s
MORE_INFO
	/msg %s HELP %s para maiores informa��es.
NO_HELP_AVAILABLE
	Nenhuma ajuda referente ao comando %s.

SERVICE_OFFLINE
	%s est� temporiariamente offline.

SQL_DISABLED
	Voc� n�o pode utilizar este comando, SQL est� desativado.

STATS_UPTIME_DHMS
	Stats ativo a %d dias, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	Stats ativo a %d dia, %02d:%02d:%02d
STATS_UPTIME_HMS
	Stats ativo a %d horas, %d minutos, %d segundos
STATS_UPTIME_HM1S
	Stats ativo a %d horas, %d minutos, %d segundo
STATS_UPTIME_H1MS
	Stats ativo a %d horas, %d minuto, %d segundos
STATS_UPTIME_H1M1S
	Stats ativo a %d horas, %d minuto, %d segundo
STATS_UPTIME_1HMS
	Stats ativo a %d hora, %d minutos, %d segundos
STATS_UPTIME_1HM1S
	Stats ativo a %d hora, %d minutos, %d segundo
STATS_UPTIME_1H1MS
	Stats ativo a %d hora, %d minuto, %d segundos
STATS_UPTIME_1H1M1S
	Stats ativo a %d hora, %d minuto, %d segundo
STATS_UPTIME_MS
	Stats ativo a %d minutos, %d segundos
STATS_UPTIME_M1S
	Stats ativo a %d minutos, %d segundo
STATS_UPTIME_1MS
	Stats ativo a %d minuto, %d segundos
STATS_UPTIME_1M1S
	Stats ativo a %d minuto, %d segundo

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stats admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Acesso negado.
PERMISSION_DENIED
	Permiss�o negada.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Sim
SAY_NO
	N�o

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN n�o foi definido; n�o foi poss�vel usar o comando restart. Configure e recompile
	novamente para ativar o comando restart.

# Module strings

STAT_MODULE_LOADED
	M�dulo %s carregado
STAT_MODULE_UNLOADED
	M�dulo %s descarregado
STAT_MODULE_LOAD_FAIL
	N�o foi poss�vel carregar o m�dulo %s
STAT_MODULE_REMOVE_FAIL
	N�o foi poss�vel descarregar o m�dulo %s
STAT_MODULE_LOAD_SYNTAX
	MODLOAD Arquivo
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD Arquivo
STAT_MODULE_NO_UNLOAD
	Este m�dulo n�o pode ser carregado.
STAT_MODULE_LIST_HEADER
	Lista de m�dulos carregados:
STAT_MODULE_LIST
	M�dulo: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	%d M�dulos carregados.
STAT_MODULE_INFO_LIST
	M�dulo: %s Vers�o: %s Ator: %s Data de carregamento: %s
STAT_MODULE_CMD_LIST
	Novo comando: /msg %s %s
STAT_MODULE_MSG_LIST
	Entregando controle de IRCd para: %s
STAT_MODULE_NO_LIST
	Nenhum m�dulo carregado
STAT_MODULE_NO_INFO
	N�o existe nenhuma inform��o referente ao m�dulo %s.
STAT_MODULE_INFO_SYNTAX
	MODINFO Arquivo
MODULE_HELP_HEADER
	Os seguintes comandos foram carregados por um m�dulo:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	Comandos do %S:
	    STATUS       Mostra as estat�sticas de sua rede.
	    LOGIN       Loga voc� como administrador.
	    LOGOUT      Desloga voc� do status de administrador.

STAT_HELP_STATUS
	Sintaxe: STATUS
	
	Mostra o n�mero atual de servidores, canais e usu�rios
	connected to the network.

STAT_HELP_SHUTDOWN
	Sintaxe: SHUTDOWN
	
	Salva todos os dados do stats e o desliga.
	
	Limitado aos Administradores do Denora

STAT_HELP_RESTART
	Sintaxe: RESTART
	
	Reinicia o denora.

STAT_HELP_RELOAD
	Sintaxe: RELOAD
	
	Recarrega o arquivo de configura��o do Denora

STAT_HELP_ADMIN_CMD
	Comandos dispon�veis para os Administradores do Denora:
	    ADMIN       Gerencia a lista de admins
	    CHANSTATS   Gerencia os canais monitorados pelo Denora
	    ENVINFO     Mostra informa��es sobre o sistema
	    EXCLUDE     Exclui certos usu�rios dos stats dos canais
	    EXPORT      Exporta os dados para um XML
	    HTML        For�a a cria��o de um arquivo HTML
	    MODLOAD     Carrega um m�dulo
	    MODUNLOAD   Descarrega um m�dulo
	    MODLIST     Lista todos os m�dulos carregados
	    MODINFO     Mostra informa��es sobre um determinado m�dulo
	    RESTART     Reinicia o Denora.
	    RELOAD      Recarrega o arquivo de configura��o do Denora
	    SET         Seta varias op��es do Denora.
	    SHUTDOWN    Desliga o Denora salvando os dados

STAT_HELP_MODLOAD
	Sintaxe: MODLOAD Arquivo

	Carrega um determinado m�dulo

	Limitado aos Administradores do Denora

STAT_HELP_MODUNLOAD
	Sintaxe: MODUNLOAD Arquivo

	Descarrega um determinado m�dulo

	Limitado aos Administradores do Denora.

STAT_HELP_MODINFO
	Sintaxe: MODINFO Arquivo

	Mostra certas informa��es referente a um determinado m�dulo

	Limitado aos Administradores do Denora

STAT_HELP_MODLIST
	Sintaxe: MODLIST

	Lista todos os m�dulos carregados.

	Limitado aos Administradores do Denora

STAT_HELP_EXCLUDE
	Sintaxe: EXCLUDE {ADD|DEL|LIST} destino
	
	Exlui certo destino do stats
	
	Limitado aos Administradores do Denora

STAT_HELP_EXPORT
	Sintaxe: EXPORT tabela 
	
	Exporta os dados para um arquivo XML, onde est�
	tabela voc� poder� utilizar uma das tabelas abaixo:
	
	channels,users,tld,servers,stats,all
	
	Limitado aos Administradores do Denora

STAT_HELP_HTML
	Sintaxe: HTML
	
	For�a a cria��o de um arquivo HTML

STAT_HELP_USERDEBUG
	Sintaxe: USERDEBUG user 
	
	Retorna certas informa��es de um determinado usu�rio

STAT_HELP_LOGIN
	Sintaxe: LOGIN nome senha
	
	Se loga como um administrador do Denora.

STAT_HELP_LOGOUT
	Sintaxe: LOGOUT
	
	Se desloga do status de administrador do Denora.

STAT_HELP_SET
	Sintaxe: SET OP��O [ON|OFF]

	Seta varias op��es do Denora

	As op��es poss�veis s�o:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	Limitado aos Administradores do Denora

STAT_HELP_ENVINFO
	Sintaxe: ENVINFO
	
	Mostras certas informa��es do sistema.
	
	Limitado aos Administradores do Denora

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	Voc� � agora um Administrador do Denora.

STAT_ADMIN_LOGGED_OUT
	Voc� se deslogou do status de admin do Denora.

STAT_ADMIN_ALREADY_LOGGED
	Voc� j� est� com o status de admin!

STAT_LOGIN_SYNTAX
	LOGIN nome senha

STAT_RELOAD
	Configura��es do denora foram recarregadas

###########################################################################
#
# STATUS messages
#
###########################################################################

STAT_STATUS_SERVER_COUNT
	Estat�sticas de servidores da %s:
	Servidores linkados atualmente:  %d
	M�ximo de servidores linkados: %d

STAT_STATUS_CHAN_COUNT
	Estat�sticas canais da %s:
	Canais sendo usados atualmente:  %d
	M�ximo de canais criados: %d

STAT_STATUS_USER_COUNT
	Estat�sticas de usu�rios da %s:
	Usu�rios conectados atualmente:  %d
	M�ximo de usu�rios conectados: %d

STAT_STATUS_SYNTAX
	STATUS [SERVERS|CHANNELS|USERS]

STAT_STATUS_PAGE
	Para mais estat�sticas, visite: %s

# SET responses
STAT_SET_SYNTAX
	SET op��o setting
STAT_SET_DEBUG_ON
	Stats est�o agora no modo debug.
STAT_SET_DEBUG_OFF
	Stats n�o est�o mais em modo debug.
STAT_SET_DEBUG_LEVEL
	Stats est�o agora modo debug (level %d).
STAT_SET_DEBUG_ERROR
	Op��o para debug deve ser ON, OFF, ou um n�mero positivo.
STAT_SET_UNKNOWN_OPTION
	Op��o SET desconhecida: %s.
STAT_SET_SQL_ON
	C�digo SQL ir� ser usado.
STAT_SET_SQL_OFF
	C�digo SQL n�o ir� ser usado.
STAT_SET_SQL_ERROR
	Op��o do SQL deve ser ON ou OFF.
STAT_SET_SQL_ERROR_INIT
	Erro durane a inicializa��o do SQL, favor checar os logs.
STAT_SET_SQL_ERROR_DISABLED
	SQL n�o est� configurado para esta rede. Por favor, edite
	o arquivo de configura��o (denora.conf).
STAT_SET_LIST_OPTION_ON
	%s est� ativado
STAT_SET_LIST_OPTION_OFF
	%s est� desativado
STAT_SET_HTML_ON
	HTML ir� ser usado.
STAT_SET_HTML_OFF
	HTML n�o ir� ser usado.
STAT_SET_HTML_ERROR
	SOp��o do SQL deve ser ON ou OFF.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Permite que voc� monitore os canais, obtendo informa��es sobre
	n�mero de frases, contagem de linhas e muito mais..
	
	CHANSTATS ADD canal
	CHANSTATS DEL canal
	CHANSTATS LIST
	CHANSTATS SET canal FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER user1 user2
	CHANSTATS RENAME user1 user2
	
	O comando CHANSTATS ADD permite voc� adicionar um canal na
	na lista de canais monitorados.
	
	O comando CHANSTATS DEL permite voc� deletar um canal da
	lista de canais monitorados.
	
	O comando CHANSTATS LIST permite voc� listar os canais que est�o
	na lista de canais monitorados.
	
	O comando CHANSTATS SET ativa os comandos de fantasia para um
	determinado canal. Voc� pode fazer com que o StatServ envie 
	mensagens normais para o canal (ON), ou utilizando notices (NOTICES).
	
	O comando CHANSTATS SUMUSER permite voc� transferir todos os stats do user 2
	para o user1 e deletando o user2.
	
	O comando CHANSTATS RENAME permite voc� renomeiar o user1 para user2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SET|SUMUSER|RENAME} [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	%s est� agora definido para ser monitorado pelo denora.

STAT_CHANSTATS_CHAN_LARGENET
	LargeNet est� ativado, voc� n�o pode adicionar mais canais.

STAT_CHANSTATS_LOGCHAN
	LogChannel n�o pode ser adicionado na lista de canais monitorados.

STAT_CHANSTATS_NOT_VALID_CHAN
	Voc� deve colocar um nome de canal V�LIDO

STAT_CHANSTATS_CHAN_ALREADY
	%s j� est� na lista de canais monitorados pelo Denora.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD canal 

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL canal

STAT_CHANSTATS_CHAN_DEL
	Deletando %s da lista de canais monitorados.

STAT_CHANSTATS_CHAN_NOTFOUND
	%s n�o est� na lista de canais monitorados.

STATS_CHANSTATS_PART
	O canal %s n�o ir� mais ser monitorado

STATS_CHANSTATS_NOCHANNELS
	Nenhum canal est� sendo monitorado

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET canal FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	Comando Fantasia para o canal %s est� ativado

STATS_CHANSTATS_FANTASY_OFF
	Comando Fantasia para o canal %s est� desativado

STATS_CHANSTATS_FANTASY_NOTICE
	Comando fantasia, enviar� mensagens para o canal %s atrav�s de notices

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER user1 user2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME user1 user2

STATS_CHANSTATS_SUMUSER_DONE
	Os dados de %s foram transferidos para %s

STATS_CHANSTATS_RENAMEUSER_DONE
	User %s foi trocado para %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} alvo

STAT_EXCLUDE_ADDED
	%s foi exclu�do do stats

STAT_EXCLUDE_ALREADY
	%s j� est� na lista de exclus�es

STAT_EXCLUDE_DELETED
	%s foi removido da lista de exclus�es

STAT_EXCLUDE_NOT_FOUND
	%s n�o est� na lista de exclus�es

STAT_EXCLUDE_LIST_HEADER
	Lista de exclus�es

# EXPORT responses
STATS_EXPORT_CHANNELS
	Canais exportados para %s

STATS_EXPORT_USERS
	Usu�rios exportados para %s

STATS_EXPORT_TLD
	Top Level Domain exportados para %s

STATS_EXPORT_SERVERS
	Servidores exportados para %s

STATS_EXPORT_STATS
	Stats exportados para %s

STATS_EXPORT_ALL
	Exportandos todas as informa��es para %s

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	%s n�o existe na db.

STATS_RENAMEUSER_EXISTING
	User %s j� existe na db.

STATS_HELP_CHANNEL_1
	Os seguintes comandos fantasias est�o dispon�veis

STATS_HELP_CHANNEL_2
	%stop10  :   Mostra o top 10 do canal

STATS_HELP_CHANNEL_3
	%stop    :   Mostra o top 3 de usu�rios do canal

STATS_HELP_CHANNEL_4
	%sgtop10 :   Mostra o top 10 dos usu�rios de toda a rede

STATS_HELP_CHANNEL_5
	%sgtop   :   Mostra o top 3 dos usu�rios de toda a rede

STATS_HELP_CHANNEL_6
	%sstats  :   Mostra os stats de um determinado nick referente ao canal

STATS_HELP_CHANNEL_7
	%sgstats :   Mostra os stats de um determinado nick referente a toda a rede

STATS_HELP_CHANNEL_8
	%speak   :   Mostra o pico de usu�rios no canal

STATS_TOP10_CHANNEL
	Top 10 do %s

STATS_TOP_CHANNEL
	Top 3 do %s

STATS_TOP10_NETWORK
	Top 10 da rede

STATS_TOP_NETWORK
	Top 3 da rede

STATS_USER_NETWORK
	Estat�sticas da rede %s

STATS_USER_CHANNEL
	Estat�sticas do canal %s em %s

STATS_MESSAGE
	%2i %-16s letras: %s, palavras: %s, linhas: %s, smileys: %s, a��es: %s

STATS_MESSAGE_ONE
	letras: %s, palavras: %s, linhas: %s, smileys: %s, a��es: %s

STATS_CHANNEL_PEAK
	O pico de usu�rios do canal � de %d

# ENVINFO responses
STAT_ENVINFO_HEADER
	Informa��es sobre o sistema:
STAT_ENVINFO_VERSION
	Vers�o do stats:          %s
STAT_ENVINFO_PROTOCOL
	Protocolo do IRCd:        %s
STAT_ENVINFO_UPLINK
	Stats ativo:              %s
STAT_ENVINFO_SQL
	C�digo SQL:               %s
STAT_ENVINFO_HTML
	C�digo HTML:              %s
STAT_ENVINFO_DEBUG
	Debugging:                %s
STAT_ENVINFO_MODULESLOADED
	N�mero de m�dulos:        %d
STAT_ENVINFO_LANGUAGE
	L�nguagem da Rede:         %s
STAT_ENVINFO_FOOTER
	Final das informa��es sobre o sistema

STATS_HTML_EXPORTED
	Arquivo HTML foi gerado

# HTML code
HTML_LANG
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<meta http-equiv="Content-Language" content="Portugu�s">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Stats da rede:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<tr><td>Usuarios: </td> <td> %ld </td> <td>Maximo: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Canais: </td> <td> %ld </td> <td>Maximo: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>IRCoperados: </td> <td> %ld </td> <td>Maximo: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Servidores: </td> <td> %ld </td> <td>Maximo: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Usuarios em away: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Stats da rede %s

HTML_TLD_TITLE
	Top Level Domain Stats

HTML_TLD_HEADER
	<table><tr><th>Codigo do pais</th><th>Pais</th><th>Contagem</th><th>Global</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Noem do servidor</th></tr>

HTML_SERVER_CONTENT
	<tr><td>Servidor: </td><td><a href="#%s"> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td>Servidor: </td><td><a href="#%s"> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) indica que o servidor online no momento<br>(Clique no nome do servidor pais mais info)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>Stats diarias:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4">(Todas as stats diarias sao resetadas a meina noite)</td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Max de users por dia: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Max de canais por dia:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">Max de ircops por dia: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Max de servidores por dia: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Versao</th><th align=right>Count</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name="%s"></a>Servidor:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Users atuais:</td><td>%ld (%ld)</td><td>Max %ld at [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Ircops atuais:</td><td>%ld (%ld)</td><td>Max %ld at [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Ping atuais</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Total de users conectados:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>Ping mais alto</td><td>%ld</td><td>at [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Servidores em splits</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Ultima visita:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>Kills de ircops</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Kills de servidores</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Canal</th><th align=right>Membros</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Canal</th><th align=right>Total de entradas</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) indica que o canal esta ativo no momento

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> Canais que nao sao bem vindos

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Canal</th><th>Total de Kicks</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) indica que o canal esta ativo no momento

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Canal</th><th>Total Topics</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) indica que o canal esta ativo no momento

HTML_TOP10_TOPIC_TITLE
	Top10 trocas de topicos

HTML_CUR_NETSTATS_TITLE
	Stats atuais da rede

HTML_DAILY_TITLE
	Stats diarias da rede

HTML_TOP10_ONLINE_CHANS_TITLE
	Top10 de canais online

HTML_SERVERLIST_TITLE
	Lista de servidores

HTML_TOP10_CHANEVER_TITLE
	Top10 do maiores canais

HTML_SERVERDETAILS_TITLE
	Detalhes do servidor

HTML_TOP10_CLIENT_TITLE
	Top10 versoes dos clientes

HTML_NETMAP_TITLE
	Mapa da rede

HTML_VERSION
	Powered by <a href="http://www.denorastats.org/">Denora</a> versao %s %s

HTML_MOST_UNWELCOME_TITLE
	Maiores canais que nao sao bem vindos

HTML_POPULAR_TITLE
	Canais populares

HTML_MOST_TOPIC_TITLE
	Canais com mais topicos

HTML_NETSTATS_TITLE
	Stats da rede

NOW_STATS_ADMIN
	%s (%s) � agora um administrador do denora.

STATS_AUTH_FAILED
	Login de %s falhou

STATS_ADMIN_LOGOUT
	%s n�o � mais um  administrador do denora.

SHUTDOWN_SAVING_DB
	Salvando dados antes de desligar..

CANNOT_OPEN_LOG
	ATEN��O: n�o foi poss�vel abrir o log de arquivo: %s

GLOBAL_LOAD_MOD
	%s carregou o m�dulo %s

GLOBAL_UNLOAD_MOD
	%s descarregou o m�dulo %s

GLOBAL_FATAL_ERROR
	ERRO FATAL! %s: %s

CONFIG_PORT_ERROR
	O n�mero das portas devem estar entre 1025..65535

CONFIG_HOSTNAME_ERROR
	Faltou definir o hostname no arquivo de configura��o, bloco connect

CONFIG_PROTOCOL_ERROR
	Faltou definir o protocol (IRCd) no arquivo de configura��o, bloco connect

CONFIG_PORT_ERROR_NOTDEF
	Faltou definir a port (n�mero da porta) no arquivo de configura��o, bloco connect

CONFIG_PASSWD_ERROR
	Faltou definir o passwd (senha de conex�o) arquivo de configura��o, bloco connect

CONFIG_PING_TO_LOW
	A frequ�ncia de Pings devem ser igual ou maior a 2 minutos

CONFIG_SQLPING_TO_LOW
	A frequ�ncia de Pings do SQL devem ser igual ou maior a 2 minutos

CONFIG_UPTIME_TO_LOW
	A frequ�ncia do uptime deve ser igual ou maior a 2 minutos

CONFIG_DELAYMOD_T0MANY
	N�mero de m�dulos carregados se excedeu (m�x 128)

CONFIG_TABLE_USER_ERROR
	Faltou definir o users do arquivo de configura��o, bloco tables

CONFIG_TABLE_CHANBANS_ERROR
	Faltou definir o chanbans do arquivo de configura��o, bloco tables

CONFIG_TABLE_CHANEXCEPT_ERROR
	Faltou definir o chanexcept do arquivo de configura��o, bloco tables

CONFIG_TABLE_ISON_ERROR
	Faltou definir o ison do arquivo de configura��o, bloco tables

CONFIG_TABLE_SERVER_ERROR
	Faltou definir o server do arquivo de configura��o, bloco tables

CONFIG_TABLE_GLINE_ERROR
	Faltou definir o gline do arquivo de configura��o, bloco tables

CONFIG_INVALID_LANG
	N�mero da linguagem est� errado no arquivo de configura��o

CONFIG_ID_HOST_ERROR
	Faltou definir o user (host) do arquivo de configura��o, bloco identity

CONFIG_ID_LANG_ERROR
	Faltou definir a language (linguagem) do arquivo de configura��o, bloco identity

CONFIG_ID_USER_ERROR
	Faltou definir o user do arquivo de configura��o, bloco identity

CONFIG_ID_DESC_ERROR
	Faltou definir o desc do arquivo de configura��o, bloco identity

CONFIG_ID_NAME_ERROR
	Faltou definir o name do arquivo de configura��o, bloco identity

CONFIG_ERROR_POSTIVE_VALUE
	Um valor positivo � esperado

CONFIG_SQL_USER_ERROR
	Faltou definir o user do arquivo de configura��o, bloco sql

CONFIG_SQL_NAME_ERROR
	Faltou definir o name do arquivo de configura��o, bloco sql

CONFIG_ADMIN_NAME_ERROR
	Faltou definir o admin name do arquivo de configura��o, bloco admin

CONFIG_ADMIN_HOST_ERROR
	Faltou definir o hostname do arquivo de configura��o, bloco admin

CONFIG_ADMIN_PASS_ERROR
	Faltou definir o passwd do arquivo de configura��o, bloco admin

CONFIG_TIMEOUT_PING_ERROR
	Faltou definir o pingfreq do arquivo de configura��o, bloco timeout

CONFIG_TIMEOUT_SQL_ERROR
	Faltou definir o sqlfreq do arquivo de configura��o, bloco timeout

CONFIG_TIMEOUT_UPTIME_ERROR
	Faltou definir o uptimefreq do arquivo de configura��o, bloco timeout

CONFIG_TIMEOUT_HTML_ERROR
	Faltou definir o html do arquivo de configura��o, bloco timeout

CONFIG_TIMEOUT_UPDATE_ERROR
	Faltou definir o update do arquivo de configura��o, bloco timeout

CONFIG_TIMEOUT_WARNING_ERROR
	Faltou definir o warning do arquivo de configura��o, bloco timeout

CONFIG_TIMEOUT_READ_ERROR
	Faltou definir o read do arquivo de configura��o, bloco timeout

CONFIG_INVALID_TIME
	Este n�o � um valor de tempo

CONFIG_HTML_UPDATE_TIME
	O time outro do HTML deve de ser maior ou igual a 2 minutos

CONFIG_NETINFO_NAME_ERROR
	Faltou definir o name do arquivo de configura��o, bloco netinfo

CONFIG_FILENAME_PID_ERROR
	Faltou definir o pid do arquivo de configura��o, bloco files

CONFIG_FILENAME_MOTD_ERROR
	Faltou definir o motd do arquivo de configura��o, bloco files

CONFIG_FILENAME_CHANDB_ERROR
	Faltou definir o ChannelDb do arquivo de configura��o, bloco files

CONFIG_FILENAME_CTCP_ERROR
	Faltou definir o ctcpDB do arquivo de configura��o, bloco files

CONFIG_FILENAME_SERVER_ERROR
	Faltou definir o ServerDb do arquivo de configura��o, bloco files

CONFIG_FILENAME_CHANSTATS_ERROR
	Faltou definir o ChannelStatsDb do arquivo de configura��o, bloco files

CONFIG_FILENAME_TLD_ERROR
	Faltou definir o TLDDB do arquivo de configura��o, bloco files

CONFIG_FILENAME_EXCLUDE_ERROR
	Faltou definir o EXCLUDEDB do arquivo de configura��o, bloco files

CONFIG_SS_DESC_ERROR
	Faltou definir o desc do arquivo de configura��o, bloco statserv

CONFIG_SS_NAME_ERROR
	Faltou definir o nick do arquivo de configura��o, bloco statserv

CONFIG_SS_TOLONG
	Nickname muito longo

CONFIG_SS_ALIASDESC_ERROR
	Faltou definir o aliasreal do arquivo de configura��o, bloco statserv

ALOG_FAILED_SQL_CONNECT
	Falha ao se conectar ao banco de dados : %s

ALOG_SQL_ENABLED
	%s: est� ativado.

ALOG_SQL_DISABLED
	MySQL: est� desativado.

ALOG_SQL_NOTSET
	MySQL Erro: Defina todas as configura��es exigidas.

ALOG_HTML_ERR_WRITE
	Erro: imposs�vel acessar o arquivo para reescrever: %s

ALOG_DB_ERROR
	WARNING! DB_READ_ERROR no %s

ALOG_DEBUG_DB_OK
	debug: %s lida com �xito

ALOG_DEBUG_UNKNOW_CMODE
	debug: modo de canal desconhecido %c

ALOG_DEBUG_LAST_LINE
	debug: �ltima linha � %s

ALOG_DEBUG_REPORT_AT
	debug: Informar o bugtracker para: http://bugs.anope.org

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Canal: recebido join de um usu�rio n�o existente/bot %s no canal %s

ALOG_DEBUG_CHANGE_MODE_CHAN
	debug: Mudando modos do canal %s para %s

ALOG_DEBUG_BAD_CHAN_MODE
	Debug: Modo mal formado detectado %s.

ALOG_DEBUG_MODE_NO_PARAM
	canal: modo %c%c sem par�mentros (?) para o canal %s

ALOG_DEBUG_FINDCHAN_BAD
	Erro: findchan() retornou sem valores

ALOG_DEBUG_FINDSTATCHAN_BAD
	Erro: findstatschan() retornou sem valores

ALOG_DEBUG_SJOIN_NONEXISTANT
	user: SJOIN de um usu�rio n�o existente/bot %s em %s

ALOG_DEBUG_JOIN_NONEXISTANT
	user: JOIN de um usu�rio n�o existente/bot %s em %s

ALOG_DEBUG_JOIN
	debug: %s entrou %s

ALOG_DEBUG_KICK_NONEXISTANT
	user: KICK de um usu�rios n�o existente/bot %s em %s: %s

ALOG_DEBUG_KICKED
	debug: kickando %s de %s

ALOG_DEBUG_PART_NONEXISTANT
	user: PART de um usu�rio n�o existente/bot %s: %s

ALOG_DEBUG_PART
	debug: %s saiu %s

ALOG_DEBUG_PART_BUG
	user: BUG saindo %s: entrada de canal encontrada but c->chan NULL

ALOG_NON_TS_MODE
	TSMODE est� ativo mas o MODE n�o possui um v�lido TS

ALOG_DEBUG_NO_FIND_CHAN
	debug: n�o foi poss�vel encontrar o canal %s

ALOG_DEBUG_TOPIC_TS
	debug: encode TOPIC TS %s convertido para %d

ALOG_TOPIC_NONEXISTANT
	canal: TOPIC %s por um usu�rio n�o existente/bot %s

ALOG_ADD_BAN_BAD
	debug: add_ban retornou sem valores

ALOG_ADD_BAN_ADDED
	debug: Adicionou ban %s para o canal %s

ALOG_ADD_EXCEPTION_ADDED
	debug: Adicionou except %s para o canal %s

ALOG_ADD_INVITE_ADDED
	debug: Adicionou invite %s para o canal %s

ALOG_CREATE_CHAN
	debug: Criando canal %s

ALOG_DELETE_CHAN
	debug: Deletando canal %s

ALOG_BAN_FREE_ERROR
	channel: BUG freeing %s: bans[%d] is NULL!

ALOG_EXCEPTION_FREE_ERROR
	BUG freeing %s: excepts[%d] is NULL!

ALOG_INVITE_FREE_ERROR
	channel: BUG freeing %s: invite[%d] is NULL!

ALOG_BAN_DEL_CHAN
	debug: Ban deletado %s do canal %s

ALOG_EXCEPTION_DEL_CHAN
	debug: Except deletada %s do canal %s

ALOG_INVITE_DEL_CHAN
	debug: Invite deletado %s do canal %s

ALOG_DEBUG_FLOOD_MODE
	debug: Modo de flod do %s setado para %s

ALOG_NO_FLOOD
	nenhuma op��o de flood

ALOG_KEY_SET_TO
	debug: Chave do canal %s setada para %s

ALOG_NO_KEY
	sem chave

ALOG_LIMIT_SET_TO
	debug: Limite do canal %s setado para %u

ALOG_REDIRECT_SET_TO
	debug: Redirecionamento do canal %s setado para %s

ALOG_NO_REDIRECT
	sem redirecionamento

ALOG_DEBUG_BAD_FINDSGROUP
	Erro: findsgroup() retornou sem valores

ALOG_LARGENET_WARNING_1
	Voc� selecionou para executar DeNora com a menor quantidade de dados

ALOG_LARGENET_WARNING_2
	armazenado no SQL, voc� s� deve usar esta op��o se voc� estiver em uma

ALOG_LARGENET_WARNING_3
	rede muito grande de 100.000 usu�rios ou ter um banco de dados SQL remoto.

CONFIG_ERROR_BASE
	Erro na conf:  %s perto da linha %d de %s

CONFIG_UNKNOWN_FILE
	desconhecido

CONFIG_ERROR_OPENING
	N�o foi poss�vel abrir o arquivo %s

ALOG_DEBUG_RESOLVED_HOST
	debug: Resolvendo %s para %s

ALOG_DEBUG_BAD_SSTRDUP
	debug: sstrdup() retornou sem NULL-arg

ALOG_RESET_DAILY
	resetando estat�stiscas di�rias

ALOG_RESETTING_WEEKLY
	resetando estat�sticas semanais

ALOG_RESETTING_MONTHLY
	resetando estat�sticas mensais

ALOG_TRY_TO_DISPLAY
	debug: tentando mostrar mensagem %s

ALOG_DONE_DISPLAY
	debug: fez a apresenta��o da mensagem %s

ALOG_UKNOWN_USER_MODE
	debug: modo desconhecido %c (em %s)

ALOG_SWHOIS_ERROR
	SWHOIS setado em um usu�rios n�o existente/bot %s.

ALOG_VHOST_FOR
	debug: %s modificou seu vHost para %s

ALOG_REALNAME_FOR
	debug: %s modificou seu realmente para %s

ALOG_USERNAME_FOR
	debug: %s modificou seu identd para %s

ALOG_UMODE_NONEXISTANT
	user: MODO %s para um nick n�o existente/bot %s: %s

ALOG_UNKNWON_MSG
	debug: mensagem do servidor desconhecida (%s)

ALOG_TRYING_LOAD_MOD
	tetando carregar o m�dulo [%s]

ALOG_STATUS
	status: [%d]

ALOG_TRYING_UNLOAD_MOD
	tentando descarregar o m�dulo [%s]

RESTARTING
	Reiniciando

RESTART_FAILED
	Falhou o comando Restart

SAVING_FFF
	debug: Salvando FFF para o disco

ALOG_SIGPIPE_WARNING
	Denora ir� executar um backtrace e continuar, no entanto pode se tornar inst�vel

ALOG_NO_STATS_BIN
	Comando Restart falhou--STATS_BIN n�o definido (reconfigurar/recompilar)

ALOG_ERR_REHASH_NO_MEM
	Erro durante o carregamento do arquivo de configura��o. Sem mem�ria!

ALOG_ERR_REHASH
	Erro ao ler/rehash o arquivo de reconfigura��o (Recebendo SIGHUP)

BACKTRACE_LINES
	Backtrace: reportando as seguintes linhas

BACKTRACE_NOT_HERE
	Backtrace: n�o dispon�vel nesta plataforma - use GDB

ALOG_DEBUG_SERVINTRO
	debug: Servidor linkado (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: Servidor linkado (%s) de %s

ALOG_DEBUG_PINGTIME
	debug: Ping Time de %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Error: findserver() retornou sem valores

ALOG_ERR_FINDSERVERSTATS
	Erro: findserverstats() retornou sem valores

ALOG_ERR_FINDSERVERSUID
	Erro: findserver_suid() retornou sem valores

ALOG_ERR_DEL_SERVSTATS
	Erro: delete_serverstats() retornou sem valores

ALOG_DEL_SERVSTATS_FOR
	debug: delete_serverstats() retornou: %s

ALOG_DEL_SERVSTATS_DONE
	debug: delete_serverstats() completo

ALOG_ERR_DEL_SERVER
	Erro: delete_server() retornou sem valores

ALOG_DEL_SERVER_FOR
	debug: delete_server() retornou: %s

ALOG_DEL_SERVER_NOQUIT
	debug: delete_server() cancelou todos os usu�rios para NOQUIT

ALOG_DEL_SERVER_DONE
	debug: delete_server() completo

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: sql_do_server_version recebeu uma vers�o desconhecida

ALOG_MOD_TRY_LOAD
	tentando carregar [%s]

ALOG_MOD_STATUS
	status: [%d][%s]

ALOG_MOD_ERROR
	Erro no m�dulo: %s

ALOG_MOD_BE_ONLY_ONE
	Voc� n�o pode carregar dois protocolos de IRCd.

ALOG_MOD_UNLOAD_SELF
	debug: %s requisitou descarregamento...

ALOG_CMD_DISPLAY_START
	debug: tentando mostrar o comando %s

ALOG_CMD_DISPLAY_END
	debug: apresenta��o de mensagem feita com sucesso %s

ALOG_CMD_DISPLAY_LIST
	debug: Lista de comandos para Resultados %s

ALOG_MSG_DISPLAY_START
	debug: tentando mostrar comando %s

ALOG_MSG_DISPLAY_END
	debug: apresenta��o de mensagem feita com sucesso %s

ALOG_MSG_DISPLAY_LIST
	debug: Resultados da lista de mensagens %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: Adicionou module CallBack: [%s] devido a execu��o de %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST para um usu�rios n�o existente/bot %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME para um usu�rios n�o existente/bot %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME para um usu�rios n�o existente/bot %s

STAT_ADMIN_ALREADY
	%s j� � um admin do denora

STAT_ADMIN_CREATED
	%s � agora um admin do denora

STAT_ADMIN_DELETED
	%s foi removido da lista de admins do denora

STAT_ADMIN_NOTADMIN
	%s n�o � um admin do denora

STAT_ADMIN_SHOW
	Username: %s Hostmask: %s ID da Lang: %d

STAT_HELP_ADMIN
	Adiciona o remove algum usu�rio da lista de admin.
	N�o funciona com usu�rios adicionados no arquivo de configura��o
	
	ADMIN ADD username senha [hostmask] [lang-number]
	ADMIN DEL username
	ADMIN SETPASS username nova-senha
	ADMIN SHOW username
	ADMIN LIST
	
	O comando ADMIN ADD permite adicionar um usu�rio na lista de admins
	
	O comando ADMIN DEL permite deletar um usu�rio na lista de admins
	
	O comando ADMIN SETPASS permite trocar a senha de admin
	
	O comando ADMIN SHOW permite voc� saber certas informa��es de um determinado admin
	
	O comando ADMIN LIST lista os admins
	
	Limitado aos Administradores do Denora

STAT_ADMIN_SYNTAX
	ADMIN {ADD|DEL|SETPASS|SHOW|LIST} [user|pass|host|langnum]

STAT_ADMIN_DELETE_CONFIG
	N�o foi poss�vel deletar o %s pois ele est� adicionado no arquivo de configura��o

STAT_CHGPASS_OK
	Senha foi alterada para %s

STAT_CHGPASS_CONFIG
	N�o foi poss�vel mudar a sennha, pois est� adicionado no arquivo de configura��o
