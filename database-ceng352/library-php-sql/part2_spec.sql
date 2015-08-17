create or replace PACKAGE ceng352
IS
  -- Author  : ABDULLAH
  -- Created : 03/03/2015 10:06:13 PM
  -- Purpose :  DO NOT EDIT THIS PACKAGE SPECIFICATION!!!
  -- Public function and procedure declarations
  PROCEDURE login(
      p_user_id IN INTEGER,
      p_email   IN VARCHAR2,
      p_RESULT OUT CHAR);
	  
  PROCEDURE borrow_book(
      p_user_id IN INTEGER,
      p_ISBN    IN VARCHAR2,
      p_RESULT OUT CHAR);
	  
  PROCEDURE return_book(
      p_user_id   IN INTEGER,
      p_ISBN      IN VARCHAR2,
      p_issuedate IN DATE);
	  
  PROCEDURE books_borrowed(
      p_user_id IN INTEGER ,
      p_books OUT SYS_REFCURSOR);
	  
  PROCEDURE books_returned(
      p_user_id IN INTEGER ,
      p_books OUT SYS_REFCURSOR);
	  
  PROCEDURE search_books(
      p_title IN VARCHAR2,
      p_books OUT SYS_REFCURSOR);
	  
  PROCEDURE search_books(
      p_year1 IN VARCHAR2,
      p_year2 IN VARCHAR2,
      p_books  OUT SYS_REFCURSOR);
END ceng352;