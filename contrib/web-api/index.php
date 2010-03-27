<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">
<html>
 <head>
   <title>Denora WEB-API Demo</title>
 </head>
 <body>

<?php

require("denora.php");

?>

<table width="100%" border=1>
 <tr>
  <td width="25%">Function</td>
  <td width="50%">What it does</td>
  <td width="25%">Example in use</td>
 </tr>
 <tr>
  <td>denora_num_users()</td>
  <td>Returns current online users</td>
  <td><?php echo(denora_num_users()); ?></td>
 </tr>
 <tr>
  <td>denora_max_num_user()</td>
  <td>Returns max online users</td>
  <td><?php echo(denora_max_num_user()); ?></td>
 </tr>
 <tr>
  <td>denora_num_opers()</td>
  <td>Returns current online opers</td>
  <td><?php echo(denora_num_opers()); ?></td>
 </tr>
 <tr>
  <td>denora_num_channels()</td>
  <td>Returns current online channels</td>
  <td><?php echo(denora_num_channels()); ?></td>
 </tr>
 <tr>
  <td>denora_max_num_channels()</td>
  <td>Returns max online channels</td>
  <td><?php echo(denora_max_num_channels()); ?></td>
 </tr>
 <tr>
  <td>denora_num_servers()</td>
  <td>Returns current online servers</td>
  <td><?php echo(denora_num_servers()); ?></td>
 </tr>
 <tr>
  <td valign=top>denora_max_stats()</td>
  <td valign=top>Returns max stats in an array</td>
  <td><?php print_r(denora_max_stats()); ?></td>
 </tr>
 <tr>
  <td valign=top>denora_num_in_chan()</td>
  <td valign=top>Returns the number of users in a channel</td>
  <td>#denora = <?php print(denora_num_in_chan("#denora")); ?></td>
 </tr>
 <tr>
  <td valign=top>denora_servername_array()</td>
  <td valign=top>Returns an array of the server names, filted to remove hidden servers set in $denora_noshow_servers, this
                 can be overriden by setting the force show</td>
  <td>Filtered : <?php print_r(denora_servername_array(1)); ?>
      <br><br>
	  Unfilted : <?php print_r(denora_servername_array(0)); ?>
 </td>
 </tr>
 <tr>
  <td valign=top>denora_servers_array()</td>
  <td valign=top>Returns an array with all the servers information</td>
  <td>Not show here due to massive info - used in denora_server_list()</td>
 </tr>
 <tr>
  <td valign=top>denora_server_array()</td>
  <td valign=top>Returns an array with all the server information for the given server</td>
  <td>Not show here due to massive info</td>
 </tr>
 <tr>
  <td valign=top>denora_is_ulined()</td>
  <td valign=top>Returns true/false if the given server is ulined</td>
  <td>services.nomadirc.net : <?php echo((denora_is_ulined("services.nomadirc.net") ? "U:lined" : "Not U:lined")); ?><br>
  	  luna.nomadirc.net : <?php echo((denora_is_ulined("luna.nomadirc.net") ? "U:lined" : "Not U:lined")); ?></td>
 </tr>

 <tr>
  <td valign=top>denora_server_motd()</td>
  <td valign=top>Returns the motd for the given server</td>
  <td><a href="#motd">Click here to see example</a></td>
 </tr>
 <tr>
  <td>denora_user_country()</td>
  <td>Returns array of the country info for the given user</td>
  <td><?php print_r(denora_user_country("trystan")); ?></td>
 </tr>
 <tr>
  <td>denora_server_current_users()</td>
  <td>Returns current users for a given server</td>
  <td><?php print_r(denora_server_current_users("luna.nomadirc.net")); ?></td>
 </tr>
 <tr>
  <td>denora_server_max_users()</td>
  <td>Returns max users for a given server</td>
  <td><?php print_r(denora_server_max_users("luna.nomadirc.net")); ?></td>
 </tr>

 <tr>
  <td>denora_server_max_users_time()</td>
  <td>Returns time of max users for a given server</td>
  <td><?php print_r(denora_server_max_users_time("luna.nomadirc.net")); ?></td>
 </tr>



 <tr>
  <td>denora_server_user_stats()</td>
  <td>Returns an array of users stats for a given server</td>
  <td><?php print_r(denora_server_user_stats("luna.nomadirc.net")); ?></td>
 </tr>
 <tr>
  <td>denora_lusers()</td>
  <td>prints /luser display</td>
  <td><a href="#lusers">Click here to see example</a></td>
 </tr>
 <tr>
  <td>denora_who_in_channel()</td>
  <td>print out the users in a channel</td>
  <td><a href="#whoin">Click here to see example</a></td>
 </tr>
 <tr>
  <td>denora_list_chans()</td>
  <td>print out the channel just like /list</td>
  <td><a href="#list">Click here to see example</a></td>
 </tr>
 <tr>
  <td>denora_num_chan_bans()</td>
  <td>Return the number of bans (+b) the channel has</td>
  <td><?php echo(denora_num_chan_bans("#denora")); ?></td>
 </tr>
 <tr>
  <td>denora_num_chan_execpt()</td>
  <td>Return the number of exceptions (+e) the channel has</td>
  <td><?php echo(denora_num_chan_except("#denora")); ?></td>
 </tr>
 <tr>
  <td>denora_get_chan_topic()</td>
  <td>Return the channel topic for the given channel</td>
  <td><?php echo(denora_get_chan_topic("#nomad")); ?></td>
 </tr>
 <tr>
  <td>denora_get_chan_topic_html()</td>
  <td>Return the channel topic for the given channel - this one is HTML friendly</td>
  <td><?php echo(denora_get_chan_topic_html("#nomad")); ?></td>
 </tr>
 <tr>
  <td>denora_num_chan_invites()</td>
  <td>Return the number of Invite exceptions (+I) the channel has</td>
  <td><?php echo(denora_num_chan_invites("#denora")); ?></td>
 </tr>
 <tr>
  <td>denora_num_away()</td>
  <td>Return the number users currently set to away</td>
  <td><?php echo(denora_num_away()); ?></td>
 </tr>
 <tr>
  <td>denora_num_sqline()</td>
  <td>Return the number sqline/resev</td>
  <td><?php echo(denora_num_sqline()); ?></td>
 </tr>
 <tr>
  <td>denora_is_sqline()</td>
  <td>Return true/false if the given nick is sqlined</td>
  <td>ChanServ : <?php echo((denora_is_sqline("Chanserv") ? "Sqlined" : "Not Sqlined")); ?><br>
  	  TemoServ : <?php echo((denora_is_sqline("temoserv") ? "Sqlined" : "Not Sqlined")); ?></td>
 </tr>
 <tr>
  <td>denora_is_online()</td>
  <td>Return true/false if the given nick is online</td>
  <td>ChanServ : <?php echo((denora_is_sqline("Chanserv") ? "Online" : "Offline")); ?><br>
  	  TemoServ : <?php echo((denora_is_sqline("temoserv") ? "Online" : "Offline")); ?></td>
 </tr>
 <tr>
  <td>denora_get_user_status()</td>
  <td>Return an array for the given nick of various status flags</td>
  <td>ChanServ : <?php print_r(denora_get_user_status("ChanServ")); ?></td>
 </tr>
 <tr>
  <td>denora_tld_current_by_code()</td>
  <td>return the current users for a given country code</td>
  <td><?php echo(denora_tld_current_by_code("US")); ?></td>
 </tr>
 <tr>
  <td>denora_tld_current_by_country</td>
  <td>return the current users for a given country name</td>
  <td><?php echo(denora_tld_current_by_country("Canada")); ?></td>
 </tr>
 <tr>
  <td>denora_tld_overall_by_code()</td>
  <td>return the overall users for a given country code</td>
  <td><?php echo(denora_tld_overall_by_code("US")); ?></td>
 </tr>
 <tr>
  <td>denora_tld_overall_by_country</td>
  <td>return the overall users for a given country name</td>
  <td><?php echo(denora_tld_overall_by_country("Canada")); ?></td>
 </tr>

 <tr>
  <td>denora_tld_array_by_code()</td>
  <td>return array of the stats for a given country code</td>
  <td><?php print_r(denora_tld_array_by_code("US")); ?></td>
 </tr>
 <tr>
  <td>denora_tld_array_by_country</td>
  <td>return array of the stats for a given country name</td>
  <td><?php print_r(denora_tld_array_by_country("Canada")); ?></td>
 </tr>
 <tr>
  <td valign=top>denora_tld_array_top10</td>
  <td valign=top>return array of the top 10 tld, it does not show tld that are currently 0</td>
  <td><?php print_r(denora_tld_array_top10()); ?></td>
 </tr>

</table><br>
<br>

<a name="lusers"></a>
<b>luser example</b><br>
<?php denora_lusers("luna.nomadirc.net"); ?>
<br>
<br>

<a name="whoin"></a>
<b>who is in a channel</b><br>
<?php denora_who_in_channel("#denora"); ?>
<br>

<a name="list"></a>
<b>Channel List</b><br>
<?php denora_list_chans(); ?>
<br>
<br>
<?php denora_server_list(); ?>
<br>

<a name="motd"></a>
<b>Server MOTD</b><br>
<?php echo(denora_server_motd("luna.nomadirc.net")); ?>

</body>
</html>