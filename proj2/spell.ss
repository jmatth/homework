; vim: set st=2 sw=2 expandtab:
; *********************************************
; *  314 Principles of Programming Languages  *
; *  Spring 2013                              *
; *  Student Version                          *
; *********************************************

;; contains "ctv", "A", and "reduce" definitions
(load "include.ss")

;; contains simple dictionary definition
(load "test-dictionary.ss")

;; -----------------------------------------------------
;; HELPER FUNCTIONS

;; *** CODE FOR ANY HELPER FUNCTION GOES HERE ***

(define build_dict_vectors
  (lambda (hashes dict)
    (if (null? hashes) '()
      (cons (build_single_dict_vector (car hashes) dict) (build_dict_vectors (cdr hashes) dict)))))

(define build_single_dict_vector
  (lambda (hash dict)
    (if (null? dict) '()
      (cons (hash (car dict)) (build_single_dict_vector hash (cdr dict))))))

(define build_word_vector
  (lambda (hashes word)
    (if (null? hashes) '()
      (cons ((car hashes) word) (build_word_vector (cdr hashes) word)))
))

(define check_spelling
  (lambda (dict_vectors word_vector)
    (if (null? word_vector) #t
      (if (check_word_hash (car dict_vectors) (car word_vector))
        (check_spelling (cdr dict_vectors) (cdr word_vector)) #f))))

(define check_word_hash
  (lambda (dict_vector word_hash)
    (if (null? dict_vector) #f
      (if (= (car dict_vector) word_hash) #t
        (check_word_hash (cdr dict_vector) word_hash)))))


;; -----------------------------------------------------
;; KEY FUNCTION

(define key
  (lambda (w)
    (foldl (lambda (char int) (+ (* 33 int) (ctv char))) 5381 w)))

;; -----------------------------------------------------
;; HASH FUNCTION GENERATORS

;; value of parameter "size" should be a prime number
(define gen-hash-division-method
  (lambda (size) ;; range of values: 0..size-1
    (lambda (k)
      (modulo (key k) size))
))

;; value of parameter "size" is not critical
;; Note: hash functions may return integer values in "real"
;;       format, e.g., 17.0 for 17

(define gen-hash-multiplication-method
  (lambda (size) ;; range of values: 0..size-1
    (lambda (k)
      (floor (* size (- (* (key k) A) (floor (* (key k) A))))))
))


;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTIONS AND HASH FUNCTION LISTS

(define hash-1 (gen-hash-division-method 701))
(define hash-2 (gen-hash-division-method 899))
(define hash-3 (gen-hash-multiplication-method 700))
(define hash-4 (gen-hash-multiplication-method 900))

(define hashfl-1 (list hash-1 hash-2 hash-3 hash-4))
(define hashfl-2 (list hash-1 hash-3))


;; -----------------------------------------------------
;; EXAMPLE HASH VALUES
;;   to test your hash function implementation
;;
;;  (hash-1 '(h e l l o)) ==> 657
;;  (hash-1 '(d a y))     ==> 46
;;  (hash-1 '(c l a s s)) ==> 183
;;
;;  (hash-2 '(h e l l o)) ==> 273
;;  (hash-2 '(d a y))     ==> 218
;;  (hash-2 '(c l a s s)) ==> 254
;;
;;  (hash-3 '(h e l l o)) ==> 595.0
;;  (hash-3 '(d a y))     ==> 546.0
;;  (hash-3 '(c l a s s)) ==> 169.0
;;
;;  (hash-4 '(h e l l o)) ==> 765.0
;;  (hash-4 '(d a y))     ==> 702.0
;;  (hash-4 '(c l a s s)) ==> 217.0


;; -----------------------------------------------------
;; SPELL CHECKER GENERATOR

(define gen-checker
  (lambda (hashfunctionlist dict)
     ((lambda (bitvectors)
        (lambda (word)
          (check_spelling bitvectors (build_word_vector hashfunctionlist word))))
      (build_dict_vectors hashfunctionlist dict))))


;; -----------------------------------------------------
;; EXAMPLE SPELL CHECKERS

(define checker-1 (gen-checker hashfl-1 dictionary))
(define checker-2 (gen-checker hashfl-2 dictionary))

;; EXAMPLE APPLICATIONS OF A SPELL CHECKER
;;
;;  (checker-1 '(a r g g g)) ==> #f
;;  (checker-2 '(h e l l o)) ==> #t

