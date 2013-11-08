-- --------------------------------------------------------
-- 
-- IMPORTANT: Load denora.sql FIRST!
-- --------------------------------------------------------

DROP TABLE IF EXISTS `ison`;
CREATE TABLE `ison` (
  `nickid` int(10) unsigned NOT NULL DEFAULT '0',
  `chanid` int(10) unsigned NOT NULL DEFAULT '0',
  `mode_lo` enum('Y','N') NOT NULL DEFAULT 'N',
  `mode_lv` enum('Y','N') NOT NULL DEFAULT 'N',
  `oplevel` int(15) DEFAULT NULL,
  PRIMARY KEY (`nickid`,`chanid`)
) ENGINE=MyISAM;


ALTER TABLE `chan` ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ll` enum('Y','N') NOT NULL default 'N',
ADD `mode_lm` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lp` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lt` enum('Y','N') NOT NULL default 'N',
ADD `mode_ua` enum('Y','N') NOT NULL default 'N',
ADD `mode_ur` enum('Y','N') NOT NULL default 'N',
ADD `mode_uu` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk_data` varchar(23) NOT NULL default '',
ADD `mode_ll_data` int(10) NOT NULL default '0',
ADD `mode_ua_data` varchar(23) NOT NULL default '',
ADD `mode_uu_data` varchar(23) NOT NULL default '';

-- --------------------------------------------------------

ALTER TABLE `user` ADD `mode_ld` enum('Y','N') NOT NULL default 'N',
ADD `mode_lg` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_lo` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lx` enum('Y','N') NOT NULL default 'N',
ADD `mode_lw` enum('Y','N') NOT NULL default 'N',
ADD `mode_ld` enum('Y','N') NOT NULL default 'N';
