<!DOCTYPE html>
<html>
<body>

<link rel="stylesheet" href="review.css">

<h2>Add A Review</h2>
<p>

<form action="addReview.php" method="GET">
Movie: <select name="mid">
<?php
	$db_connection = mysql_connect("localhost", "cs143", "");
	mysql_select_db("CS143", $db_connection);

	$id = $_GET['mid'];

	$title_query = "SELECT title FROM Movie WHERE id=$id";
	$title_result = mysql_fetch_row(mysql_query($title_query, $db_connection));
	$title = $title_result[0];

	echo "<option value='$id'>$title</option>";

	mysql_close($db_connection);
?>
</select>
<br/>
Name: <input type="text" name="name" maxlength="20"><br/>
I give this movie 
<select name="rating">
<option value="1">1</option>
<option value="2">2</option>
<option value="3">3</option>
<option value="4">4</option>
<option value="5">5</option>
</select>
out of 5 points (with 5 being the highest). </br>
<textarea name="comment" cols="63" rows="8"></textarea>
<input type="submit" name="submit" value="Add Review!" />
</form> 
</p>



<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

$default_name = 'Anonymous';


/* MySQL insertion */
if (isset($_GET["submit"]))				
{

	if (empty($_GET['name']))
		$name = $default_name;
	else
		$name = $_GET['name'];

	$time_query = "SELECT NOW()";
	$time_result = mysql_fetch_row(mysql_query($time_query, $db_connection));

	$review = $_GET['comment'];
	$rating = $_GET['rating']; 
	$mid = $_GET['mid'];
	$time = $time_result[0];

	$insert_query = "INSERT INTO Review (name, time, mid, rating, comment)
	VALUES ('$name', '$time', '$mid', '$rating', '$review')";						

	$result = mysql_query($insert_query, $db_connection);

	if (!$result) 
	{
		echo "Something bad happened...Please try again! <br/>";
	    //$message  = 'Invalid query: ' . mysql_error() . "\n";
	    //$message .= 'Whole query: ' . $insert_query;
	    //die($message);
	}
	else
		echo "Review added succesfully! <br/>";				


}
mysql_close($db_connection);
?>

<a href='home.php'>Go Home</a></br>
</html>
</body>