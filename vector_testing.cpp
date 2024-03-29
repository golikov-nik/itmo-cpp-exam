#include <gtest/gtest.h>
#include <numeric>
#include "fault_injection.h"
#include "counted.h"
#include "vector.h"

typedef vector<counted> container;
typedef vector<int> container_int;

TEST(correctness, default_ctor)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        EXPECT_EQ(0u, c.size());
        EXPECT_TRUE(c.empty());
    });
}

TEST(correctness, push_back_simple)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(1u, c.size());
        c.push_back(8);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(2u, c.size());
        c.push_back(15);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(3u, c.size());
        c.push_back(16);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(4u, c.size());
        c.push_back(23);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(5u, c.size());
        c.push_back(42);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(6u, c.size());
    });
}

TEST(correctness, push_back_big)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        
        for (size_t i = 0; i != 20; ++i)
        {
            int val = (i + 42) % 13;
            c.push_back(val);
        }

        for (size_t i = 0; i != 20; ++i)
        {
            int val = (i + 42) % 13;
            EXPECT_EQ(val, c[i]);
        }
    });
}

TEST(correctness, pop_back)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(5u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(4u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(3u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(2u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(1u, c.size());
        c.pop_back();
        EXPECT_TRUE(c.empty());
        EXPECT_EQ(0u, c.size());
    });
}

TEST(correctness, copy_ctor)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container d = c;
        d[2] = 10;
        EXPECT_EQ(3, c[2]);

        d.push_back(4);
        EXPECT_EQ(3u, c.size());
        EXPECT_EQ(4u, d.size());
    });
}

TEST(correctness, copy_ctor_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        container d = c;
    });
}

TEST(correctness, assignment)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container d;
        d.push_back(4);
        d.push_back(5);

        d = c;
        EXPECT_EQ(3u, d.size());
        EXPECT_EQ(1, d[0]);
        EXPECT_EQ(2, d[1]);
        EXPECT_EQ(3, d[2]);
    });
}

TEST(correctness, assignment_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        container d;
        d = c;
    });
}

TEST(correctness, assignment_empty_2)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        container d;
        d.push_back(1);
        d.push_back(2);
        d.push_back(3);
        d = c;
    });
}

TEST(correctness, assignment_self)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c = c;
    });
}

TEST(correctness, assignment_self_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c = c;
    });
}

TEST(correctness, subscript)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, const_subscript)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        container const& cc = c;
        EXPECT_EQ(4, cc[0]);
        EXPECT_EQ(8, cc[1]);
        EXPECT_EQ(15, cc[2]);
        EXPECT_EQ(16, cc[3]);
        EXPECT_EQ(23, cc[4]);
        EXPECT_EQ(42, cc[5]);
    });
}

TEST(correctness, insert)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.insert(c.begin(), 15);
        c.insert(c.begin() + 1, 42);
        c.insert(c.begin() + 1, 16);
        c.insert(c.begin() + 2, 23);
        c.insert(c.begin(), 4);
        c.insert(c.begin() + 1, 8);

        EXPECT_EQ(6u, c.size());
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, erase)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin() + 2);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(16, c[2]);
        EXPECT_EQ(23, c[3]);
        EXPECT_EQ(42, c[4]);
    });
}

TEST(correctness, erase_begin)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin());
        EXPECT_EQ(8, c[0]);
        EXPECT_EQ(15, c[1]);
        EXPECT_EQ(16, c[2]);
        EXPECT_EQ(23, c[3]);
        EXPECT_EQ(42, c[4]);
    });
}

TEST(correctness, erase_end)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.end() - 1);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
    });
}

TEST(correctness, erase_range_begin)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin(), c.begin() + 2);
        EXPECT_EQ(15, c[0]);
        EXPECT_EQ(16, c[1]);
        EXPECT_EQ(23, c[2]);
        EXPECT_EQ(42, c[3]);
    });
}

TEST(correctness, erase_range_middle)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin() + 2, c.end() - 1);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(42, c[2]);
    });
}

TEST(correctness, erase_range_end)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin() + 3, c.end());
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
    });
}

TEST(correctness, erase_range_all)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(c.begin(), c.end());
        EXPECT_TRUE(c.empty());
    });
}

