create table Users
(
  userid INTEGER,
  email VARCHAR2(30) check (email like '%edu.tr'),
  name VARCHAR2(30) not null check (length (name) > 0),
  surname VARCHAR2(30) not null check (length (surname) > 0),
  primary key (userid)
);


create table BookInfo
(
  isbn VARCHAR2(13) check (length (isbn) = 10 or length (isbn) = 13),
  title VARCHAR2(100) unique,
  pubYear CHAR(4),
  quantity INTEGER,
  primary key (isbn)
);


create table BorrowedBy
(
  userid INTEGER,
  isbn VARCHAR2(13),
  issueDate DATE default sysdate,
  dayReturned DATE,
  primary key (userid, isbn, issueDate),
  foreign key (userid) references Users (userid) on delete cascade,
  foreign key (isbn) references BookInfo
);


create or replace trigger isbnFormatChange
before insert on BookInfo
for each row
begin
  if (length (:new.isbn) = 10) then
    :new.isbn := substr (:new.isbn, 1, 9);
    :new.isbn := concat (978, :new.isbn);
    :new.isbn := concat (:new.isbn, mod ((10 - mod ((to_number (substr (:new.isbn, 1, 1)) + 3 * to_number (substr (:new.isbn, 2, 1)) + to_number (substr (:new.isbn, 3, 1)) + 3 * to_number (substr (:new.isbn, 4, 1)) + to_number (substr (:new.isbn, 5, 1)) + 3 * to_number (substr (:new.isbn, 6, 1)) + to_number (substr (:new.isbn, 7, 1)) + 3 * to_number (substr (:new.isbn, 8, 1)) + to_number (substr (:new.isbn, 9, 1)) + 3 * to_number (substr (:new.isbn, 10, 1)) + to_number (substr (:new.isbn, 11, 1)) + 3 * to_number (substr (:new.isbn, 12, 1))), 10)), 10));
  end if;
end;
/

insert into Users
values (1, 'e1848324@metu.edu.tr', 'Kudaiar', 'Pirimbaev');

insert into BookInfo
values ('9781449324452', 'OraclePL/SQL Programming', '2014', 2);

insert into BookInfo
values ('9781430263005', 'Expert Oracle Database Architecture', '2015', 1);

insert into BookInfo
values ('9780071494458', 'Oracle Database 11g PL/SQL Programming', '2008', 3);

insert into BookInfo
values ('1449343031', 'a', '2000', 1);

insert into BookInfo
values ('0470277653', 'b', '2000', 1);

insert into BookInfo
values ('1590599683', 'c', '2000', 1);

insert into BookInfo
values ('1484207610', 'd', '2000', 1);

insert into BookInfo
values ('0071496637', 'e', '2000', 1);

insert into BookInfo
values ('0071780262', 'f', '2000', 1);

insert into BookInfo
values ('0071498508', 'g', '2000', 1);

insert into BookInfo
values ('0137142838', 'h', '2000', 1);

insert into BookInfo
values ('1484211138', 'i', '2000', 1);

insert into BookInfo
values ('1430239549', 'j', '2000', 1);
