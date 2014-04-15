
/*

DROP TABLE IF EXISTS `mission_board`;
CREATE TABLE IF NOT EXISTS `mission_board` (
	`id` int(11) unsigned NOT NULL,
	`title` varchar(30) NOT NULL default '',
	`desc` varchar(255) NOT NULL default '',
	`mob_list` varchar(50) NOT NULL default '',
	`mob_qty` varchar(50) NOT NULL default '',
	`item_list` varchar(50) NOT NULL default '',
	`item_qty` varchar(50) NOT NULL default '',
	`class_limitation` int(11) unsigned NOT NULL default '0',
	`class_branch` int(11) unsigned NOT NULL default '0',
	`min_lv` smallint(6) unsigned NOT NULL default '1',
	`max_lv` smallint(6) unsigned NOT NULL default '99',
	`repeat` smallint(6) unsigned NOT NULL default '0',
	`duration` int(11) unsigned NOT NULL default '0',
	`reward_list` varchar(50) NOT NULL default '',
	`reward_qty` varchar(50) NOT NULL default '',
	`base_exp` int(11) unsigned NOT NULL default '0',
	`job_exp` int(11) unsigned NOT NULL default '0',
	`zeny` int(11) unsigned NOT NULL default '0',
	`cash` int(11) unsigned NOT NULL default '0',
	`aid` int(11) unsigned NOT NULL default '0',
	`name` varchar(30) NOT NULL default '',
	`time_update` datetime NOT NULL default '0000-00-00 00:00:00',
	`npc_id` varchar(255) NOT NULL default '',
	`redo_delay` smallint(6) unsigned NOT NULL default '0',
	PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


##DROP TABLE IF EXISTS `player_mission`;
CREATE TABLE IF NOT EXISTS `player_mission` (
	`id` int(11) unsigned NOT NULL,
	`mission_id` int(11) unsigned NOT NULL,
	`aid` int(11) unsigned NOT NULL default '0',
	`cid` int(11) unsigned NOT NULL default '0',
	`name` varchar(30) NOT NULL default '',
	`mob_hunt` varchar(50) NOT NULL default '',
	`expire` int(11) unsigned NOT NULL default '0',
	`starting` datetime NOT NULL default '0000-00-00 00:00:00',
	`completion` datetime NOT NULL default '0000-00-00 00:00:00',
	PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

*/