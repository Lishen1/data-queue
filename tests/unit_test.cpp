#include <gtest/gtest.h>

#include <data_queue/data_queue.h>

#include <chrono>
#include <thread>

using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
namespace daqu
{
  template<>
  float extract(const tp::duration& value) { return value.count(); }

} // namespace daqu

TEST(storage_data_accessor, simple_get_inrange_data_iter_test)
{
  using buffT = std::vector<daqu::stamped_data<std::string, tp>>;
  buffT buffer;

  tp ref_tp0 = std::chrono::steady_clock::now();

  buffer.reserve(5);

  buffer.emplace_back("one", std::chrono::steady_clock::now());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  buffer.emplace_back("two", std::chrono::steady_clock::now());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  buffer.emplace_back("three", std::chrono::steady_clock::now());
  tp ref_tp1 = std::chrono::steady_clock::now();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  buffer.emplace_back("four", std::chrono::steady_clock::now());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  buffer.emplace_back("five", std::chrono::steady_clock::now());

  tp ref_tp2 = std::chrono::steady_clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  daqu::storage_data_accessor<buffT> accesor(buffer);

  using namespace std::chrono_literals;

  std::chrono::nanoseconds ts_thresh = 1ms;

  auto res = accesor.get(ref_tp1, ts_thresh);

  bool in_range = accesor.in_range(ref_tp1);
  bool in_range2 = accesor.in_range(std::chrono::steady_clock::now());

  //auto dat       = accesor.get_data_inter(res.iterator, ref_tp1);
  
  EXPECT_EQ(res.status, daqu::storage_access_status::success);
  EXPECT_EQ(in_range, true);
  EXPECT_EQ(in_range2, false);
}
struct simp_interpolation
{
  using tp = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  daqu::stamped_data<std::string, tp> operator()(const daqu::stamped_data<std::string, tp>& l, const float w0,
                                                 const daqu::stamped_data<std::string, tp>& r, const float w1)
  {
    return l;
  }
};
TEST(storage_data_accessor, simple_get_inrange_data_iter_test_2)
{
  using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using buffT = std::vector<daqu::stamped_data<std::string, tp>>;
  buffT buffer;

  auto before_buffer_timestamp = std::chrono::steady_clock::now();

  tp ref_tp0 = std::chrono::steady_clock::now();
  buffer.reserve(5);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  auto t = std::chrono::steady_clock::now();
  buffer.emplace_back("one", t);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  auto accesor = daqu::access(buffer);

  EXPECT_EQ(accesor.in_range(before_buffer_timestamp), false); 
  EXPECT_EQ(accesor.in_range(t), false);

  auto t2 = std::chrono::steady_clock::now();
  buffer.emplace_back("two", t2);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  EXPECT_EQ(accesor.in_range(before_buffer_timestamp), false);
  EXPECT_EQ(accesor.in_range(t2), false);

  auto t3 = std::chrono::steady_clock::now();
  buffer.emplace_back("three", t3);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  EXPECT_EQ(accesor.in_range(before_buffer_timestamp), false);
  EXPECT_EQ(accesor.in_range(t2), true);

  tp ref_tp1 = std::chrono::steady_clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  EXPECT_EQ(accesor.in_range(ref_tp1), false);

  tp ref_tp2 = std::chrono::steady_clock::now();

  std::vector<daqu::stamped_data<std::string, tp>> buffer2 {};
  auto accesor2 = daqu::access(buffer2);
  EXPECT_EQ(accesor2.in_range(ref_tp2), false);

  using namespace std::chrono_literals;
  std::chrono::nanoseconds ts_thresh = 1ms;

  auto r1 = accesor.get(before_buffer_timestamp);
  auto r2 = accesor.get(t3);
  EXPECT_EQ(r1, buffer.begin());

  EXPECT_EQ(r2, std::prev(buffer.end()));

  auto res = accesor.get(ref_tp1, ts_thresh);
  auto res2 = accesor2.get(t, ts_thresh);

  bool in_range  = accesor.in_range(ref_tp1);
  bool in_range2 = accesor.in_range(std::chrono::steady_clock::now());

  auto dat = accesor.get_data_inter(res.it, ref_tp1);

  auto dat2 = accesor.get_data_inter(res.it, ref_tp1, simp_interpolation{});

  EXPECT_EQ(res.status, daqu::storage_access_status::timestamp_diff_larger_then_thresh);
  EXPECT_EQ(res2.status, daqu::storage_access_status::not_enough_elements); /**/
  EXPECT_EQ(in_range, false);
  EXPECT_EQ(in_range2, false);
  EXPECT_EQ(dat.data, dat2.data);
}
