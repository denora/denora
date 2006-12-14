Denora Version svn
-------------------

ALTER TABLE `server` ADD `lastpingtime` INT( 15 ) NOT NULL AFTER `maxpingtime` ;

ALTER TABLE `user` ADD `lastquitmsg` VARCHAR( 255 ) NOT NULL AFTER `lastquit` ;

Denora Version 1.1.0
-------------------

  /* this query is for ircds with +f flood modes */
  ALTER TABLE `chan` CHANGE `mode_lf_data` `mode_lf_data` VARCHAR( 255 ) NOT NULL 

  ALTER TABLE `server` ADD `opers` INT( 15 ) NOT NULL;
  ALTER TABLE `server` ADD `maxopers` INT( 15 ) NOT NULL;
  ALTER TABLE `server` ADD `maxopertime` INT( 15 ) NOT NULL;

  ALTER TABLE chan ADD maxusers INT( 15 ) NOT NULL, ADD maxusertime INT( 15 ) NOT NULL, 
				   ADD kickcount INT( 15 ) NOT NULL ;

  ALTER TABLE server ADD `ping` INT( 15 ) NOT NULL , ADD `highestping` INT( 15 ) NOT NULL , 
                     ADD `maxpingtime` INT( 15 ) NOT NULL ;

  ALTER TABLE `server` ADD `uline` INT( 2 ) NOT NULL ;
  ALTER TABLE `server` ADD `ircopskills` INT( 10 ) NOT NULL;
  ALTER TABLE `server` ADD `serverkills` INT( 10 ) NOT NULL;

DROP TABLE IF EXISTS `stats`;
CREATE TABLE `stats` (
  `id` mediumint(15) NOT NULL auto_increment,
  `day` int(15) NOT NULL default '0',
  `month` int(15) NOT NULL default '0',
  `year` int(15) NOT NULL default '0',
  `time_0` int(15) NOT NULL default '0',
  `time_1` int(15) NOT NULL default '0',
  `time_2` int(15) NOT NULL default '0',
  `time_3` int(15) NOT NULL default '0',
  `time_4` int(15) NOT NULL default '0',
  `time_5` int(15) NOT NULL default '0',
  `time_6` int(15) NOT NULL default '0',
  `time_7` int(15) NOT NULL default '0',
  `time_8` int(15) NOT NULL default '0',
  `time_9` int(15) NOT NULL default '0',
  `time_10` int(15) NOT NULL default '0',
  `time_11` int(15) NOT NULL default '0',
  `time_12` int(15) NOT NULL default '0',
  `time_13` int(15) NOT NULL default '0',
  `time_14` int(15) NOT NULL default '0',
  `time_15` int(15) NOT NULL default '0',
  `time_16` int(15) NOT NULL default '0',
  `time_17` int(15) NOT NULL default '0',
  `time_18` int(15) NOT NULL default '0',
  `time_19` int(15) NOT NULL default '0',
  `time_20` int(15) NOT NULL default '0',
  `time_21` int(15) NOT NULL default '0',
  `time_22` int(15) NOT NULL default '0',
  `time_23` int(15) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;


CREATE TABLE `channelstats` (
  `id` mediumint(15) NOT NULL auto_increment,
  `day` int(15) NOT NULL default '0',
  `month` int(15) NOT NULL default '0',
  `year` int(15) NOT NULL default '0',
  `time_0` int(15) NOT NULL default '0',
  `time_1` int(15) NOT NULL default '0',
  `time_2` int(15) NOT NULL default '0',
  `time_3` int(15) NOT NULL default '0',
  `time_4` int(15) NOT NULL default '0',
  `time_5` int(15) NOT NULL default '0',
  `time_6` int(15) NOT NULL default '0',
  `time_7` int(15) NOT NULL default '0',
  `time_8` int(15) NOT NULL default '0',
  `time_9` int(15) NOT NULL default '0',
  `time_10` int(15) NOT NULL default '0',
  `time_11` int(15) NOT NULL default '0',
  `time_12` int(15) NOT NULL default '0',
  `time_13` int(15) NOT NULL default '0',
  `time_14` int(15) NOT NULL default '0',
  `time_15` int(15) NOT NULL default '0',
  `time_16` int(15) NOT NULL default '0',
  `time_17` int(15) NOT NULL default '0',
  `time_18` int(15) NOT NULL default '0',
  `time_19` int(15) NOT NULL default '0',
  `time_20` int(15) NOT NULL default '0',
  `time_21` int(15) NOT NULL default '0',
  `time_22` int(15) NOT NULL default '0',
  `time_23` int(15) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

CREATE TABLE `serverstats` (
  `id` mediumint(15) NOT NULL auto_increment,
  `day` int(15) NOT NULL default '0',
  `month` int(15) NOT NULL default '0',
  `year` int(15) NOT NULL default '0',
  `time_0` int(15) NOT NULL default '0',
  `time_1` int(15) NOT NULL default '0',
  `time_2` int(15) NOT NULL default '0',
  `time_3` int(15) NOT NULL default '0',
  `time_4` int(15) NOT NULL default '0',
  `time_5` int(15) NOT NULL default '0',
  `time_6` int(15) NOT NULL default '0',
  `time_7` int(15) NOT NULL default '0',
  `time_8` int(15) NOT NULL default '0',
  `time_9` int(15) NOT NULL default '0',
  `time_10` int(15) NOT NULL default '0',
  `time_11` int(15) NOT NULL default '0',
  `time_12` int(15) NOT NULL default '0',
  `time_13` int(15) NOT NULL default '0',
  `time_14` int(15) NOT NULL default '0',
  `time_15` int(15) NOT NULL default '0',
  `time_16` int(15) NOT NULL default '0',
  `time_17` int(15) NOT NULL default '0',
  `time_18` int(15) NOT NULL default '0',
  `time_19` int(15) NOT NULL default '0',
  `time_20` int(15) NOT NULL default '0',
  `time_21` int(15) NOT NULL default '0',
  `time_22` int(15) NOT NULL default '0',
  `time_23` int(15) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;



Denora Version 1.0.0
-------------------

