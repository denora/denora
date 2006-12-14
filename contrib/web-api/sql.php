<?php

/* 
  SQL Library of functions so that we can access just about ever sql function via a
  error checking, error handling library without specifics of the SQL coming in the
  way of a developers wants

  sql_init()        : Very important to call this before you start to run any commands, this loads the
                      config into memory.

  sql_destory()     : this guts out memory all of the variables that the SQL lib puts up there

  sql_db_connect()  : returns resource id of the database link, this replaces connect/select db calls
                      this is the core call for all sql connects.

  sql_query()       : queries the sql database, send as you would the standard query calls

  sql_fetch_array() : Fetch the row as an Array, send as you would the standard fetch_array() calls

  sql_fetch_field() : Fetch a field data for one record, no need to call connect, query, or close all 
                      this is an all inclusive call.

  sql_db_close()    : Close up the Connections and call free resources

  sql_num_rows()    : Get the number of rows, affected by a query

  sql_update_data() : Send the query command to UPDATE a specific field of a record

  sql_search()      : Search the database, needs work, lots of work
  
  sql_query_num_rows() : does connect, does the query, returns count and closes the connection
*/


function sql_empty_table($table)
{
 $link = sql_db_connect();
 $q = sql_query("DELETE FROM $table");
 sql_db_close($link,$q);
}

function sql_query_num_rows($str)
{
 $link = sql_db_connect();
 $query = sql_query($str);
 $num = sql_num_rows($query);
 return ($num > 0 ? $num : "0");
}

/**
 * @return resource
 * @param table string
 * @param for string
 * @param fields string
 * @desc method of searching the database for a record or records
 */
function sql_search($table,$for,$fields)
{
 $link = sql_db_connect();
 $res = sql_query("select * FROM $table WHERE $fields LIKE '%$for%'");

 while ($data = sql_fetch_array($res)) {
  print_r($data);
 }
}


/**
 * @return Resource
 * @desc Connect to the database
 * Parm 1 : Database Type Override
 * Parm 2 : Host override
 * Parm 3 : User name override
 * Parm 4 : Password override
 * Parm 5 : Table name override
 * Parm 6 : Connection Type override
 * Parm 7 : Port if not passed via the Host argument
 */
function sql_db_connect() 
{
 global $np_db_host, $np_db_user, $np_db_pass, $np_db_name, $np_db_type, $np_db_port,
        $np_db_connect, $debug;
 /* You can override the default by sending the type you need to use */
 $type = (@func_get_arg(0) ? func_get_arg(0) : $np_db_type);
 /* You can send the HOST via the function call */
 $host = (@func_get_arg(1) ? func_get_arg(1) : $np_db_host);
 /* You can send the USER via the function call */
 $user = (@func_get_arg(2) ? func_get_arg(2) : $np_db_user);
 /* You can send the PASSWORD via the function call */
 $pass = (@func_get_arg(3) ? func_get_arg(3) : $np_db_pass);
 /* You can send the DATABASE via the function call */
 $db = (@func_get_arg(4) ? func_get_arg(4) : $np_db_name);
 /* You can send the Stand Connect or Perm Connect via the function call */
 $dbcon = (@func_get_arg(5) ? func_get_arg(5) : $np_db_connect);
 /* You can send the Port Address via the function call */
 $port = @func_get_arg(6);
 
 /* MySQL */

 if ($type == 1) {
  $host = ($port ? "$host:$port" : $host);
  if ($dbcon == 1) {
   if ($link_id = mysql_connect($host, $user, $pass)) {
    sql_query("USE $db",$link_id);
    return $link_id; 
   }
   else { return FALSE; }
  }
  else {
   if ($link_id = mysql_pconnect($host, $user, $pass)) {
    sql_query("USE $db");
    return $link_id; 
   }
   else { return FALSE; }
  }
 }
 else { return FALSE; } 
}

/**
 * @return resource
 * @param qstr string
 * @desc alias of the _query or _exec functions
 */
