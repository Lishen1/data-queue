#include <benchmark/benchmark.h>

#include <data_queue/data_queue.h>

#include <algorithm>
#include <chrono>
#include <iterator>
#include <numeric>
#include <vector>

#include <ctime>

/*
 *
 * Benchmark iterator get(const time_value_type& ts) const noexcept
 *
 */
namespace
{
  void BM_creation_accessor_get_at_start(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(0)}));
    }
  }
  void BM_creation_accessor_get_in_middle(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0) / 2)}));
    }
  }

  void BM_creation_accessor_get_random_access(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(rand_value)}));
    }
  }
  void BM_creation_accessor_get_at_end(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0))}));
    }
  }
} // namespace

BENCHMARK(BM_creation_accessor_get_at_start)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_in_middle)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_random_access)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_at_end)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);

/*
 *
 * Benchmark auto get(const time_value_type& target_ts, const different_time_value_type& max_ts_diff) const noexcept
 *
 */
namespace
{
  void BM_creation_accessor_get_with_treshold_at_start(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(0)}, std::chrono::microseconds(2)));
    }
  }
  void BM_creation_accessor_get_with_treshold_in_middle(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0) / 2)}, std::chrono::microseconds(2)));
    }
  }

  void BM_creation_accessor_get_with_treshold_random_access(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(rand_value)}, std::chrono::microseconds(2)));
    }
  }
  void BM_creation_accessor_get_with_treshold_at_end(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0))}, std::chrono::microseconds(2)));
    }
  }
} // namespace

BENCHMARK(BM_creation_accessor_get_with_treshold_at_start)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_with_treshold_in_middle)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_with_treshold_random_access)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_with_treshold_at_end)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);

/*
 *
 * Benchmark bool in_range(const time_value_type& target_ts) const noexcept
 *
 */
namespace
{
  void BM_creation_accessor_in_range_at_start(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).in_range(tp{std::chrono::microseconds(0)}));
    }
  }
  void BM_creation_accessor_in_range_in_middle(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).in_range(tp{std::chrono::microseconds(state.range(0) / 2)}));
    }
  }

  void BM_creation_accessor_in_range_random_access(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).in_range(tp{std::chrono::microseconds(rand_value)}));
    }
  }
  void BM_creation_accessor_in_range_at_end(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).in_range(tp{std::chrono::microseconds(state.range(0))}));
    }
  }
  void BM_creation_accessor_in_range_outofbound(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);
    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).in_range(tp{std::chrono::microseconds(state.range(0) + rand_value)}));
    }
  }
} // namespace

BENCHMARK(BM_creation_accessor_in_range_at_start)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_in_range_in_middle)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_in_range_random_access)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_in_range_at_end)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_in_range_outofbound)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);

namespace daqu
{
  template <>
  float extract(const std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>::duration& value)
  {
    return value.count();
  }

} // namespace daqu

/*
 *
 * Benchmark value_type get_data_inter(const iterator& iter, const time_value_type& target_ts, Interpolation interpolation = {}) const noexcept
 *
 */
namespace
{
  void BM_creation_accessor_get_data_inter_at_start(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });
    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(0)});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);
    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}));
    }
  }
  void BM_creation_accessor_get_data_inter_in_middle(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });
    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0) / 2)});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);
    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}));
    }
  }

  void BM_creation_accessor_get_data_inter_random_access(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(rand() % state.range(0))});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}));
    }
  }
  void BM_creation_accessor_get_data_inter_at_end(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });
    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0))});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);
    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}));
    }
  }

  struct int_interpolation
  {
    using tp = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    daqu::stamped_data<int, tp> operator()(const daqu::stamped_data<int, tp>& l, const float w0, const daqu::stamped_data<int, tp>& r, const float w1,
                                           const tp& tar_ts)
    {
      return daqu::stamped_data<int, tp>(int(l.data * w0 + r.data * w1), tar_ts);
    }
  };

    void BM_creation_accessor_get_data_inter_with_interpolation_at_start(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });
    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(0)});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);
    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}, int_interpolation{}));
    }
  }
  void BM_creation_accessor_get_data_inter_with_interpolation_in_middle(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });
    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0) / 2)});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);
    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}, int_interpolation{}));
    }
  }

  void BM_creation_accessor_get_data_inter_with_interpolation_random_access(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });

    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(rand() % state.range(0))});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);

    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}, int_interpolation{}));
    }
  }
  void BM_creation_accessor_get_data_inter_with_interpolation_at_end(benchmark::State& state)
  {
    // Perform setup here
    using tp    = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
    using buffT = std::vector<daqu::stamped_data<int, tp>>;
    buffT buffer;
    buffer.reserve(state.range(0));

    std::vector<int> timestamps(state.range(0));
    std::iota(timestamps.begin(), timestamps.end(), 0);

    std::transform(timestamps.begin(), timestamps.end(), std::back_inserter(buffer),
                   [](const auto& value) { return daqu::stamped_data<int, tp>(value, tp{std::chrono::microseconds(value)}); });
    auto result_it = daqu::access(buffer).get(tp{std::chrono::microseconds(state.range(0))});
    srand(time(nullptr));
    const auto rand_value = rand() % state.range(0);
    for (auto _ : state)
    {
      benchmark::DoNotOptimize(daqu::access(buffer).get_data_inter(result_it, tp{std::chrono::microseconds(rand_value)}, int_interpolation{}));
    }
  }

} // namespace

BENCHMARK(BM_creation_accessor_get_data_inter_with_interpolation_at_start)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_data_inter_with_interpolation_in_middle)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_data_inter_with_interpolation_random_access)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
BENCHMARK(BM_creation_accessor_get_data_inter_with_interpolation_at_end)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);

BENCHMARK_MAIN();