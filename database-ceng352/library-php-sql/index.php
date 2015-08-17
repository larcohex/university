<?php
	include ("login.php");

	if (isset ($_SESSION["login_user"]))
	{
		header ("location: list_books.php");
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
			<span><?php echo $error; ?></span>
		</form>
	</body>
</html>