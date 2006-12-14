<?php

//
// mysql connection
//

if (!function_exists('dump'))
{
	function dump($var)
	{
		echo '<pre>'; var_dump($var); echo '</pre>';
	}
}
$db = parse_ini_file(dirname(__FILE__) . '/mysql.ini.php');

$lid = mysql_connect($db['server'], $db['user'], $db['pass']);
mysql_select_db($db['db'], $lid);

?>
