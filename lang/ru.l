# Russian language file.
#
# Denora (c) 2004-2007 Denora Team
# Contact us at dev@denorastats.org
#
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id: ru.l 30 2007-12-18 07:28:15Z Kein $
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
# For example, "Espaсol (Spanish)" or "Franзais (French)".

LANG_NAME
	Русский (Russian)

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
	Вск
	Пн
	Вт
	Ср
	Чт
	Пт
	Сб
# %A
STRFTIME_DAYS_LONG
	Воскресенье
	Понедельник
	Вторник
	Среда
	Четверг
	Пятница
	Суббота
# %b
STRFTIME_MONTHS_SHORT
	Янв
	Фев
	Мар
	Апр
	Май
	Июн
	Июл
	Авг
	Сен
	Окт
	Ноя
	Дек
# %B
STRFTIME_MONTHS_LONG
	Январь
	Февраль
	Март
	Апрель
	Май
	Июнь
	Июль
	Август
	Сентябрь
	Октябрь
	Ноябрь
	Декабрь


# This is used in StatServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Внутренняя ошибка - невозможно выполнить запрос.
UNKNOWN_COMMAND
	Неизвестная команда %s.
UNKNOWN_COMMAND_HELP
	Неизвестная команда %s. Используйте "/msg %s HELP" для помощи по командам.
SYNTAX_ERROR
	Синтаксис: %s
MORE_INFO
	/msg %s HELP %s для более подробной информации.
NO_HELP_AVAILABLE
	Справочная информация по %s отсутствует.

SERVICE_OFFLINE
	%s на данный момент неактивен (offline).

SQL_DISABLED
	Вы не можете использовать данную команду - работа с SQL отключена.

STATS_UPTIME_DHMS
	Статистика ведется %d дней, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	Статистика ведется %d день, %02d:%02d:%02d
STATS_UPTIME_HMS
	Статистика ведется %d часов, %d минут, %d секунд
STATS_UPTIME_HM1S
	Статистика ведется %d часов, %d минут, %d секунду
STATS_UPTIME_H1MS
	Статистика ведется %d часов, %d минуту, %d секунд
STATS_UPTIME_H1M1S
	Статистика ведется %d часов, %d минуту, %d секунду
STATS_UPTIME_1HMS
	Статистика ведется %d час, %d минут, %d секунд
STATS_UPTIME_1HM1S
	Статистика ведется %d час, %d минут, %d секунду
STATS_UPTIME_1H1MS
	Статистика ведется %d час, %d минуту, %d секунд
STATS_UPTIME_1H1M1S
	Статистика ведется %d час, %d минуту, %d секунду
STATS_UPTIME_MS
	Статистика ведется %d минут, %d секунд
STATS_UPTIME_M1S
	Статистика ведется %d минут, %d секунд
STATS_UPTIME_1MS
	Статистика ведется %d минуту, %d секунд
STATS_UPTIME_1M1S
	Статистика ведется up %d минуту, %d секунд

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stats admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Доступ запрещен.
PERMISSION_DENIED
	У вас недостаточно прав для использования данной команды.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Да
SAY_NO
	Нет

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN не указан - выполнение команды RESTART невозможно.
	Перезапустите предустановочный скрипт \2configure\2 и
	пересоберите Denora'у с поддержкой команды RESTART.

# Module strings

STAT_MODULE_LOADED
	Модуль %s успешно загружен.
STAT_MODULE_UNLOADED
	Модуль %s успешно выгружен.
STAT_MODULE_LOAD_FAIL
	Невозможно загрузить модуль %s.
STAT_MODULE_REMOVE_FAIL
	Невозможно выгрузить модуль %s.
STAT_MODULE_LOAD_SYNTAX
	MODLOAD название_файла
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD название_файла
STAT_MODULE_NO_UNLOAD
	Данный модуль не может быть выгружен.
STAT_MODULE_LIST_HEADER
	Список текущих модулей:
STAT_MODULE_LIST
	Модуль: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	Всего модулей загружено: %d
STAT_MODULE_INFO_LIST
	Модуль: %s Версия: %s Автор: %s Загружен: %s
STAT_MODULE_CMD_LIST
	Предоставляемая команда: /msg %s %s
STAT_MODULE_MSG_LIST
	Предоставляемый IRCD-обработчик для: %s
STAT_MODULE_NO_LIST
	Список загруженых модулей пуст.
STAT_MODULE_NO_INFO
	Информация о модуле %s недоступна.
STAT_MODULE_INFO_SYNTAX
	MODINFO название_файла
MODULE_HELP_HEADER
	Данный модуль автоматически добавил следующие команды:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	Список команд %S:
	    STATS       Показывает статистику сети.
	    LOGIN       Идентификация на сервисах статистики.
	    LOGOUT      Деидентификация на сервисах статистики.

STAT_HELP_STATS
	Синтаксис: STATS
	
	Выводит статистику о количестве серверов сети, количестве
	созданных каналов и о количестве пользователей сети.

STAT_HELP_SHUTDOWN
	Синтаксис: SHUTDOWN
	
	Сохраняет базу данных сервисов статистики и завершает их работу.
	
	Данная команда доступна только Администраторам Статистики

