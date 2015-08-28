<?php
	session_start();
	if (!empty ($_SESSION["userid"]))
	{
		$conn = oci_connect ("e1848324", "jqsuR", "//144.122.71.31:8085/xe");
		if (!$conn)
		{
			echo "<!DOCTYPE HTML>";
			echo "<html>";
			echo "<head>";
			echo "<title>Database Error</title>";
			echo "</head>";
			echo "<body>";
			echo "<span>Can't connect to database</span><br>";
			echo "<a href = 'logout.php'>Log Out</a>";
			echo "</body>";
			echo "</html>";
		}
		else
		{
			$stid = oci_parse ($conn, "begin ceng352.borrow_book (:userid, :isbn, :result); end;");
			oci_bind_by_name ($stid, ":userid", $_SESSION["userid"]);
			oci_bind_by_name ($stid, ":isbn", $_GET["isbn"]);
			oci_bind_by_name ($stid, ":result", $result);
			oci_execute ($stid);
			if (strcmp (substr ($result, 0, 1), "T") == 0)
			{
				$host  = $_SERVER['HTTP_HOST'];
				$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
				$extra = 'list_books.php';
				header ("Location: http://$host$uri/$extra");
			}
			else
			{
				echo "<!DOCTYPE HTML>";
				echo "<html>";
				echo "<head>";
				echo "<title>Error</title>";
				echo "</head>";
				echo "<body>";
				echo "<span>Can't borrow this book</span><br>";
				$host  = $_SERVER['HTTP_HOST'];
				$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
				$extra1 = 'list_books.php';
				$extra2 = 'logout.php';
				echo "<a href = 'http://$host$uri/$extra1'>Back to List</a><br>";
				echo "<a href = 'http://$host$uri/$extra2'>Log Out</a>";
				echo "</body>";
				echo "</html>";
			}
			oci_free_statement ($stid);
			oci_free_statement ($curs);
		}
		oci_close ($conn);
	}
	else
	{
		if (session_destroy())
		{
			$host  = $_SERVER['HTTP_HOST'];
			$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
			$extra = 'index.php';
			header ("Location: http://$host$uri/$extra");
		}
	}
?>