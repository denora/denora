-- phpMyAdmin SQL Dump
-- version 2.6.2-pl1
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: May 14, 2005 at 06:53 PM
-- Server version: 4.0.20
-- PHP Version: 5.0.3
-- 
-- $Id: asuka.sql 712 2006-09-16 11:24:27Z trystan $
--
-- Database: `denora_asuka`
-- 

-- --------------------------------------------------------

DROP TABLE IF EXISTS `aliases`;
CREATE TABLE `aliases` (
  `nick` varchar(32) NOT NULL default '',
  `uname` varchar(32)  NOT NULL default '',
  `ignore` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`nick`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `chan`;
CREATE TABLE `chan` (
  `chanid` int(10) unsigned NOT NULL auto_increment,
  `channel` varchar(33) binary NOT NULL default '',
  `currentusers` mediumint(15) NOT NULL default '0',
  `maxusers` int(15) NOT NULL default '0',
  `maxusertime` int(15) NOT NULL default '0',
  `topic` text,
  `topicauthor` varchar(31) default NULL,
  `topictime` datetime default NULL,
  `kickcount` int(15) NOT NULL default '0',
  `mode_lc` enum('Y','N') NOT NULL default 'N',
  `mode_ld` enum('Y','N') NOT NULL default 'N',
  `mode_li` enum('Y','N') NOT NULL default 'N',
  `mode_lk` enum('Y','N') NOT NULL default 'N',
  `mode_ll` enum('Y','N') NOT NULL default 'N',
  `mode_lm` enum('Y','N') NOT NULL default 'N',
  `mode_ln` enum('Y','N') NOT NULL default 'N',
  `mode_lp` enum('Y','N') NOT NULL default 'N',
  `mode_lr` enum('Y','N') NOT NULL default 'N',
  `mode_ls` enum('Y','N') NOT NULL default 'N',
  `mode_lt` enum('Y','N') NOT NULL default 'N',
  `mode_lu` enum('Y','N') NOT NULL default 'N',
  `mode_uc` enum('Y','N') NOT NULL default 'N',
  `mode_ud` enum('Y','N') NOT NULL default 'N',
  `mode_un` enum('Y','N') NOT NULL default 'N',
  `mode_lk_data` varchar(23) NOT NULL default '',
  `mode_ll_data` int(10) NOT NULL default '0',
  PRIMARY KEY  (`chanid`),
  UNIQUE KEY `channel` (`channel`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `chanbans`;
CREATE TABLE `chanbans` (
  `id` mediumint(15) NOT NULL auto_increment,
  `chan` varchar(255) NOT NULL default '0',
  `bans` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `cstats`;
CREATE TABLE `cstats` (
  `chan` varchar(32) NOT NULL default '',
  `type` tinyint(3) unsigned NOT NULL default '0',
  `letters` int(10) unsigned default '0',
  `timeadded` int(10) unsigned default '0',
  `words` int(10) unsigned default '0',
  `line` int(10) unsigned default '0',
  `actions` int(10) unsigned default '0',
  `smileys` int(10) unsigned default '0',
  `kicks` int(10) unsigned default '0',
  `modes` int(10) unsigned default '0',
  `topics` int(10) unsigned default '0',
  `lastspoke` int(10) unsigned default '0',
  `time0` int(10) unsigned NOT NULL default '0',
  `time1` int(10) unsigned NOT NULL default '0',
  `time2` int(10) unsigned NOT NULL default '0',
  `time3` int(10) unsigned NOT NULL default '0',
  `time4` int(10) unsigned NOT NULL default '0',
  `time5` int(10) unsigned NOT NULL default '0',
  `time6` int(10) unsigned NOT NULL default '0',
  `time7` int(10) unsigned NOT NULL default '0',
  `time8` int(10) unsigned NOT NULL default '0',
  `time9` int(10) unsigned NOT NULL default '0',
  `time10` int(10) unsigned NOT NULL default '0',
  `time11` int(10) unsigned NOT NULL default '0',
  `time12` int(10) unsigned NOT NULL default '0',
  `time13` int(10) unsigned NOT NULL default '0',
  `time14` int(10) unsigned NOT NULL default '0',
  `time15` int(10) unsigned NOT NULL default '0',
  `time16` int(10) unsigned NOT NULL default '0',
  `time17` int(10) unsigned NOT NULL default '0',
  `time18` int(10) unsigned NOT NULL default '0',
  `time19` int(10) unsigned NOT NULL default '0',
  `time20` int(10) unsigned NOT NULL default '0',
  `time21` int(10) unsigned NOT NULL default '0',
  `time22` int(10) unsigned NOT NULL default '0',
  `time23` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`chan`,`type`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `ctcp`;
CREATE TABLE `ctcp` (
  `id` mediumint(15) NOT NULL auto_increment,
  `version` text NOT NULL,
  `count` mediumint(25) NOT NULL default '0',
  `overall` int(15) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `current`;
CREATE TABLE `current` (
  `type` varchar(255) NOT NULL default '',
  `val` int(15) NOT NULL default '0',
  `time` int(15) NOT NULL default '0'
) TYPE=MyISAM;

INSERT INTO `current` VALUES ('users', 0, 0);
INSERT INTO `current` VALUES ('chans', 0, 0);
INSERT INTO `current` VALUES ('daily_users', 0, 0);
INSERT INTO `current` VALUES ('servers', 0, 0);
INSERT INTO `current` VALUES ('opers', 0, 0);


-- --------------------------------------------------------

DROP TABLE IF EXISTS `glines`;
CREATE TABLE `glines` (
  `id` mediumint(15) NOT NULL auto_increment,
  `user` varchar(255) NOT NULL default '',
  `host` varchar(255) NOT NULL default '',
  `setby` varchar(255) NOT NULL default '',
  `setat` int(20) NOT NULL default '0',
  `expires` int(20) NOT NULL default '0',
  `reason` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `ison`;
CREATE TABLE `ison` (
  `nickid` int(10) unsigned NOT NULL default '0',
  `chanid` int(10) unsigned NOT NULL default '0',
  `mode_lo` enum('Y','N') NOT NULL default 'N',
  `mode_lv` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`nickid`,`chanid`),
  KEY `nickid` (`nickid`),
  KEY `chanid` (`chanid`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `maxvalues`;
CREATE TABLE `maxvalues` (
  `type` varchar(10) NOT NULL default '',
  `val` int(10) unsigned NOT NULL default '0',
  `time` datetime NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`type`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `server`;
CREATE TABLE `server` (
  `servid` int(10) unsigned NOT NULL auto_increment,
  `server` varchar(64) NOT NULL default '',
  `hops` varchar(50) NOT NULL default '0',
  `comment` varchar(255) NOT NULL default '',
  `linkedto` int(10) unsigned default NULL,
  `connecttime` datetime default NULL,
  `online` enum('Y','N') NOT NULL default 'Y',
  `lastsplit` datetime default NULL,
  `version` varchar(255) NOT NULL default '',
  `uptime` mediumint(15) NOT NULL default '0',
  `motd` text,
  `currentusers` int(15) NOT NULL default '0',
  `maxusers` int(15) NOT NULL default '0',
  `maxusertime` int(15) NOT NULL default '0',
  `ping` int(15) NOT NULL default '0',
  `highestping` int(15) NOT NULL default '0',
  `maxpingtime` int(15) NOT NULL default '0',
  `lastpingtime` int(15) NOT NULL default '0',
  `uline` int(2) NOT NULL default '0',
  `ircopskills` int(10) NOT NULL default '0',
  `serverkills` int(10) NOT NULL default '0',
  `opers` int(15) NOT NULL default '0',
  `maxopers` int(15) NOT NULL default '0',
  `maxopertime` int(15) NOT NULL default '0',
  PRIMARY KEY  (`servid`),
  UNIQUE KEY `server` (`server`),
  KEY `linkedto` (`linkedto`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `tld`;
CREATE TABLE `tld` (
  `id` mediumint(15) NOT NULL auto_increment,
  `code` varchar(10) NOT NULL default '',
  `country` varchar(255) NOT NULL default '',
  `count` mediumint(15) NOT NULL default '0',
  `overall` mediumint(15) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `nickid` int(10) unsigned NOT NULL auto_increment,
  `nick` varchar(31) NOT NULL default '',
  `hopcount` varchar(15) NOT NULL default '',
  `realname` varchar(51) NOT NULL default '',
  `hostname` varchar(64) NOT NULL default '',
  `nickip` varchar(255) NOT NULL default '',
  `username` varchar(11) NOT NULL default '',
  `connecttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `servid` int(10) unsigned NOT NULL default '0',
  `server` varchar(255) NOT NULL default '',
  `away` enum('Y','N') NOT NULL default 'N',
  `awaymsg` text,
  `ctcpversion` text,
  `online` enum('Y','N') NOT NULL default 'Y',
  `lastquit` datetime default NULL,
  `lastquitmsg` varchar(255) NOT NULL default '',
  `countrycode` varchar(255) NOT NULL default '',
  `country` varchar(255) NOT NULL default '',
  `mode_ld` enum('Y','N') NOT NULL default 'N',
  `mode_lg` enum('Y','N') NOT NULL default 'N',
  `mode_lh` enum('Y','N') NOT NULL default 'N',
  `mode_li` enum('Y','N') NOT NULL default 'N',
  `mode_lk` enum('Y','N') NOT NULL default 'N',
  `mode_ln` enum('Y','N') NOT NULL default 'N',
  `mode_lo` enum('Y','N') NOT NULL default 'N',
  `mode_lr` enum('Y','N') NOT NULL default 'N',
  `mode_lt` enum('Y','N') NOT NULL default 'N',
  `mode_lw` enum('Y','N') NOT NULL default 'N',
  `mode_lx` enum('Y','N') NOT NULL default 'N',
  `mode_ui` enum('Y','N') NOT NULL default 'N',
  `mode_uo` enum('Y','N') NOT NULL default 'N',
  `mode_ur` enum('Y','N') NOT NULL default 'N',
  `mode_ux` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`nickid`),
  UNIQUE KEY `nick` (`nick`),
  KEY `servid` (`servid`)
) TYPE=MyISAM;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `ustats`;
CREATE TABLE `ustats` (
  `uname` varchar(32) NOT NULL default '',
  `chan` varchar(32) NOT NULL default '',
  `type` tinyint(3) unsigned NOT NULL default '0',
  `letters` int(10) unsigned default '0',
  `words` int(10) unsigned default '0',
  `line` int(10) unsigned default '0',
  `actions` int(10) unsigned default '0',
  `smileys` int(10) unsigned default '0',
  `kicks` int(10) unsigned default '0',
  `kicked` int(10) unsigned default '0',
  `modes` int(10) unsigned default '0',
  `topics` int(10) unsigned default '0',
  `ljoin` int(10) unsigned default '0',
  `wasted` int(10) unsigned default '0',
  `lastspoke` int(10) unsigned default '0',
  `firstadded` int unsigned default 0,
  `time0` int(10) unsigned NOT NULL default '0',
  `time1` int(10) unsigned NOT NULL default '0',
  `time2` int(10) unsigned NOT NULL default '0',
  `time3` int(10) unsigned NOT NULL default '0',
  `time4` int(10) unsigned NOT NULL default '0',
  `time5` int(10) unsigned NOT NULL default '0',
  `time6` int(10) unsigned NOT NULL default '0',
  `time7` int(10) unsigned NOT NULL default '0',
  `time8` int(10) unsigned NOT NULL default '0',
  `time9` int(10) unsigned NOT NULL default '0',
  `time10` int(10) unsigned NOT NULL default '0',
  `time11` int(10) unsigned NOT NULL default '0',
  `time12` int(10) unsigned NOT NULL default '0',
  `time13` int(10) unsigned NOT NULL default '0',
  `time14` int(10) unsigned NOT NULL default '0',
  `time15` int(10) unsigned NOT NULL default '0',
  `time16` int(10) unsigned NOT NULL default '0',
  `time17` int(10) unsigned NOT NULL default '0',
  `time18` int(10) unsigned NOT NULL default '0',
  `time19` int(10) unsigned NOT NULL default '0',
  `time20` int(10) unsigned NOT NULL default '0',
  `time21` int(10) unsigned NOT NULL default '0',
  `time22` int(10) unsigned NOT NULL default '0',
  `time23` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`uname`,`chan`,`type`)
) TYPE=MyISAM;

-- --------------------------------------------------------

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

-- --------------------------------------------------------

DROP TABLE IF EXISTS `channelstats`;
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

-- --------------------------------------------------------

DROP TABLE IF EXISTS `serverstats`;
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

INSERT INTO `maxvalues` (`type`, `val`, `time`) VALUES ('users', 0, '0000-00-00 00:00:00');
INSERT INTO `maxvalues` (`type`, `val`, `time`) VALUES ('channels', 0, '0000-00-00 00:00:00');
INSERT INTO `maxvalues` (`type`, `val`, `time`) VALUES ('servers', 0, '0000-00-00 00:00:00');
INSERT INTO `maxvalues` (`type`, `val`, `time`) VALUES ('opers', 0, '0000-00-00 00:00:00');
