SELECT a.UserID, a.TotalCount, b.CompletedCount, b.CompletedCount::numeric / a.TotalCount::numeric as Rate
From
	(
	  SELECT u.UserID, COUNT(*) as TotalCount
	  From User_Missions As u
	  Group By u.UserID
	) As a 
Join
	(
	  SELECT u.UserID,  COUNT(*) as CompletedCount
	  From User_Missions As u
	  Where u.Status = '已完成'
	  Group By u.UserID
	) As b 
On a.UserID = b.UserID
Order By Rate