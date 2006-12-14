<?php

//******************************************
// count script execution time
$start = doubleval(time() + microtime());
//
// ******************************************

require_once('common.php');

if ($_GET['order']) { $order = $_GET['order']; } else { $order = "letters"; }
if ($_GET['type']) { $type = $_GET['type']; } else { $type = "0"; }

$q = mysql_query("SELECT * FROM cstats WHERE type=".$type." AND letters > 0 ORDER BY ".$order." DESC;");

echo "<table border=\"1\">";
echo "<tr>",
	"<th>no.</th>",
	"<th>channel</th>",
	"<th><a href=\"channels.php?order=letters&type=".$type."\">letters</a></th>",
	"<th><a href=\"channels.php?order=words&type=".$type."\">words</a></th>",
	"<th><a href=\"channels.php?order=line&type=".$type."\">lines</a></th>",
	"<th><a href=\"channels.php?order=actions&type=".$type."\">actions</a></th>",
	"<th><a href=\"channels.php?order=smileys&type=".$type."\">smileys</a></th>",
	"<th><a href=\"channels.php?order=kicks&type=".$type."\">kicks</a></th>",
	"<th><a href=\"channels.php?order=modes&type=".$type."\">modes</a></th>",
	"<th><a href=\"channels.php?order=topics&type=".$type."\">topics</a></th>",
	"</tr>\n";
$i=1;
while ($r = mysql_fetch_row($q)) {
	echo "<tr><td>", $i,
	    "</td><td>", "<a href=\"chanstats.php?order=".$order."&type=".$type."&chan=".substr($r[0],1)."\">".$r[0]."</a>", // channel
	    "</td><td>", $r[3], // letters 
	    "</td><td>", $r[4], // words 
	    "</td><td>", $r[5], // lines
	    "</td><td>", $r[6], // actions
	    "</td><td>", $r[7], // smileys
	    "</td><td>", $r[8], // kicks
	    "</td><td>", $r[9], // modes
	    "</td><td>", $r[10], // topics
	    "</td></tr>\n";
	    $i++;
}
echo "</table>\n";
echo "<br>show <a href=\"channels.php?order=".$order."&type=0\">total</a>, ",
    "<a href=\"channels.php?order=".$order."&type=1\">daily</a>, ",
    "<a href=\"channels.php?order=".$order."&type=2\">weekly</a>, ",
    "<a href=\"channels.php?order=".$order."&type=3\">monthly</a> stats";

//*****************************************************************
// show script execution time 
echo "\n<br><br>\n";
echo "  ", round(((time() + microtime())-$start),6), " sec";
//*****************************************************************

?>