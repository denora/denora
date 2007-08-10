#!/usr/bin/perl

 ###########################################################################
 #   Copyright (C) 2005-2007 by Hal9000                                    #
 #   hal9000@denorastats.org                                               #
 #                                                                         #
 #   This program is free software; you can redistribute it and/or modify  #
 #   it under the terms of the GNU General Public License as published by  #
 #   the Free Software Foundation; either version 2 of the License, or     #
 #   (at your option) any later version.                                   #
 #                                                                         #
 #   This program is distributed in the hope that it will be useful,       #
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 #   GNU General Public License for more details.                          #
 #                                                                         #
 #   You should have received a copy of the GNU General Public License     #
 #   along with this program; if not, write to the                         #
 #   Free Software Foundation, Inc.,                                       #
 #   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
 ###########################################################################

# !!! IMPORTANT !!!
# PLEASE read the README file for instructions on how to operate this converter!

$sm_filename = "statsmod.dat";
$cs_filename = "ChannelStats.db";
$c_filename = "chan.db";
$dc_filename = "denora.sql";
$rc_filename = "denorarc";
$sb_filename = "backup.sql";

print "\n";
print ".========================================================.\n";
print "| Welcome to the stats.mod -> denora chanstats converter |\n";
print "|========================================================|\n";
print " \\         v1.0.0 - coded by Hal9000\@denorastats.org    \/\n";
print "  \`----------------------------------------------------\´\n";
print "     Please contact the author for feedback. Thank you!\n\n";
if (! -e $sm_filename) {
	print"ERROR: file $sm_filename not found\n";
	$serror = 1;
}
if (! -e $cs_filename) {
	print"ERROR: file $cs_filename not found\n";
	$serror = 1;
}
if (! -e $c_filename) {
	print"ERROR: file $c_filename not found\n";
	$serror = 1;
}
if (! -e $rc_filename) {
	print"ERROR: file $rc_filename not found\n";
	$serror = 1;
}
if ($serror eq 1) {
	print"Please make sure you run this converter in the denora directory\n";
	print"and that the required files are present.\n";
	print"Read the documentation in the README file!\n\n";
	exit;
}

print "Make sure you read the README file before proceeding.\n";
print "PLEASE SHUT DOWN DENORA BEFORE PROCEEDING!\n";
print "MAKE SURE YOU DISABLE ANY CRON CHECKS TO KEEP DENORA FROM STARTING UP DURING CONVERSION!\n\n";
print "The following options are available:\n\n";
print "[1] Convert and merge stats.mod database into denora with automatic MySQL import\n";
print "[2] Convert and merge stats.mod database into denora with manual MysQL import\n";
print "[0] Quit\n\n";
$cmode = &promptUser("Please type the number of the desired option:","1");

if ($_ eq "0") {
	exit;
}

print "\nStopping denora...\n";
system "$rc_filename stop";

print "Loading stats.mod database file...";
open(INPUT, "$sm_filename") || die("\nERROR: Unable to open $sm_filename\n");
	@lines=<INPUT>;
close(INPUT);
print " done.\n";

print "Loading ChannelStats.db database file...";
open(CSINPUT, "$cs_filename") || die("\nERROR: Unable to open $cs_filename\n");
	@cslines=<CSINPUT>;
close(CSINPUT);
print " done.\n";

print "Loading chan.db database file...";
open(CINPUT, "$c_filename") || die("\nERROR: Unable to open $c_filename\n");
	@clines=<CINPUT>;
close(CINPUT);
print " done.\n";

if ($cmode eq "1" || $cmode eq "2") {
	print "\n";
	$conv_t3 = &promptUser("Did you use the stats.mod database this month? (y/n)","y");
	$conv_t2 = &promptUser("Did you use the stats.mod database this week? (y/n)","y");
	$conv_t1 = &promptUser("Did you use the stats.mod database today? (y/n)","y");
	print "\n";
}

print "Parsing and converting data...";

$cchan = 0;
$ctcount = 0;
$ckcount = 0;
$cjcount = 0;

