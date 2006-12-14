<?php

/*
  Denora to web
  - Trystan Scott Lee
*/

require("denora.cfg.php");         // Build with the config file
require("sql.php");          	   // SQL Library
if ($denora_server_type) {
	require(sprintf("ircds/%s.php", $denora_server_type));
}

if (!isset($denora_format_time)) {
	$denora_format_time = "D M d G:i:s";
}

$denora_resource_id;

function denora_init()
{
  global $denora_resource_id;

  if (!$denora_resource_id = sql_db_connect()) {
	echo("There was an error");
  }
}

function denora_chanstats_chans_array()
{
 global $denora_cstats_db;

 $x = 0;
 $temp = null;

 $query = sql_query("SELECT * FROM `$denora_cstats_db` WHERE `type` = 0");
 if ($query) {
  while ($data = sql_fetch_array($query)) {
   $temp[$x]            = $data;
   $temp[$x]['urlchan'] = denora_remove_first($data['channel']);
   $x++;
  }
  return $temp;
 }
 else {
  return false;
 }
}

function denora_remove_first()
{
 $str = (@func_get_arg(0) ? func_get_arg(0) : null);
	
 if (!$str) {
  return false;
 }

 return substr($str,1,strlen($str));
}

/*
  returns current users flagged as online
*/
function denora_num_users()
{
 global $denora_user_db;
 
 if ($denora_user_db) {
	return  sql_query_num_rows(sprintf("select * from %s where online = 'Y'", $denora_user_db));
 } else {
	return 0;
 }
}

/*
  returns current number of opers
*/
function denora_num_opers()
{
 global $denora_user_db, $ircd;

 if ($ircd['supports_hidden_ops']) {
	 return sql_query_num_rows(sprintf("select * from %s where mode_lo = 'Y' and online = 'Y'  and %s != 'Y'",$denora_user_db, $ircd['hidden_op_mode']));
 } else {
	 return sql_query_num_rows(sprintf("select * from %s where mode_lo = 'Y' and online = 'Y'", $denora_user_db));
 }
}

/*
  returns current number of channels
*/
function denora_num_channels()
{
 global $denora_chan_db;
 if ($denora_chan_db) {
	return sql_query_num_rows(sprintf("select * from %s", $denora_chan_db));
 } else {
 	return 0;
 }
}

/*
  returns current number user in a channel
*/
function denora_num_in_chan($chan)
{
  global $denora_chan_db;
  if ($denora_chan_db) {
   $query = sql_query(sprintf("select currentusers from %s where channel='%s'", $denora_chan_db, $chan));
   $data = sql_fetch_array($query);
   return $data['currentusers'];
  } else {
	return 0;
  }
}

/*
  returns current number user on a given server
*/
function denora_server_current_users($server)
{
  global $denora_server_db, $denora_resource_id;
 
  $qstr = sprintf("select currentusers from %s where server='%s'", $denora_server_db, $server);
  $query = sql_query($qstr);
  $data = sql_fetch_array($query);
  if (isset($data['currentusers'])) { return $data['currentusers']; }
  else { return 0; }
}

/*
  returns max number user on a given server
*/
function denora_server_max_users($server)
{
  global $denora_server_db;

  $query = sql_query(sprintf("select maxusers from %s where server='%s'", $denora_server_db, $server));
  $data = sql_fetch_array($query);
  if (isset($data['maxusers'])) { return $data['maxusers']; }
  else { return 0; }
}

/*
  returns the time of max number user on a given server
*/
function denora_server_max_users_time($server)
{
  global $denora_server_db, $denora_format_time;

  if (isset($denora_server_db)) {
    if ($server) {
     $query = sql_query(sprintf("select maxusertime from %s where server='%s'", $denora_server_db, $server));
     $data = sql_fetch_array($query);
     return (isset($data['maxusertime']) ? date($denora_format_time, $data['maxusertime']) : 0);
   } else {
	  return 0;
   }
  } else {
	return 0;
  }
}