STAT_HELP_RESTART
	Синтаксис: RESTART
	
	Перезапускает сервисы статистики.
	(то есть завершает их работу и тут же стартует заново)

STAT_HELP_RELOAD
	Синтаксис: RELOAD
	
	Перезагрузка конфигурационного файла сервисов статистики.

STAT_HELP_ADMIN_CMD
	Список команд, доступных только Администраторам Статистики:
	    ADMIN       Управление списком Администраторов.
	    CHANSTATS   Управление списком каналов статистики.
	    ENVINFO     Вывод различной дополнительной информации.
	    EXCLUDE     Список обьектов исключемых из статистики.
	    EXPORT      Экспорт текущих данных в XML.
	    HTML        Создание HTML-файла статистики.
	    MODLOAD     Загрузка модуля.
	    MODUNLOAD   Выгрузка модуля.
	    MODLIST     Список загруженных модулей.
	    MODINFO     Запрос информации о загруженном модуле.
	    RESTART     Перезапуск сервисов статистики.
	    RELOAD      Перезагрузка конфигурационного файла.
	    SET         Установка или смена различных опций "на лету".
	    SHUTDOWN    Завершение работы сервисов.

STAT_HELP_MODLOAD
	Синтаксис: MODLOAD название_файла

	Команда MODLOAD позволяет вам загрузить какой-либо
	дополнительный модуль из директории с модулями (modules/).

	Данная команда доступна только Администраторам Статистики

STAT_HELP_MODUNLOAD
	Синтаксис: MODUNLOAD название_файла

	Команда MODUNLOAD позволяет вам выгрузить какой-либо
	дополнительный модуль из списка текущих загруженных модулей.

	Данная команда доступна только Администраторам Статистики.

STAT_HELP_MODINFO
	Синтаксис: MODINFO название_файла

	Выводит различную справочную информацию об указанном модуле.

	Данная команда доступна только Администраторам Статистики.

STAT_HELP_MODLIST
	Синтаксис: MODLIST

	Выводит список всех загруженных модулей.

	Данная команда доступна только Администраторам Статистики.

STAT_HELP_EXCLUDE
	Синтаксис: EXCLUDE {ADD|DEL|LIST} ник|#канал
	
	Исключает указанный обьект из статистики.
	
	Данная команда доступна только Администраторам Статистики

STAT_HELP_EXPORT
	Синтаксис: EXPORT что_именно 
	
	Экспортирует указанные данные в XML.
	Что_именно в данном случае может быть:
	<channels|users|tld|servers|stats|all>
	
	Данная команда доступна только Администраторам Статистики

STAT_HELP_HTML
	Синтаксис: HTML
	
	Заставляет сервисы статистики сгенерировать HTML-отчет.

STAT_HELP_USERDEBUG
	Синтаксис: USERDEBUG user 
	
	Отображает отладочную информацию об указанном пользователе.

STAT_HELP_LOGIN
	Синтаксис: LOGIN логин пароль
	
	Позволяет вам идентифицироваться на сервисах статистики
	в качестве Администратора Статистики

STAT_HELP_LOGOUT
	Синтаксис: LOGOUT
	
	Деидентифицирует вас на сервисах статистики.
	(эффект обратный идентификации)

STAT_HELP_SET
	Синтаксис: SET OPTION [ON|OFF]

	Данная команда позволяет вам устанавливать различные
	опции сервисов статистики "на лету".

	Доступные опции:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	Данная команда доступна только Администраторам Статистики.

STAT_HELP_ENVINFO
	Синтаксис: ENVINFO
	
	Показывает различную дополнительную информацию.
	
	Данная команда доступна только Администраторам Статистики.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	Вы успешно идентифицировались как Администратор Статистики.

STAT_ADMIN_LOGGED_OUT
	Вы были деидентифицированы.

STAT_ADMIN_ALREADY_LOGGED
	Вы уже идентифицированы на сервисах статистики.

STAT_LOGIN_SYNTAX
	LOGIN логин пароль

STAT_RELOAD
	Конфигурационный файл сервисов статистики успешно перезагружен.

###########################################################################
#
# STATS messages
#
###########################################################################

STAT_STATS_SERVER_COUNT
	Статистика о серверах сети %s:
	Текущее количество серверов:  %d
	Максимально прилинковано серверов: %d

STAT_STATS_CHAN_COUNT
	Статистика о каналах сети %s:
	Текущее количество каналов:  %d
	Максимально создано каналов: %d

STAT_STATS_USER_COUNT
	Статистика о пользователях сети %s
	Текущее кол-во пользователей:  %d
	Максимальное кол-во пользователей: %d

STAT_STATS_SYNTAX
	STATS [SERVERS|CHANNELS|USERS]

STAT_STATS_PAGE
	Для более подробной информации см. %s

# SET responses
STAT_SET_SYNTAX
	SET опция параметр
STAT_SET_DEBUG_ON
	Сервисы статистики перешли в режим отладки.
STAT_SET_DEBUG_OFF
	Режим отладки отключен.
