#include <gtest/gtest.h>

extern "C"{
    #include "../include/vector.h"
}

Circle createCircle(vec3 c, double r){
    return {c, r};
}

::testing::AssertionResult CStructEqual(const vec3& a, const vec3& b) {
     if(a.x != b.x && a.y != b.y && a.z != b.z)
        return ::testing::AssertionFailure() << "value mismatch:";
    
    return ::testing::AssertionSuccess();
}

TEST(rayTest, CircleIntersectionPositive) {
    vec3 origin = {0, 0, 0};
    vec3 direction = {0, 0, 1};
    vec3 centre = {0, 0, 10};
    vec3 hit = {0, 0, 9};

    double tExcpected = 9;
    hitRecord h;
    
    isIntersectingCircle(createCircle(centre, 1), {origin, direction}, &h);
    
    ASSERT_TRUE(CStructEqual(h.hPoint, hit));
    ASSERT_EQ(h.t, tExcpected);
}

TEST(rayTest, CircleIntersectionNegative){
    vec3 origin = {0, 0, 0};
    vec3 direction = {0, 0, -1};
    vec3 centre = {0, 0, 10};
    vec3 hit = {0, 0, 9};

    double tExcpected = -9;
    hitRecord h;
    
    isIntersectingCircle(createCircle(centre, 1), {origin, direction}, &h);

    EXPECT_TRUE(CStructEqual(h.hPoint, hit));
    ASSERT_EQ(h.t, tExcpected);
}


TEST(rayTest, CircleNoIntersection){
    vec3 origin = {0, 0, 0};
    vec3 direction = {1, 10, 0};
    vec3 centre = {0, 0, 10};
    vec3 hit = {0, 0, 9};

    double tExcpected = 9;
    hitRecord h;

    int result = isIntersectingCircle(createCircle(centre, 1), {origin, direction}, &h);
    ASSERT_EQ(result, -1);
}

TEST(rayTest, CircleInsideIntersection){
    vec3 origin = {0, 0, 0};
    vec3 direction = {0, 0, 1};
    vec3 centre = {0, 0, 0};
    vec3 hit = {0, 0, 9};

    double tExcpected = 9;

    hitRecord h;
    isIntersectingCircle(createCircle(centre, 10), {origin, direction}, &h);
    printf("%f %f %f", h.hPoint.x, h.hPoint.y, h.hPoint.z);
}