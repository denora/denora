-- Denora Version 1.5.1
-- --------------------

Unreal IRCD
ALTER TABLE `user` ADD `mode_ui` enum('Y','N') NOT NULL default 'N';
ALTER TABLE `chan` ADD `mode_uz` enum('Y','N') NOT NULL default 'N';

-- Denora Version 1.5.0
-- --------------------
ALTER TABLE server ADD country VARCHAR( 255 ) NOT NULL DEFAULT '' AFTER server;
ALTER TABLE server ADD countrycode VARCHAR( 3 ) NOT NULL DEFAULT '' AFTER country;
ALTER TABLE `chan` CHANGE `channel` `channel` VARCHAR( 255 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '';
ALTER TABLE `chan` ADD `mode_ur` ENUM('Y','N') NOT NULL DEFAULT 'N' AFTER `mode_lt`;
ALTER TABLE  `cstats` CHANGE  `letters`  `letters` BIGINT( 20 ) UNSIGNED NULL DEFAULT  '0';
ALTER TABLE  `ustats` CHANGE  `letters`  `letters` BIGINT( 20 ) UNSIGNED NULL DEFAULT  '0';


All IRCds -> reload your specific sql file to fix ISON table changes

-- Denora Version 1.4.5
-- --------------------
-- No changes.


-- Denora Version 1.4.4
-- --------------------
-- No changes.


-- Denora Version 1.4.3
-- --------------------
-- No changes.


-- Denora Version 1.4.2
-- --------------------
-- No changes.


-- Denora Version 1.4.1
-- --------------------
-- No changes.


-- Denora Version 1.4.0
-- --------------------

CREATE TABLE `admin` (
  `id` tinyint(3) unsigned NOT NULL auto_increment,
  `uname` varchar(32) NOT NULL default '',
  `passwd` varchar(32) NOT NULL default '',
  `level` tinyint(1) NOT NULL default '0',
  `host` varchar(32) NOT NULL default '',
  `lang` tinyint(2) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `uname` (`uname`)
) TYPE=MyISAM

ALTER TABLE `server` ADD `lastpingtime` INT( 15 ) NOT NULL AFTER `maxpingtime` ;

ALTER TABLE `user` ADD `lastquitmsg` VARCHAR( 255 ) NOT NULL AFTER `lastquit` ;


-- Denora Version 1.1.0
-- --------------------

  -- this query is for ircds with +f flood modes
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


-- Denora Version 1.0.0
-- -------------------
-- First release.