STAT_SET_DEBUG_LEVEL
	Сервисы статистики перешли в режим отладки (уровень %d).
STAT_SET_DEBUG_ERROR
	Параметр для опции DEBUG должен быть ON, OFF, или положительным числом.
STAT_SET_UNKNOWN_OPTION
	Неизвестная опция %s.
STAT_SET_SQL_ON
	Режим работы сервисов статистики с SQL БД успешно активирован.
STAT_SET_SQL_OFF
	Режим работы сервисов статистики с SQL БД успешно деактивирован.
STAT_SET_SQL_ERROR
	Значение для опции SQL должно быть либо ON либо OFF.
STAT_SET_SQL_ERROR_INIT
	Ошибка инициализации SQL, проверьте логи на наличие ошибок.
STAT_SET_SQL_ERROR_DISABLED
	Параметры для работы сервисов с SQL БД данной сети не указаны.
	Внесите необходимые опции/коррективы в файл denora.conf.
STAT_SET_LIST_OPTION_ON
	Режим %s: включен.
STAT_SET_LIST_OPTION_OFF
	Режим %s: отключен.
STAT_SET_HTML_ON
	Опция генерации HTML-отчета успешно активирована.
STAT_SET_HTML_OFF
	Опция генерации HTML-отчета успешно деактивирована.
STAT_SET_HTML_ERROR
	Значение для опции HTML должно быть либо ON либо OFF.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Данная команда позволит вам активировать сбор статистики указанного
	канала. Сервисы будут собирать статистическую информацию о кол-ве
	сказаных слов на канале, кол-ве букв в предложениях и т. д.
	
	CHANSTATS ADD #канал
	CHANSTATS DEL #канал
	CHANSTATS LIST
	CHANSTATS SET #канал FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER ник1 ник2
	CHANSTATS RENAME ник1 ник2
	
	CHANSTATS ADD - добавляет указанный канал в список "статистических"
	каналов.
	
	CHANSTATS DEL - антипод предыдущей команды, удаляет канал из списка
	"статистических" каналов.
	
	CHANSTATS LIST - выводит текущий список "статистических" каналов,
	то есть тех каналов, на которых ведется сбор статистики.
	
	CHANSTATS SET - позволяет активировать или деактивировать публичные
	fantasy-команды на указанном канале. Опция NOTICE аналогична опции ON,
	за тем лишь исключением, что ответ на fantasy-команды будет выводиться
	посредством нотиса (уведомления).
	
	CHANSTATS SUMUSER - суммирует статистические данные ника1 и ника2,
	и удаляет ник2 (ник1 будет сохранен с новой стат. информацией).
	
	HANSTATS RENAME - позволяет переименовать ник1 в ник2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SET|SUMUSER|RENAME} [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	Канал %s успешно добавлен в список каналов для сбора статистики.

STAT_CHANSTATS_CHAN_LARGENET
	Вы не можете добавлять новые каналы для сбора статистики,
	так как включена опция LargeNet.

STAT_CHANSTATS_LOGCHAN
	Сервисный канал не может быть добавлен в список "статистических" каналов.

STAT_CHANSTATS_NOT_VALID_CHAN
	Указано некорректное имя канала.

STAT_CHANSTATS_CHAN_ALREADY
	Канал %s уже присутствует в списке "статистических" каналов
	и для него уже ведется сбор статистики.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD #канал 

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL #канал

STAT_CHANSTATS_CHAN_DEL
	Канал %s успешно удален из списка "статистических" каналов.

STAT_CHANSTATS_CHAN_NOTFOUND
	Канал %s не найден в списке "статистических" каналов.

STATS_CHANSTATS_PART
	Сбор статистики для канала %s прекращен.

STATS_CHANSTATS_NOCHANNELS
	В списке "статистических" каналов нет ни одной записи...

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET #канал FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	Режим fantasy-команд для канала %s активирован.

STATS_CHANSTATS_FANTASY_OFF
	Режим fantasy-команд для канала %s деактивирован.

STATS_CHANSTATS_FANTASY_NOTICE
	Режим fantasy-команд для канала %s активирован, вывод
	информации будет осуществляться посредством нотисов

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER ник1 ник2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME ник1 ник2

STATS_CHANSTATS_SUMUSER_DONE
	Все статистические данные пользователя %s перенесены пользователю %s

STATS_CHANSTATS_RENAMEUSER_DONE
	Пользователь %s был переименован в %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} ник|#канал

STAT_EXCLUDE_ADDED
	В список исключений из статистики добавлен новый обьект: %s

STAT_EXCLUDE_ALREADY
	Обьект %s уже содержится в списке исключений.

STAT_EXCLUDE_DELETED
	Обьект %s успешно удален из списка исключений статистики.

STAT_EXCLUDE_NOT_FOUND
	Обьект %s не обнаружен в списке исключений из статистики, удалять нечего.

STAT_EXCLUDE_LIST_HEADER
	Список исключений:

# EXPORT responses
STATS_EXPORT_CHANNELS
	База каналов успешно экспортирована в %s

STATS_EXPORT_USERS
	База пользователей успешно экспортирована в %s

STATS_EXPORT_TLD
	TLD-база (доменов первого уровня) успешно экспортирована в %s

