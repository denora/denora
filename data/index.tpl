<html>
<head>
!HTMLLANG!
<meta http-equiv="Content-Style-Type" content="text/css" />

<title>Network Statistics by Denora</title>
<style type="text/css">
<!--

BODY {background-color:#FFFFFF}
P	{font-family:Verdana, serif;font-size:10pt}

TH	{background-color:#66555F;color:#ECECEC;font-family:Arial, Helvetica, sans-serif;font-size:8pt;font-weight:bold}
TH.secondary	{background-color:#457295;color:#ECECEC;font-family:Verdana, serif;font-size:10pt;font-weight:normal;text-align:left}
TD.tablebg	{background-color:#000000}
TD.cat	{background-color:#2d51a3;font-family:Verdana, serif;font-size:12pt}
TD.row1	{background-color:#91bbdb;color:#ECECEC;}
TD.row2	{background-color:#cbddeb}

SPAN.title	{font-family:Arial, Helvetica, sans-serif;font-size:26pt}
SPAN.cattitle	{font-family:Verdana, serif;font-size:12pt;font-weight:bold; color:#ECECEC;}
SPAN.gen	{font-family:Verdana, serif;font-size:10pt}
SPAN.gensmall	{font-family:Verdana, serif;font-size:8pt}
SPAN.courier	{font-family:courier;font-size:10pt}

A {text-decoration:none}
A:hover {color:#EDF2F2;text-decoration:underline}

HR {height:2px}

//-->
</style>
</head>
<body bgcolor="#444444" link="#EDF2F2" vlink="#DDEDED">

<a name="top"></a>

<table width="98%" cellpadding="0" cellspacing="0" border="0" align="center">
	<tr>
		<td class="tablebg" width="100%"><table width="100%" cellspacing="1" cellpadding="4" border="0">
			<tr>
				<td class="row2"><table width="100%" cellspacing="0" border="0">
					<tr>
						<td>&nbsp;</td>
						<td align="right" valign="top" bordercolorlight="#000000" bordercolor="#000000"><span class=title>!TITLE!</span></td>
					</tr>
				</table></td>
			</tr>
			<tr>
				<td class="row1"><table width="100%" cellspacing="0" cellpadding="4" border="0">
					<tr>
	 		<td width="90%"><span class="gensmall"><center><a href="#map">!NETMAPTITLE!</a>
                          <font color="#444444">
                          |</font> <a href="#netstats">!NETSTATSTITLE!</a> <font color="#444444">|</font>
                          <a href="#daily">!DAILYSTATSTITLE!</a> <font color="#444444">|</font>
                          <a href="#top10chan">!TOP10ONLINECHANTITLE!</a> <font color="#444444"> |</font>
                          <a href="#srvlist">!SERVERLISTTILE!</a>
                          <font color="#444444">|</font> <a href="#popchan">!POPULARCHANTITLE!</a>
                          <font color="#444444"> |</font> <a href="#kickchan">!MOSTKICKTITLE!</a>
                          <font color="#444444"> |</font> <a href="#topicchan">!MOSTTOPICTITLE!</a>
			<font color="#444444"> |</font> <a href="#srvdet">!SERVERDETAILSTITLE!</a>  |</font> <a href="#clivers">!TOP10CLIENTVERTITLE!</a>
			  </center></span></td>
					</tr>
				</table></td>
			</tr>
		</table></td>
	</tr>
</table></form>


<table width="98%" cellspacing="0" cellpadding="4" border="0" align="center">
	<tr>
		<td align="left" valign="bottom">
          <p align="right"><span class="gensmall">!VERSION!</span></p>
        </td>
	</tr>
</table>

<table width="98%" cellpadding="0" cellspacing="0" border="0" align="center">
	<tr>
		<td class="tablebg"><table width="100%" cellpadding="0" cellspacing="0" border="1">
			<tr>
	 		<td class="cat" height="30"><span class="cattitle">!NETMAPTITLE!</span></td>
			<tr>
				<td class="row1" align="center">
				<a name=map>!MAP!
			</td>
			</tr>

			<tr>
	 	 	<td class="cat" height="30"><span class="cattitle">!TLDTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=tldmap>!TLDMAP!
			</td>
			</tr>
	
			<tr>
	 	 	<td class="cat" height="30"><span class="cattitle">!CURNETSTATSTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=netstats>!NETSTATS!
			</td>
			</tr>
	
			<tr>
	 		 <td class="cat" height="30"><span class="cattitle">!DAILYSTATSTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=daily>!DAILYSTATS!
			</td>
			</tr>
			<tr>
	 	 	<td class="cat" height="30"><span class="cattitle">!TOP10ONLINECHANTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=top10chan>!DAILYTOPCHAN!
			</td>
			</tr>
	 	 	<td class="cat" height="30"><span class="cattitle">!TOP10CLIENTVERTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=clivers>!CLIENTSTATS!
			</td>
			</tr>
	 		<td class="cat" height="30"><span class="cattitle">!SERVERLISTTILE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=srvlist>!SRVLIST!
			</td>
			</tr>
	
			<tr>
	 		<td class="cat" height="30"><span class="cattitle">!TOP10CHANEVERTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=popchan>!TOP10CHAN!
			</td>
			</tr>
			<tr>
	 	 	<td class="cat" height="30"><span class="cattitle">!TOP10KICKSTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=kickchan>!TOP10KICKS!
			</td>
			</tr>
			<tr>
	 	 	<td class="cat" height="30"><span class="cattitle">!TOP10TOPICSTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=topicchan>!TOP10TOPICS!
			</td>
			</tr>
			<tr>
	 	 	<td class="cat" height="30"><span class="cattitle">!SERVERDETAILSTITLE!</span></td>
			</tr>
			<tr>
				<td class="row1" align="center">
				<a name=srvdet>!SRVLISTDET!
			</td>
			</tr>
		</table></td>
	</tr>
</table>

<br clear="all" />
!CREDITS!

</body>
</html>
