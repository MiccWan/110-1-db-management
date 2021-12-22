SELECT a.MissionID, a.TotalCount, b.CompletedCount, b.CompletedCount::numeric / a.TotalCount::numeric as Rate
From
	(
	  SELECT u.MissionId, COUNT(*) as TotalCount
	  From User_Missions As u
	  Group By u.MissionID
	) As a 
Join
	(
	  SELECT u.MissionId,  COUNT(*) as CompletedCount
	  From User_Missions As u
	  Where u.Status = '已完成'
	  Group By u.MissionID
	) As b 
On a.MissionId = b.MissionId

---------------------------------------

select
  MissionId,
  count(*) filter (where status='已完成') as CompletedCount, -- debug
  count(*) as TotalCount, -- debug
  count(*) filter (where status='已完成')::float / count(*) as complete_rate
from user_missions
group by MissionId