STATS_EXPORT_SERVERS
	База серверов успешно экспортирована в %s

STATS_EXPORT_STATS
	Общая статистика успешно экспортирована в %s

STATS_EXPORT_ALL
	Вся информация успешно экспортирована в %s

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	Пользователь %s не найден в базе данных статистики.

STATS_RENAMEUSER_EXISTING
	Пользователь %s уже существует в базе данных статистики.

STATS_HELP_CHANNEL_1
	На канале доступны следующие fantsasy-команды:

STATS_HELP_CHANNEL_2
	%stop10  :   Первая 10-ка самых общительных пользователей канала

STATS_HELP_CHANNEL_3
	%stop    :   Первая 3-ца самых общительных пользователей канала

STATS_HELP_CHANNEL_4
	%sgtop10 :   Первая 10-ка самых общительных пользователей все сети

STATS_HELP_CHANNEL_5
	%sgtop   :   Первая 3-ца самых общительных пользователей все сети

STATS_HELP_CHANNEL_6
	%sstats  :   Детальная статистика пользователя канала

STATS_HELP_CHANNEL_7
	%sgstats :   Глобальная статистика пользователя сети

STATS_HELP_CHANNEL_8
	%speak   :   Пик посещения канала (количество человек)

STATS_TOP10_CHANNEL
	Первая 10-ка для %s

STATS_TOP_CHANNEL
	Первая 3-ца для %s

STATS_TOP10_NETWORK
	Первая 10-ка всей сети

STATS_TOP_NETWORK
	Первая 3-ца всей сети

STATS_USER_NETWORK
	Общесетевая статистика для %s

STATS_USER_CHANNEL
	Локальная статистика для %s на %s

STATS_MESSAGE
	%2i %-16s букв: %s, слов: %s, строк: %s, смайлов: %s, действий: %s

STATS_MESSAGE_ONE
	букв: %s, слов: %s, строк: %s, смайлов: %s, действий: %s

STATS_CHANNEL_PEAK
	Максимальное количество пользователей достигало %d

# ENVINFO responses
STAT_ENVINFO_HEADER
	Дополнительная информация о сервисах статистики:
STAT_ENVINFO_VERSION
	Версия сервисов:            %s
STAT_ENVINFO_PROTOCOL
	IRCd-протокол:              %s
STAT_ENVINFO_UPLINK
	Uplink-сервер:              %s
STAT_ENVINFO_SQL
	SQL-режим:                  %s
STAT_ENVINFO_HTML
	HTML-режим:                 %s
STAT_ENVINFO_DEBUG
	Режим отладки:              %s
STAT_ENVINFO_MODULESLOADED
	Кол-во загруженных модулей: %d
STAT_ENVINFO_LANGUAGE
	Язык сети:                  %s
STAT_ENVINFO_FOOTER
	Конец списка дополнительной информации.

STATS_HTML_EXPORTED
	HTML-файл со статистическими данными успешно сгенерирован.

# HTML code
HTML_LANG
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
	<meta http-equiv="Content-Language" content="Russian">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Статистика сети:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<tr><td>Текущее кол-во пользов.: </td> <td> %ld </td> <td>Максимальное кол-во пользов.: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Текущее кол-во каналов.: </td> <td> %ld </td> <td>Максимальное кол-во каналов.: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>Текущее кол-во IRC-опов: </td> <td> %ld </td> <td>Максимальное кол-во IRC-опов: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Текущее кол-во серверов.: </td> <td> %ld </td> <td>Максимальное кол-во серверов: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Пользователей в away: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Статистика для сети %s

HTML_TLD_TITLE
	Текущий Top-список доменов первого уровня

HTML_TLD_HEADER
	<table><tr><th>Код страны</th><th>Страна</th><th>Кол-во</th><th>Всего</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Имя сервера</th></tr>

HTML_SERVER_CONTENT
	<tr><td>Сервер: </td><td><a href="#%s"> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td>Сервер: </td><td><a href="#%s"> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) означает, что сервер на данный момент в сети<br>(Щелкните курсором по имени сервера для более подробной информации)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>Дневная статистика сети:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4">(Вся дневная статистика в полночь обнуляется)</td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Пользователей за сегодня: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Каналов за сегодня:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">IRC-опов за сегодня: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Серверов за сегодня: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Версия</th><th align=right>Кол-во</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name="%s"></a>Сервер:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Текущее кол-во пользоват.:</td><td>%ld (%ld)</td><td>Макс. %ld на [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Текущее кол-во IRC-опов:</td><td>%ld (%ld)</td><td>Макс. %ld на [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Пинг сервера</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Общее кол-во пользоват.:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>Самый высокий пинг</td><td>%ld</td><td>на [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Кол-во сплитов сервера</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Последний раз был в сети:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>Кол-во Kill'ов от IRC-опов</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Кол-во Kill'ов от сервера</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Канал</th><th align=right>Посетителей</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Канал</th><th align=right>Всего заходов</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) указывает на то, что канал существовал и был активен на момент генерации отчета

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> "неприветливых" каналов (лидеры по кикам)

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Канал</th><th>Всего киков</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) указывает на то, что канал существовал и был активен на момент генерации отчета

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Канал</th><th>Всего топиков</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) указывает на то, что канал существует на данный момент