function denora_server_motd($server)
{
  global $denora_server_db;

  $query = sql_query(sprintf("select motd from %s where server='%s'", $denora_server_db, $server));
  $data = sql_fetch_array($query);
  $motd = str_replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;", $data['motd']); 
  $motd = str_replace(" ", "&nbsp;", $motd); 
  return nl2br($motd);
}

function denora_server_user_stats($server)
{
  global $denora_server_db;

  $query = sql_query(sprintf("select currentusers, maxusers, maxusertime from %s where server='%s'", $denora_server_db, $server));
  $data = sql_fetch_array($query);
  $array['currentusers'] = $data['currentusers'];
  $array['maxusers'] = $data['maxusers'];
  $array['maxusertime'] = $data['maxusertime'];
  return $array;
}

function denora_tld_current_by_code($tld)
{
  global $denora_tld_db;

  $query = sql_query(sprintf("select count from %s where code='%s'", $denora_tld_db, $tld));
  $data = sql_fetch_array($query);
  return $data['count'];
}

function denora_tld_current_by_country($tld)
{
  global $denora_tld_db;

  $query = sql_query(sprintf("select count from %s where country='%s'", $denora_tld_db, $tld));
  $data = sql_fetch_array($query);
  return $data['count'];
}

function denora_tld_overall_by_code($tld)
{
  global $denora_tld_db;

  $query = sql_query(sprintf("select overall from %s where code='%s'", $denora_tld_db, $tld));
  $data = sql_fetch_array($query);
  return $data['overall'];
}

function denora_tld_overall_by_country($tld)
{
  global $denora_tld_db;

  $query = sql_query(sprintf("select overall from %s where country='%s'", $denora_tld_db, $tld));
  $data = sql_fetch_array($query);
  return $data['overall'];
}

function denora_tld_array_by_country($tld)
{
  global $denora_tld_db;

  $query = sql_query(sprintf("select * from %s where country='%s'", $denora_tld_db, $tld));
  $data = sql_fetch_array($query);
  $array['overall'] = $data['overall'];
  $array['count'] = $data['count'];
  $array['countrycode'] = $data['code'];
  $array['country'] = $data['country'];
  return $array;
}

function denora_tld_array_by_code($tld)
{
  global $denora_tld_db;

  $query = sql_query(sprintf("select * from %s where code='%s'", $denora_tld_db, $tld));
  $data = sql_fetch_array($query);
  $array = NULL;
  if ($data) {
   $array['overall'] = $data['overall'];
   $array['count'] = $data['count'];
   $array['countrycode'] = $data['code'];
   $array['country'] = $data['country'];
  }
  if ($array) {
    return $array;
  } else {
	return NULL;
  }
}

function denora_tld_array_top10()
{
  global $denora_tld_db;
  $i = 0;
  $array = array();

  $query = sql_query(sprintf("select * from %s ORDER BY count DESC LIMIT 10", $denora_tld_db));
  while ($data = sql_fetch_array($query)) {
	  if ($data['count']) {
   	    $array[$i]['overall'] = $data['overall'];
  	    $array[$i]['count'] = $data['count'];
	    $array[$i]['countrycode'] = $data['code'];
	    $array[$i++]['country'] = $data['country'];
	  }
  }
  return $array;
}

function denora_num_chan_bans($chan)
{
  global $denora_chanbans_db;
  return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE chan='%s'", $denora_chanbans_db, $chan));
}

function denora_num_chan_except($chan)
{
  global $denora_chanexcept_db, $ircd;
  if ($ircd['chanexception']) {
    return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE chan='%s'", $denora_chanexcept_db, $chan));
  } else {
	return 0;
  }
}

function denora_num_sqline()
{
  global $denora_gline_db, $denora_sqline_db, $ircd;
  if ($ircd['sqlinetable']) {
    return sql_query_num_rows(sprintf("SELECT * FROM %s", $denora_sqline_db));
  } else if ($ircd['glinetable']) {
    return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE type='Q'", $denora_gline_db));
  } else {
	return 0;
  }
}

