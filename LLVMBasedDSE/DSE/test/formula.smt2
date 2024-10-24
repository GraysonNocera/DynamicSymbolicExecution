; 
(set-info :status unknown)
(declare-fun X0 () Int)
(declare-fun X1 () Int)
(assert
 (let (($x12 (< X0 0)))
 (= $x12 true)))
(assert
 (let (($x14 (= X1 1024)))
 (= $x14 false)))
(check-sat)