TEST(correctness, reserve)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.reserve(10);
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, reserve_2)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        c.reserve(100);

        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, front_back)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        EXPECT_EQ(1, c.front());
        EXPECT_EQ(3, c.back());
    });
}

TEST(correctness, const_front_back)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container const& cc = c;
        EXPECT_EQ(1, cc.front());
        EXPECT_EQ(3, cc.back());
    });
}

TEST(correctness, front_back_ref)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        EXPECT_EQ(&c.front(), &c[0]);
        EXPECT_EQ(&c.back(), &c[2]);
        c.front() = 10;
        c.back() = 20;
        EXPECT_EQ(10, c[0]);
        EXPECT_EQ(20, c[2]);
    });
}

TEST(correctness, const_front_back_ref)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        
        container const& cc = c;
        EXPECT_EQ(&c.front(), &cc[0]);
        EXPECT_EQ(&c.back(), &cc[2]);
    });
}

TEST(correctness, data_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.data();
        
        container const& cc = c;
        cc.data();
    });
}

TEST(correctness, data_small)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(42);
        EXPECT_EQ(&c[0], c.data());
        
        container const& cc = c;
        EXPECT_EQ(&cc[0], cc.data());
    });
}

TEST(correctness, data)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        EXPECT_EQ(&c[0], c.data());
        
        container const& cc = c;
        EXPECT_EQ(&cc[0], cc.data());
    });
}

TEST(correctness, push_back_element_of_itself)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(0);
        c.push_back(1);
        
        for (size_t i = 0; i != 20; ++i)
            c.push_back(*(c.end() - 2));

        for (size_t i = 0; i != 22; ++i)
            EXPECT_EQ((int)i % 2, c[i]);
    });
}


TEST(correctness, push_back_element_of_itself_single)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(42);
        c.push_back(c.front());
        c.push_back(c.front());
        c.push_back(c.front());
        c.push_back(c.front());
        
        for (size_t i = 0; i != c.size(); ++i)
            EXPECT_EQ(42, c[i]);
    });
}

TEST(correctness, insert_element_of_itself_1)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);
        c.insert(c.begin() + 2, c[2]);

        EXPECT_EQ(6u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(2, c[1]);
        EXPECT_EQ(3, c[2]);
        EXPECT_EQ(3, c[3]);
        EXPECT_EQ(4, c[4]);
        EXPECT_EQ(5, c[5]);
    });
}

TEST(correctness, insert_element_of_itself_2)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);
        c.insert(c.begin() + 3, c[2]);

        EXPECT_EQ(6u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(2, c[1]);
        EXPECT_EQ(3, c[2]);
        EXPECT_EQ(3, c[3]);
        EXPECT_EQ(4, c[4]);
        EXPECT_EQ(5, c[5]);
    });
}

TEST(correctness, insert_element_of_itself_single)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(42);
        c.insert(c.begin(), c[0]);

        EXPECT_EQ(2u, c.size());
        for (size_t i = 0; i != 2; ++i)
            EXPECT_EQ(42, c[i]);
    });
}

TEST(correctness, iterators)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);

        container::iterator i = c.begin();
        EXPECT_NE(c.end(), i);
        EXPECT_EQ(1, *i);
        ++i;
        EXPECT_NE(c.end(), i);
        EXPECT_EQ(2, *i);
        ++i;
        EXPECT_NE(c.end(), i);
        EXPECT_EQ(3, *i);
        ++i;
        EXPECT_NE(c.end(), i);
        EXPECT_EQ(4, *i);
        ++i;
        EXPECT_NE(c.end(), i);
        EXPECT_EQ(5, *i);
        ++i;
        EXPECT_EQ(c.end(), i);
    });
}

TEST(correctness, const_iterators)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);

        container const& cc = c;
        container::const_iterator i = cc.begin();
        EXPECT_NE(cc.end(), i);
        EXPECT_EQ(1, *i);
        ++i;
        EXPECT_NE(cc.end(), i);
        EXPECT_EQ(2, *i);
        ++i;
        EXPECT_NE(cc.end(), i);
        EXPECT_EQ(3, *i);
        ++i;
        EXPECT_NE(cc.end(), i);
        EXPECT_EQ(4, *i);
        ++i;
        EXPECT_NE(cc.end(), i);
        EXPECT_EQ(5, *i);
        ++i;
        EXPECT_EQ(cc.end(), i);
    });
}

