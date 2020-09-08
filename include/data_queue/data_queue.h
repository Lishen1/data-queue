#pragma once
#include "types.h"
#include <algorithm>

namespace daqu
{
  namespace detail
  {
      template <typename timeT>
      struct interpolate_data
      {
        stamped_data<std::string, timeT> operator()(const stamped_data<std::string, timeT>& a, const float& w0,
                                                    const stamped_data<std::string, timeT>& b, const float& w1) {
          stamped_data<std::string, timeT> c;
          c.data = a.data + b.data;
          c.ts   = a.ts;
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
  decltype(timeT() - timeT()) time_adiff(const timeT& a, const timeT& b)
  {
    return (a > b ? a - b : b - a);
  }

  template <typename Container>
  class storage_data_accessor
  {

  public:
    template <typename iteratorT, typename time_diffT>
    struct result
    {
      iteratorT             iterator;
      time_diffT            time_diff;
      storage_access_status status;
    };

    using iterator       = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using value_type     = typename Container::value_type;

    template <typename timeT>
    struct result_type
    {
      using type = result<iterator, decltype(timeT() - timeT())>;
    };

    storage_data_accessor(Container& buff) : _storage(buff){};

    /// \brief return iter with equal or greater timestamp
    template <typename timeT>
    iterator get(const timeT& ts) const
    {

      iterator i = std::lower_bound(_storage.begin(), _storage.end(), ts, [](const value_type& a, const timeT& b) { return a.ts < b; });

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

    template <typename timeT>
    auto get(const timeT& target_ts, const decltype(timeT() - timeT())& max_ts_diff) const
    {
      iterator closest = get(target_ts);

      result<iterator, decltype(timeT() - timeT())> res;
      res.iterator = closest;

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

    template <typename timeT>
    bool in_range(const timeT& target_ts)
    {
      return _storage.size() > 2 && target_ts <= (last())->ts && target_ts >= (_storage.begin())->ts;
    }

    template <typename timeT, typename Interpolation = detail::interpolate_data<timeT>>
    value_type get_data_inter(const iterator& iter, const timeT& target_ts, Interpolation interpolation = {})
    {
      auto inter = [&](const iterator& l, const iterator& r, const timeT& ts) {
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

} // namespace daqu
