<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Denora Pie Charts</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
</head>
<body>

<?php

require_once('common.php');


$count = NULL;
$version = NULL;

/* 1. */

$q =  mysql_query("SELECT COUNT(*) FROM user WHERE online=\"Y\" and mode_us=\"N\";");
if ($q) {
 $sum = mysql_fetch_row($q);
 $sum = $sum[0];
} else {
 $sum = 0;
}

echo "<div style=\"text-align: center;\"><h1>$sum Clients in the network</h1><br />\n";

/* 2. */

$q = mysql_query("SELECT user.ctcpversion, COUNT(*) AS version_count ".
		    "FROM user ".
		    "WHERE user.online=\"Y\" ".
			"AND user.mode_us=\"N\" ".
		    "GROUP by user.ctcpversion ".
		    "ORDER BY version_count DESC;");

$i=0;
$buf=0;
if ($q) {
 while ($r = mysql_fetch_row($q)) {
    $i++;
    if ($i == $db['maxdisplay']) {
		$version[$i] = "other";
		$count[$i] = $sum - $buf;
		break;
    }
    if ($r[0]) {
		if (strlen($r[0]) > 43) {
			$version[$i] = substr($r[0], 0, 43) . "...";
		}
		else {
			$version[$i] = $r[0]; //get first 50 chars then add '...'
		}
    } else { 
	$version[$i] = "no version reply";
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
$pie->setLegends($version);

// Display creation time of the graph
$pie->DisplayCreationTime();

// Height of the pie 3d effect
$pie->set3dHeight(15);

// set the filename 
$pie->SetFileName("version_network.png");

// generate the graph
$pie->display();

// display the graph
echo "<img src=\"version_network.png\" alt=\"\" /></div>";
?>

</body>
</html>
