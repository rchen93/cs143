<!DOCTYPE html>
<html>
<body>

<link rel="stylesheet" href="add-style.css">

Add new movie: <br/><br/>
		<form action="addMovieInfo.php" method="POST">			
			Title : <input type="text" name="title" maxlength="20"><br/><br/>
			Company: <input type="text" name="company" maxlength="50"><br/><br/>
			Year : <input type="text" name="year" maxlength="4"><br/><br/>	<!-- Todo: validation-->	
			MPAA Rating : <select name="mpaarating">
					<option value="G">G</option>
					<option value="PG">PG</option>
					<option value="PG-13">PG-13</option>
					<option value="R">R</option>
					<option value="NC-17">NC-17</option>
					</select>
			<br/><br/>
			Genre : 
					<br/>
					<input type="checkbox" name="genre_Action" value="Action">Action</input><br/>
					<input type="checkbox" name="genre_Adult" value="Adult">Adult</input><br/>
					<input type="checkbox" name="genre_Adventure" value="Adventure">Adventure</input><br/>
					<input type="checkbox" name="genre_Animation" value="Animation">Animation</input><br/>
					<input type="checkbox" name="genre_Comedy" value="Comedy">Comedy</input><br/>
					<input type="checkbox" name="genre_Crime" value="Crime">Crime</input><br/>
					<input type="checkbox" name="genre_Documentary" value="Documentary">Documentary</input><br/>
					<input type="checkbox" name="genre_Drama" value="Drama">Drama</input><br/>
					<input type="checkbox" name="genre_Family" value="Family">Family</input><br/>
					<input type="checkbox" name="genre_Fantasy" value="Fantasy">Fantasy</input><br/>
					<input type="checkbox" name="genre_Horror" value="Horror">Horror</input><br/>
					<input type="checkbox" name="genre_Musical" value="Musical">Musical</input><br/>
					<input type="checkbox" name="genre_Mystery" value="Mystery">Mystery</input><br/>
					<input type="checkbox" name="genre_Romance" value="Romance">Romance</input><br/>
					<input type="checkbox" name="genre_Sci-Fi" value="Sci-Fi">Sci-Fi</input><br/>
					<input type="checkbox" name="genre_Short" value="Short">Short</input><br/>
					<input type="checkbox" name="genre_Thriller" value="Thriller">Thriller</input><br/>
					<input type="checkbox" name="genre_War" value="War">War</input><br/>
					<input type="checkbox" name="genre_Western" value="Western">Western</input><br/>
					
			<br/>
			
			<input type="submit" name="submit" value="Submit"/>
					</form>
		<hr/>

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

$error = false;

/* Check required fields */
if (isset($_POST['title']))
{
	if (empty($_POST['title']))
		$error = true;
}
else
	$error = true;

/* Check selected genres */
$genres = array();
$allGenres = array("genre_Action", "genre_Adult", "genre_Adventure", 
	"genre_Animation", "genre_Comedy", "genre_Crime", "genre_Documentary", 
	"genre_Drama", "genre_Family", "genre_Fantasy", "genre_Horror", 
	"genre_Musical", "genre_Mystery", "genre_Romance", "genre_Sci-Fi",
	"genre_Short", "genre_Thriller", "genre_War", "genre_Western");

foreach ($allGenres as $key)
{
	if(isset($_POST[$key]))
	{
		if(!empty($_POST[$key]))
			$genres[] = $_POST[$key];
	}
}

/* MySQL insertion */

if (isset($_POST["submit"]))
{
	if ($error)
		echo "Title is missing!";
	else
	{
		$lookup_query = "SELECT id FROM MaxMovieID";
		$lookup_result = mysql_fetch_row(mysql_query($lookup_query, $db_connection));
		$id = $lookup_result[0];

		$insertmovie_query = "INSERT INTO Movie (id, title, year, rating, company)
		VALUES ($id, '$_POST[title]', '$_POST[year]', '$_POST[mpaarating]', '$_POST[company]')";
		$newmovie = mysql_query($insertmovie_query, $db_connection);

		for ($i = 0; $i < count($genres); $i++)
		{
			$insertgenre_query = "INSERT INTO MovieGenre (mid, genre) VALUES ($id, '$genres[$i]')";
			$mapgenre = mysql_query($insertgenre_query, $db_connection);
			if ($mapgenre)
			{
				echo "'$genres[$i]'' mapped to $id </br>";			/* Remove later */
			}
			else 
				echo "Failed!";
		}

		if (!$newmovie) 
		{
    		$message  = 'Invalid query: ' . mysql_error() . "\n";
    		$message .= 'Whole query: ' . $insertmovie_query;
    		die($message);
		}
		else
		{
			echo "$_POST[title] added successfully!<br/>";
			$update_query = "UPDATE MaxMovieID SET id=id+1";
			if (mysql_query($update_query, $db_connection))
			{
				echo "MaxMovieID updated!";			/* Remove later */
			}
			else
				echo "Failed";						 	/* Remove later */
		}


	}

}

mysql_close($db_connection);



?>
			
<a href='home.php'>Go Home</a></br>
</html>
</body>