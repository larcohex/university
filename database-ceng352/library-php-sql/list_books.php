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
			echo "<span>Can't connect to database</span><br><br><br>";
			echo "<a href = 'logout.php'>Log Out</a>";
			echo "</body>";
			echo "</html>";
		}
		else
		{
			$curs = oci_new_cursor ($conn);
			$stid = oci_parse ($conn, "begin ceng352.books_borrowed (:userid, :books); end;");
			oci_bind_by_name ($stid, ":userid", $_SESSION["userid"]);
			oci_bind_by_name ($stid, ":books", $curs, -1, OCI_B_CURSOR);
			oci_execute ($stid);
			oci_execute ($curs);

			echo "<!DOCTYPE HTML>";
			echo "<html>";
			echo "<head>";
			echo "<title>List Books</title>";
			echo "</head>";
			echo "<body>";
			echo "<table border = '1'>";
			echo "<tr>";
			echo "<th>ISBN</th><th>Title</th><th>Pub.Year</th><th>Issue Date</th><th>Return?</th>";
			echo "</tr>";
			while ($row = oci_fetch_array ($curs))
			{
				$host  = $_SERVER['HTTP_HOST'];
				$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
				$extra = 'return_book.php';
				echo "<tr>";
				echo "<td>" . $row[0] . "</td><td>" . $row[1] . "</td><td>" . $row[2] . "</td><td>" . $row[3] . "</td><td><a href = 'http://$host$uri/$extra?&isbn=" . $row[0] . "&issueDate=" . $row[3] . "'>Return</a></td>";
				echo "</tr>";
			}
			echo "</table>";
			echo "<caption>Books borrowed</caption><br><br><br>";


			oci_free_statement ($stid);
			oci_free_statement ($curs);
			$curs = oci_new_cursor ($conn);
			$stid = oci_parse ($conn, "begin ceng352.books_returned (:userid, :books); end;");
			oci_bind_by_name ($stid, ":userid", $_SESSION["userid"]);
			oci_bind_by_name ($stid, ":books", $curs, -1, OCI_B_CURSOR);
			oci_execute ($stid);
			oci_execute ($curs);


			echo "<table border = '1'>";
			echo "<tr>";
			echo "<th>ISBN</th><th>Title</th><th>Pub.Year</th><th>Issue Date</th>";
			echo "</tr>";
			while ($row = oci_fetch_array ($curs))
			{
				$host  = $_SERVER['HTTP_HOST'];
				$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
				$extra = 'return_book.php';
				echo "<tr>";
				echo "<td>" . $row[0] . "</td><td>" . $row[1] . "</td><td>" . $row[2] . "</td><td>" . $row[3] . "</td><td><a href = 'http://$host$uri/$extra?&isbn=" . $row[0] . "&issueDate=" . $row[3] . "'>Return</a></td>";
			}
			echo "</table>";
			echo "<caption>Books returned</caption><br><br><br>";


			$host  = $_SERVER['HTTP_HOST'];
			$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
			$extra1 = 'search_by_title.php';
			$extra2 = 'logout.php';
			echo "<a href = 'http://$host$uri/$extra1'>Search Books</a><br>";
			echo "<a href = 'http://$host$uri/$extra2''>Log Out</a>";
			echo "</body>";
			echo "</html>";
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