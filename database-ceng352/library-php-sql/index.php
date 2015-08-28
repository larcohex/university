<?php
/*	header('Content-type: text/html; charset=utf-8');
	ini_set('display_errors',1);
	error_reporting(E_ALL); */
	include ("login.php");

	if (isset ($_SESSION["login_user"]))
	{
		$host  = $_SERVER['HTTP_HOST'];
		$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
		$extra = 'list_books.php';
		header ("Location: http://$host$uri/$extra");
	}
?>
<!DOCTYPE HTML>
<html>
	<head>
		<title>Login</title>
	</head>
	<body>
		<form name = "login" method = "post" action = "">
			<input name = "userid" type = "text" placeholder = "UserID"> <br>
			<input name = "email" type = "text" placeholder = "Email"> <br>
			<input name = "submit" type = "submit" value = "Submit">
			<span><?php if (isset($error)) echo $error; ?></span>
		</form>
	</body>
</html>