HTML_TOP10_TOPIC_TITLE
	Первая 10-ка каналов с частой сменой топика

HTML_CUR_NETSTATS_TITLE
	Текущая статистика сети

HTML_DAILY_TITLE
	Дневная статистика сети

HTML_TOP10_ONLINE_CHANS_TITLE
	Первая 10-ка активных каналов сети

HTML_SERVERLIST_TITLE
	Список серверов

HTML_TOP10_CHANEVER_TITLE
	Первая 10-ка всех каналов в целом

HTML_SERVERDETAILS_TITLE
	Детально о серверах

HTML_TOP10_CLIENT_TITLE
	Статистика по версиям IRC-клиентов

HTML_NETMAP_TITLE
	Карта сети

HTML_VERSION
	Статистика предоставлена продуктом <a href="http://denorastats.org/">Denora</a> версии %s %s

HTML_MOST_UNWELCOME_TITLE
	Самые "неприветливые" каналы

HTML_POPULAR_TITLE
	Популярные каналы

HTML_MOST_TOPIC_TITLE
	Статистика по топикам

HTML_NETSTATS_TITLE
	Сетевая статистика

NOW_STATS_ADMIN
	%s (%s) идентифицировался как Администратор Статистики.

STATS_AUTH_FAILED
	Неудачная попытка идентификации %s на сервисах статистики.

STATS_ADMIN_LOGOUT
	%s деидентифицировался на сервисах статистики.

SHUTDOWN_SAVING_DB
	Обновление базы данных перед выключением, пожалуйста подождите.

CANNOT_OPEN_LOG
	Внимание: невозможно открыть лог-файл: %s

GLOBAL_LOAD_MOD
	%s загрузил модуль %s

GLOBAL_UNLOAD_MOD
	%s выгрузил модуль %s

GLOBAL_FATAL_ERROR
	FATAL ERROR! %s: %s

CONFIG_PORT_ERROR
	Номер порта должен быть в диапазоне чисел 1025..65535

CONFIG_HOSTNAME_ERROR
	Не верно указана или отсутствует опция hostname в блоке connect.

CONFIG_PROTOCOL_ERROR
	Не верно указан или отсутствует модуль IRCD-протокола в блоке connect.

CONFIG_PORT_ERROR_NOTDEF
	Не верно указан или отсутствует номер порта в блоке connect.

CONFIG_PASSWD_ERROR
	Не верно указана или отсутствует опция passwd в блоке connect.

CONFIG_PING_TO_LOW
	Опция pingfreq должна быть равна 2-ум или более минутам.

CONFIG_SQLPING_TO_LOW
	Опция sqlfreq должна быть равна 2-ум или более минутам.

CONFIG_UPTIME_TO_LOW
	Опция uptimefreq должна быть равна 2-ум или более минутам.

CONFIG_DELAYMOD_T0MANY
	Слишком большое кол-во delayed-модулей (макс. 128)

CONFIG_TABLE_USER_ERROR
	Не верно указана или отсутствует опция users в блоке tables.

CONFIG_TABLE_CHANBANS_ERROR
	Не верно указана или отсутствует опция chanbans в блоке tables.

CONFIG_TABLE_CHANEXCEPT_ERROR
	Не верно указана или отсутствует опция chanexcept в блоке tables.

CONFIG_TABLE_ISON_ERROR
	Не верно указана или отсутствует опция ison в блоке tables.

CONFIG_TABLE_SERVER_ERROR
	Не верно указана или отсутствует опция server в блоке tables.

CONFIG_TABLE_GLINE_ERROR
	Не верно указана или отсутствует опция gline в блоке tables.

CONFIG_INVALID_LANG
	Некорректный номер языка

CONFIG_ID_HOST_ERROR
	Не верно указана или отсутствует опция user(@host) в блоке identify.

CONFIG_ID_LANG_ERROR
	Не верно указана или отсутствует опция language в блоке identify.

CONFIG_ID_USER_ERROR
	Не верно указана или отсутствует опция user в блоке identify.

CONFIG_ID_DESC_ERROR
	Не верно указана или отсутствует опция desc в блоке identify.

CONFIG_ID_NAME_ERROR
	Не верно указана или отсутствует опция name в блоке identify.

CONFIG_ERROR_POSTIVE_VALUE
	Ожидалось положительное значение директивы.

CONFIG_SQL_USER_ERROR
	Не верно указана или отсутствует опция user в блоке sql.

CONFIG_SQL_NAME_ERROR
	Не верно указана или отсутствует опция name в блоке sql.

CONFIG_ADMIN_NAME_ERROR
	Не верно указана или отсутствует опция name в блоке admin.

CONFIG_ADMIN_HOST_ERROR
	Не верно указана или отсутствует опция hostname в блоке admin.

CONFIG_ADMIN_PASS_ERROR
	Не верно указана или отсутствует опция passwd в блоке admin.

CONFIG_TIMEOUT_PING_ERROR
	Не верно указана или отсутствует опция pingfreq в блоке timeout.

