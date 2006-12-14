#! /usr/local/bin/php

<?php

mysql_connect('localhost', 'denora', 'passwordhere');
mysql_select_db('denora');
$result = mysql_query('SHOW TABLES');
$row = mysql_fetch_row($result);
$tables = $row[0];
while ($row = mysql_fetch_row($result)) $tables .= ', ' . $row[0];
mysql_free_result($result);
$result = mysql_query('OPTIMIZE TABLE ' . $tables);
$out = '';
while ($row = mysql_fetch_row($result))
{
  foreach ($row as $value) $out .= $value . "\t";
  $out .= "\n";
}
mail('denora@example.com', 'Daily Cleanup', $out);

?> 