function denora_is_ulined($server)
{
  global $denora_server_db;
  return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE uline='1' and server='%s'", $denora_server_db, $server));
}

function denora_is_sqline($host)
{
  global $denora_gline_db, $denora_sqline_db, $ircd;
  if ($ircd['sqlinetable']) {
    return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE mask='%s'", $denora_sqline_db, $host));
  } else if ($ircd['glinetable']) {
    return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE type='Q' and host='%s'", $denora_gline_db, $host));
  } else {
	return FALSE;
  }
}

function denora_is_online($nick)
{
  global $denora_user_db;
  return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE nick='%s' and online='Y'", $denora_user_db, $nick));
}

function denora_get_user_status($nick)
{
  global $denora_user_db;
  $query = sql_query(sprintf("select * from %s WHERE nick='%s'", $denora_user_db, $nick));
  $data = sql_fetch_array($query);
  $array = array();

  if ($data) {
	$array['online'] = ($data['online'] == 'Y' ? 1 : 0);
	$array['offline'] = ($data['online'] == 'N' ? 1 : 0);
	$array['oper'] = ($data['mode_lo'] == 'Y' ? 1 : 0);
	$array['away'] = ($data['away'] == 'Y' ? 1 : 0);
	return $array;
  } else {
 	return NULL;
  }
}

function denora_get_chan_topic($chan)
{
  global $denora_chan_db;
  $query = sql_query(sprintf("select * from %s WHERE channel='%s'", $denora_chan_db, $chan));
  $data = sql_fetch_array($query);
  if ($data) {
	return $data['topic'];
  } else {
	return NULL;
  } 
}

function denora_get_chan_topic_html($chan)
{
  global $denora_chan_db;
  $query = sql_query(sprintf("select * from %s WHERE channel='%s'", $denora_chan_db, $chan));
  $data = sql_fetch_array($query);
  if ($data) {
	return denora_colorconvert($data['topic']);
  } else {
	return NULL;
  } 
}

function denora_num_chan_invites($chan)
{
  global $denora_chaninvite_db, $ircd;
  if ($ircd['chaninvites']) {
	  return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE chan='%s'", $denora_chaninvite_db, $chan));
  } else {
	return 0;
  }
}

function denora_num_away()
{
  global $denora_user_db;
  return sql_query_num_rows(sprintf("SELECT * FROM %s WHERE away='Y'", $denora_user_db));
}


