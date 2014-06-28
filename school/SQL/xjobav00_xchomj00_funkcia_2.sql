
CREATE OR REPLACE FUNCTION calc_func RETURN NUMBER IS 
v_max_facturs number;
BEGIN

SELECT ID_Pracovnik into v_max_facturs
FROM (
   SELECT p.ID_Pracovnik  , COUNT(*)
   FROM Pracovnik p JOIN faktura f ON f.ID_Pracovnik=p.ID_Pracovnik
   GROUP BY p.ID_Pracovnik
   )
WHERE ROWNUM=1;

return v_max_facturs;


END calc_func;
/
------------------------------------