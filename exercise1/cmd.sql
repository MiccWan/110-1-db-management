-- DROP TABLE EMPLOYEE, PROJECT, WORKS_ON;

-- P1

CREATE TABLE EMPLOYEE(
  Fname VARCHAR(20),
  Minit CHAR(1),
  Lname VARCHAR(20),
  Ssn CHAR(9) PRIMARY KEY,
  Bdate date,
  Address VARCHAR(40),
  Sex CHAR(1),
  Salary integer,
  Super_ssn CHAR(9),
  Dno smallint,
  CONSTRAINT fk_Super_ssn FOREIGN KEY(Super_ssn) REFERENCES EMPLOYEE(Ssn) ON DELETE
  SET
    NULL
);

CREATE TABLE PROJECT(
  Pname CHAR(20),
  Pnumber smallint PRIMARY KEY,
  Plocation VARCHAR(20),
  Dnum smallint
);

CREATE TABLE WORKS_ON(
  Essn CHAR(9),
  Pno smallint,
  Hours decimal,
  CONSTRAINT fk_Pno FOREIGN KEY(Pno) REFERENCES PROJECT(Pnumber) ON DELETE CASCADE
);

COPY EMPLOYEE
FROM
  'D:\projects\110-1-db-management\exercise1\data\Employee_c.csv' DELIMITER ',' CSV HEADER;

COPY PROJECT
FROM
  'D:\projects\110-1-db-management\exercise1\data\Project_c.csv' DELIMITER ',' CSV HEADER;

COPY WORKS_ON
FROM
  'D:\projects\110-1-db-management\exercise1\data\Works_On_c.csv' DELIMITER ',' CSV HEADER;

-- P2

SELECT
  E.Fname,
  E.Lname
FROM
  EMPLOYEE AS E,
  EMPLOYEE AS S
WHERE
  E.Super_ssn = S.Ssn
  AND S.Fname = 'Franklin'
  AND S.Lname = 'Wong';

-- P3

SELECT E.Fname, E.Lname
FROM EMPLOYEE AS E
WHERE E.Ssn IN (
  SELECT Essn
  FROM WORKS_ON
  WHERE Pno IN (
    SELECT Pnumber
    FROM PROJECT
    WHERE Dnum = 5
  )
  EXCEPT(
    SELECT Essn
    FROM WORKS_ON
    WHERE Pno IN (
      SELECT Pnumber
      FROM PROJECT
      WHERE Dnum <> 5
    )
  )
);

-- P4 
-- Get names, employees in department 5, works more than 10 hr on ProductX 

SELECT E.Fname, E.Lname
FROM EMPLOYEE AS E
WHERE E.Ssn IN (
  SELECT W.Essn
  FROM WORKS_ON AS W, PROJECT AS P
  WHERE P.Pname = 'ProductX'
    AND P.Pnumber = W.Pno
    AND W.Hours > 10
);