DROP PROCEDURE IF EXISTS origin_db.update_exp_result;

DELIMITER ;;

CREATE PROCEDURE origin_db.update_exp_result()
BEGIN
SET SQL_SAFE_UPDATES = 0;
SET @task_per_exp :=18;

-- Update current experiment result and finish time
-- exp. finish_time = latest execute task finish time 
UPDATE origin_db.exe_rs rs
INNER JOIN (
	SELECT 
 		FLOOR((t.task_id -1)/ @task_per_exp + 1) AS exp_id, 
		COUNT(t.task_id) AS total,
		SUM( IF (t.cur_status = 'Succedded' ,1,0)) AS succedded,
		SUM( IF (t.cur_status = 'Created' ,1,0)) AS expired,
		SUM( IF (t.cur_status IN('Canceled','Error' , 'Running', 'ToReRun'),1,0)) AS failed,
        MAX(finish_time) AS t
	FROM origin_db.tasks t WHERE t.task_type = 'ExecuteTask'
 	GROUP BY exp_id
	) tmp ON rs.exp_id = tmp.exp_id
SET rs.total = tmp.total, 
	rs.succedded = tmp.succedded,
	rs.expired = tmp.expired,
	rs.failed = tmp.failed,
    rs.finish_time = t;

	UPDATE origin_db.exe_rs rs
	INNER JOIN (
		SELECT 
			FLOOR((t.task_id -1)/ @task_per_exp + 1) AS exp_id, 
			MAX(finish_time) AS st
		FROM origin_db.tasks t WHERE t.task_type = 'Charging'
		GROUP BY exp_id
		) tmp ON rs.exp_id = tmp.exp_id
	SET rs.start_time = tmp.st;

 	UPDATE origin_db.exe_rs rs
  SET duration = TIMEDIFF(finish_time,start_time);
    
	SELECT *  FROM origin_db.exe_rs;
END ;;

DELIMITER ;
-- CALL exp_db.update_exp_result();
-- SELECT * FROM exp_db.exe_rs;