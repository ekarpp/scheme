(print "\nlambda.scm\n")

(define f (lambda (x) (+ x)))
(define g (lambda (y) (* y (f y))))

(define v 3)
(g v)
(print "should be " (* v v) ": " (g v) "\n")

(define v 5)
(g v)
(print "should be " (* v v) ": " (g v) "\n")


(define f (lambda (x) (+ x)))
(define g (lambda (x) (* x (f x))))

(define v 10)
(print "should be " (* v v) ": " (g v) "\n")
