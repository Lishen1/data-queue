#pragma once
#include "types.h"
#include <algorithm>
#include <type_traits>

namespace daqu
{
  namespace detail
  {
      template <typename T, typename timeT>
      struct default_interpolation_data
      {
        stamped_data<T, timeT> operator()(const stamped_data<T, timeT>& a, const float& w0, const stamped_data<T, timeT>& b,
                                                    const float& w1)
        {
          return a;
        }
      };
  }

  enum class storage_access_status
  {
    success = 0,
    timestamp_diff_larger_then_thresh,
    not_enough_elements,
    timestamp_unorder
  };

  template <typename timeT>
  auto time_adiff(const timeT& a, const timeT& b)
  {
    return (a > b ? a - b : b - a);
  }

  template <typename Container>
  class storage_data_accessor
  {

  public:
    static_assert(std::is_same_v<typename Container::value_type::stamped_data_category, daqu::stamped_data_category_tag>, "works only with daqu::stamped_data type.");

    using iterator                      = typename Container::iterator;
    using const_iterator                = typename Container::const_iterator;
    using value_type                    = typename Container::value_type;
    using data_value_type               = typename value_type::data_value_type;
    using time_value_type               = typename value_type::time_value_type;
    using different_time_value_type     = decltype(std::declval<time_value_type>() - std::declval<time_value_type>());

    struct result
    {
      iterator                  it;
      different_time_value_type time_diff;
      storage_access_status status;
    };

    storage_data_accessor(Container& buff) : _storage(buff){
    };

    /// \brief return iter with equal or greater timestamp
    iterator get(const time_value_type& ts) const
    {
      iterator i = std::lower_bound(_storage.begin(), _storage.end(), ts, [](const value_type& a, const time_value_type& b) { return a.ts < b; });

      if (i != _storage.end()) // found
      {
        if (i == _storage.begin())
          return i;

        const auto& d1 = time_adiff(i->ts, ts);
        iterator    i2 = std::prev(i);

        if (i2 != _storage.begin())
        {
          const auto& d2 = time_adiff(i2->ts, ts);
          if (d2 < d1)
            i = i2;
        }
        return i; // found
      }
      else
      {
        if (!_storage.empty())
          return last();

        return _storage.end(); // not found
      }

    }

    auto get(const time_value_type& target_ts, const different_time_value_type& max_ts_diff) const
    {
      iterator closest = get(target_ts);

      result res{};
      res.it = closest;

      if (closest == _storage.end())
      {
        res.status = storage_access_status::not_enough_elements;
        return res;
      }

      auto ts_diff  = time_adiff(target_ts, closest->ts);
      res.time_diff = ts_diff;

      if (ts_diff > max_ts_diff)
      {
        res.status = storage_access_status::timestamp_diff_larger_then_thresh;
        return res;
      }

      res.status = storage_access_status::success;

      return res;
    }

    bool in_range(const time_value_type& target_ts)
    {
      return _storage.size() > 2 && target_ts <= (last())->ts && target_ts >= (_storage.begin())->ts;
    }

    template <typename Interpolation = detail::default_interpolation_data<data_value_type, time_value_type>>
    value_type get_data_inter(const iterator& iter, const time_value_type& target_ts, Interpolation interpolation = {})
    {
      auto inter = [&](const iterator& l, const iterator& r, const time_value_type& ts) {
        float range = (r->ts - l->ts).count();

        float w1 = float((ts - l->ts).count()) / range;
        float w0 = float((r->ts - ts).count()) / range;

        return interpolation(*l, w0, *r, w1);
      };

      if (iter->ts > target_ts)
      {
        if (iter == _storage.begin())
          return *iter;

        return inter(std::prev(iter), iter, target_ts);
      }
      else if (iter->ts < target_ts)
      {
        if (iter == last())
          return *iter;

        return inter(iter, std::next(iter), target_ts);
      }
      else
        return *iter;
    }
    
  private:
    iterator last() const { return std::prev(_storage.end()); }

    Container& _storage;
  };
  template<typename Container>
  auto access(Container& container)
  {
    return storage_data_accessor<Container>(container);
  }

} // namespace daqu