CONFIG_TIMEOUT_SQL_ERROR
	Не верно указана или отсутствует опция sqlfreq в блоке timeout.

CONFIG_TIMEOUT_UPTIME_ERROR
	Не верно указана или отсутствует опция uptimefreq в блоке timeout.

CONFIG_TIMEOUT_HTML_ERROR
	Не верно указана или отсутствует опция htmlupdate в блоке timeout.

CONFIG_TIMEOUT_UPDATE_ERROR
	Не верно указана или отсутствует опция update в блоке timeout.

CONFIG_TIMEOUT_WARNING_ERROR
	Не верно указана или отсутствует опция warning в блоке timeout.

CONFIG_TIMEOUT_READ_ERROR
	Не верно указана или отсутствует опция read в блоке timeout.

CONFIG_INVALID_TIME
	Указано не верное значение по времени.

CONFIG_HTML_UPDATE_TIME
	Опция htmlupdate должна быть равна 2-ум или более минутам.

CONFIG_NETINFO_NAME_ERROR
	Не верно указана или отсутствует опция name в блоке netinfo.

CONFIG_FILENAME_PID_ERROR
	Не верно указана или отсутствует опция pid в блоке filenames.

CONFIG_FILENAME_MOTD_ERROR
	Не верно указана или отсутствует опция motd в блоке filenames.

CONFIG_FILENAME_CHANDB_ERROR
	Не верно указана или отсутствует опция channeldb в блоке filenames.

CONFIG_FILENAME_CTCP_ERROR
	Не верно указана или отсутствует опция ctcpdb в блоке filenames.

CONFIG_FILENAME_SERVER_ERROR
	Не верно указана или отсутствует опция serverdb в блоке filenames.

CONFIG_FILENAME_CHANSTATS_ERROR
	Не верно указана или отсутствует опция chanstatsdb в блоке filenames.

CONFIG_FILENAME_TLD_ERROR
	Не верно указана или отсутствует опция tlddb в блоке filenames.

CONFIG_FILENAME_EXCLUDE_ERROR
	Не верно указана или отсутствует опция excludedb в блоке filenames.

CONFIG_SS_DESC_ERROR
	Не верно указана или отсутствует опция real в блоке statserv.

CONFIG_SS_NAME_ERROR
	Не верно указана или отсутствует опция nick в блоке statserv.

CONFIG_SS_TOLONG
	Указан слишком длинный ник.

CONFIG_SS_ALIASDESC_ERROR
	Не верно указана или отсутствует опция aliasreal в блоке statserv.

ALOG_FAILED_SQL_CONNECT
	Ошибка подключения к SQL-базе данных : %s

ALOG_SQL_ENABLED
	%s: успешно включено.

ALOG_SQL_DISABLED
	MySQL: успешно отключено.

ALOG_SQL_NOTSET
	MySQL Error: указаны не все необходимые опции в sql-блоке.

ALOG_HTML_ERR_WRITE
	Error: недостаточно прав для записи файла: %s

ALOG_DB_ERROR
	WARNING! DB_READ_ERROR в %s

ALOG_DEBUG_DB_OK
	debug: %s успешно считано

ALOG_DEBUG_UNKNOW_CMODE
	debug: неизвестный канальный режим %c

ALOG_DEBUG_LAST_LINE
	debug: последняя строка %s

ALOG_DEBUG_REPORT_AT
	debug: отпишитесь об ошибке на http://bugtracker.nomadirc.net

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Channel: получен join несуществующего пользователя %s, ID канала %d

ALOG_DEBUG_CHANGE_MODE_CHAN
	debug: изменение режимов для %s на %s

AL0G_DEBUG_BAD_CHAN_MODE
	Debug: обнаружен некорректный режим на %s.

ALOG_DEBUG_MODE_NO_PARAM
	channel: режим %c%c на канале %s указан без параметров (?)

ALOG_DEBUG_FINDCHAN_BAD
	Error: функция findchan() вызвана без параметров (NULL-значение)

ALOG_DEBUG_FINDSTATCHAN_BAD
	Error: функция findstatschan() вызвана без параметров (NULL-значение)

ALOG_DEBUG_SJOIN_NONEXISTANT
	user: получен SJOIN для несуществующего пользователя %s на %s

ALOG_DEBUG_JOIN_NONEXISTANT
	user: получен JOIN для несуществующего пользователя %s на %s

ALOG_DEBUG_JOIN
	debug: %s зашел на %s

ALOG_DEBUG_KICK_NONEXISTANT
	user: получен KICK для несуществующего пользователя %s на %s: %s

ALOG_DEBUG_KICKED
	debug: кикнут пользователь %s с канала %s

ALOG_DEBUG_PART_NONEXISTANT
	user: получен PART от несуществующего пользователя %s: %s

ALOG_DEBUG_PART
	debug: %s покидает %s

ALOG_DEBUG_PART_BUG
	user: BUG при покидании канала %s: запись о канале обнаружена, однако c->chan NULL

ALOG_NON_TS_MODE
	Режим TSMODE включен, однако MODE не имеет валидного TS

