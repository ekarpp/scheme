(print "\nrecursion.scm\n")

(define fact
  (lambda (n)
    (if (= n 1)
        n
        (* n (fact (- n 1))))))

(define f
  (lambda (n prod)
    (if (= n 1)
        prod
        (f (- n 1) (* n prod)))))
(define fac
  (lambda (n) (f n 1)))

(print "should be 120: " (fact 5) "\n")
(print "should be 720: " (fact 6) "\n")
