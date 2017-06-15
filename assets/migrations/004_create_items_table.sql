CREATE TABLE items (
	id INTEGER PRIMARY KEY,
	channel_id INTEGER DEFAULT 0,
	title TEXT DEFAULT NULL,
	link TEXT DEFAULT NULL,
	description TEXT DEFAULT NULL,
	author TEXT DEFAULT NULL,
	category TEXT DEFAULT NULL,
	comments TEXT DEFAULT NULL,
	guid TEXT DEFAULT NULL,
	pub_date TEXT DEFAULT NULL,
	source TEXT DEFAULT NULL,
	read INTEGER DEFAULT 0,
	status INTEGER DEFAULT 0,
	FOREIGN KEY (channel_id) REFERENCES channels(id) ON DELETE CASCADE ON UPDATE NO ACTION
);