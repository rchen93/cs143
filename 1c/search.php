<!DOCTYPE html>
<html>
<body>


<h2> Search for Actors/Movies </h2>

<form action="search.php" method="GET">		
		Search: <input type="text" name="keyword"></input>
		<input type="submit" name = "submit" value="Search"/>
</form>

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

if (isset($_GET["submit"]))				
{
	$keyword = $_GET['keyword'];
	if (empty($keyword))
	{
		echo "Please enter a search term. <br/>";
	}
	else
	{
		echo "<h4> Results for \"$keyword\" </h4>";

		$words = preg_split('/\s+/', trim($keyword));	// Splits words by spaces
		$length = count($words);

		/* Actor Query */
		echo "<b> Searching records in Actor database... </b> <br/>";
		if ($length == 1)
		{
			$actor_query = "SELECT id, first, last FROM Actor WHERE first LIKE 
							'%$words[0]%' OR last LIKE '%$words[0]%'";
			$actor_result = mysql_query($actor_query, $db_connection);


			while ($actor_row = mysql_fetch_row($actor_result))
			{
				$aid = $actor_row[0];
				$name = "$actor_row[1] $actor_row[2]";
				echo "<a href='http://192.168.56.20/~cs143/showActorInfo.php?aid=$aid'> $name </a> <br/>";	
			}
		}
		else if ($length == 2)
		{
			$actor_query = "SELECT id, first, last FROM Actor WHERE (first LIKE 
							'%$words[0]%' AND last LIKE '%$words[1]%') OR 
							(first LIKE '%$words[1]%' AND last LIKE '%$words[0]%')";
			$actor_result = mysql_query($actor_query, $db_connection);
			while ($actor_row = mysql_fetch_row($actor_result))
			{
				$aid = $actor_row[0];
				$name = "$actor_row[1] $actor_row[2]";
				echo "<a href='http://192.168.56.20/~cs143/showActorInfo.php?aid=$aid'> $name </a> <br/>";					
			}
		}
		else
			echo "No names found! <br/>"; 

		/* Movie Query */
		/* Need to hyperlink Movie titles */
		echo "<b> Searching records in Movie database... </b> <br/>";

		// [[:<:]] - word boundary marker, so it matches on words in different order
		$where_clause = "";
		for ($i = 0; $i < $length; $i++)
		{
			if ($i == 0)
				$where_clause .= "title REGEXP '[[:<:]]$words[$i][[:>:]]'";
			else
				$where_clause .= "AND title REGEXP '[[:<:]]$words[$i][[:>:]]'";
		}

		//echo $where_clause;
		$movie_query = "SELECT id, title FROM Movie WHERE $where_clause";
		$movie_result = mysql_query($movie_query, $db_connection);

		while ($row = mysql_fetch_row($movie_result))
		{
			$mid = $row[0];
			$title = $row[1];
			
			echo "<a href='http://192.168.56.20/~cs143/showMovieInfo.php?mid=$mid'>" .
			 "$title </a> <br/>";
		}

	}






}

mysql_close($db_connection);
?>

</html>
</body>