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
$expr = $_GET["expr"]; //expression to be matched
//$expr = preg_replace('/\s/', '', $expr);

$pattern = '/\s*[+ -]?[0-9]+\.?[0-9]*\s*(((\+\s*\-)|(\-\s*\+)|(\*\s*\-)|(\*\s*\+)|(\/\s*\+)|(\/\s*\-)|(\-\s+\-)|(\+\s+\+)|([\/\+\*\-]\s*))[0-9]+\.?[0-9]*)*/'; // pattern to match against 
$zero = FALSE;

//preg_match($pattern, $expr, $matches);
//var_dump($matches);

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

