#include <iostream>
#include "gtest/gtest.h"
#include "test.h"

/**
 * Tests of apply
 */

S a("A");
S b("B");
S c("C");
S x("X");
S y("Y");
S z("Z");

extern S apply(S f, S args);
extern S LAMBDA;
extern S NLAMBDA;

TEST(Apply, Empty) {
    S f = list(LAMBDA, NIL, NIL);
    EXPECT_EQ(apply(f, NIL).eval(), NIL);
}

TEST(Apply, ReturnEmptyList) {
    S f = list(NLAMBDA, list(x, y), list());
    EXPECT_EQ(apply(f, list(a, b)), list());
}

TEST(Apply, ReturnSameAtom) {
    S f = list(NLAMBDA, list(x, y), z.q());
    EXPECT_EQ(apply(f, list(a, b)), z);
}

TEST(Apply, ReturnID) {
    S f = list(NLAMBDA, list(x), x);
    EXPECT_EQ(apply(f, list(a)), a);
}

TEST(Apply, Car3) {
    S f = list(NLAMBDA, list(x, y, z), x);
    EXPECT_EQ(apply(f, list(a, b, c)), a);
}

TEST(Apply, Rac3) {
    S f = list(NLAMBDA, list(x, y, z), z);
    EXPECT_EQ(apply(f, list(a, b, c)), c);
}

TEST(Apply, CarFirst) {
    S f = list(NLAMBDA, list(x, y), list(CAR, x));
    EXPECT_EQ(apply(f, list(list(a,b), c)), a);
}

TEST(Apply, CarLast) {
    S f = list(NLAMBDA, list(x, y), list(CAR, y));
    EXPECT_EQ(apply(f, list(c, list(a,b))), a);
}

TEST(Apply, Swap) {
    S f = list(NLAMBDA, list(x, y), list(CONS, y, x));
    EXPECT_EQ(apply(f, list(a, b)), b.cons(a));
}

TEST(Apply, Set) {
    S f = list(NLAMBDA, list(x, y), list(SET, x, y));
    EXPECT_EQ(apply(f, list(a, b)), b);
}

TEST(Apply, SetAndCar) {
    S f = list(NLAMBDA, list(x, y), list(SET, x.q(), list(CAR, y)));
    EXPECT_EQ(apply(f, list(a, list(b, c))), b);
}


TEST(Apply, LambdaIdCar) {
    S f = list(LAMBDA, list(x), x);
    EXPECT_EQ(apply(f, list(CAR.cons(list(list(a,b).q())))), a);
}

TEST(Apply, LambdaRac3QuoteEval) {
    S f = list(LAMBDA, list(x, y, z), z);
    EXPECT_EQ(apply(f, list(a.q(), b.q(), c.q())), c);
}

TEST(Apply, LambdaRac3CarFirstEval) {
    S f = list(LAMBDA, list(x, y, z), z);
    EXPECT_EQ(apply(f, list(CAR.cons(list(list(a, NIL).q())), b.q(), c.q())), c);
}

TEST(Apply, LambdaRac3CarLastEval) {
    S f = list(LAMBDA, list(x, y, z), z);
    EXPECT_EQ(apply(f, list(a.q(), b.q(), CAR.cons(list(list(c, NIL).q())))), c);
}

TEST(Apply, Mirror) {
    S f = list(NLAMBDA, list(x), list(CONS, list(CDR, x), list(CAR, x)));
    EXPECT_EQ(apply(f, list(a.cons(b))), b.cons(a));
}
