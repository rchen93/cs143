<!DOCTYPE html>
<html>
<body>

Add new actor/director: <br/>
<form action="input.php" method="GET">
	Identity:	<input type="radio" name="identity" value="Actor" checked="true">Actor
				<input type="radio" name="identity" value="Director">Director<br/>
	<hr/>
	First Name:	<input type="text" name="first" maxlength="20"><br/>
	Last Name:	<input type="text" name="last" maxlength="20"><br/>
	Sex:		<input type="radio" name="sex" value="Male" checked="true">Male
				<input type="radio" name="sex" value="Female">Female<br/>
				
	Date of Birth:	<input type="text" name="dob"><br/>
	Date of Death:	<input type="text" name="dod"> (leave blank if alive now)<br/>
	<input type="submit" name="submit" value="Submit"/>
</form>
<hr/>

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

$fields = array("identity", "first", "last", "sex", "dob");
$error = false;
$dod = false;

foreach ($fields as $key)
{
	if (isset($_GET[$key]))
	{
		if (empty($_GET[$key]))
			$error = true;
	}
	else
		$error = true;
}

if (isset($_GET["submit"]))
{
	if ($error)
		echo "At least one field is missing!";
	else
	{
		echo "All fields have been set!</br>";

		$query = "INSERT INTO $_GET[identity] (id, last, first, sex, dob, dod)
		VALUES ('68999', '$_GET[last]', '$_GET[first]', '$_GET[sex]', '$_GET[dob]', '$_GET[dod]')";
		$result = mysql_query($query, $db_connection);
		if (!$result) 
		{
    		$message  = 'Invalid query: ' . mysql_error() . "\n";
    		$message .= 'Whole query: ' . $query;
    		die($message);ceez
		}
		else
			echo "$_GET[identity] added successfully!";
	}

}
mysql_close($db_connection);



?>
			





</html>
</body>