foreach $line (@lines)
{
	chop($line);
	($d1,$d2,$d3,$d4,$d5,$d6,$d7,$d8,$d9,$d10,$d11,$d12,$d13,$d14,$d15,$d16,$d17)=split(/ /,$line);

	# Get total chanstats for users
	if (substr($d1,0,1) eq "#" && $d2 ne "!" && $d2 ne "@" ) {
		$d1 =~ s/'/''/g;
		$d2 =~ s/'/''/g;
		# Insert the ustats channel record or update it if it already exists
		$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d2', '$d1', 0, $d4, $d5, $d7, $d8, $d9, $d10, $d12, $d15) ON DUPLICATE KEY UPDATE words=words+$d4, letters=letters+$d5, topics=topics+$d7, line=line+$d8, actions=actions+$d9, modes=modes+$d10, kicks=kicks+$d12, smileys=smileys+$d15;\n";
		# Insert the ustats global record or update it if it already exists
		$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d2', 'global', 0, $d4, $d5, $d7, $d8, $d9, $d10, $d12, $d15) ON DUPLICATE KEY UPDATE words=words+$d4, letters=letters+$d5, topics=topics+$d7, line=line+$d8, actions=actions+$d9, modes=modes+$d10, kicks=kicks+$d12, smileys=smileys+$d15;\n";
		# Insert the cstats record or update it if it already exists
		$dmp .= "INSERT INTO cstats (chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d1', 0, $d4, $d5, $d7, $d8, $d9, $d10, $d12, $d15) ON DUPLICATE KEY UPDATE words=words+$d4, letters=letters+$d5, topics=topics+$d7, line=line+$d8, actions=actions+$d9, modes=modes+$d10, kicks=kicks+$d12, smileys=smileys+$d15;\n";
	}

	# Get daily chanstats for users
	if ($d1 eq "@" && $d2 eq "daily") {
		$d3 =~ s/'/''/g;
		$d4 =~ s/'/''/g;
		if ($conv_t1 eq "y") {
			# Insert the ustats channel record or update it if it already exists
			$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d4', '$d3', 1, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
			# Insert the ustats global record or update it if it already exists
			$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d4', 'global', 1, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
			# Insert the cstats record or update it if it already exists
			$dmp .= "INSERT INTO cstats (chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d3', 1, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
		}
		else {
			# Ensure records exist
			$dmp .= "INSERT IGNORE INTO ustats (uname, chan, type) VALUES ('$d4', '$d3', 1);\n";
			$dmp .= "INSERT IGNORE INTO ustats (uname, chan, type) VALUES ('$d4', 'global', 1);\n";
			$dmp .= "INSERT IGNORE INTO cstats (chan, type) VALUES ('$d3', 1);\n";
		}
	}

	# Get weekly chanstats for users
	if ($d1 eq "@" && $d2 eq "weekly") {
		$d3 =~ s/'/''/g;
		$d4 =~ s/'/''/g;
		if ($conv_t2 eq "y") {
			# Insert the ustats channel record or update it if it already exists
			$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d4', '$d3', 2, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
			# Insert the ustats global record or update it if it already exists
			$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d4', 'global', 2, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
			# Insert the cstats record or update it if it already exists
			$dmp .= "INSERT INTO cstats (chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d3', 2, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
		}
		else {
			# Ensure records exist
			$dmp .= "INSERT IGNORE INTO ustats (uname, chan, type) VALUES ('$d4', '$d3', 2);\n";
			$dmp .= "INSERT IGNORE INTO ustats (uname, chan, type) VALUES ('$d4', 'global', 2);\n";
			$dmp .= "INSERT IGNORE INTO cstats (chan, type) VALUES ('$d3', 2);\n";
		}
	}

	# Get monthly chanstats for users
	if ($d1 eq "@" && $d2 eq "monthly") {
		$d3 =~ s/'/''/g;
		$d4 =~ s/'/''/g;
		if ($conv_t3 eq "y") {
			# Insert the ustats channel record or update it if it already exists
			$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d4', '$d3', 3, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
			# Insert the ustats global record or update it if it already exists
			$dmp .= "INSERT INTO ustats (uname, chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d4', 'global', 3, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
			# Insert the cstats record or update it if it already exists
			$dmp .= "INSERT INTO cstats (chan, type, words, letters, topics, line, actions, modes, kicks, smileys) VALUES ('$d3', 3, $d5, $d6, $d8, $d9, $d10, $d11, $d13, $d16) ON DUPLICATE KEY UPDATE words=words+$d5, letters=letters+$d6, topics=topics+$d8, line=line+$d9, actions=actions+$d10, modes=modes+$d11, kicks=kicks+$d13, smileys=smileys+$d16;\n";
		}
		else {
			# Ensure records exist
			$dmp .= "INSERT IGNORE INTO ustats (uname, chan, type) VALUES ('$d4', '$d3', 3);\n";
			$dmp .= "INSERT IGNORE INTO ustats (uname, chan, type) VALUES ('$d4', 'global', 3);\n";
			$dmp .= "INSERT IGNORE INTO cstats (chan, type) VALUES ('$d3', 3);\n";
		}
	}

	# Get user data
	if ($d1 eq "@" && $d2 eq "user") {
		$d3 =~ s/'/''/g;
		# Insert the user if not already existing
		$dmp .= "INSERT IGNORE INTO aliases (nick,uname) VALUES ('$d3', '$d3');\n";
	}

	# Update the ChannelStats.db file
	if (substr($d1,0,1) eq "#" && $d2 eq "@") {
		my $csmatch = 0;
		foreach $elt (@cslines) {
			if ($elt =~ /name:$d1\000/i) {
				$csmatch = 1;
				last;
			}
		}
		if ($csmatch eq "0") {
			$csflags = 3;
			push(@cslines, "name:$d1\000flags:$csflags\000\n");
		}
	}

	# Update the chan.db file
	if (substr($d1,0,1) eq "#" && $d2 eq "@") {
		$chanpeak = $d3
	}
	if (substr($d1,0,1) eq "#" && $d2 eq "!") {
		$chanptime = $d3
	}
	if (substr($d1,0,1) eq "#" && $d2 ne "!" && $d2 ne "@" ) {
		my $ccount = "-1";
		my $cmatch = 0;
		my $cnomatch = 0;
		if ($cchan eq 0) {
			$cchan = $d1;
		}
		if ($cchan eq $d1) {
			$ctcount = $ctcount + $d7;
			$ckcount = $ckcount + $d12;
			$cjcount = $cjcount + $d14;
			$cpeak = $chanpeak;
			$cptime = $chanptime;
		}
		else {
			foreach $elt (@clines) {
				$ccount++;
				if ($elt =~ /name:$cchan\000/i) {
					$cmatch = 1;
					last;
				}
			}
			if ($cmatch eq 1) {
				#if channel is present in chan.db we merge the data and write the line into the array index
				chop(@clines[$ccount]);
				if ($ccount eq 0) {
					($cc1,$cc2,$cc3,$cc4,$cc5,$cc6,$cc7,$cc8,$cc9) = split(/\000/,@clines[0]);
					($cc2a,$cc2b) = split(/:/,$cc5);
					($cc3a,$cc3b) = split(/:/,$cc6);
					($cc4a,$cc4b) = split(/:/,$cc7);
					($cc5a,$cc5b) = split(/:/,$cc8);
					($cc6a,$cc6b) = split(/:/,$cc9);
				}
				else {
					($cc1,$cc2,$cc3,$cc4,$cc5,$cc6) = split(/\000/,@clines[$ccount]);
					($cc2a,$cc2b) = split(/:/,$cc2);
					($cc3a,$cc3b) = split(/:/,$cc3);
					($cc4a,$cc4b) = split(/:/,$cc4);
					($cc5a,$cc5b) = split(/:/,$cc5);
					($cc6a,$cc6b) = split(/:/,$cc6);
				}
				$cc2t = $cc2b + $ckcount;
				$cc3t = $cc3b + $cjcount;
				$cc4t = $cc4b + $ctcount;
				if ($cpeak > $cc5b) {
					$cc5t = $cpeak;
					$cc6t = $cptime;
				}
				else {
					$cc5t = $cc5b;
					$cc6t = $cc6b;
				}
				if ($ccount eq 0) {
					@clines[0] = "$cc1\000$cc2\000$cc3\000$cc4\000kickcnt:$cc2t\000joincnt:$cc3t\000topiccnt:$cc4t\000maxusercnt:$cc5t\000maxusertime:$cc6t\000\n";
				}
				else {
					@clines[$ccount] = "$cc1\000kickcnt:$cc2t\000joincnt:$cc3t\000topiccnt:$cc4t\000maxusercnt:$cc5t\000maxusertime:$cc6t\000\n";
				}
			}
			else {
				#if channel is not present in chan.db we just append it to the array
				push(@clines, "name:$cchan\000kickcnt:$ckcount\000joincnt:$cjcount\000topiccnt:$ctcount\000maxusercnt:$cpeak\000maxusertime:$cptime\000\n");
			}
			$cchan = $d1;
			$ctcount = 0;
			$ckcount = 0;
			$cjcount = 0;
		}
	}
}

