#include <gtest/gtest.h>

#include <data_queue/data_queue.h>


TEST(storage_data_accessor, creation)
{
  std::vector<int> data;
  daqu::storage_data_accessor<std::vector<int>> store{data};;
  //EXPECT_ANY_THROW(throw std::runtime_error("fuk"));
}
