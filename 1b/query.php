<!DOCTYPE html>
<html>
<body>

<h1> <i>Noobest</i> Movie Database </h1>
By Roger and Kailin

<p>Type an SQL query in the following box:</p>
 
<p>
<form action="query.php" method="GET">
<textarea name="query" cols="60" rows="8"></textarea>
<input type="submit" value="Submit" />
</form> 
</p>


<?php
$query = $_GET["query"];
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

if (!empty($query))
{ 
	$result = mysql_query($query, $db_connection);
	$nfield = mysql_num_fields($result);

	echo "<table border='1'>";	

	echo "<tr>";
	for($i = 0; $i < $nfield; $i++)
		{
			echo "<th>";
			echo mysql_field_name($result, $i);
			echo "</th>";			
		}
	echo "</tr>";

	while ($row = mysql_fetch_row($result))
	{
		echo "<tr>";
		for($i = 0; $i < $nfield; $i++)
		{
		  	echo "<td>" . $row[$i] . "</td>";
		}
		echo "</tr>";

	}

	echo"</table>";


}

mysql_close($db_connection);
?>

</html>
</body>