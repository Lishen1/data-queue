#include <data_queue/data_queue.h>

#include <vector>
#include <chrono>
#include <string>
#include <thread>
#include <functional>
#include <optional>

#include <iostream>

using timestamp    = std::chrono::high_resolution_clock;
using tp = timestamp::time_point;

constexpr auto get_coefficients(const float ls, const float rs, const float range)
{
  float w1 = ls / range;
  float w0 = rs / range;
  return std::make_tuple(w1, w0);
}

struct simp_interpolation {
  using simp_function_type = std::function<daqu::stamped_data<std::string, tp>(const daqu::stamped_data<std::string, tp>&, const float,
                                                                               const daqu::stamped_data<std::string, tp>&, const float)>;


  std::optional<simp_function_type> simp_function;
  simp_interpolation() = default;

  simp_interpolation(simp_function_type super_simp) : simp_function{super_simp} {}

  daqu::stamped_data<std::string, tp> operator()(const daqu::stamped_data<std::string, tp>& l, const daqu::stamped_data<std::string, tp>& r,
                                                 const tp& ts)
  {
    auto [w1, w0] = get_coefficients(static_cast<float>((ts - l.ts).count()), static_cast<float>((r.ts - ts).count()),
                                           static_cast<float>((r.ts - l.ts).count()));
    if (simp_function.has_value())
    {
      return simp_function.value()(l, w0, r, w1);
    }

    return l;
  }
};

void create_simp_accessor() {

  using buffer_type = std::vector<daqu::stamped_data<std::string, tp>>;

  buffer_type buffer;

  tp timestamp_before_buffer = timestamp::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  buffer.emplace_back("one", timestamp::now());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  tp timestamp_inmiddle_buffer = timestamp::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  buffer.emplace_back("two", timestamp::now());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  buffer.emplace_back("three", timestamp::now());
  tp timestamp_after_buffer = timestamp::now();

  
  using namespace std::chrono_literals;
  std::chrono::nanoseconds ts_thresh = 1ms;

  auto res = daqu::access(buffer).get(timestamp_inmiddle_buffer);
  auto res_with_tresh = daqu::access(buffer).get(timestamp_inmiddle_buffer, ts_thresh);

  bool in_range_1 = daqu::access(buffer).in_range(timestamp_before_buffer);
  bool in_range_2 = daqu::access(buffer).in_range(timestamp_inmiddle_buffer);
  bool in_range_3 = daqu::access(buffer).in_range(timestamp_after_buffer);

  auto dat = daqu::access(buffer).get_data_inter(res_with_tresh.it, timestamp_after_buffer);

  auto dat_with_interpolation = daqu::access(buffer).get_data_inter(res_with_tresh.it, timestamp_after_buffer, [](auto a, auto b, auto ts) { 

    return b;
  });

  auto dat_with_interpolation_simp
      = daqu::access(buffer).get_data_inter(res_with_tresh.it, timestamp_after_buffer,
                                            simp_interpolation{});

  auto dat_with_interpolation_super_simp = daqu::access(buffer).get_data_inter(
      res_with_tresh.it, timestamp_after_buffer,
      simp_interpolation{[](const daqu::stamped_data<std::string, tp> &l, const float w0, const daqu::stamped_data<std::string, tp> &r, const float w1) {
  
      return l;
  }});

  std::cout << "res: " << res->data << "\n";

  std::cout << "timestamp_before_buffer: " << in_range_1 << "\n";
  std::cout << "timestamp_inmiddle_buffer: " << in_range_2 << "\n";
  std::cout << "timestamp_after_buffer: " << in_range_3 << "\n";

  std::cout << "dat: " << dat.data << "\n";
  std::cout << "dat_with_interpolation: " << dat_with_interpolation.data << "\n";
  std::cout << "dat_with_interpolation_simp: " << dat_with_interpolation_simp.data << "\n";
  std::cout << "dat_with_interpolation_super_simp: " << dat_with_interpolation_super_simp.data << "\n";


}

int main() {
  create_simp_accessor();
}
