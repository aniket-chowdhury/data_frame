#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include "data_frame.hpp"
#include <vector>
#include <iostream>
#include <tuple>
#include <typeinfo>
#include <type_traits>
#include <string>
#include "gtest/gtest.h"
#include "data_frame.hpp"
using namespace boost::numeric::ublas;
using namespace std::string_literals;
TEST(Data_frame, adding_column) {
    using type_collection = type_list<int, std::string>::types;  
    std::vector<int> int_vec = {0, 1, 2, 3, 4};
    std::vector<std::string> str_vec = {"test0", "test1", "test2", "test3", "test4"};
    data_frame df(type_collection{});
    df.add_column("int_vec", int_vec);
    df.add_column("str_vec", str_vec);
    EXPECT_EQ(df.get_cur_rows(), 5);
    EXPECT_EQ(df.get_cur_cols(), 2);
}
TEST(Data_frame, initialize_from_tuple) {
    using namespace std::string_literals;
    auto df1 = make_from_tuples(std::vector{std::make_tuple(1, 3.3, "hello"s), 
                                            std::make_tuple(2, 2.2, "world"s), 
                                            std::make_tuple(3, 1.1, "github"s)}, 
                                            {"int_vec", "double_vec", "str_vec"});
    EXPECT_EQ(df1->get_cur_rows(), 3);
    EXPECT_EQ(df1->get_cur_cols(), 3);
    auto* df2 = make_from_tuples({  {0, 3.4, "hello"s}, 
                                    {2, 2.2, "world"s}, 
                                    {3, 1.1, "github"s}   }, 
                                    {"int_vec", "double_vec", "str_vec"}, 
                                    std::tuple<int, double, std::string>{});
    EXPECT_EQ(df2->get_cur_rows(), 3);
    EXPECT_EQ(df2->get_cur_cols(), 3);
    using type_collection = type_list<int, double, std::string>::original_types; 
    data_frame_type_builder type_builder = {type_collection{}};
    using data_frame_type = decltype(type_builder)::data_frame_type;
    data_frame_type* df3 = make_from_tuples({   {1, 3.3, "hello"s}, 
                                                {2, 2.2, "world"s}, 
                                                {3, 1.1, "github"s}   }, 
                                                {"int_vec", "double_vec", "str_vec"}, 
                                                type_collection{});
    EXPECT_EQ(df3->get_cur_rows(), 3);
    EXPECT_EQ(df3->get_cur_cols(), 3);
    using type_collection = type_list<int, double, std::string>::original_types; 
    data_frame df4(type_collection{});
    df4.from_tuples(std::vector{std::make_tuple(1, 3.3, "hello"s), 
                                            std::make_tuple(2, 2.2, "world"s), 
                                            std::make_tuple(3, 1.1, "bili"s)}, 
                                            {"int_vec", "double_vec", "str_vec"});
    EXPECT_EQ(df4.get_cur_rows(), 3);
    EXPECT_EQ(df4.get_cur_cols(), 3);
    data_frame df5(type_collection{});
    df5.from_tuples({   {1, 3.3, "hello"s}, 
                        {2, 2.2, "world"s}, 
                        {3, 1.1, "github"s}   }, 
                        {"int_vec", "double_vec", "str_vec"}, 
                        type_collection{});
    EXPECT_EQ(df5.get_cur_rows(), 3);
    EXPECT_EQ(df5.get_cur_cols(), 3);
}
TEST(Data_frame, sort_with_single_column) {
    using type_collection = type_list<int, double, std::string>::original_types; 
    data_frame df2 = type_collection{};
    df2.from_tuples(std::vector{std::make_tuple(1, 3.3, "hello"s), 
                                std::make_tuple(2, 2.2, "world"s), 
                                std::make_tuple(3, 1.1, "bili"s)}, 
                    {"int_vec", "double_vec", "str_vec"});
    std::vector<int> new_index = df2.order<int>("int_vec");
    int len = df2.get_cur_rows();
    for (int i = 0; i < len; i++)
        std::cout << new_index[i] << " ";
    std::cout << std::endl;
    EXPECT_EQ(new_index[0], 2);
    EXPECT_EQ(new_index[1], 1);
    EXPECT_EQ(new_index[2], 0);
}
TEST(Data_frame, print_with_index) {
    using type_collection = type_list<int, double, std::string>::types;    
    data_frame df2 = type_collection{};
    df2.from_tuples(std::vector{std::make_tuple(1, 3.3, "hello"s), 
                                std::make_tuple(2, 2.2, "world"s), 
                                std::make_tuple(3, 1.1, "bili"s)}, 
                    {"int_vec", "double_vec", "str_vec"});
    df2.print_with_index({0, 1, 2});
}
TEST(Data_frame, copy_with_index) {
    using type_collection = type_list<int, std::string>::types;    
    std::vector<int> int_vec = {0, 1, 2, 3, 4};
    std::vector<std::string> str_vec = {"test0"s, "test1"s, "test2"s, "test3"s, "test4"s};
    data_frame df{type_collection{}};
    df.add_column("int_vec", int_vec);
    df.add_column("str_vec", str_vec);
    auto df3 = df.copy_with_index({1, 2, 0, 2});
    EXPECT_EQ(df3.get_cur_rows(), 4);
    EXPECT_EQ(df3.get_cur_cols(), 2);
}
TEST(Data_frame, filt_repeat_types) {
    using type_collection2 = type_list<double, std::string, long, long>::types;
    data_frame df4(type_collection2{});
    df4.from_tuples(std::vector{std::make_tuple(3.3, "hello"s, 10L), 
                                std::make_tuple(2.2, "world"s, 40L), 
                                std::make_tuple(1.1, "bili"s, 50L)}, 
                    {"double_vec", "str_vec", "long_vec"});
    df4.print_with_index({0, 1, 2});
    EXPECT_EQ(df4.get_cur_rows(), 3);
    EXPECT_EQ(df4.get_cur_cols(), 3);
}
TEST(Data_frame, copy_with_range_test) {
    using type_collection2 = type_list<double, std::string, long, long>::types;
    data_frame df4(type_collection2{});
    df4.from_tuples(std::vector{std::make_tuple(3.3, "hello"s, 10L), 
                                std::make_tuple(2.2, "world"s, 40L), 
                                std::make_tuple(1.1, "bili"s, 50L)}, 
                    {"double_vec", "str_vec", "long_vec"});
    range r(0, 2);
    data_frame df5 = df4.copy_with_range(r);
    EXPECT_EQ(df5.get_cur_rows(), 2);
    EXPECT_EQ(df5.get_cur_cols(), 3);
}
TEST(Data_frame, copy_with_slice_test) {
    using type_collection2 = type_list<double, std::string, long, long>::types;
    data_frame df4(type_collection2{});
    df4.from_tuples(std::vector{std::make_tuple(3.3, "hello"s, 10L), 
                                std::make_tuple(2.2, "world"s, 40L), 
                                std::make_tuple(1.1, "bili"s, 50L)}, 
                    {"double_vec", "str_vec", "long_vec"});
    slice s (0, 1, 3);
    data_frame df5 = df4.copy_with_slice(s);
    EXPECT_EQ(df5.get_cur_rows(), 3);
    EXPECT_EQ(df5.get_cur_cols(), 3);
}
TEST(Data_frame, create_view_with_range) {
    using type_collection2 = type_list<double, std::string, long, long>::types;
    data_frame df4(type_collection2{});
    df4.from_tuples(std::vector{std::make_tuple(3.3, "hello"s, 10L), 
                                std::make_tuple(2.2, "world"s, 40L), 
                                std::make_tuple(1.1, "bili"s, 50L)}, 
                    {"double_vec", "str_vec", "long_vec"});
    range r(0, 2);
    data_frame_view df5 = df4.create_view_with_range(r);
    EXPECT_EQ(df5.get_cur_rows(), 2);
    EXPECT_EQ(df5.get_cur_cols(), 3);
}
TEST(Data_frame, apply_with_index_test) {
    using type_collection2 = type_list<double, long>::types;
    data_frame df4(type_collection2{});
    df4.from_tuples(std::vector{std::make_tuple(3.3, 10L), 
                                std::make_tuple(2.2, 40L), 
                                std::make_tuple(1.1, 50L)}, 
                    {"double_vec", "long_vec"});
    df4.apply_with_index({0, 1}, [](auto& t) {
        return t * 2;
    });
    df4.print_with_index({0, 1, 2});
}
TEST(Data_frame, merge_types) {
    using type_collection1 = type_list<double, long>::types;
    using type_collection2 = type_list<std::string, int, double>::types;
    auto new_tuple = merge_types(type_collection1{}, type_collection2{});
    std::get<int>(new_tuple);
    std::get<double>(new_tuple);
    std::get<long>(new_tuple);
    std::get<std::string>(new_tuple);
}
TEST(Data_frame, combine_data_frames) {
    using type_collection1 = type_list<double, long>::types;
    using type_collection2 = type_list<std::string, int, double>::types;
    data_frame df1 = type_collection1{};
    data_frame df2 = type_collection2{};
    df1.from_tuples(std::vector{std::make_tuple(3.3, 10L), 
                                std::make_tuple(2.2, 40L)}, 
                                {"double_vec", "long_vec"});
    df2.from_tuples(std::vector{std::make_tuple(3.3, "hello"s, 10), 
                                std::make_tuple(3.3, "hello"s, 10),
                                std::make_tuple(2.2, "world"s, 40),
                                std::make_tuple(2.2, "world"s, 40),  
                                std::make_tuple(1.1, "bili"s, 50)}, 
                                {"double_vec", "str_vec", "int_vec"});
    auto df3 = combine<double>(df1, df2, "double_vec");
    EXPECT_EQ(df3->get_cur_rows(), 4);
}
