#include <gtest/gtest.h>

#include <data_queue/data_queue.h>

#include <chrono>
#include <thread>

using tp = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
namespace daqu
{
  template <>
  float extract(const tp::duration& value)
  {
    return value.count();
  }

} // namespace daqu

TEST(storage_data_accessor, exact_access_test)
{
  using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using buffT = std::vector<daqu::stamped_data<int, tp>>;
  buffT buffer;

  {
    auto r0 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{0}});
    EXPECT_EQ(r0, buffer.end());
    EXPECT_EQ(r0, buffer.cend());
  }

  {
    buffer.emplace_back(10, tp{std::chrono::nanoseconds{1}});

    auto r0 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{0}});
    EXPECT_EQ(r0->data, 10);

    auto r1 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{100}});
    EXPECT_EQ(r1->data, 10);
  }

  {
    buffer.emplace_back(20, tp{std::chrono::nanoseconds{100}});

    auto r0 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{1}});
    EXPECT_EQ(r0->data, 10);

    auto r1 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{100}});
    EXPECT_EQ(r1->data, 20);

    auto r2 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{51}});
    EXPECT_EQ(r2->data, 20);

    auto r3 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{49}});
    EXPECT_EQ(r3->data, 10);
  }

  {
    buffer.emplace_back(30, tp{std::chrono::nanoseconds{200}});

    auto r0 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{1}});
    EXPECT_EQ(r0->data, 10);

    auto r1 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{100}});
    EXPECT_EQ(r1->data, 20);

    auto r2 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{200}});
    EXPECT_EQ(r2->data, 30);

    auto r3 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{51}});
    EXPECT_EQ(r3->data, 20);

    auto r4 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{49}});
    EXPECT_EQ(r4->data, 10);

    auto r5 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{150}});
    EXPECT_EQ(r5->data, 30);

    auto r6 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{149}});
    EXPECT_EQ(r6->data, 20);

    auto r7 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{400}});
    EXPECT_EQ(r7->data, 30);
  }
}

TEST(storage_data_accessor, interop_access_test)
{

  struct int_interpolation
  {
    using tp = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
    daqu::stamped_data<int, tp> operator()(const daqu::stamped_data<int, tp>& l, const float w0, const daqu::stamped_data<int, tp>& r, const float w1,
                                           const tp& tar_ts)
    {
      return daqu::stamped_data<int, tp>(int(l.data * w0 + r.data * w1), tar_ts);
    }
  };

  auto int_inter = int_interpolation();

  using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using buffT = std::vector<daqu::stamped_data<int, tp>>;
  buffT buffer;

  {
    buffer.emplace_back(10, tp{std::chrono::nanoseconds{0}});
    buffer.emplace_back(20, tp{std::chrono::nanoseconds{100}});
    buffer.emplace_back(30, tp{std::chrono::nanoseconds{200}});

    auto r0  = daqu::access(buffer).get(tp{std::chrono::nanoseconds{0}});
    auto ir0 = daqu::access(buffer).get_data_inter(r0, tp{std::chrono::nanoseconds{0}}, int_inter);
    EXPECT_EQ(ir0.data, 10);

    auto r1  = daqu::access(buffer).get(tp{std::chrono::nanoseconds{100}});
    auto ir1 = daqu::access(buffer).get_data_inter(r1, tp{std::chrono::nanoseconds{100}}, int_inter);
    EXPECT_EQ(ir1.data, 20);

    auto r2  = daqu::access(buffer).get(tp{std::chrono::nanoseconds{100}});
    auto ir2 = daqu::access(buffer).get_data_inter(r1, tp{std::chrono::nanoseconds{50}}, int_inter);
    EXPECT_EQ(ir2.data, 15);
  }
}