ALOG_DEBUG_NO_FIND_CHAN
	debug: невозможно найти канал %s

ALOG_DEBUG_TOPIC_TS
	debug: закодированное TS-значение TOPIC'а %s преобразовано в %d

ALOG_TOPIC_NONEXISTANT
	channel: значение TOPIC'а %s для несуществующего канала %s

ALOG_ADD_BAN_BAD
	debug: функция add_ban вызвана без параметров (NULL-значение)

ALOG_ADD_BAN_ADDED
	debug: добавлен бан %s на канале %s

ALOG_ADD_EXCEPTION_ADDED
	debug: добавлено исключение %s на канале %s

ALOG_ADD_INVITE_ADDED
	debug: добавлено автоприглашение %s на канале %s

ALOG_CREATE_CHAN
	debug: создание канала %s

ALOG_DELETE_CHAN
	debug: удаление канала %s

ALOG_BAN_FREE_ERROR
	channel: BUG удаления %s: bans[%d] is NULL!

ALOG_EXCEPTION_FREE_ERROR
	channel: BUG удаления %s: excepts[%d] is NULL!

ALOG_INVITE_FREE_ERROR
	channel: BUG удаления %s: invite[%d] is NULL!

ALOG_BAN_DEL_CHAN
	debug: удален бан %s с канала: %s

ALOG_EXCEPTION_DEL_CHAN
	debug: удалено исключение %s с канала: %s

ALOG_INVITE_DEL_CHAN
	debug: удалено автоприглашение %s с канала: %s

ALOG_DEBUG_FLOOD_MODE
	debug: режим флуда для канала %s теперь: %s

ALOG_NO_FLOOD
	не указан

ALOG_KEY_SET_TO
	debug: Ключ для канала %s теперь: %s

ALOG_NO_KEY
	не указан

ALOG_LIMIT_SET_TO
	debug: лимит пользователей для  %s теперь: %u

ALOG_REDIRECT_SET_TO
	debug: опции перенаправления для %s теперь: %s

ALOG_NO_REDIRECT
	без перенаправления

ALOG_DEBUG_BAD_FINDSGROUP
	Error: функция findsgroup() вызвана без параметров (NULL-значение)

ALOG_LARGENET_WARNING_1
	Вы выбрали вариант запуска сервисов статистики с наименьшим кол-вом данных

ALOG_LARGENET_WARNING_2
	сохраняемых в БД SQL. Стоит заметить, что использовать данный режим рекомендуется

ALOG_LARGENET_WARNING_3
	только для ОЧЕНЬ больших сетей (100,000+ поль.) и при работе с удаленной SQL БД.

CONFIG_ERROR_BASE
	Conf Error:  %s near line %d of %s

CONFIG_UNKNOWN_FILE
	неизвестен

CONFIG_ERROR_OPENING
	Невозможно открыть конфигурационный файл %s

ALOG_DEBUG_RESOLVED_HOST
	debug: адрес вида %s отрезолвлен в %s

ALOG_DEBUG_BAD_SSTRDUP
	debug: функция sstrdup() вызвана без параметров (NULL-значение)

ALOG_RESET_DAILY
	очистка дневной статистики

ALOG_RESETTING_SUNDAY
	очистка недельной статистики (в Воскресенье)

ALOG_RESETTING_MONDAY
	очистка недельной статистики (в Понедельник)

ALOG_RESETTING_TUESDAY
	очистка недельной статистики (во Вторник)

ALOG_RESETTING_WEDNESDAY
	очистка недельной статистики (в Среду)

ALOG_RESETTING_THURSDAY
	очистка недельной статистики (в Четверг)

ALOG_RESETTING_FRIDAY
	очистка недельной статистики (в Пятницу)

ALOG_RESETTING_SATURDAY
	очистка недельной статистики (в Субботу)

ALOG_RESETTING_MONTHLY
	очистка месячной статистики 

ALOG_TRY_TO_DISPLAY
	debug: попытка отобразить сообщение %s

ALOG_DONE_DISPLAY
	debug: успешно отображено сообщение %s

ALOG_UKNOWN_USER_MODE
	debug: неизвестный пользовательский режим %c (в %s)

ALOG_SWHOIS_ERROR
	SWHOIS установлен для несуществующего пользователя %s.

ALOG_VHOST_FOR
	debug: %s изменил свой vhost на %s

ALOG_REALNAME_FOR
	debug: %s изменил свое имя на %s

ALOG_USERNAME_FOR
	debug: %s изменил свой идент на %s

ALOG_UMODE_NONEXISTANT
	user: MODE %s для несуществующего пользователя %s: %s

ALOG_UNKNWON_MSG
	debug: неизвестное сообщение от сервера (%s)

ALOG_TRYING_LOAD_MOD
	попытка загрузить [%s]

ALOG_STATUS
	статус: [%d]

ALOG_TRYING_UNLOAD_MOD
	попытка выгрузить [%s]

RESTARTING
	Перезапуск...

RESTART_FAILED
	Перезапуск неудачен

SAVING_FFF
	debug: сохранение FFF-информации на диск

