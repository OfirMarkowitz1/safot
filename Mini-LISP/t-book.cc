#include <string.h>
#include <gtest/gtest.h>
#include "test.h"
/**
 * Mini-lisp book - full testing
 */

extern S a;
extern S b;
extern S c;
extern S x;
extern S y;
extern S z;
extern S LAMBDA;
extern S NLAMBDA;

/*
    (atom nil) ⇒ T
    (atom t) ⇒ T
    (atom '(a a)) ⇒ NIL
    (atom 'a) ⇒ T
*/
TEST(Book, AtomicFunctionsAtom) {
    EXPECT_EQ(parse("(atom nil)").eval(), T);
    EXPECT_EQ(parse("(atom t)").eval(), T);
    EXPECT_EQ(parse("(atom '(a a))").eval(), NIL);
    EXPECT_EQ(parse("(atom 'a)").eval(), T);
}

/*
    (car '(b.a)) ⇒ B.A
    (car '(b a)) ⇒ B
    (car '(a)) ⇒ A
    (car 'a) ⇒ ✗
    (car ()) ⇒ ✗
*/
TEST(Book, AtomicFunctionsCar) {
    EXPECT_EQ(parse("(car '(b.a))").eval(), b.cons(a));
    EXPECT_EQ(parse("(car '(b a))").eval(), b);
    EXPECT_EQ(parse("(car '(a))").eval(), a);
    EXPECT_EXCEPTION(parse("(car 'a)").eval(), a, CAR);
    EXPECT_EXCEPTION(parse("(car ())").eval(), NIL, CAR);
}

/*
    (cdr '(a.b)) ⇒ NIL
    (cdr '(a b)) ⇒ (b)
    (cdr '(b)) ⇒ NIL
    (cdr t) ⇒ ✗
    (cdr ()) ⇒ ✗
    (cdr nil) ⇒ ✗
*/
TEST(Book, AtomicFunctionsCdr) {
    EXPECT_EQ(parse("(cdr '(a.b))").eval(), NIL);
    EXPECT_EQ(parse("(cdr '(a b))").eval(), list(b));
    EXPECT_EQ(parse("(cdr '(b))").eval(), NIL);
    EXPECT_EXCEPTION(parse("(cdr t)").eval(), T, CDR);
    EXPECT_EXCEPTION(parse("(cdr ())").eval(), NIL, CDR);
    EXPECT_EXCEPTION(parse("(cdr nil)").eval(), NIL, CDR);
}

/*
    (cond (t 'A)) ⇒ A
    (cond (nil 'A) (t 'B)) ⇒ B
    (cond (nil 'A) (t 'B) (t 'C)) ⇒ B
    (cond (nil 'A) (nil 'B) (nil 'C)) ⇒ nil
    (cond) ⇒ nil
*/
TEST(Book, AtomicFunctionsCond) {
    EXPECT_EQ(parse("(cond (t 'A))").eval(), a);
    EXPECT_EQ(parse("(cond (nil 'A) (t 'B))").eval(), b);
    EXPECT_EQ(parse("(cond (nil 'A) (t 'B) (t 'C))").eval(), b);
    EXPECT_EQ(parse("(cond (nil 'A) (nil 'B) (nil 'C))").eval(), NIL);
    EXPECT_EQ(parse("(cond)").eval(), NIL);
}

/*
    (cons 'a '(b c)) ⇒ (A B C)
    (cons 'b nil) ⇒ (b)
    (cons 'a 'b) ⇒ (A.B)
*/
TEST(Book, AtomicFunctionsCons) {
    EXPECT_EQ(parse("(cons 'a '(b c))").eval(), list(a, b, c));
    EXPECT_EQ(parse("(cons 'b nil)").eval(), list(b));
    EXPECT_EQ(parse("(cons 'a 'b)").eval(), a.cons(b));
}

/*
    (eq t t) ⇒ T
    (eq t nil) ⇒ NIL
    (eq nil nil) ⇒ T
    (eq 'a 'a) ⇒ T
    (eq '(a a) '(a a)) ⇒ NIL
*/
TEST(Book, AtomicFunctionsEq) {
    EXPECT_EQ(parse("(eq t t)").eval(), T);
    EXPECT_EQ(parse("(eq t nil)").eval(), NIL);
    EXPECT_EQ(parse("(eq nil nil)").eval(), T);
    EXPECT_EQ(parse("(eq 'a 'a)").eval(), T);
    EXPECT_EQ(parse("(eq '(a a) '(a a))").eval(), NIL);
}

/*
    (error) ⇒ ✗
    (error A) ⇒ ✗
    (error 'my−error 'message) ⇒ ✗
*/
TEST(Book, AtomicFunctionsError) {
    S err = S("ERROR");
    EXPECT_EXCEPTION(parse("(error)").eval(), list(err), NIL);
    EXPECT_EXCEPTION(parse("(error A)").eval(), list(err, a), list(a));
    EXPECT_EXCEPTION(parse("(error 'my 'message)").eval(), list(err, S("MY").q(), S("MESSAGE").q()), list(S("MY").q(), S("MESSAGE").q()));
}

