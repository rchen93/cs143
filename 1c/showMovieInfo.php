<!DOCTYPE html>
<html>
<body>

<h2> Show Movie Info </h2>

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

/* Max possible Movie ID */
$lookup_query = "SELECT id FROM MaxMovieID";
$lookup_result = mysql_fetch_row(mysql_query($lookup_query, $db_connection));
$max = $lookup_result[0] - 1;

/* Generate a random valid Movie ID */
/*do {
	$id = mt_rand(1, $max);
	echo "ID: " . $id . "<br/>";
	$id_query = "SELECT * FROM Movie WHERE id=$id";
	$result = mysql_query($id_query, $db_connection);
} while (mysql_num_rows($result) == 0);*/

// Hardcode Check
$id = 6;
$id_query = "SELECT * FROM Movie WHERE id=$id";
$result = mysql_query($id_query, $db_connection);

$nfield = mysql_num_fields($result);
$rows = mysql_fetch_row($result);

echo "<h4> Profile </h4>";
for ($i = 1; $i < $nfield; $i++)
{
	echo $rows[$i] . "<br/>";	// Add headings to show what information represents
								// Maybe combine the year with Title, e.g. Title(yyyy)
}

/* Directors */
$dir_query = "SELECT CONCAT(D.first, SPACE(1), D.last) name 
FROM MovieDirector MD, Director D WHERE MD.mid=$id AND D.id=MD.did";
$dir_result = mysql_query($dir_query, $db_connection);

if (mysql_num_rows($dir_result) == 0)
{
	echo "No directors found! <br/>"; 	// Change to blank later?
}
else
{
	while($dir_row = mysql_fetch_row($dir_result))
	{
		echo $dir_row[0] . "<br/>";
	}
}

/* Genres */
$genre_query = "SELECT genre FROM MovieGenre WHERE mid=$id";
$genre_result = mysql_query($genre_query, $db_connection);
while ($genre_row = mysql_fetch_row($genre_result))
{
	echo $genre_row[0] . "<br/>";
}

echo "<h4> Cast </h4>";

$cast_query = "SELECT CONCAT(A.first, SPACE(1), A.last) name, MA.role 
FROM MovieActor MA, Actor A WHERE MA.mid=$id AND A.id=MA.aid";
$cast_result = mysql_query($cast_query, $db_connection);

if (mysql_num_rows($cast_result) == 0)
{
	echo "No cast members found! <br/>"; 
}
else
{
	while ($row = mysql_fetch_row($cast_result))
	{
		$name = $row[0];				// Hyperlink $name
		$role = $row[1];

		echo $name . " as " . $role . "<br/>";				
	}
}

echo "<h4> User Reviews </h4>";

$avg_query = "SELECT AVG(rating) FROM Review WHERE mid=$id";
$avg_result = mysql_fetch_row(mysql_query($avg_query, $db_connection));

$count_query = "SELECT COUNT(*) FROM Review WHERE mid=$id";
$count_result = mysql_fetch_row(mysql_query($count_query, $db_connection));

if ($avg_result[0] == NULL)
{
	echo "Average Score: Not Available due to 0 reviews. <br/>";
}
else
	echo "Average Score: $avg_result[0]/5 (5.0 is best) based on $count_result[0] review(s). <br/>";

/* Add a Add Comment button */

$comment_query = "SELECT * FROM Review WHERE mid=$id";
$comment_result = mysql_query($comment_query, $db_connection);

while ($com_row = mysql_fetch_row($comment_result))
{
	$name = $com_row[0];
	$time = $com_row[1];
	$rating = $com_row[3];
	$com = $com_row[4];

	echo "On $time, $name rated this movie $rating point(s) and said: <br/>";
	echo $com . "<br/> <br/>";
}


mysql_close($db_connection);
?>

</html>
</body>