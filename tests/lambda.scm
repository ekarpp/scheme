(print "\nlambda.scm\n")

(define f (lambda (x) (+ x)))
(define g (lambda (y) (* y (f y))))

(define out (lambda (v) (print "should be " (* v v) ": " (g v) "\n")))

(define v 5)
(out v)

(define v 3)
(out v)

(define f (lambda (x) (+ x)))
(define g (lambda (x) (* x (f x))))

(define v 10)
(out v)
