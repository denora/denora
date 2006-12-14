<?php

/*
   Display Unreal Glines
*/

$db['server'] = "localhost";
$db['user']   = "user";
$db['pass']   = "password";
$db['db']     = "denora";

$lid = mysql_connect($db['server'], $db['user'], $db['pass']);
mysql_select_db($db['db'], $lid);

$q = mysql_query("SELECT * FROM glines");


echo("<table><tr>
          <td>Type</td>
          <td>user</td>
          <td>host</td>
          <td>set by</td>
		  <td>set at</td>
		  <td>expires</td>
		  <td>reason</td>
		 </tr>");

while ($data = mysql_fetch_array($q)){
 printf("<tr>
          <td>%s</td>
          <td>%s</td>
          <td>%s</td>
          <td>%s</td>
		  <td>%s</td>
		  <td>%s</td>
		  <td>%s</td>
		 </tr>", $data['type'], $data['user'], $data['host'], $data['setby'], $data['setat'],
				 $data['expires'], $data['reason']);
}

echo("</table>");