print " done.\n";

print "Creating SQL output file...";
open(OUTPUT, ">$dc_filename") or die("\nERROR: Unable to save $dc_filename\n") ;
	print OUTPUT $dmp;
close(OUTPUT);
print " done.\n\n";

if ($cmode eq "1") {
	$sql_host = &promptUser("Please specify the hostname of your mysql server","localhost");
	$sql_db = &promptUser("Please specify the denora sql database name","denora");
	$sql_user = &promptUser("Please specify the sql username for the denora database","denora");
	$sql_pass = &promptUser("Please specify the password");
	$sql_bin = &promptUser("Please specify the path to the mysql binary", "/usr/bin/mysql");
	$sql_dump = $sql_bin . "dump";
	$sql_bup = &promptUser("Do you wish to backup your current SQL database before merging? (y/n)", "n");
	if ($sql_bup eq "y") {
		print "\nBacking up SQL database, please wait...";
		system "$sql_dump -h$sql_host -u$sql_user -p$sql_pass $sql_db > $sb_filename";
		if ($? eq 0) {
			print " done.";
		}
		else {
			print "ERROR: Unable to connect to the database or to back up the data correctly.\n";
			exit;
		}
	}
	print "\nConnecting to SQL database and merging data, please wait...";
	system "$sql_bin -h$sql_host -u$sql_user -p$sql_pass -D$sql_db < $dc_filename";
	if ($? eq 0) {
		system "rm $dc_filename";
		print " done.\n";
	}
	else {
		print "ERROR: Unable to connect to the database or to merge the data correctly.\n";
		exit;
	}
}

