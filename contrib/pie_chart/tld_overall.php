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
$country = NULL;

echo "<div style=\"text-align: center;\"><h1>TLD overall</h1><br />\n";

/* 1. */

$q = mysql_query(sprintf("SELECT %s.count FROM %s WHERE %s.count != 0;", $db['tldtable'], $db['tldtable'], $db['tldtable']));
if ($q) {
while ($data = mysql_fetch_array($q)) {
	$sum = $sum + $data[0];
}
} else {
$sum = 0;
}

/* 2. */

$q = mysql_query(sprintf("SELECT %s.country, %s.count
		    FROM %s WHERE %s.count != 0
		    ORDER BY %s.count DESC;", $db['tldtable'], $db['tldtable'], $db['tldtable'], $db['tldtable'], $db['tldtable']));

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
  $pie->SetFileName("tld_overall.png");

  // generate the graph
  $pie->display();

  // display the graph
  echo "<img src=\"tld_overall.png\" alt=\"\" /></div>";
?>

</body>
</html>
