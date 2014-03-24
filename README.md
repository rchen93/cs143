CS143: Database Systems
=====

This is a repository for the projects assigned in Professor Cho's Winter 2014 CS143 class.
The README.txt contained in each project was for submission purposes.
The following projects will have their minimum requirements listed, and all links used
are relative URLs.

### Project 1A: PHP Web Calculator 
----------------------------------------

We built a small Web calculator application in PHP to become familiarized with HTML and PHP.
We used HTTP GET requests to get the user input.
Requirements: 
<ul>
<li> Supports +,-,* and / operators </li>
<li> Supports integer (1234) and real (12.34) numbers </li>
<li> Does not support parantheses </li>
<li> Handle errors gracefully, i.e., do not display raw PHP error messages </li>
</ul>

### Project 1B: Web Query Interface 
----------------------------------------

We built a web query interface that allowed users to enter MySQL queries for our MySQL database.
We were allowed to assume that users would not attempt SQL injection attacks; thus, we did not use
the mysql_real_escape_string() function when processing the MySQL queries. We used HTTP GET requests to
get the user input.
<br/>
This was also our first usage of MySQL, as we created nine tables and loaded the given datasets
into them. 
<br/>
Requirements: 
<ul>
<li> Three primary key constraints </li>
<li> Six referential integrity constraints </li>
<li> Three CHECK constraints </li>
</ul>

### Project 1C: Hobo IMDB 
----------------------------------------

We built a basic movie database site that used the tables and data created in Part B.
The project was very open-ended in terms of how we wanted to structure the required 
functionalities. We used basic HTML/CSS techniques to create a minimalistic website, 
screenshots are provided.
<br/>
Requirements: 
<ul>
<li> A page that lets users to add actor and/or director information </li>
<li> A page that lets users to add movie information </li>
<li> A page that lets users to add comments to movies </li>
<li> A page that lets users to add "actor to movie" relation(s) </li>
<li> A page that lets users to add "director to movie" relation(s) </li>
<li> A page that shows actor information </li>
	<ul>
		<li> Show links to the movies that the actor was in </li>
	</ul>
<li> A page that shows movie information </li>
	<ul>
		<li> Show links to the actors/actresses that were in this movie </li>
		<li> Show the average score of the movie based on user feedbacks </li>
		<li> Show all user comments </li>
		<li> Contain "Add Comment" button </li>
	</ul>
<li> A page that lets users to add "actor to movie" relation(s). </li>
<li> A search page that supports multi-words searches </li>
</ul>


### Project 2: Bruinbase 
----------------------------------------

We implemented a B+ tree index for a very simplified relational database
and only needed to support insertion of keys. 
We also implemented LOAD and SELECT commands for our database. 

### Authors 
----------------------------------------

Kailin Chang <br/>
Roger Chen