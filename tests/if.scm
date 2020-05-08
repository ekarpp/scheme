(print "\nif.scm\n")

(print "should be 1: " (if (= 1 1) 1 0) "\n")
(print "should be 0: " (if (> 1 2 3) 1 0) "\n")
(print "should be 1: " (if (< 1 2 3) (*) (+)) "\n")
(print "should be 1: " (if (> 3 2 1) (*) (+)) "\n")
(print "should be 1: " (if (= 1 1) (if (< 1 2) 1 0) ()) "\n")
