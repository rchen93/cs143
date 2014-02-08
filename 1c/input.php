<!DOCTYPE html>
<html>
<body>

Add new actor/director: <br/>
<form action="input.php" method="POST">
	Identity:	<input type="radio" name="identity" value="Actor" checked="true">Actor
				<input type="radio" name="identity" value="Director">Director<br/>
	<hr/>
	First Name:	<input type="text" name="first" maxlength="20"><br/>
	Last Name:	<input type="text" name="last" maxlength="20"><br/>
	Sex:		<input type="radio" name="sex" value="Male" checked="true">Male
				<input type="radio" name="sex" value="Female">Female<br/>
				
	Date of Birth:	<input type="text" name="dob"><br/>
	Date of Death:	<input type="text" name="dod"> (leave blank if alive now)<br/>
	<input type="submit" value="Submit"/>
</form>
<hr/>

<?php
$query = $_POST["query"];
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("CS143", $db_connection);

if (!empty($query))
{ 


}
mysql_close($db_connection);
?>
			





</html>
</body>