ALOG_SIGPIPE_WARNING
	Сервисы статистики организуют вывод backtrace, однако их последующая работа может быть нестабильна.

ALOG_NO_STATS_BIN
	Попытка перезапуска неудачна-- не указан STATS_BIN (перезапустите configure)

ALOG_ERR_REHASH_NO_MEM
	Возникла ошибка во время перезагрузки конфига, недостаточно памяти!

ALOG_ERR_REHASH
	Ошибка чтения конфигурационного файла (получен сигнал SIGHUP)

BACKTRACE_LINES
	Backtrace: пошлите отчет, включающий в себя нижеследующие строки

BACKTRACE_NOT_HERE
	Backtrace: не доступен для данной програм. платформы, используйте GDB

ALOG_DEBUG_SERVINTRO
	debug: обнаружен сервер (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: обнаружен сервер (%s) от %s

ALOG_DEBUG_PINGTIME
	debug: время пинга от %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Error: функция findserver() вызвана без параметров (NULL-значение)

ALOG_ERR_FINDSERVERSTATS
	Error: функция findserverstats() вызвана без параметров (NULL-значение)

ALOG_ERR_FINDSERVERSUID
	Error: функция findserver_suid() вызвана без параметров (NULL-значение)

ALOG_ERR_DEL_SERVSTATS
	Error: функция delete_serverstats() вызвана без параметров (NULL-значение)

ALOG_DEL_SERVSTATS_FOR
	debug: функция delete_serverstats() вызвана с параметрами %s

ALOG_DEL_SERVSTATS_DONE
	debug: процедура delete_serverstats() успешно завершена

ALOG_ERR_DEL_SERVER
	Error: функция delete_server() вызвана без параметров (NULL-значение)

ALOG_DEL_SERVER_FOR
	debug: функция delete_server() вызвана с параметрами %s

ALOG_DEL_SERVER_NOQUIT
	debug: функция delete_server() очистила список всех пользователей для NOQUIT

ALOG_DEL_SERVER_DONE
	debug: процедура delete_server() успешно завершена

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: функция sql_do_server_version получила неизвестный формат версии

ALOG_MOD_TRY_LOAD
	попытка загрузить [%s]

ALOG_MOD_STATUS
	статус: [%d][%s]

ALOG_MOD_ERROR
	Ошибка модуля: %s

ALOG_MOD_BE_ONLY_ONE
	Вы не можете загрузить сразу два модуля протокола.

ALOG_MOD_UNLOAD_SELF
	debug: получен запрос на выгрузку %s

ALOG_CMD_DISPLAY_START
	debug: попытка отображения команды %s

ALOG_CMD_DISPLAY_END
	debug: успешно выполнено отображение команды %s

ALOG_CMD_DISPLAY_LIST
	debug: отображение списка команд для %s

ALOG_MSG_DISPLAY_START
	debug: попытка отобразить сообщение %s

ALOG_MSG_DISPLAY_END
	debug: успешно отображено сообщение %s

ALOG_MSG_DISPLAY_LIST
	debug: отображение списка сообщений для %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: добавлен CallBack модуля: [%s] во время выполнения %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST для несуществующего пользователя %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME для несуществующего пользователя %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME для несуществующего пользователя %s

STAT_ADMIN_ALREADY
	Пользователь %s уже присутствует в списке Администраторов Статистики.

STAT_ADMIN_CREATED
	Пользователь %s добавлен в список Администраторов Статистики.

STAT_ADMIN_DELETED
	Пользователь %s удален из списка Администраторов Статистики.

STAT_ADMIN_NOTADMIN
	Пользователь %s не содержится в списке Администраторов Статистики.

STAT_ADMIN_SHOW
	Ник: %s Хостмаска: %s Номер языка: %d

STAT_HELP_ADMIN
	Позволяет удалять или добавлять новых Администраторов Статистики.
	Данный список хранится и обрабатывается отдельно от списка
	администраторов в конфигурационном файле сервисов.
	
	ADMIN ADD ник пароль [хостмаска] [номер_языка]
	ADMIN DEL ник
	ADMIN SETPASS ник новый_пароль
	ADMIN SHOW ник
	ADMIN LIST
	
	ADMIN ADD - позволяет добавить нового Администратора Статистики
	
	ADMIN DEL - антипод предыдущей команды, удаляет существующего Администратора.
	
	ADMIN SETPASS - позволяет изменить пароль указанного Администратора.
	
	ADMIN SHOW - показывает детальную информацию об аккаунте Администратора.
	
	ADMIN LIST - выводит список всех существующих Администраторов Статистики.
	
	Данная команда доступна только Администраторам Статистики

STAT_ADMIN_SYNTAX
	ADMIN {ADD|DEL|SETPASS|SHOW|LIST} [ник|пароль|хостмаска|номер_языка]

STAT_ADMIN_DELETE_CONFIG
	Администратор %s не может быть удален из списка,
	так как он прописан в конфигурационном файле сервисов.

STAT_CHGPASS_OK
	Пароль для аккаунта %s успешно изменен.

STAT_CHGPASS_CONFIG
	Вы не можете изменить пароль для Администраторов,
	прописанных в конфигурационном файле сервисов.
