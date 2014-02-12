<!DOCTYPE html>
<html>
<body>

<link rel="stylesheet" href="review.css">

<h2>Add A Review</h2>
<p>
<form action="addReview.php" method="POST">
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
if (isset($_POST["submit"]))				
{

	if (empty($_POST['name']))
		$name = $default_name;
	else
		$name = $_POST['name'];

	$time_query = "SELECT NOW()";
	$time_result = mysql_fetch_row(mysql_query($time_query, $db_connection));

	$review = $_POST['comment'];
	$rating = $_POST['rating']; 
	$time = $time_result[0];

	$insert_query = "INSERT INTO Review (name, time, mid, rating, comment)
	VALUES ('$name', '$time', 6, '$rating', '$review')";						// Hardcoded a movie id, change later

	$result = mysql_query($insert_query, $db_connection);

	if (!$result) 
	{
	    $message  = 'Invalid query: ' . mysql_error() . "\n";
	    $message .= 'Whole query: ' . $insert_query;
	    die($message);
	}
	else
		echo "Review added succesfully! <br/>";				/* Remove later */

}
mysql_close($db_connection);
?>

<a href='home.php'>Go Home</a></br>
</html>
</body>