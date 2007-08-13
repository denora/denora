<a href=?method=denora.getstat&type=users>Users Count</a><br>
<a href=?method=denora.getstat&type=chans>Channel Count</a><br>
<a href=?method=denora.channeldata&type=nomad>Channel Data</a><br>
<a href=?method=denora.channellist&type=chans>Channel List</a><br>
<br>

<?php

$method = (isset($_GET['method']) ? $_GET['method'] : "denora.getstat");
$type = (isset($_GET['type']) ? $_GET['type'] : "users");

if ($method == "denora.channeldata") {
	$string = xmlrpc_encode_request($method,array(sprintf("#%s", $type), "topic"));
} else {
	$string = xmlrpc_encode_request($method,$type);
}

printf("XMLRPC call");
echo("<pre>");
print(htmlspecialchars($string));
echo("</pre>");

$data = NULL;

$fp = @fsockopen ("66.45.245.58", 9090, $errno, $errstr, 30);
if (!$fp) { 
   echo "Error in Connecting : $errstr ($errno)<br>\n"; 
} else { 
   fputs ($fp, $string); 
   while (!feof($fp)) { 
       $data .= fgets($fp,128); 
   } 
   fclose ($fp); 
}

printf("XMLRPC client data");
echo("<pre>");
printf("%s", htmlspecialchars($data));
echo("</pre>");

$array = xmlrpc_decode($data);

print_r($array);


?>
