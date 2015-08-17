create or replace PACKAGE BODY CENG352
AS


  PROCEDURE login(
      p_user_id IN INTEGER,
      p_email   IN VARCHAR2,
      p_RESULT OUT CHAR)
  AS
    /*If needed, you may add your local variables here*/
      n_of_found INTEGER;
  BEGIN
      
      /*You should add your code below*/
	/* dbms_output.put_line('DELETE THIS LINE..'); */
      SELECT COUNT (*)
      INTO n_of_found
      FROM Users
      WHERE Users.userid = p_user_id AND Users.email = p_email;
      IF (n_of_found > 0) THEN
        p_RESULT := 'T';
      ELSE
        p_RESULT := 'F';
      END IF;


  END login;
  
  
  PROCEDURE borrow_book(
      p_user_id IN INTEGER,
      p_ISBN    IN VARCHAR2,
      p_RESULT OUT CHAR)
  IS
	/*If needed, you may add your local variables here*/
      n_of_found INTEGER;
  BEGIN

      /*You should add your code below*/
	/* dbms_output.put_line('DELETE THIS LINE..'); */
      SELECT COUNT (*)
      INTO n_of_found
      FROM Users
      WHERE Users.userid = p_user_id;
      IF n_of_found > 0 THEN
        SELECT COUNT (*)
        INTO n_of_found
        FROM BookInfo
        WHERE BookInfo.isbn = p_ISBN AND BookInfo.quantity > 0;
        IF n_of_found > 0 THEN
          UPDATE BookInfo
          SET quantity = quantity - 1
          WHERE isbn = p_ISBN;

          INSERT INTO BorrowedBy
          VALUES (p_user_id, p_ISBN, sysdate, null);
          p_RESULT := 'T';
        ELSE
          p_RESULT := 'F';
        END IF;
      ELSE
        p_RESULT := 'F';
      END IF;      
  END borrow_book;
  
  
  PROCEDURE return_book
    (
      p_user_id   IN INTEGER,
      p_ISBN      IN VARCHAR2,
      p_issuedate IN DATE
    )
  IS
      n_of_found INTEGER;
  BEGIN

     /*You should add your code below*/
	/* dbms_output.put_line('DELETE THIS LINE..'); */

      SELECT COUNT (*)
      INTO n_of_found
      FROM BorrowedBy
      WHERE BorrowedBy.userid = p_user_id AND BorrowedBy.isbn = p_ISBN AND BorrowedBy.issueDate = p_issuedate;
      IF n_of_found > 0 THEN
        UPDATE BorrowedBy
        SET dayReturned = sysdate
        WHERE userid = p_user_id AND isbn = p_ISBN AND issueDate = p_issuedate;
        UPDATE BookInfo
        SET quantity = quantity + 1
        WHERE isbn = p_ISBN;
      END IF;
  END return_book;
  
  
  PROCEDURE books_borrowed(
      p_user_id IN INTEGER ,
      p_books OUT SYS_REFCURSOR)
  IS
  BEGIN
  
    /*You should add your code below*/
	/* dbms_output.put_line('DELETE THIS LINE..'); */

      OPEN p_books FOR SELECT BookInfo.isbn, title, pubYear, issueDate FROM BookInfo INNER JOIN BorrowedBy
      ON BookInfo.isbn = BorrowedBy.isbn WHERE BorrowedBy.userid = p_user_id AND BorrowedBy.dayReturned IS NULL;

      EXCEPTION
      WHEN no_data_found THEN NULL;

  
  END books_borrowed;
  
  
  
  PROCEDURE books_returned(
      p_user_id IN INTEGER ,
      p_books OUT SYS_REFCURSOR)
  IS
  BEGIN

      /*You should add your code below*/
	/* dbms_output.put_line('DELETE THIS LINE..'); */

      OPEN p_books FOR SELECT BookInfo.isbn, title, pubYear, issueDate FROM BookInfo INNER JOIN BorrowedBy
      ON BookInfo.isbn = BorrowedBy.isbn WHERE BorrowedBy.userid = p_user_id AND BorrowedBy.dayReturned IS NOT NULL;

      EXCEPTION
      WHEN no_data_found THEN NULL;

  END books_returned;
  
  
 PROCEDURE search_books(
      p_title IN VARCHAR2,
      p_books OUT SYS_REFCURSOR)
  IS
  BEGIN
        /*You should add your code below*/
	/* dbms_output.put_line('DELETE THIS LINE..'); */

      OPEN p_books FOR SELECT * FROM BookInfo
      WHERE BookInfo.title LIKE concat (p_title, '%');

      EXCEPTION
      WHEN no_data_found THEN NULL;


  END search_books;
  
  
  PROCEDURE search_books(
      p_year1 IN VARCHAR2,
      p_year2 IN VARCHAR2,
      p_books  OUT SYS_REFCURSOR)
  IS
  BEGIN
        /*You should add your code below*/
	/* dbms_output.put_line('DELETE THIS LINE..'); */

      OPEN p_books FOR SELECT * FROM BookInfo
      WHERE to_number (BookInfo.pubYear) > to_number (p_year1) AND to_number (BookInfo.pubYear) < to_number (p_year2);

      EXCEPTION
      WHEN no_data_found THEN NULL;

  END search_books;
  
  
END CENG352;