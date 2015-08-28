<?php
	session_start();
	if (isset ($_POST["submit"]))
	{
		if (empty ($_POST["userid"]) || empty ($_POST["email"]))
		{
			$error = "Please don't leave information blank";
		}
		else
		{
			$conn = oci_connect ("e1848324", "jqsuR", "//144.122.71.31:8085/xe");
			if (!$conn)
			{
				$error = "Error connecting to database";
			}
			else
			{
				$stid = oci_parse ($conn, "begin ceng352.login (:userid, :email, :result); end;");
				oci_bind_by_name ($stid, ":userid", $_POST["userid"]);
				oci_bind_by_name ($stid, ":email", $_POST["email"]);
				oci_bind_by_name ($stid, ":result", $result);
				oci_execute ($stid);
				if (strcmp (substr ($result, 0, 1), "T") == 0)
				{
					$_SESSION["userid"] = $_POST["userid"];
					$host  = $_SERVER['HTTP_HOST'];
					$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
					$extra = 'list_books.php';
					header ("Location: http://$host$uri/$extra");
				}
				else
				{
					$error = "Username or password is invalid ";
				}
				oci_free_statement ($stid);
			}
			oci_close ($conn);
		}
	}
?>