function sql_query($qstr)
{
 global $np_db_type;

 $resc = @func_get_arg(1);

 //echo $qstr;

 /* You can override the default by sending the type you need to use */
 $type = (@func_get_arg(2) ? func_get_arg(2)  : $np_db_type);
 if ($type == 1) { 
  if ($resc) { $r = @mysql_query($qstr,$resc); }
  else { $r = @mysql_query($qstr); }
  $num = @mysql_errno();
  $msg = @mysql_error();
  //echo($msg);
 }
 return $r;
}

/**
 * @return Array
 * @param resc resource
 * @desc Alias to _fetch_array() functions
 */
function sql_fetch_array($resc)
{
 global $np_db_type, $debug;
 /* You can override the default by sending the type you need to use */
 $type = (@func_get_arg(1) ? func_get_arg(1)  : $np_db_type);
 if ($type == 1) { $r = @mysql_fetch_array($resc); }
 return $r;
}

/**
 * @return mixed
 * @param table string
 * @param wherefield string
 * @param where mixed
 * @param field string
 * @desc Return data from one filed of a table
 */
function sql_fetch_field($table,$wherefield,$where,$field)
{
 $link = sql_db_connect();
 $result = sql_query("SELECT $field FROM $table WHERE $wherefield = '$where'", $link);
 $data = sql_fetch_array($result);
 $a = (isset($data[$field]) ? $data[$field] : FALSE);
 sql_db_close($link,$result);
 return $a;
}

/**
 * @return array
 * @param table string
 * @param wherefield string
 * @param where mixed
 * @desc Returns all the fields for a given row
 */
function sql_fetch_fields($table,$wherefield,$where)
{
 $fields = null;
 $n = func_num_args();
 if ($n <= 3) { return FALSE; }
 for ($i = 2; $i <= $n - 1; $i++) {
  $fields .= func_get_arg($i) . ",";
 }
 $fields = substr($fields, 0, -1);
 $link = sql_db_connect();
 $result = sql_query("SELECT $fields FROM $table WHERE $wherefield = '$where'", $link);
 $data = sql_fetch_array($result);
 $d = (isset($data) ? $data : FALSE);
 sql_db_close($link,$result);
 return $d;
}

/**
 * @return array
 * @param qstr string
 * @desc returns an array of all the fields the meet the query
 */
function sql_mult_row_fetch_fields($qstr)
{
 $link = sql_db_connect();
 $result = sql_query($qstr, $link);
 $c = array();
 $cc = 1;
 while($data = sql_fetch_array($result)) { $c[$cc++] = $data; }
 return $c;
}


/**
 * @return boolean
 * @param table string
 * @param field string
 * @param value mixed
 * @param wherefield string
 * @param where mixed
 * @desc Update a field in a row of the database
 */
function sql_update_data($table,$field,$value,$wherefield,$where)
{
 $link = sql_db_connect();
 $result = sql_query("UPDATE $table SET $field = '$value' WHERE $wherefield = '$where'",$link);
 $n = sql_num_rows($result);
 return ($n ? TRUE : FALSE); 
}

/**
 * @return void
 * @param link resource
 * @param result resource
 * @desc Close up and clean up the database results
 */
function sql_db_close($link,$result) 
{
 global $np_db_type, $debug;
 if ($np_db_type == 1) {
  @mysql_close($link);
  @mysql_free_result($result);
 }
}

/**
 * @return mixed
 * @param result resource
 * @param int integer
 * @desc Alias to the the _result functions
 */
function sql_result($result,$int) 
{
 global $np_db_type, $debug;
 if ($np_db_type == 1) {
  return @mysql_result($result,$int);
 }
}


/**
 * @return int
 * @param result resource
 * @desc Alias to _num_rows functions
 */
function sql_num_rows($result) 
{
 global $np_db_type, $debug;
 $type = (@func_get_arg(1) ? func_get_arg(1)  : $np_db_type);
 if ($type == 1) { $r = @mysql_num_rows($result); }
 return $r;
}


