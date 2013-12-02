-- --------------------------------------------------------
-- 
-- IMPORTANT: Load denora.sql FIRST!
-- --------------------------------------------------------

-- --------------------------------------------------------

DROP TABLE IF EXISTS `ison`;
CREATE TABLE `ison` (
  `nickid` int(10) unsigned NOT NULL DEFAULT '0',
  `chanid` int(10) unsigned NOT NULL DEFAULT '0',
  `mode_lh` enum('Y','N') NOT NULL DEFAULT 'N',
  `mode_lo` enum('Y','N') NOT NULL DEFAULT 'N',
  `mode_lv` enum('Y','N') NOT NULL DEFAULT 'N',
  `oplevel` int(15) DEFAULT NULL,
  PRIMARY KEY (`nickid`,`chanid`)
) ENGINE=MyISAM;
    
ALTER TABLE `chan` ADD `mode_la` enum('Y','N') NOT NULL default 'N',
ADD `mode_lc` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ll` enum('Y','N') NOT NULL default 'N',
ADD `mode_lm` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lp` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lt` enum('Y','N') NOT NULL default 'N',
ADD `mode_lz` enum('Y','N') NOT NULL default 'N',
ADD `mode_ua` enum('Y','N') NOT NULL default 'N',
ADD `mode_uc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ul` enum('Y','N') NOT NULL default 'N',
ADD `mode_um` enum('Y','N') NOT NULL default 'N',
ADD `mode_un` enum('Y','N') NOT NULL default 'N',
ADD `mode_uo` enum('Y','N') NOT NULL default 'N',
ADD `mode_uq` enum('Y','N') NOT NULL default 'N',
ADD `mode_us` enum('Y','N') NOT NULL default 'N',
ADD `mode_ut` enum('Y','N') NOT NULL default 'N',
ADD `mode_uu` enum('Y','N') NOT NULL default 'N',
ADD `mode_uz` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk_data` varchar(23) NOT NULL default '',
ADD `mode_ul_data` varchar(33) NOT NULL default '',
ADD `mode_ll_data` int(10) NOT NULL default '0',
ADD `mode_ua_data` varchar(23) NOT NULL default '',
ADD `mode_uu_data` varchar(23) NOT NULL default '';

-- --------------------------------------------------------

ALTER TABLE `user` ADD `mode_la` enum('Y','N') NOT NULL default 'N',
ADD `mode_lc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ld` enum('Y','N') NOT NULL default 'N',
ADD `mode_lf` enum('Y','N') NOT NULL default 'N',
ADD `mode_lg` enum('Y','N') NOT NULL default 'N',
ADD `mode_lh` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lo` enum('Y','N') NOT NULL default 'N',
ADD `mode_lq` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lx` enum('Y','N') NOT NULL default 'N',
ADD `mode_lw` enum('Y','N') NOT NULL default 'N',
ADD `mode_lz` enum('Y','N') NOT NULL default 'N',
ADD `mode_ub` enum('Y','N') NOT NULL default 'N',
ADD `mode_uc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ud` enum('Y','N') NOT NULL default 'N',
ADD `mode_uh` enum('Y','N') NOT NULL default 'N',
ADD `mode_ui` enum('Y','N') NOT NULL default 'N',
ADD `mode_ul` enum('Y','N') NOT NULL default 'N',
ADD `mode_uo` enum('Y','N') NOT NULL default 'N',
ADD `mode_ur` enum('Y','N') NOT NULL default 'N',
ADD `mode_uw` enum('Y','N') NOT NULL default 'N',
ADD `mode_ux` enum('Y','N') NOT NULL default 'N';
    
