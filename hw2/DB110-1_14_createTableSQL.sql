--------------------
-- Drop Table Subscriptions;

CREATE TABLE Subscriptions (
  SubscriptionID Varchar(50) Not Null,
  PlanID Varchar(50) Not Null,
  PayerID Varchar(50) Not Null,
  SubscriberID Varchar(50) Not Null,
  CreatedAt Date, 
  EndedAt Date
);

Copy Subscriptions(SubscriptionID, PlanID, PayerID, SubscriberID, CreatedAt, EndedAt) 
From 'E:\projects\110-1-db-management\hw2\subscriptions.csv'
Delimiter ',' 
CSV Header;

Select * 
From Subscriptions;
--------------------
-- Drop Table StateChanges;

CREATE TABLE StateChanges (
  SubscriptionID Varchar(50),
  FromState Varchar(15), 
  ToState Varchar(15),
  CreatedAt Date
);

Copy StateChanges(SubscriptionID, FromState, ToState, CreatedAt) 
From 'E:\projects\110-1-db-management\hw2\state_changes.csv'
Delimiter ',' 
CSV Header;

Select * 
From StateChanges;
--------------------
-- Drop Table SubscriptionPlans;

CREATE TABLE SubscriptionPlans (
  PlanID Varchar(50),
  PlanName Varchar(100)
);

Copy SubscriptionPlans(PlanID, PlanName) 
From 'E:\projects\110-1-db-management\hw2\subscription_plans.csv'
Delimiter ',' 
CSV Header;

Select * 
From SubscriptionPlans;
--------------------
-- Drop Table User_Missions;

CREATE TABLE User_Missions (
  AnswerID Varchar(50),
  UserID Varchar(50),
  MissionID Varchar(50),
  Status Varchar(50), 
  CorrectCnt Numeric, 
  AnsweredCnt Numeric,
  AnswerDuration Numeric,
  StartedAt Date, 
  FinishedAt Date,
  CreatedAt Date
);

Copy User_Missions(AnswerID, UserID, MissionID, Status, CorrectCnt, AnsweredCnt, AnswerDuration, StartedAt, FinishedAt, CreatedAt) 
From 'E:\projects\110-1-db-management\hw2\user_missions.csv'
Delimiter ',' 
CSV Header;

Select * 
From User_Missions;
--------------------
-- Drop Table Answers;

CREATE TABLE Answers (
  AnswerID Varchar(50),
  UserID Varchar(50),
  QuestionID Varchar(50),
  MissionID Varchar(50),
  SectionName Varchar(100),
  BookVolumeName Varchar(100),
  SubjectName Varchar(100),
  PublisherName Varchar(100),
  CourseName Varchar(100),
  IsCorrect Int,
  CostTime Int,
  IsGCAttacted Varchar(50),
  BattleType Varchar(50),
  CreatedAt Date
);

Copy Answers(AnswerID, UserID, QuestionID, MissionID, SectionName, BookVolumeName, SubjectName, 
			 PublisherName, CourseName, IsCorrect, CostTime, IsGCAttacted, BattleType, CreatedAt) 
From 'E:\projects\110-1-db-management\hw2\answers.csv'
Delimiter ',' 
CSV Header;

Select * 
From Answers;
--------------------