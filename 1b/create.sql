CREATE TABLE Movie (id int, 
					title varchar(100), 
					year int, 
					rating varchar(10), 
					company varchar(50), 
					PRIMARY KEY(id),		-- Movie id's must be unique
					CHECK(title IS NOT NULL))	-- Movie must have a title
					ENGINE = INNODB; 

CREATE TABLE Actor (id int, 
					last varchar(20), 
					first varchar(20), 
					sex varchar(6), 
					dob date, 
					dod date, 
					PRIMARY KEY(id),	-- Actor id's must be unique 
					CHECK(dob IS NOT NULL),	-- The actor must have a date of birth
					CHECK(sex='Male' OR sex='Female'),		-- The gender of the actor must be male or female
					CHECK(dod >= dob)) 	ENGINE = INNODB;		-- An actor cannot die before their birth

CREATE TABLE Director (id int, 
					last varchar(20), 
					first varchar(20), 
					dob date, 
					dod date, 
					PRIMARY KEY(id),	-- Director id's must be unique and must have a date of birth
					CHECK(dob IS NOT NULL),		-- The director must have a date of birth
					CHECK(dod >= dob)) ENGINE = INNODB;		-- A director cannot die before their birth

CREATE TABLE MovieGenre (mid int, genre varchar(20),
						FOREIGN KEY (mid) references Movie(id)) ENGINE = INNODB;	-- If mid is in MovieGenre, the id must be in Movie

CREATE TABLE MovieDirector (mid int, 
							did int,
							FOREIGN KEY (mid) references Movie(id), 		-- If mid is in MovieDirector, the id must be in Movie
							FOREIGN KEY (did) references Director(id)) 	ENGINE = INNODB;	-- If did is in MovieDirector, the id must be in Director

CREATE TABLE MovieActor (mid int, 
						aid int, 
						role varchar(50),
						FOREIGN KEY (mid) references Movie(id),		-- If mid is in MovieActor, the id must be in Movie							
						FOREIGN KEY (aid) references Actor(id)) ENGINE = INNODB;	-- If aid is in MovieActor, the id must be in Actor

CREATE TABLE Review (name varchar(20), 
					time timestamp, 
					mid int, 
					rating int, 
					comment varchar(500),
					FOREIGN KEY (mid) references Movie(id)) ENGINE = INNODB;	-- The mid of the movie referenced in Review, the id must be in Movie

CREATE TABLE MaxPersonID (id int) ENGINE = INNODB;

CREATE TABLE MaxMovieID (id int) ENGINE = INNODB;