TEST(correctness, reverse_iterators)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);

        container::reverse_iterator i = c.rbegin();
        EXPECT_NE(c.rend(), i);
        EXPECT_EQ(5, *i);
        ++i;
        EXPECT_NE(c.rend(), i);
        EXPECT_EQ(4, *i);
        ++i;
        EXPECT_NE(c.rend(), i);
        EXPECT_EQ(3, *i);
        ++i;
        EXPECT_NE(c.rend(), i);
        EXPECT_EQ(2, *i);
        ++i;
        EXPECT_NE(c.rend(), i);
        EXPECT_EQ(1, *i);
        ++i;
        EXPECT_EQ(c.rend(), i);
    });
}

TEST(correctness, const_reverse_iterators)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c.push_back(4);
        c.push_back(5);

        container const& cc = c;
        container::const_reverse_iterator i = cc.rbegin();
        EXPECT_NE(cc.rend(), i);
        EXPECT_EQ(5, *i);
        ++i;
        EXPECT_NE(cc.rend(), i);
        EXPECT_EQ(4, *i);
        ++i;
        EXPECT_NE(cc.rend(), i);
        EXPECT_EQ(3, *i);
        ++i;
        EXPECT_NE(cc.rend(), i);
        EXPECT_EQ(2, *i);
        ++i;
        EXPECT_NE(cc.rend(), i);
        EXPECT_EQ(1, *i);
        ++i;
        EXPECT_EQ(cc.rend(), i);
    });
}

TEST(correctness, comparison_empty_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        EXPECT_TRUE(c == c2);
        EXPECT_FALSE(c != c2);
        EXPECT_TRUE(c <= c2);
        EXPECT_FALSE(c < c2);
        EXPECT_TRUE(c >= c2);
        EXPECT_FALSE(c > c2);
    });
}

TEST(correctness, comparison_empty_non_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        c2.push_back(1);
        EXPECT_FALSE(c == c2);
        EXPECT_TRUE(c != c2);
        EXPECT_TRUE(c <= c2);
        EXPECT_TRUE(c < c2);
        EXPECT_FALSE(c >= c2);
        EXPECT_FALSE(c > c2);
    });
}

TEST(correctness, comparison)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        c.push_back(1);
        c2.push_back(2);
        EXPECT_FALSE(c == c2);
        EXPECT_TRUE(c != c2);
        EXPECT_TRUE(c <= c2);
        EXPECT_TRUE(c < c2);
        EXPECT_FALSE(c >= c2);
        EXPECT_FALSE(c > c2);
    });
}

TEST(correctness, comparison_long)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        c.push_back(5);
        c.push_back(5);
        c.push_back(1);
        c.push_back(5);
        c2.push_back(5);
        c2.push_back(5);
        c2.push_back(2);
        EXPECT_FALSE(c == c2);
        EXPECT_TRUE(c != c2);
        EXPECT_TRUE(c <= c2);
        EXPECT_TRUE(c < c2);
        EXPECT_FALSE(c >= c2);
        EXPECT_FALSE(c > c2);
    });
}

TEST(correctness, swap_empty_self)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        swap(c, c);
    });
}

TEST(correctness, swap_single_self)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        swap(c, c);
    });
}

TEST(correctness, swap_self)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        swap(c, c);
    });
}

TEST(correctness, swap_empty_single)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        c2.push_back(1);
        swap(c, c2);
        EXPECT_EQ(1u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(0u, c2.size());
    });
}

TEST(correctness, swap_empty_non_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        c2.push_back(1);
        c2.push_back(2);
        c2.push_back(3);
        swap(c, c2);
        EXPECT_EQ(3u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(2, c[1]);
        EXPECT_EQ(3, c[2]);
        EXPECT_EQ(0u, c2.size());
    });
}

TEST(correctness, swap_single_non_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        c.push_back(4);
        c2.push_back(1);
        c2.push_back(2);
        c2.push_back(3);
        swap(c, c2);
        EXPECT_EQ(3u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(2, c[1]);
        EXPECT_EQ(3, c[2]);
        EXPECT_EQ(1u, c2.size());
        EXPECT_EQ(4, c2[0]);
    });
}

