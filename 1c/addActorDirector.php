<!DOCTYPE html>
<html>
<body>

<link rel="stylesheet" href="add-style.css">

Add new actor/director: <br/>
<form action="addActorDirector.php" method="POST">
	Identity:	<input type="radio" name="identity" value="Actor" checked="true">Actor
				<input type="radio" name="identity" value="Director">Director<br/>
	<hr/>
	First Name:	<input type="text" name="first" maxlength="20"><br/><br/>
	Last Name:	<input type="text" name="last" maxlength="20"><br/><br/>
	Sex:		<input type="radio" name="sex" value="Male" checked="true">Male
				<input type="radio" name="sex" value="Female">Female<br/><br/>
				
	Date of Birth:	<input type="text" name="dob"> (Please enter in the form of yyyymmdd)<br/><br/>
	Date of Death:	<input type="text" name="dod"> (Leave blank if still alive)<br/><br/>
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
	if (isset($_POST[$key]))
	{
		if (empty($_POST[$key]))
			$error = true;
	}
	else
		$error = true;
}

/* Check if dates are valid */
if (!$error)
{
	if (strlen($_POST['dob']) != 8 || !ctype_digit($_POST['dob']))
	{
		$error = true;
		$error_message = "Invalid Date of Birth!";
	}
	else if (isset($_POST['dod']))
	{
		if (!empty($_POST['dod']))
		{
			if (strlen($_POST['dod']) != 8 || !ctype_digit($_POST['dod']))
			{
				$error = true;
				$error_message = "Invalid Date of Death!";
			}

			if ($_POST['dod'] < $_POST['dob'])
			{
				$error = true;
				$error_message = "Date of Death cannot precede Date of Birth!";
				
			}
		}
	}

}

/* MySQL insertion */
if (isset($_POST["submit"]))
{
	if ($error)
		echo $error_message;
	else
	{
		// echo "All fields have been set!</br>";			/* Remove later */

		$lookup_query = "SELECT id FROM MaxPersonID";
		$lookup_result = mysql_fetch_row(mysql_query($lookup_query, $db_connection));
		$id = $lookup_result[0];

		if ($_POST['identity'] == "Actor")
		{
			$insert_query = "INSERT INTO $_POST[identity] (id, last, first, sex, dob, dod)
			VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[sex]', '$_POST[dob]', '$_POST[dod]')";
			$result = mysql_query($insert_query, $db_connection);
		}
		else
		{
			$insert_query = "INSERT INTO $_POST[identity] (id, last, first, dob, dod)
			VALUES ($id, '$_POST[last]', '$_POST[first]', '$_POST[dob]', '$_POST[dod]')";
			$result = mysql_query($insert_query, $db_connection);
		}

		if (!$result) 
		{
    		echo "Something bad happened...Please try again! <br/>";
		}
		else
		{
			echo "$_POST[identity] added successfully!<br/>";		
			$update_query = "UPDATE MaxPersonID SET id=id+1";
			$update_result = mysql_query($update_query, $db_connection);

			/*
			if ($update_result)
			{
				echo "MaxPersonID updated!";			
			}
			else
				echo "Failed";						 	
			*/
		}
	}

}

mysql_close($db_connection);



?>
			


<a href='home.php'>Go Home</a></br>
</html>
</body>