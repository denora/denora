<?

$mysqluser = "test";
$mysqlpassword = "password";
$mysqlhost = "localhost";
$mysqldb   = "denora";
$mysql_usertable = "user";
$mysql_servertable = "server";
$mysql_isontable = "ison";
$mysql_chantable = "chan";


$link = mysql_connect ($mysqlhost, $mysqluser, $mysqlpassword) or die ("Can't connect");
mysql_select_db($mysqldb);

$q = mysql_query(sprintf("select * from %s where nick=\"$nickname\"", $mysql_usertable));
if (mysql_num_rows($q) < 1) 
{
		mysql_close($link);
		die($nickname ." No such nick/channel");
}
while ($row = mysql_fetch_array($q))
{
	$id = $row["nickid"];
	$rname = $row["realname"];
	$hostname = $row["hostname"];
	$username = $row["username"];
	$connecttime = $row["connecttime"];
	$servid  = $row["servid"];
	$server = $row["server"];
	$away = $row["away"];
	if ($away == "Y") $awaymsg = $row["awaymsg"];
	$isircop = $row["mode_lo"];
	if (isset($row["mode_la"])) {
		$issadmin = $row["mode_la"];
	}
	if (isset($row["mode_lr"])) {
		$isreg = $row["mode_lr"];
	}
}

$q_servers = mysql_query(sprintf("select comment from %s where servid=%d", $mysql_servertable, $servid));
$r = mysql_fetch_row($q_servers);
$scomm = $r[0];

$chid = Array();
$chname = Array();
$channels = NULL;
$q_channs = mysql_query(sprintf("select channel from %s,%s,%s where nick = \"$nickname\" and (%s.chanid=%s.chanid and %s.nickid=%s.nickid)", $mysql_isontable, $mysql_chantable, $mysql_usertable, $mysql_isontable, $mysql_chantable, $mysql_usertable, $mysql_isontable));
while ($r = mysql_fetch_array ($q_channs))
{
       	$tmp = $r["channel"];
       	$q_mode = mysql_query(sprintf("select mode_ls,mode_lp,chanid from %s where channel=\"$tmp\"", $mysql_chantable));
		$q_mode_r = mysql_fetch_row($q_mode);
       	if ($q_mode_r[0] == "N" && $q_mode_r[1] == "N") 
       	{
			$q_getop=mysql_query(sprintf("select mode_lo,mode_lv from %s where chanid=%d and nickid=%d", $mysql_isontable, $q_mode_r[2], $id));
			$chmode = mysql_fetch_row($q_getop);
			if ($chmode[0]=="Y") $channels = $channels . " @" . $tmp;
       		elseif ($chmode[1]=="Y") $channels = $channels . " +" . $tmp;
       		else $channels = $channels . " " . $tmp;
       	}
}
mysql_close($link);


echo "<table width=\"550\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"center\">";
echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname is ".$username."@".$hostname." * ".$rname."</font></td></tr>";
echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname is connected since $connecttime</font></td></tr>";
if (!is_null($channels)) echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname on $channels</font></td></tr>";
echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname using $server $scomm</font></td></tr>";
if ($isreg == "Y") echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname has identified for this nick</font></td></tr>";
if ($away == "N") echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname is not away</font></td></tr>";
else echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname is away: $awaymsg</font></td></tr>";
if ($isircop == "Y") echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname is an IRC Operator</font></td></tr>";
if ($issadmin == "Y") echo "<tr><td><font face=\"Arial, Helvetica, sans-serif\" size=\"2\">$nickname is a Service Administrator</font></td></tr>";
echo "<tr><td align=\"center\"><font face=\"Arial, Helvetica, sans-serif\" size=\"2\"><hr></font></td></tr>";
echo "</table>";
?>
