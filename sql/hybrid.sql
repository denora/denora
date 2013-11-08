-- --------------------------------------------------------
-- 
-- IMPORTANT: Load denora.sql FIRST!
-- --------------------------------------------------------

DROP TABLE IF EXISTS `ison`;
CREATE TABLE `ison` (
  `nickid` int(10) unsigned NOT NULL default '0',
  `chanid` int(10) unsigned NOT NULL default '0',
  `mode_lh` enum('Y','N') NOT NULL default 'N',
  `mode_lo` enum('Y','N') NOT NULL default 'N',
  `mode_lv` enum('Y','N') NOT NULL default 'N',
  PRIMARY KEY  (`nickid`,`chanid`)
) ENGINE=MyISAM;


ALTER TABLE `chan` ADD `mode_la` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ll` enum('Y','N') NOT NULL default 'N',
ADD `mode_lm` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lp` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lt` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk_data` varchar(23) NOT NULL default '',
ADD `mode_ll_data` int(10) NOT NULL default '0';

-- --------------------------------------------------------

ALTER TABLE `user` ADD `mode_la` enum('Y','N') NOT NULL default 'N',
ADD `mode_lb` enum('Y','N') NOT NULL default 'N',
ADD `mode_lc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ld` enum('Y','N') NOT NULL default 'N',
ADD `mode_lf` enum('Y','N') NOT NULL default 'N',
ADD `mode_lg` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ll` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lo` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lu` enum('Y','N') NOT NULL default 'N',
ADD `mode_lw` enum('Y','N') NOT NULL default 'N',
ADD `mode_lx` enum('Y','N') NOT NULL default 'N',
ADD `mode_ly` enum('Y','N') NOT NULL default 'N',
ADD `mode_lz` enum('Y','N') NOT NULL default 'N',
ADD `mode_ud` enum('Y','N') NOT NULL default 'N',
ADD `mode_ug` enum('Y','N') NOT NULL default 'N';
