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
			echo "<!DOCTYPE HTML>";
			echo "<html>";
			echo "<head>";
			echo "<title>Search by years</title>";
			echo "</head>";
			echo "<body>";
			echo "<form name = 'login' method = 'post' action = ''>";
			echo "<input name = 'year1' type = 'text' placeholder = 'Min year'><br>";
			echo "<input name = 'year2' type = 'text' placeholder = 'Max year'><br>";
			echo "<input name = 'submit' type = 'submit' value = 'Submit'>";
			echo "</form>";
			if (isset ($_POST["submit"]))
			{
				if (empty ($_POST["year1"]) || empty ($_POST["year2"]))
				{
					echo "<span>Please don't leave information blank<span>";
				}
				else
				{
					$curs = oci_new_cursor ($conn);
					$stid = oci_parse ($conn, "begin ceng352.search_books (:year1, :year2, :books); end;"
					oci_bind_by_name ($stid, ":year1", $_POST["year1"]);
					oci_bind_by_name ($stid, ":year2", $_POST["year2"]);
					oci_bind_by_name ($stid, ":books", $curs, -1, OCI_B_CURSOR);
					oci_execute ($stid);
					oci_execute ($curs);
					echo "<table border = '1'>";
					echo "<tr>";
					echo "<th>ISBN</th><th>Title</th><th>Pub.Year</th><th>Quantity</th><th>Borrow?</th>" 
					echo "</tr>";
					while ($row = oci_fetch_array ($curs))
					{
						echo "<tr>";
						echo "<td>" . $row["isbn"] . "</td><td>" . $row["title"] . "</td><td>" . $row["pubYear"] . "</td><td>" . $row["quantity"] . "</td><td><a href = '/borrow_book.php?isbn=" . $row["isbn"] . "'>Borrow</a></td>";
						echo "</tr>";
					}
					echo "</table>";
				}
			}
			echo "<a href = 'search_by_title.php'>Search By Title</a>";
			echo "<a href = 'list_books.php'>Back to your list</a>";
			echo "<a href = 'logout.php'>Log Out</a>";
			echo "</body>";
			echo "</html>";
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