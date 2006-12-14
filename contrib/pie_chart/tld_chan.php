<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Denora Pie Charts</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
</head>
<body>

<?php

require_once('common.php');

if (isset($_GET['chan'])) { $chan = $_GET['chan']; } else { $chan = "#denora"; }
$chan_ = $chan;
if (!ereg('^#',$chan_)) { $chan_ = '#'.$chan; }

echo "<div style=\"text-align: center;\"><h1>TLD</h1><br />\n";

$count = NULL;
$country = NULL;

/* 1. */

$q = mysql_query("SELECT COUNT(*) FROM chan, ison, user ".
		    "WHERE chan.chanid = ison.chanid ".
		    "AND ison.nickid = user.nickid ".
		    "AND channel=\"".mysql_escape_string($chan_)."\" ".
		    "AND user.online=\"Y\" ".
                    "AND user.mode_us=\"N\";");
if ($q) {
 $sum = mysql_fetch_row($q);
 $sum = $sum[0];
} else {
 $sum = 0;
}

echo "<h2>$sum users in $chan</h2>";

/* 2. */

$q = mysql_query("SELECT user.country, COUNT(*) AS country_count
		    FROM user, chan, ison
		    WHERE user.nickid=ison.nickid
			AND ison.chanid=chan.chanid
			AND chan.channel=\"".mysql_escape_string($chan_)."\"
			AND user.online=\"Y\"
                        AND user.mode_us=\"N\"
		    GROUP by user.country
			ORDER BY country_count DESC;");

$i=0;
$buf=0;
if ($q) {
 while ($r = mysql_fetch_row($q)) {
    $i++;
    if ($i == $db['maxdisplay']) {
		$country[$i] = "other";
		$count[$i] = $sum - $buf;
		break;
    }
    if ($r[0]) {	
		 $country[$i] = $r[0]; 
	} else {
		 $country[$i] = "other";
	}
    $count[$i] = $r[1];
    $buf = $buf + $count[$i];
 }
}

/*************************************************************************************/
/* draw
/************************************************************************************/
require_once('piegraph.class.php');

// class call with the width, height & data
$pie = new PieGraph(250, 150, $count);

// colors for the data
 $pie->setColors(array("#808080", "#FF0000", "#00FF00", 
                       "#FFFF00", "#0000FF", "#FF00FF", 
                       "#00FFFF", "#ff8800", "#22aa11",
                       "#33ff33", "0afff0" ));

// legends for the data
  if ($country) {
$pie->setLegends($country);
  }
// Display creation time of the graph
$pie->DisplayCreationTime();

// Height of the pie 3d effect
$pie->set3dHeight(15);

// set the filename 
$pie->SetFileName("tld_chan.png");

// generate the graph
$pie->display();

// display the graph
echo "<img src=\"tld_chan.png\" alt=\"\" /></div>";
?>

</body>
</html>