TEST(correctness, swap_non_empty_non_empty)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c, c2;
        c.push_back(4);
        c.push_back(5);
        c2.push_back(1);
        c2.push_back(2);
        c2.push_back(3);
        swap(c, c2);
        EXPECT_EQ(3u, c.size());
        EXPECT_EQ(1, c[0]);
        EXPECT_EQ(2, c[1]);
        EXPECT_EQ(3, c[2]);
        EXPECT_EQ(2u, c2.size());
        EXPECT_EQ(4, c2[0]);
        EXPECT_EQ(5, c2[1]);
    });
}

TEST(exceptions, nothrow_default_ctor)
{
    faulty_run([]
    {
        EXPECT_NO_THROW(
        {
            counted::no_new_instances_guard g;
            container c;
        });
    });
}

TEST(exceptions, nothrow_subscript)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        EXPECT_NO_THROW(
        {
            container const& cc = c;
            cc[0];
        });
    });
}

TEST(exceptions, reserve)
{
    faulty_run([]
    {
        counted::no_new_instances_guard g;
        container_int c;
        c.reserve(10);

        EXPECT_NO_THROW(
        {
            for (size_t i = 0; i != 10; ++i)
                c.push_back(42);
        });
    });
}

TEST(my_tests, max_align_t) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    using dtype = max_align_t;
    vector<dtype> v;
    EXPECT_EQ(v.size(), 0u);
    for (size_t i = 0; i != 100; ++i) {
      v.push_back(dtype());
      EXPECT_EQ(v.size(), i + 1);
    }
  });
}

TEST(my_tests, count_value_constructor) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    container c(10, 42);
    ASSERT_EQ(c.size(), 10u);
    for (size_t i = 0; i != 10; ++i) {
      ASSERT_EQ(c[i], 42);
    }
    container d(0, 1);
    container e(1, 5);
    container_int ci(11);
    ASSERT_EQ(ci.size(), 11u);
    for (auto const& x : ci) {
      ASSERT_EQ(x, 0);
    }
    std::string s = "hello";
    vector<std::string> strs(10, s);
    for (auto const& x : strs) {
      ASSERT_EQ(x, s);
    }
  });
}

TEST(my_tests, range_constructor) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 0);
    container c(v.begin() + 3, v.begin() + 6);
    ASSERT_EQ(c.size(), 3u);
    for (size_t i = 0; i != 3; ++i) {
      ASSERT_EQ(c[i], i + 3);
    }
    vector d(c.begin(), c.end());
    ASSERT_EQ(d.size(), 3u);
    for (size_t i = 0; i != 3; ++i) {
      ASSERT_EQ(d[i], i + 3);
    }
    vector e(d.begin(), d.begin());
    ASSERT_EQ(e.size(), 0u);
  });
}

TEST(my_tests, assign) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    container c(5, 0);
    container d;
    d.assign(c.begin(), c.end());
    ASSERT_EQ(d.size(), 5u);
    for (size_t i = 0; i != 5; ++i) {
      ASSERT_EQ(d[i], c[i]);
    }
    d.assign(d.begin(), d.begin() + 3);
    for (size_t i = 0; i != 3; ++i) {
      ASSERT_EQ(d[i], 0);
    }
  });
}

TEST(my_tests, move_constructor) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    container c;
    c.push_back(1);
    c.push_back(2);
    c.push_back(3);

    container d = std::move(c);
    d[2] = 10;
    EXPECT_EQ(10, d[2]);

    d.push_back(4);
    EXPECT_EQ(4u, d.size());
  });
}

TEST(my_tests, clear) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    container c;
    c.clear();
    ASSERT_EQ(c.size(), 0u);
    c.push_back(1);
    c.clear();
    ASSERT_EQ(c.capacity(), 1u);
    ASSERT_EQ(c.size(), 0u);
    c.push_back(1);
    c.push_back(2);
    ASSERT_EQ(c.size(), 2u);
    auto cap = c.capacity();
    c.clear();
    ASSERT_EQ(c.capacity(), cap);
    ASSERT_EQ(c.size(), 0u);
    c.clear();
    ASSERT_EQ(c.capacity(), cap);
    ASSERT_EQ(c.size(), 0u);
  });
}

