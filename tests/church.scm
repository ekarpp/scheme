#|
does not work because lambda does not
save the environment it was evaluated in
|#

(print "\nchurch.scm\n")

(define zero
  (lambda (f)
    (lambda (x) x)))

;; increment church numeral by one
(define cn-incr
  (lambda (cn)
    (lambda (f)
      (lambda (x)
        (f ((cn f) x))))))
;; return church numeral equal to n
(define cn-n
  (lambda (n)
    (if (= n 0)
        zero
        (cn-incr (cn-n (- n 1))))))

;; map church numeral to int
(define cn-to-int
  (lambda (cn)
    ((cn (lambda (n) (+ 1 n))) 0)))

;; add two church numerals
(define cn-add
  (lambda (cn1 cn2)
    (lambda (f)
      (lambda (x) ((cn1 f) ((cn2 f) x))))))

;; multiply two church numerals
(define cn-prod
  (lambda (cn1 cn2)
    (lambda (f)
      (lambda (x) ((cn1 (cn2 f)) x)))))

(print "should be 0: " (cn-to-int zero) "\n")

(print "should be 1: " (cn-to-int (cn-incr zero)) "\n")

(define v1 10)
(print "should be " v1 ": " (cn-to-int (cn-n v1)) "\n")

(define v1 12)
(define v2 18)
(print "should be " (+ v1 v2) ": " (cn-to-int (cn-add (cn-n v1) (cn-n v2))) "\n")

(define v1 5)
(define v2 3)
(print "should be " (* v1 v2) ": " (cn-to-int (cn-prod (cn-n v1) (cn-n v2))) "\n")

(define v1 3)
(define v2 4)
(define v3 2)
(define v4 5)
(print "should be " (* (+ v1 v2) (* v3 v4)) ": "
       (cn-to-int (cn-prod (cn-add (cn-n v1) (cn-n v2)) (cn-prod (cn-n v3) (cn-n v4)))) "\n")

;; very slow
;;(define v1 10000)
;;(print "should be " v1 ": " (cn-to-int (cn-n v1)) "\n")
