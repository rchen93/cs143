<!DOCTYPE html>
<html>
<body>

<link rel="stylesheet" href="add-style.css">

<h2>Add New Actor To A Movie</h2>
	<form action="addMovieActor.php" method="POST">
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
		Actor: <select name="aid">
				<?php
					$db_connection = mysql_connect("localhost", "cs143", "");
					mysql_select_db("CS143", $db_connection);
					$actor_query = "SELECT id, first, last FROM Actor ORDER BY first, last";
					$actor_result = mysql_query($actor_query, $db_connection);
					while ($actor_row = mysql_fetch_row($actor_result))
					{
						$aid = $actor_row[0];
						$name = "$actor_row[1] $actor_row[2]";

						echo "<option value='$aid'>$name </option>";
					}

					mysql_close($db_connection);
				?>
				</select>

				<br/><br/>
		Role: <input type="text" maxlength="50" name="role"></input>
			  <br/><br/>
				<input type="submit" name="submit" value="Submit"/>
	</form>
	<hr/>

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

$error = false;

if (isset($_POST["role"]))
{
	if (empty($_POST["role"]))
		$error = true;
}
else
	$error = true;

if (isset($_POST["submit"]))
{
	if ($error)
		echo "Please enter a role! <br/>";
	else
	{
		echo "mid is: $_POST[mid] <br/>";		// Remove later
		echo "aid is: $_POST[aid] <br/>";		// Remove later

		$insert_query = "INSERT INTO MovieActor (mid, aid, role) 
		VALUES ('$_POST[mid]', '$_POST[aid]', '$_POST[role]')";
		$insert_result = mysql_query($insert_query, $db_connection);

		if (!$insert_result) 
		{
    		$message  = 'Invalid query: ' . mysql_error() . "\n";
    		$message .= 'Whole query: ' . $insert_query;
    		die($message);
		}
		else
		{
			echo "Role added successfully!<br/>";
		}
	}
}

mysql_close($db_connection);
?>

<h3><a href='home.php'>Go Home</a></h3></br>
</html>
</body>