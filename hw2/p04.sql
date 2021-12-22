SELECT
	a.UserId,
	a.Year,
	a.Month,
	Count(*) filter (Where a.IsCorrect=1) as TotalAnswer,
	Count(*) as CorrectAnswers
From (
	SELECT UserId, QuestionId, IsCorrect, Extract(Year from createdAt) as Year, Extract(Month from createdAt) as Month
	From Answers
) As a
Group By a.UserId, a.Year, a.Month