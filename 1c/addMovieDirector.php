<!DOCTYPE html>
<html>
<body>

<link rel="stylesheet" href="add-style.css">

Add new director in movie: <br/><br/>
	<form action="addMovieDirector.php" method="POST">
		Movie: <select name="mid">
				<?php
					$db_connection = mysql_connect("localhost", "cs143", "");
					mysql_select_db("CS143", $db_connection);
					$movie_query = "SELECT id, title, year FROM Movie ORDER BY title";
					$movie_result = mysql_query($movie_query, $db_connection);
					while ($movie_row = mysql_fetch_row($movie_result))
					{
						$mid = $movie_row[0];
						$title = $movie_row[1]; 
						$year = $movie_row[2];

						echo "<option value='$mid'>$title ($year) </option>";
					}

					mysql_close($db_connection);
				?>
				</select>

				<br/><br/>
		Director: <select name="did">
				<?php
					$db_connection = mysql_connect("localhost", "cs143", "");
					mysql_select_db("CS143", $db_connection);
					$dir_query = "SELECT id, first, last FROM Director ORDER BY first, last";
					$dir_result = mysql_query($dir_query, $db_connection);
					while ($dir_row = mysql_fetch_row($dir_result))
					{
						$did = $dir_row[0];
						$name = "$dir_row[1] $dir_row[2]";

						echo "<option value='$did'>$name </option>";
					}

					mysql_close($db_connection);
				?>
				</select>

				</br><br/>
				<input type="submit" name="submit" value="Submit"/>
	</form>
	<hr/>

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

if (isset($_POST["submit"]))
{
	echo "mid: $_POST[mid] <br/>";
	echo "did: $_POST[did] <br/>";
	
	$insert_query = "INSERT INTO MovieDirector (mid, did) 
	VALUES ('$_POST[mid]', '$_POST[did]')";
	$insert_result = mysql_query($insert_query, $db_connection);

	if (!$insert_result) 
	{
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $insert_query;
		die($message);
	}
	else
	{
		echo "Director added successfully!<br/>";
	}
}

mysql_close($db_connection);
?>
</html>
</body>