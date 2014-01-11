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
// DECIMAL POINTS
$expr = $_GET["expr"]; //expression to be matched
$pattern = '/^([+ -]?[0-9]+\.?[0-9]*)(((\+\-)|(\-\+)|(\*\-)|(\*\+)|(\/\+)|(\/\-)|[\/\+\*\-])[0-9]+\.?[0-9]*)*$/'; // pattern to match against
preg_match($pattern, $expr, $matches); 

$zero = FALSE;

function e($errno, $errstr, $errfile, $errline) {
    if ($errstr == "Division by zero") {
        global $zero;
        $zero = TRUE;
        //echo $errstr."<br />";
    }
}

set_error_handler('e');

if (!empty($expr) && preg_match($pattern, $expr, $matches))
{

	eval("\$ans = $expr;"); 

	if ($zero == TRUE) 
	{ 
    	echo "Error: Division by Zero <br />";
	} 
	else 
	{
		echo "Answer: ". $ans;
	}

}
else if (!empty($expr))
{
	echo "Invalid expression: $expr";
}
?>

</body>
</html>

