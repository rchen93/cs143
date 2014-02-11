<!DOCTYPE html>
<html>
<body>

Add new actor/director: <br/>
<form action="addActorDirector.php" method="GET">
	Identity:	<input type="radio" name="identity" value="Actor" checked="true">Actor
				<input type="radio" name="identity" value="Director">Director<br/>
	<hr/>
	First Name:	<input type="text" name="first" maxlength="20"><br/>
	Last Name:	<input type="text" name="last" maxlength="20"><br/>
	Sex:		<input type="radio" name="sex" value="Male" checked="true">Male
				<input type="radio" name="sex" value="Female">Female<br/>
				
	Date of Birth:	<input type="text" name="dob"> (Please enter in the form of yyyymmdd)<br/>
	Date of Death:	<input type="text" name="dod"> (Leave blank if still alive)<br/>
	<input type="submit" name="submit" value="Submit"/>
</form>
<hr/>

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

$fields = array("identity", "first", "last", "sex", "dob");
$error = false;
$error_message = "At least one field is missing!";

/* Check required fields */
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

/* Check if dates are valid */
if (!$error)
{
	if (strlen($_GET['dob']) != 8 || !ctype_digit($_GET['dob']))
	{
		$error = true;
		$error_message = "Invalid Date of Birth!";
	}
	else if (isset($_GET['dod']))
	{
		if (!empty($_GET['dod']))
		{
			if (strlen($_GET['dod']) != 8 || !ctype_digit($_GET['dod']))
			{
				$error = true;
				$error_message = "Invalid Date of Death!";
			}
		}
	}

}

/* MySQL insertion */
if (isset($_GET["submit"]))
{
	if ($error)
		echo $error_message;
	else
	{
		echo "All fields have been set!</br>";			/* Remove later */

		$lookup_query = "SELECT id FROM MaxPersonID";
		$lookup_result = mysql_fetch_row(mysql_query($lookup_query, $db_connection));
		$id = $lookup_result[0];

		$insert_query = "INSERT INTO $_GET[identity] (id, last, first, sex, dob, dod)
		VALUES ($id, '$_GET[last]', '$_GET[first]', '$_GET[sex]', '$_GET[dob]', '$_GET[dod]')";
		$result = mysql_query($insert_query, $db_connection);

		if (!$result) 
		{
    		$message  = 'Invalid query: ' . mysql_error() . "\n";
    		$message .= 'Whole query: ' . $insert_query;
    		die($message);
		}
		else
		{
			echo "$_GET[identity] added successfully!<br/>";
			$update_query = "UPDATE MaxPersonID SET id=id+1";
			if (mysql_query($update_query, $db_connection))
			{
				echo "MaxPersonID updated!";			/* Remove later */
			}
			else
				echo "Failed";						 	/* Remove later */
		}
	}

}

mysql_close($db_connection);



?>
			





</html>
</body>