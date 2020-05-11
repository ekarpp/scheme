(print "\nlambda.scm\n")

(define f (lambda (x) (+ x)))
(define g (lambda (y) (* y (f y))))

(define v 3)
(print "should be " (* v v) ": " (g v) "\n")

(define v 5)
(print "should be " (* v v) ": " (g v) "\n")


(define f (lambda (x) (+ x)))
(define g (lambda (x) (* x (f x))))

(define v 10)
(print "should be " (* v v) ": " (g v) "\n")

(define q (lambda (x) ((lambda (n) (* n x)) x)))
(print "should be 100: " (q 10) "\n")
(print "should be 400: " (q 20) "\n")
