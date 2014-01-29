/*
This violates the primary key constraint that Movie id's must be unique
by trying to add a movie with an existing id
*/
INSERT INTO Movie 
VALUES (272, 'Frozen', 2013, 'PG-13', 'Disney');
/*
ERROR 1062 (23000): Duplicate entry '272' for key 1
*/

/*
This violates the primary key constraint that Actor id's must be unique
by trying to add an actor with an existing id
*/
INSERT INTO Actor
VALUES (1, 'Tolkien', 'J.R.R.', 'Male', '18920103', '19730902');
/*
ERROR 1062 (23000): Duplicate entry '1' for key 1
*/

/*
This violates the primary key contsraint that Director id's must be unique
by trying to add a director with an existing id
*/
INSERT INTO Director
VALUES (37146, 'Edwards', 'Blake', '19220706', '20101215');
/*
ERROR 1062 (23000): Duplicate entry '37146' for key 1
*/

/*
This violates the referential constraint that mid in MovieGenre must be 
contained in Movie by trying to add a movie with an id that doesn't exist
in Movie table
*/
INSERT INTO MovieGenre
VALUES(4761, "Comedy");
/*
ERROR1452 (23000): Cannot add or update a child row: a foreign key constraint
fails ('CS143/MovieGenre', CONSTRAINT 'MovieGenre_ibfk_1' 
FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))
*/

/*
This violates the referential constraint that mid in MovieDirector must be
contained in Movie by trying to add a movie with an id that doesn't exist in 
Movie Table
*/
INSERT INTO MovieDirector
VALUES(4761, 54809);
/*
ERROR1452 (23000): Cannot add or update a child row: a foreign key constraint
fails ('CS143/MovieDirector', CONSTRAINT 'MovieDirector_ibfk_1' 
FOREIGN KEY ('mid') REFERENCES 'Movie' ('id'))
*/

/*
This violates the referential constraint that did in MovieDirector must be
contained in Director by trying to add a movie with a director's id that doesn't exist in 
Director Table
*/
INSERT INTO MovieDirector
VALUES(2965, 540000);
/*
ERROR1452 (23000): Cannot add or update a child row: a foreign key constraint
fails ('CS143/MovieDirector', CONSTRAINT 'MovieDirector_ibfk_1' 
FOREIGN KEY ('mid') REFERENCES 'Director' ('id'))
*/

/*
This violates the referential constraint that mid in MovieActor must be
contained in Movie by trying to add a movie with an id that doesn't exist in 
Movie Table
*/
INSERT INTO MovieActor
VALUES(4761, 54809);

/*
This violates the referential constraint that aid in MovieActor must be
contained in Actor by trying to add a movie with an actor's id that doesn't exist in 
Actor Table
*/
INSERT INTO MovieActor
VALUES(2965, 540000);

/*
This violates the referential constraint that mid in Review must be
contained in Movie by trying to add an review with a movie id that doesn't exist in 
Movie Table
*/
INSERT INTO Review
VALUES(4761, 54809);


/*
This violates the CHECK constraint that movies must have a title
by updating the title to an existing movie to NULL
*/
UPDATE Movie
SET title=NULL
WHERE id=272

/*
This violates the CHECK constraint that actors must either be
Male or Female by updating sex to an existing actor to Alien
*/
UPDATE Actor
SET sex='Alien'
WHERE id=1

/*
This violates the CHECK constraint that actors cannot die before their
birth by updating an existing actor's date of death to before their
date of birth
*/
UPDATE Actor
SET dod=19740213
WHERE id=1