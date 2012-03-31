/*************************************************************************/
/* (c) 2004-2012 Denora Team                                             */
/* Contact us at info@denorastats.org                                       */
/*                                                                       */
/* Please read COPYING and README for furhter details.                   */
/*                                                                       */
/* Based on the original code of Anope by Anope Team.                    */
/* Based on the original code of Thales by Lucas.                        */
/*                                                                       */
/*                       */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/* Define the largest buffer into which we can copy a xmlrpc reply       */
/*************************************************************************/

#define XMLRPC_BUFSIZE         8192

/*************************************************************************/
/* Defines for the xmlrpc_set_options() function                         */
/*************************************************************************/

#define XMLRPC_ON          "on"
#define XMLRPC_OFF         "off"
#define XMLRPC_HTTP_HEADER 1
#define XMLRPC_ENCODE      2
#define XMLRPC_INTTAG      3
#define XMLRPC_I4          "i4"
#define XMLRPC_INT         "integer"

/*************************************************************************/
/* Defines for the various xmlrcp tags                                   */
/*************************************************************************/

#define XMLRPC_DATETIME_TAG "dateTime.iso8601"
#define XMLRPC_BOOLEAN_TAG  "boolean"
#define XMLRPC_STRING_TAG   "string"
#define XMLRPC_BASE64_TAG   "base64"
#define XMLRPC_I4_TAG       "i4"
#define XMLRPC_DOUBLE_TAG   "double"

/*************************************************************************/
/* Define the output format for the datetime tag in iso8601 format       */
/*************************************************************************/

#define XMLRPC_DATETIME_FORMAT "%Y%m%dT%I:%M:%S"

/*************************************************************************/
/* structs that handle the commands and command hash                     */
/*************************************************************************/

typedef struct XMLRPCCmd_ XMLRPCCmd;
typedef struct XMLRPCCmdHash_ XMLRPCCmdHash;


E XMLRPCCmdHash *XMLRPCCMD[MAX_CMD_HASH];

typedef struct xmlrpc_settings {
	char *(*setbuffer)(char *buffer, int len);
	char *encode;
    int httpheader;
    char *inttagstart;
	char *inttagend;
} XMLRPCSet;

struct XMLRPCCmd_ {
    int (*func)(deno_socket_t socketid, int ac, char **av);
	char *name;
    int core;
    char *mod_name;
    XMLRPCCmd *next;
};

struct XMLRPCCmdHash_ {
        char *name;
        XMLRPCCmd *xml;
        XMLRPCCmdHash *next;
};




