#include <gtest/gtest.h>
#include <unordered_map>
#include "uint128_t.h"

using namespace std;

TEST(Hash, BASE){
    unordered_map<uint128_t, string> map;
    uint128_t a = 1000000ULL << 32;
    map.insert(make_pair(a, "hello"));
    map.insert(make_pair(1, "bye"));
    ASSERT_EQ(map[a], "hello");
}