print "Merging data into ChannelStats.db file...";
system "cp $cs_filename $cs_filename.bak";
open(CSOUTPUT, ">$cs_filename") or die("\nERROR: Unable to save $cs_filename\n") ;
	print CSOUTPUT @cslines;
close(CSOUTPUT);
print " done.\n";

print "Merging data into chan.db file...";
system "cp $c_filename $c_filename.bak";
open(COUTPUT, ">$c_filename") or die("\nERROR: Unable to save $c_filename\n") ;
	print COUTPUT @clines;
close(COUTPUT);
print " done.\n";

if ($cmode eq "2") {
	print "\nConversion finished.\nPlease load the $dc_filename file into your MySQL database BEFORE starting denora.\n";
}
else {
	print "\nConversion finished.\nThe stats.mod data has been merged in your denora installation.\n\n";
	print "Starting denora...\n";
	system "$rc_filename start";
}

print "\n";

sub promptUser {
   local($promptString,$defaultValue) = @_;
   if ($defaultValue) {
      print $promptString, " [", $defaultValue, "]: ";
   } else {
      print $promptString, ": ";
   }
   $| = 1;
   $_ = <STDIN>;
   chomp;
   if ("$defaultValue") {
      return $_ ? $_ : $defaultValue;
   } else {
      return $_;
   }
}