TEST(my_tests, emplace_back) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    container c;
    c.emplace_back(1);
    c.emplace_back(2);
    c.emplace_back(3);
    ASSERT_EQ(c.size(), 3u);
    c.push_back(4);
    ASSERT_EQ(c.size(), 4u);
    c.emplace_back(5);
    ASSERT_EQ(c.size(), 5u);
    vector<std::string> v;
    v.emplace_back("hello");
    v.emplace_back();
    v.emplace_back();
    ASSERT_EQ(v.size(), 3u);
    ASSERT_EQ(v[0], "hello");
    ASSERT_EQ(v[1], "");
    ASSERT_EQ(v[2], "");
  });
}

TEST(my_tests, shrink_to_fit) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    container c;
    c.shrink_to_fit();
    ASSERT_EQ(c.size(), 0u);
    c.push_back(1);
    ASSERT_EQ(c.size(), 1u);
    ASSERT_EQ(c[0], 1);
    c.shrink_to_fit();
    ASSERT_EQ(c.size(), 1u);
    ASSERT_EQ(c[0], 1);
    c.push_back(2);
    ASSERT_EQ(c.size(), 2u);
    c.shrink_to_fit();
    ASSERT_EQ(c.size(), c.capacity());
    ASSERT_EQ(c.size(), 2u);
    ASSERT_EQ(c[0], 1);
    ASSERT_EQ(c[1], 2);
    c.pop_back();
    c.pop_back();
    c.shrink_to_fit();
    ASSERT_EQ(c.size(), 0u);
    c.shrink_to_fit();
    ASSERT_EQ(c.size(), 0u);
    c.push_back(1);
    ASSERT_EQ(c.size(), 1u);
    ASSERT_EQ(c[0], 1);
    c.shrink_to_fit();
    ASSERT_EQ(c.size(), 1u);
    ASSERT_EQ(c[0], 1);
    c.push_back(2);
    ASSERT_EQ(c.size(), 2u);
    c.shrink_to_fit();
    ASSERT_EQ(c.size(), c.capacity());
    ASSERT_EQ(c.size(), 2u);
    ASSERT_EQ(c[0], 1);
    ASSERT_EQ(c[1], 2);
  });
}

TEST(my_tests, move_push_back) {
  faulty_run([] {
    using std::string;
    std::vector<string> v;
    vector<string> my_v;
    for (size_t i = 0; i != 10; ++i) {
      v.emplace_back("hello");
      my_v.push_back(std::move(v[i]));
      ASSERT_EQ(my_v.size(), i + 1);
      ASSERT_EQ(my_v[i], "hello");
    }
  });
}

TEST(my_tests, resize) {
  faulty_run([] {
    counted::no_new_instances_guard g;
    container c;
    for (size_t i = 0; i != 5; ++i) {
      c.push_back(i);
    }
    c.resize(3);
    ASSERT_EQ(3u, c.size());
    ASSERT_EQ(c[0], 0);
    ASSERT_EQ(c[1], 1);
    ASSERT_EQ(c[2], 2);
    c.resize(0);
    ASSERT_EQ(0, c.size());
    for (size_t i = 0; i != 5; ++i) {
      c.push_back(i);
    }
    c.resize(7u, 1);
    ASSERT_EQ(7u, c.size());
    for (size_t i = 5; i != 7; ++i) {
      ASSERT_EQ(c[i], 1);
    }
    c.resize(3, 2);
    ASSERT_EQ(3u, c.size());
    ASSERT_EQ(c[0], 0);
    ASSERT_EQ(c[1], 1);
    ASSERT_EQ(c[2], 2);
    c.resize(1);
    ASSERT_EQ(1u, c.size());
    ASSERT_EQ(c[0], 0);
    container_int ci;
    ci.resize(10);
    ASSERT_EQ(ci.size(), 10u);
    for (int x : ci) {
      ASSERT_EQ(x, 0);
    }
    ci.resize(5, 5);
    ASSERT_EQ(ci.size(), 5u);
    for (int x : ci) {
      ASSERT_EQ(x, 0);
    }
    ci.resize(0);
    ASSERT_EQ(0u, ci.size());
  });
}