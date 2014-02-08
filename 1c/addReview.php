<!DOCTYPE html>
<html>
<body>
<h2>Add A Review</h2>
<p>
<form action="addReview.php" method="GET">
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
<input type="submit" value="Add Review!" />
</form> 
</p>


<?php
$name = $_GET["name"];
$review = $_GET["comment"];
$rating = $_GET["rating"]; 


$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);


$insert_query = "INSERT INTO $Review (name, time, mid, rating, comment)
VALUES ('$_GET[name]', 11:00, 12398, '$_GET[rating]', '$_GET[comment]')";
$result = mysql_query($insert_query, $db_connection);




?>
</html>
</body>