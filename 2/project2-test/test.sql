LOAD xsmall FROM 'xsmall.del' WITH INDEX
SELECT COUNT(*) FROM xsmall /*did not work*/
SELECT * FROM xsmall WHERE key < 2500

LOAD small FROM 'small.del' WITH INDEX
SELECT COUNT(*) FROM small /*did not work*/
SELECT * FROM small WHERE key > 100 AND key < 500

LOAD medium FROM 'medium.del' WITH INDEX
SELECT COUNT(*) FROM medium /*did not work*/
SELECT * FROM medium WHERE key = 489 /*did not work - should be fixed*/ 

LOAD large FROM 'large.del' WITH INDEX
SELECT COUNT(*) FROM large
SELECT * FROM large WHERE key > 4500
SELECT * FROM large WHERE key > 4500 AND key > 0

LOAD xlarge FROM 'xlarge.del' WITH INDEX
SELECT COUNT(*) FROM xlarge
SELECT * FROM xlarge WHERE key = 4240

/* not working either*/
SELECT * FROM xlarge WHERE key > 400 AND key < 500 AND key > 100 AND key < 4000000

