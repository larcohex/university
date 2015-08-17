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
			echo "<title>Database Error</title>"
			echo "</head>";
			echo "<body>";
			echo "<span>Can't connect to database</span><br>";
			echo "<a href = 'logout.php'>Log Out</a>";
			echo "</body>";
			echo "</html>";
		}
		else
		{
			$stid = oci_parse ($conn, "begin ceng352.return_book (:userid, :isbn, :issue_date); end;"
			oci_bind_by_name ($stid, ":userid", $_SESSION["userid"]);
			oci_bind_by_name ($stid, ":isbn", $_GET["isbn"]);
			oci_bind_by_name ($stid, ":issue_date", $_GET["issueDate"]);
			oci_execute ($stid);
			header ("location: list_books.php");
			oci_free_statement ($stid);
			oci_free_statement ($curs);
		}
		oci_close ($conn);
	}
	else
	{
		if (session_destroy())
		{
			header ("location: index.php");
		}
	}
?>