DROP TABLE IF EXISTS `tb_account`;
CREATE TABLE `tb_account` (
  `account_id` int(11) UNSIGNED NOT NULL auto_increment,
  `role_id` int(11) NOT NULL DEFAULT  DEFAULT '0',
  `user_name` char(32) NOT NULL DEFAULT '',
  `password` char(32) NOT NULL DEFAULT '', 
  `create_time` int(11) default '0',
  PRIMARY KEY  (`account_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `tb_role`;
CREATE TABLE `tb_role` (
  `role_id` char(32) NOT NULL DEFAULT 0,
  `role_money` char(32) NOT NULL DEFAULT 0,
  PRIMARY KEY  (`role_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;