-- List the names of all the actors in the movie 'Die Another Day'
SELECT CONCAT(first, SPACE(1), last)
FROM Actor A, MovieActor MA, Movie M 
WHERE M.title = 'Die Another Day' AND MA.mid = M.id AND A.id = MA.aid;  

-- How many actors are in multiple movies?
SELECT COUNT(MA.aid)
FROM (SELECT aid
From MovieActor
Group By aid
HAVING COUNT(*) > 1) MA; 

-- How many movies have multiple genres?
SELECT COUNT(MG.mid)
FROM (SELECT mid
FROM MovieGenre
GROUP By mid
HAVING COUNT(*) > 1) MG; 
