<!DOCTYPE html>
<html>
<body>

<h1> Calculator </h1>

<br>

<form method="GET">
	<input type="text" name="expr">
	<input type="submit" value="Calculate">
</form>

<?php
// TO DO: DON'T DIVIDE BY ZERO
$expr = $_GET["expr"]; //expression to be matched
$pattern = '/^[+ -]?[0-9]+(((\+\-)|(\-\+)|(\*\-)|(\*\+)|(\/\+)|(\/\-)|[\/\+\*\-])[0-9]+)*$/'; // pattern to match against
preg_match($pattern, $expr, $matches); 
if (!empty($expr) && preg_match($pattern, $expr, $matches))
{
	eval("\$ans = $expr;"); // eval is weird

	echo "Answer: ". $ans;
}
else if (!empty($expr))
{
	echo "Invalid expression: $expr";
}
?>

</body>
</html>

