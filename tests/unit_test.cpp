#include <gtest/gtest.h>

#include <data_queue/data_queue.h>

#include <chrono>
#include <thread>

TEST(storage_data_accessor, simple_get_inrange_data_iter_test)
{
  using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
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

  auto d0 = ref_tp0 < ref_tp1;
  auto d1 = ref_tp2 < ref_tp1;

  auto d2 = ref_tp2 - ref_tp1;

  std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds> dd = ref_tp2 - ref_tp1;

  using namespace std::chrono_literals;

  std::chrono::nanoseconds ts_thresh = 1ms;

  auto res = accesor.get(ref_tp1, ts_thresh);

  bool in_range = accesor.in_range(ref_tp1);
  bool in_range2 = accesor.in_range(std::chrono::steady_clock::now());

  //auto dat       = accesor.get_data_inter(res.iterator, ref_tp1);
  
  EXPECT_EQ(res.status, daqu::storage_access_status::success);
  EXPECT_EQ(in_range, true);
}

TEST(storage_data_accessor, simple_get_inrange_data_iter_test_2)
{
  using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;
  using buffT = std::vector<daqu::stamped_data<std::string, tp>>;
  buffT buffer;

  tp ref_tp0 = std::chrono::steady_clock::now();

  buffer.reserve(5);
  auto t = std::chrono::steady_clock::now();
  buffer.emplace_back("one", t);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  tp ref_tp1 = std::chrono::steady_clock::now();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  tp ref_tp2 = std::chrono::steady_clock::now();

  auto accesor = daqu::make_storage_accessor<std::vector<daqu::stamped_data<std::string, tp>>>(buffer);
  std::vector<daqu::stamped_data<std::string, tp>> buffer2 {};
  auto accesor2 = daqu::make_storage_accessor<std::vector<daqu::stamped_data<std::string, tp>>>(buffer2);

  auto d0 = ref_tp0 < ref_tp1;
  auto d1 = ref_tp2 < ref_tp1;

  auto d2 = ref_tp2 - ref_tp1;

  std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds> dd = ref_tp2 - ref_tp1;

  using namespace std::chrono_literals;

  std::chrono::nanoseconds ts_thresh = 1ms;

  auto res = accesor.get(ref_tp1, ts_thresh);
  auto res2 = accesor2.get(t, ts_thresh);

  bool in_range  = accesor.in_range(ref_tp1);
  bool in_range2 = accesor.in_range(std::chrono::steady_clock::now());

  auto dat = accesor.get_data_inter(res.it, ref_tp1);

  auto dat2 = accesor.get_data_inter(
      res.it, ref_tp1,
      [](const daqu::stamped_data<std::string, tp>& a, const float& w0, const daqu::stamped_data<std::string, tp>& b, const float& w1) {

        return a;
      });

  EXPECT_EQ(res.status, daqu::storage_access_status::timestamp_diff_larger_then_thresh);
  EXPECT_EQ(res2.status, daqu::storage_access_status::not_enough_elements); /**/
  EXPECT_EQ(in_range, false);
  EXPECT_EQ(dat.data, dat2.data);
}