TEST(storage_data_accessor, threshold_access_test)
{

  using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using buffT = std::vector<daqu::stamped_data<int, tp>>;
  buffT buffer;

  std::chrono::nanoseconds thesh{20};

  {

    auto r0 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{21}}, thesh);
    EXPECT_EQ(r0.status, daqu::storage_access_status::not_enough_elements);
  }

  {
    buffer.clear();
    buffer.emplace_back(10, tp{std::chrono::nanoseconds{0}});

    auto r0 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{21}}, thesh);
    EXPECT_EQ(r0.status, daqu::storage_access_status::timestamp_diff_larger_then_thresh);
    EXPECT_EQ(r0.time_diff, std::chrono::nanoseconds{21});

    auto r1 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{41}}, thesh);
    EXPECT_EQ(r1.status, daqu::storage_access_status::timestamp_diff_larger_then_thresh);
    EXPECT_EQ(r1.time_diff, std::chrono::nanoseconds{41});
  }

  {
    buffer.clear();
    buffer.emplace_back(10, tp{std::chrono::nanoseconds{0}});
    buffer.emplace_back(20, tp{std::chrono::nanoseconds{100}});
    buffer.emplace_back(30, tp{std::chrono::nanoseconds{200}});

    auto r0 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{0}}, thesh);
    EXPECT_EQ(r0.status, daqu::storage_access_status::success);
    EXPECT_EQ(r0.time_diff, std::chrono::nanoseconds{0});
    EXPECT_EQ(r0.it->data, 10);

    auto r1 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{100}}, thesh);
    EXPECT_EQ(r1.status, daqu::storage_access_status::success);
    EXPECT_EQ(r1.time_diff, std::chrono::nanoseconds{0});
    EXPECT_EQ(r1.it->data, 20);

    auto r2 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{200}}, thesh);
    EXPECT_EQ(r2.status, daqu::storage_access_status::success);
    EXPECT_EQ(r2.time_diff, std::chrono::nanoseconds{0});
    EXPECT_EQ(r2.it->data, 30);

    auto r3 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{20}}, thesh);
    EXPECT_EQ(r3.status, daqu::storage_access_status::success);
    EXPECT_EQ(r3.time_diff, std::chrono::nanoseconds{20});
    EXPECT_EQ(r3.it->data, 10);

    auto r4 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{120}}, thesh);
    EXPECT_EQ(r4.status, daqu::storage_access_status::success);
    EXPECT_EQ(r4.time_diff, std::chrono::nanoseconds{20});
    EXPECT_EQ(r4.it->data, 20);

    auto r5 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{180}}, thesh);
    EXPECT_EQ(r5.status, daqu::storage_access_status::success);
    EXPECT_EQ(r5.time_diff, std::chrono::nanoseconds{20});
    EXPECT_EQ(r5.it->data, 30);

    auto r6 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{21}}, thesh);
    EXPECT_EQ(r6.status, daqu::storage_access_status::timestamp_diff_larger_then_thresh);
    EXPECT_EQ(r6.time_diff, std::chrono::nanoseconds{21});

    auto r7 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{121}}, thesh);
    EXPECT_EQ(r7.status, daqu::storage_access_status::timestamp_diff_larger_then_thresh);
    EXPECT_EQ(r7.time_diff, std::chrono::nanoseconds{21});

    auto r8 = daqu::access(buffer).get(tp{std::chrono::nanoseconds{179}}, thesh);
    EXPECT_EQ(r8.status, daqu::storage_access_status::timestamp_diff_larger_then_thresh);
    EXPECT_EQ(r8.time_diff, std::chrono::nanoseconds{21});
  }
}

TEST(storage_data_accessor, in_range_test)
{

  using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using buffT = std::vector<daqu::stamped_data<int, tp>>;
  buffT buffer;

  std::chrono::nanoseconds thesh{20};

  buffer.emplace_back(10, tp{std::chrono::nanoseconds{0}});
  buffer.emplace_back(20, tp{std::chrono::nanoseconds{100}});
  buffer.emplace_back(30, tp{std::chrono::nanoseconds{200}});

  EXPECT_EQ(daqu::access(buffer).in_range(tp{std::chrono::nanoseconds{0}}), true);

  EXPECT_EQ(daqu::access(buffer).in_range(tp{std::chrono::nanoseconds{10}}), true);

  EXPECT_EQ(daqu::access(buffer).in_range(tp{std::chrono::nanoseconds{200}}), true);

  EXPECT_EQ(daqu::access(buffer).in_range(tp{std::chrono::nanoseconds{201}}), false);
  EXPECT_EQ(daqu::access(buffer).in_range(tp{std::chrono::nanoseconds{301}}), false);
}