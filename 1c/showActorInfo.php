<!DOCTYPE html>
<html>
<body>

<link rel="stylesheet" href="show.css">

<h2> Show Actor Info </h2>


<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

/* Max possible Actor ID */
$lookup_query = "SELECT id FROM MaxPersonID";
$lookup_result = mysql_fetch_row(mysql_query($lookup_query, $db_connection));
$max = $lookup_result[0] - 1;

/* If a valid Actor ID was passed in from URL */
if (isset($_GET['aid']))
{
	$id = $_GET['aid'];
	//echo "ID: $id <br/>"; 			
	$id_query = "SELECT * FROM Actor WHERE id=$id";
	$result = mysql_query($id_query, $db_connection);
}
else
{
	/* Generate a random valid Actor ID */
	do {
		$id = mt_rand(1, $max);
		// echo "ID: $id <br/>";						
		$id_query = "SELECT * FROM Actor WHERE id=$id";
		$result = mysql_query($id_query, $db_connection);
	} while (mysql_num_rows($result) == 0);
}

// Hardcoded Actor because many actors do not have roles yet
//$id = 52794;
//$id_query = "SELECT * FROM Actor WHERE id=52794";
//$result = mysql_query($id_query, $db_connection);

$nfield = mysql_num_fields($result);
$rows = mysql_fetch_row($result);

echo "<h4> Profile </h4>";

echo "Name: $rows[2] $rows[1]<br/>";		// First name

for ($i = 3; $i < $nfield; $i++)
{
	if ($i == 5)
	{
		echo ucfirst(mysql_field_name($result, $i)) . ": Still Alive <br/>";
	}
	else
	{
		echo ucfirst(mysql_field_name($result, $i)) . ": " . $rows[$i] . "<br/>";	
	}
}

echo "<h4> Filmography </h4>";

$film_query = "SELECT * FROM MovieActor WHERE aid=$id";
$film_result = mysql_query($film_query, $db_connection);

if (mysql_num_rows($film_result) == 0)
{
	if ($rows[3] == "Male")
		echo "Aspiring Actor :) <br/>"; 
	else
		echo "Aspiring Actress :) <br/>";
}
else
{
	while ($row = mysql_fetch_row($film_result))
	{
		$mid = $row[0];
		$role = $row[2];

		$title_query = "SELECT title FROM Movie WHERE id=$mid";
		$title_result = mysql_fetch_row(mysql_query($title_query, $db_connection));
		$title = $title_result[0];

		echo "$role in ";
		echo "<a href='http://192.168.56.20/~cs143/showMovieInfo.php?mid=$mid'>" .
			 "$title</a> <br/>";

	}
}

mysql_close($db_connection);
echo "<br/>";
include 'search.php';

?>

<a href='home.php'>Go Home</a></br>

</html>
</body>