function denora_lusers($server, $rettype)
{
 global $denora_server, $denora_user_db;

 $array = denora_max_stats();

 $user = sql_query_num_rows(sprintf("select * from %s where mode_li != 'Y' and online = 'Y'", $denora_user_db));
 $iuser = sql_query_num_rows(sprintf("select * from %s where mode_li = 'Y' and online = 'Y'", $denora_user_db));
 $tempval = denora_server_current_users(($server ? $server : $denora_server));
 $luser = ($tempval ? $tempval : 0);
 $tempval = denora_server_max_users(($server ? $server : $denora_server));
 $lusermax = ($tempval ? $tempval : 0);
 $server = denora_num_servers();
 $op = denora_num_opers();
 $chan = denora_num_channels();
 $total = denora_num_users();


 if ($rettype == 1) {
  return sprintf("There are %s users and %s invisible on %s servers<br>
                 %s operator(s) online<br>
                 %s channels formed<br>
				 Current Local Users: %s  Max: %s<br>
                 Current Global Users: %s  Max: %s<br>",$user,$iuser,$server,$op,$chan,$luser,$lusermax, $total,$array['users']);
 } else {
  printf("There are %s users and %s invisible on %s servers<br>
                 %s operator(s) online<br>
                 %s channels formed<br>
				 Current Local Users: %s  Max: %s<br>
                 Current Global Users: %s  Max: %s<br>",$user,$iuser,$server,$op,$chan,$luser,$lusermax, $total,$array['users']);
 }
}

function denora_max_num_channels()
{
 $array = denora_max_stats();
 return $array['channels'];
}

function denora_max_num_user()
{
 $array = denora_max_stats();
 return $array['users'];
}

function denora_max_stats()
{
 global $denora_maxvalues;

 $link = sql_db_connect();
 $query = sql_query(sprintf("SELECT * FROM %s", $denora_maxvalues));
 $c = 0;
 $array = NULL;
 while ($data = sql_fetch_array($query)) {
  $array[$c++] = array($data['type'] => $data['val']);
 }

 if ($array) {
  $c = array("opers" => $array[3]['opers'],
			"servers" => $array[2]['servers'], 
            "channels" => $array[1]['channels'],
            "users" => $array[0]['users']);
 } else {
  $c = array("opers" => 0,
			"servers" => 0, 
            "channels" => 0,
            "users" => 0);
 }
 return $c;
}

function denora_servername_array($show)
{
 global $denora_noshow_servers, $denora_server_db;	
 $noshow = array();
 $no = explode(",",$denora_noshow_servers);

 for ($i = 0; $i <= count($no) - 1; $i++) {
	$noshow[$i] = strtolower($no[$i]);
 }

 $link = sql_db_connect();
 $query = sql_query(sprintf("SELECT server FROM %s", $denora_server_db));
 $n = array();
 $c = 0;
 while ($data = sql_fetch_array($query)) {
   if ($show) {
	   $n[$c++] = $data['server'];
   } else {
	   if (!in_array(strtolower($data['server']),$noshow)) {  
			$n[$c++] = $data['server']; 
	   }
   }
 }
 return $n;
}

function denora_servers_array()
{
 global $denora_noshow_servers, $denora_server_db;

 $show = (@func_get_arg(0) ? func_get_arg(0) : 0);

 $noshow = array();
 $no = explode(",",$denora_noshow_servers);

 for ($i = 0; $i <= count($no) - 1; $i++) {
	$noshow[$i] = strtolower($no[$i]);
 }

 $link = sql_db_connect();
 $query = sql_query(sprintf("SELECT * FROM %s", $denora_server_db));
 $n = array();
 $c = 0;
 while ($data = sql_fetch_array($query)) {
   if ($show) {
	   $n[$c++] = $data;
   } else {
	   if (!in_array(strtolower($data['server']),$noshow)) {  
			$n[$c++] = $data; 
	   }
   }
 }
 return $n;
}

function denora_server_array()
{
 global $denora_server_db;

 $show = (@func_get_arg(0) ? func_get_arg(0) : 0);

 $link = sql_db_connect();
 $query = sql_query(sprintf("SELECT * FROM %s WHERE server='%s'", $denora_server_db, $show));
 $data = sql_fetch_array($query);
 return $data;
}

function denora_num_servers()
{
 $x = count(denora_servername_array(1));
 return $x - 1;
}

function denora_user_country($user)
{
 global $denora_user_db;

 $link = sql_db_connect();
 $query = sql_query(sprintf("SELECT countrycode, country FROM %s WHERE nick='%s'", $denora_user_db, $user));
 if (sql_num_rows($query)) {
	 $array = sql_fetch_array($query);
     $temp['country'] = $array['country'];
     $temp['countrycode'] = $array['countrycode'];
	 return $temp;
 } else {
	return NULL;
 }
}

function sort_by_field($a,$f,$t) 
{
 $x = (@func_get_arg(3) ? func_get_arg(3) : "0");
 $s1 = array();
 $count = ($x == "0" ? count($a) - 1 : count($a));
 for ($i = $x; $i <= $count; $i++) { $s1[$i] = $a[$i][$f]; }
 if (eregi("^(1|asc)$",$t)) { asort($s1); }
 else {
  if (eregi("^(2|desc)$",$t)) { arsort($s1); }
  else { asort($s1); }
 }
 $s2 = array();
 $i = $x; 
 foreach ($s1 as $k => $v) { $s2[$i++] = $a[$k]; }
 return $s2;
}

function denora_format_date($str)
{
 global $denora_format_time;
 
 if (!$str) { return false; }
 
 $format = (@func_get_arg(1) ? func_get_arg(1) : $denora_format_time);
 $e = explode(" ",$str);
 $t = explode(":",$e[1]);
 $d = explode("-",$e[0]);
 $time = mktime($t[0],$t[1],$t[2],$d[1],$d[2],$d[0]);
 return date($format,$time);
}

function denora_who_in_channel($chan)
{
 global $denora_chan_db, $denora_ison_db, $denora_user_db;

 $link = sql_db_connect();
 $query = sql_query("select chanid from $denora_chan_db where channel = '$chan'",$link);
 $darray = sql_fetch_array($query);
 $carray = NULL;
 sql_db_close($link,$query);
 $link = sql_db_connect();
 $query = sql_query("select * from $denora_ison_db where chanid = '$darray[chanid]'",$link);
 $x = 0;
 while ($data = sql_fetch_array($query)) { 
   $carray[$x]['nickid'] = $data['nickid'];
   if (isset($data['mode_la'])) {
	   $carray[$x]['a'] = $data['mode_la']; 
   }
   if (isset($data['mode_lh'])) {
	   $carray[$x]['h'] = $data['mode_lh']; 
   }
   $carray[$x]['o'] = $data['mode_lo']; 
   $carray[$x]['v'] = $data['mode_lv'];
   if (isset($data['mode_lq'])) {
	   $carray[$x++]['q'] = $data['mode_lq']; 
   }
 } 
 sql_db_close($link,$query);

 $link = sql_db_connect();
 $y = 0;

 $str = "Users in channel<br><table width=\"100%\" border=\"1\"><tr><td width=\"30%\"><b>Username</b></td><td  width=\"40%\">Address</td><td>Mode</td></tr>";

 if ($carray) {
  for ($x = 0; $x <= count($carray) - 1; $x++)  {
   $id = $carray[$x]['nickid'];
   $query = sql_query("select * from $denora_user_db where nickid = '$id'",$link);
   $array = sql_fetch_array($query);
    $mode = null;
   if ($carray[$x]['a'] == 'Y') { $mode = "+a"; }
   if ($carray[$x]['h'] == 'Y') { $mode .= " +h"; }
   if ($carray[$x]['o'] == 'Y') { $mode .= " +o"; }
   if ($carray[$x]['v'] == 'Y') { $mode .= " +v"; }
   if ($carray[$x]['q'] == 'Y') { $mode .= " +q"; }
   if ($mode == null) { $mode = "No mode set"; }
   $str .= sprintf("<tr><td>%s</td><td>%s</td><td>%s</td></tr>",$array['nick'],$array['hiddenhostname'],$mode);
  }
 }
 $str .= sprintf("</table>");

 echo $str;
}

function denora_list_chans()
{
 global $denora_chan_db, $denora_ison_db;

 $link = sql_db_connect();
 $query = sql_query(sprintf("select * from %s WHERE mode_ls != 'Y' and mode_lp != 'Y'", $denora_chan_db));

 $str = "<table width=\"100%\" border=\"1\">
          <tr>
           <td>Channel</td>
           <td>Users</td>
           <td>Topic</td>
          </tr>";

 $array = array();
 $c = 0;

 while ($data = sql_fetch_array($query)) {
  $num = sql_query(sprintf("SELECT * FROM %s where chanid = '$data[chanid]'", $denora_ison_db));
  $number = sql_num_rows($num);
  $array[$c] = $data;
  $array[$c]['name'] = substr($data['channel'],1,1);
  $array[$c++]['numusers'] = $number;
 }

 $array = sort_by_field($array,"name","asc"); 

// print_r($array);

 for ($i = 0; $i <= count($array) - 1; $i++) {
  if (strlen($array[$i]['topic']) < 1) { $topic = "&nbsp;"; }
  else {
   $topic = denora_colorconvert(htmlspecialchars($array[$i]['topic']));
  }
  $thrity = "30%";
  $str .= sprintf("<tr><td valign=top>%s</td>
             <td valign=top>%s</td>
             <td>%s</td>
			 </tr>",
  
  $array[$i]['channel'],
  $array[$i]['numusers'],
  $topic);
 }
 $str .= "</table>";
 echo $str;
}

function denora_colorconvert($log) {
 $color["color:0;"] = "color:#FFFFFF;";
 $color["color:1;"] = "color:#000000;";		
 $color["color:2;"] = "color:#00007F;";
 $color["color:3;"] = "color:#009300;";		
 $color["color:4;"] = "color:#FF0000;";
 $color["color:5;"] = "color:#7F0000;";
 $color["color:6;"] = "color:#9C009C;";
 $color["color:7;"] = "color:#FC7F00;";
 $color["color:8;"] = "color:#FFFF00;";
 $color["color:9;"] = "color:#00FC00;";
 $color["color:10;"]= "color:#009393;";
 $color["color:11;"]= "color:#00FFFF;";
 $color["color:12;"]= "color:#0000FC;";
 $color["color:13;"]= "color:#FF00FF;";
 $color["color:14;"]= "color:#7F7F7F;";
 $color["color:15;"]= "color:#D2D2D2;";
 $ctrl->k = chr(03);
 $log = ereg_replace("$ctrl->k([[:digit:]]{1,2}),([[:digit:]]{1,2})",
                     "</font><font style=\"color:\\1;background-color:\\2;\">",$log);
 $log = ereg_replace("$ctrl->k([[:digit:]]{1,2})","</font><font style=\"color:\\1;\">",$log);
 $log = str_replace($ctrl->k,"</font>",$log);
 $log = strtr($log,$color);

 $ctrl_b = chr(02);
 $log = ereg_replace("$ctrl_b([^\r$ctrl_b]*)[$ctrl_b]","<b>\\1</b>",$log);
 $log = ereg_replace("$ctrl_b([^\r$ctrl_b]*)\r","<b>\\1</b>\r",$log);
 $ctrl_u = chr(31);
 $log = ereg_replace("$ctrl_u([^\r$ctrl_u]*)[$ctrl_u]","<u>\\1</u>",$log);
 $log = ereg_replace("$ctrl_u([^\r$ctrl_u]*)\r","<u>\\1</u>\r",$log);
 $ctrl_i = chr(22);
 $log = ereg_replace("$ctrl_i([^\r$ctrl_i]*)[$ctrl_i]","<i>\\1</i>",$log);
 $log = ereg_replace("$ctrl_i([^\r$ctrl_i]*)\r","<i>\\1</i>\r",$log);
 $ctrl_s = chr(18);
 $log = ereg_replace("$ctrl_s([^\r$ctrl_s]*)[$ctrl_s]","<s>\\1</s>",$log);
 $log = ereg_replace("$ctrl_s([^\r$ctrl_s]*)\r","<s>\\1</s>\r",$log);
 $log = ereg_replace("(\*\*\*[^\r]*)\r","<font color=#009300>\\1</font>\r",$log);
 $log = ereg_replace("([".chr(1)."-".chr(127)."])[".chr(128)."-".chr(255)."]([".chr(1)."-".chr(127)."])","\\1 \\2",$log);
 $log = ereg_replace("([".chr(1)."-".chr(127)."])[".chr(128)."-".chr(255)."]([".chr(1)."-".chr(127)."])","\\1 \\2",$log);
 $log = strtr($log,array("\r\n"=>" </span><br>\r\n"));
 return $log;
}

function denora_server_list()
{
 $sa = denora_server_array(1);

 $str = "<table width=\"100%\" border=\"1\"><tr><td>Name</td></tr>";
 
 for ($i = 0; $i <= count($sa) - 1; $i++) {
   $str .= sprintf("<tr><td>%s</td></tr>", $sa[$i]['server']);
 }
 $str .= "</table>";
 echo $str;
}

