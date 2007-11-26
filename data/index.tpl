<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
!HTMLLANG!
<meta http-equiv="Content-Style-Type" content="text/css">
<title>Network Statistics by Denora</title>
<style type="text/css"> 
<!-- 
body  {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: smaller;
	background: #F0F0F0;
	margin: 0;
	padding: 0;
	text-align: center;
	color: #333333;
}
a {
	text-decoration: none;
	color: #003366;
}
a:hover {
	text-decoration: underline;
	color: #0066FF;
}
div#container { 
	width: 780px;
	background: #FFFFFF;
	margin: 0px auto 0px auto;
	border: 1px solid #999999;
	text-align: left;
} 
div#header { 
	padding: 0 10px 0 20px;
	text-align: center;
} 
div#header h1 {
	margin: 0;
	padding: 10px 0;
	color: #993366;
	font-size: x-large;
}
div#menu {
	padding: 15px 10px 15px 20px;
	background: #EBEBEB;
	text-align: center;
	border-bottom: 1px solid #CCCCCC;
}
div#menu a {
	white-space: nowrap;
}
div#content { 
	margin: 0px;
	padding: 0 20px;
	background: #FFFFFF;
}
div#content th { 
	color: #669966;
}
div#content h2 { 
	color: #993366;
	font-size: medium;
}
div#footer { 
	padding: 2px;
	background:#EBEBEB; 
	text-align: center;
	border-top: 1px solid #CCCCCC;
	margin: 20px 0px 0px 0px;
}
div#footer h6 { 
	padding: 0px;
	margin: 0px;
}
div.gotop {
	text-align: right;
	font-style: italic;
	font-weight: bold;
	border-bottom: 1px dotted #CCCCCC;
}
--> 
</style>
</head>

<body>
<div id="header">
<a name="top"></a>
  <h1>!TITLE!</h1>
</div>
<div id="container">
  <div id="menu">
    <a href="#netstats">!NETSTATSTITLE!</a> &middot; 
    <a href="#daily">!DAILYSTATSTITLE!</a> &middot; 
    <a href="#srvlist">!SERVERLISTTILE!</a> &middot;
    <a href="#tldmap">!TLDTITLE!</a> &middot; 
    <a href="#clivers">!TOP10CLIENTVERTITLE!</a> &middot; 
	<a href="#top10chan">!TOP10ONLINECHANTITLE!</a> &middot; 
    <a href="#popchan">!POPULARCHANTITLE!</a> &middot; 
    <a href="#kickchan">!MOSTKICKTITLE!</a> &middot; 
    <a href="#topicchan">!MOSTTOPICTITLE!</a> &middot; 
    <a href="#map">!NETMAPTITLE!</a> &middot; 
    <a href="#srvdet">!SERVERDETAILSTITLE!</a>
  </div>
  <div id="content">  
    <a name="netstats"></a>
    <h2>!CURNETSTATSTITLE!</h2>
    !NETSTATS!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="daily"></a>
    <h2>!DAILYSTATSTITLE!</h2>
    !DAILYSTATS!
    <div class="gotop"><a href="#top">Top</a></div>
	<a name="srvlist"></a>
    <h2>!SERVERLISTTILE!</h2>
    !SRVLIST!
    <div class="gotop"><a href="#top">Top</a></div>    
    <a name="tldmap"></a>
    <h2>!TLDTITLE!</h2>
    !TLDMAP!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="clivers"></a>
    <h2>!TOP10CLIENTVERTITLE!</h2>
    !CLIENTSTATS!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="top10chan"></a>
    <h2>!TOP10ONLINECHANTITLE!</h2>
    !DAILYTOPCHAN!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="popchan"></a>
    <h2>!TOP10CHANEVERTITLE!</h2>
    !TOP10CHAN!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="kickchan"></a>
    <h2>!TOP10KICKSTITLE!</h2>
    !TOP10KICKS!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="topicchan"></a>
    <h2>!TOP10TOPICSTITLE!</h2>
    !TOP10TOPICS!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="map"></a>
    <h2>!NETMAPTITLE!</h2>
    !MAP!
    <div class="gotop"><a href="#top">Top</a></div>
    <a name="srvdet"></a>
    <h2>!SERVERDETAILSTITLE!</h2>
    !SRVLISTDET!
    <div class="gotop"><a href="#top">Top</a></div>
  </div>
  <div id="footer">
    <h6>!CREDITS!<br>!VERSION!</h6>
  </div>
</div>
</body>
</html>