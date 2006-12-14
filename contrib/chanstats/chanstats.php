<?php

//******************************************
// count script execution time
$start = doubleval(time() + microtime());
//
// ******************************************

require_once('common.php');



if ($_GET['chan']) { $chan = $_GET['chan']; } else { $chan = "global"; }
$chan_ = $chan;
if (($chan != "global") && !ereg('^#',$chan)) { $chan_ = '#'.$chan; }



if ($_GET['order']) { $order = $_GET['order']; } else { $order = "letters"; }
if ($_GET['type']) { $type = $_GET['type']; } else { $type = "0"; }

$q = mysql_query("SELECT * FROM ustats WHERE (type=".$type." AND chan=\"".mysql_escape_string($chan_)."\" AND letters > 0) ORDER BY ".$order." DESC;");

echo "<table border=\"1\">";
echo "<tr>",
	"<th>no.</th>",
	"<th>nick</th>",
	"<th><a href=\"chanstats.php?order=letters&type=".$type."&chan=".$chan."\">letters</a></th>",
	"<th><a href=\"chanstats.php?order=words&type=".$type."&chan=".$chan."\">words</a></th>",
	"<th><a href=\"chanstats.php?order=line&type=".$type."&chan=".$chan."\">lines</a></th>",
	"<th><a href=\"chanstats.php?order=actions&type=".$type."&chan=".$chan."\">actions</a></th>",
	"<th><a href=\"chanstats.php?order=smileys&type=".$type."&chan=".$chan."\">smileys</a></th>",
	"<th><a href=\"chanstats.php?order=kicks&type=".$type."&chan=".$chan."\">kicks</a></th>",
	"<th><a href=\"chanstats.php?order=modes&type=".$type."&chan=".$chan."\">modes</a></th>",
	"<th><a href=\"chanstats.php?order=topics&type=".$type."&chan=".$chan."\">topics</a></th>",
	"</tr>\n";
$i=1;
while ($r = mysql_fetch_row($q)) {
	echo "<tr><td>", $i,
	    "</td><td>", $r[0], // uname
	    "</td><td>", $r[3], // letters 
	    "</td><td>", $r[4], // words 
	    "</td><td>", $r[5], // lines
	    "</td><td>", $r[6], // actions
	    "</td><td>", $r[7], // smileys
	    "</td><td>", $r[8], // kicks
	    "</td><td>", $r[10], // modes
	    "</td><td>", $r[11], // topics
	    "</td></tr>\n";
	    $i++;
}
echo "</table>\n";
echo "<br>show <a href=\"chanstats.php?order=".$order."&type=0&chan=".$chan."\">total</a>, ",
    "<a href=\"chanstats.php?order=".$order."&type=1&chan=".$chan."\">daily</a>, ",
    "<a href=\"chanstats.php?order=".$order."&type=2&chan=".$chan."\">weekly</a>, ",
    "<a href=\"chanstats.php?order=".$order."&type=3&chan=".$chan."\">monthly</a> stats";

//*****************************************************************
// show script execution time
echo "\n<br><br>\n";
echo "  ", round(((time() + microtime())-$start),6), " sec";
//*****************************************************************


?>