/*
    (set 'a '(b c)) ⇒ (b c)
    (set 'b nil) ⇒ NIL
*/
TEST(Book, AtomicFunctionsSet) {
    EXPECT_EQ(parse("(set 'a '(b c))").eval(), list(b, c));
    EXPECT_EQ(parse("(set 'b nil)").eval(), NIL);
}

/*
    ((lambda (x) (car (cdr x))) '(a b) ⇒ B
*/
TEST(Book, BuiltInFunctionsLambda) {
    EXPECT_EQ(parse("((lambda (x) (car (cdr x))) '(a b))").eval(), b);
}

/*
    nil ⇒ NIL
    () ⇒ NIL
    (eq nil t) ⇒ NIL
*/
TEST(Book, BuiltInFunctionsNil) {
    EXPECT_EQ(parse("nil").eval(), NIL);
    EXPECT_EQ(parse("()").eval(), NIL);
    EXPECT_EQ(parse("(eq nil t)").eval(), NIL);
}

/*
    (null t) ⇒ NIL
    (null nil) ⇒ T
    (null 'a) ⇒ NIL
    (null '(a a)) ⇒ NIL
*/
TEST(Book, BuiltInFunctionsNull) {
    EXPECT_EQ(parse("(null t)").eval(), NIL);
    EXPECT_EQ(parse("(null nil)").eval(), T);
    EXPECT_EQ(parse("(null 'a)").eval(), NIL);
    EXPECT_EQ(parse("(null '(a a))").eval(), NIL);
}

/*
    (ndefun f (x y) (y x)) ⇒ f
*/
TEST(Book, BuiltInFunctionsNdefun) {
    EXPECT_EQ(parse("(ndefun f (x y) (y x))").eval(), S("F"));
}

/*
    (nlambda (x y) (y x)) ⇒ (nlambda (x y) (y x))
*/
TEST(Book, BuiltInFunctionsNlambda) {
    EXPECT_EQ(parse("(nlambda (x y) (y x))").eval(), list(NLAMBDA, list(x,y), list(y,x)));
}

/*
    (quote a) ⇒ A
    (quote (b c)) ⇒ (B C)
    'a ⇒ A
    '(b c) ⇒ (B C)
*/
TEST(Book, BuiltInFunctionsQuote) {
    EXPECT_EQ(parse("(quote a)").eval(), a);
    EXPECT_EQ(parse("(quote (b c))").eval(), list(b, c));
    EXPECT_EQ(parse("'a").eval(), a);
    EXPECT_EQ(parse("'(b c)").eval(), list(b, c));
}

/*
    t ⇒ T
    (eq nil nil) ⇒ T
*/
TEST(Book, BuiltInFunctionsT) {
    EXPECT_EQ(parse("t").eval(), T);
    EXPECT_EQ(parse("(eq nil nil)").eval(), T);
}

/* Zcar */
TEST(Book, Zcar) {
    parse("(defun zcar(x) (cond ((atom x) x) (t (car x))))").eval();
    EXPECT_EQ(parse("(zcar 'a)").eval(), a);
    EXPECT_EQ(parse("(zcar '(a b))").eval(), a);
    EXPECT_EXCEPTION(parse("(zcar)").eval(), NIL, MISSING);
}

/* Mirror */
TEST(Book, Mirror) {
    parse("(defun mirror (x) (cons (cdr x) (car x)))").eval();
    EXPECT_EQ(parse("(mirror '(a b))").eval(), list(b).cons(a));
    EXPECT_EXCEPTION(parse("(mirror 'a)").eval(), a, CDR); // Failed in (cdr x)
    EXPECT_EXCEPTION(parse("(mirror)").eval(), NIL, MISSING);
}

/* Eval on eval */
TEST(Book, EvalOnEval) {
    EXPECT_EQ(parse("(eval (eval T))").eval(), T);
    EXPECT_EQ(parse("(eval (atom (eval (atom 'a))))").eval(), T);
    EXPECT_EQ(parse("(eval (atom (eval (cdr '(a)))))").eval(), T);
    EXPECT_EQ(parse("(eval (null (eval (cdr '(a)))))").eval(), T);
    EXPECT_EQ(parse("(eval (atom (eval (car '(b a)))))").eval(), T);
    EXPECT_EQ(parse("(eval (eval (eval (eval T))))").eval(), T);
    EXPECT_EQ(parse("(eval (eval (eval (eval NIL))))").eval(), NIL);
    EXPECT_EXCEPTION(parse("(eval (atom (eval (car 'a))))").eval(), a, CAR);
    EXPECT_EXCEPTION(parse("(eval atom (atom (eval (car 'a))))").eval(),
                     list(S("EVAL"), S("ATOM"), list(S("ATOM"), list(S("EVAL"), list(S("CAR"), list(S("QUOTE"), a))))),
                     REDUNDANT);
}
