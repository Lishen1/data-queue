#pragma once
#include "types.h"
#include <algorithm>

namespace daqu
{

  enum class storage_acces_status
  {
    success = 0,
    timestamp_diff_larger_then_thresh,
    not_enough_elements,
    timestamp_unorder
  };

  template <typename starageT>
  class storage_data_accesor
  {

  public:
    template <typename iteratorT, typename time_diffT>
    struct result
    {
      iteratorT            iterator;
      time_diffT           time_diff;
      storage_acces_status status;
    };

    using iterator   = typename starageT::iterator;
    using value_type = typename starageT::value_type;

    storage_data_accesor(starageT& buff) : _storage(buff){};

    /// \brief return iter with equal or greater timestamp
    template <typename timeT>
    iterator get(const timeT& ts)
    {

      iterator i = std::lower_bound(_storage.begin(), _storage.end(), ts, [](const value_type& a, const timeT& b) { return a.ts < b; });

      if (i != _storage.end()) // found
      {
        if (i == _storage.begin())
          return i;

        const auto& d1 = (i->ts > ts ? i->ts - ts : ts - i->ts);
        iterator    i2 = i - 1;
        if (i2 != _storage.begin())
        {
          const auto& d2 = (i2->ts > ts ? i2->ts - ts : ts - i2->ts);
          if (d2 < d1)
            i = i2;
        }
        return i; // found
      }
      else
      {
        if (_storage.size() > 0)
          return _storage.end() - 1;

        return _storage.end(); // not found
      }
    }

    template <typename timeT>
    auto get(const timeT& target_ts, const decltype(timeT() - timeT())& max_ts_diff)
    {
      iterator closest = get(target_ts);

      result<iterator, decltype(timeT() - timeT())> res;
      res.iterator = closest;

      if (closest == _storage.end())
      {
        res.status = storage_acces_status::not_enough_elements;
        return res;
      }

      auto ts_diff  = target_ts >= closest->ts ? target_ts - closest->ts : closest->ts - target_ts;
      res.time_diff = ts_diff;

      if (ts_diff > max_ts_diff)
      {
        res.status = storage_acces_status::timestamp_diff_larger_then_thresh;
        return res;
      }

      res.status = storage_acces_status::success;

      return res;
    }

    template <typename timeT>
    bool in_range(const timeT& target_ts)
    {
      return _storage.size() > 2 && target_ts <= (_storage.end() - 1)->ts && target_ts >= (_storage.begin())->ts;
    }

    template <typename timeT>
    value_type get_data_inter(const iterator& iter, const timeT& target_ts)
    {
      if (iter->ts > target_ts)
      {
        if (iter == _storage.begin())
          return *iter;

        iterator prev_iter = iter - 1;

        double range = (iter->ts - prev_iter->ts).count();

        double w1 = double((target_ts - prev_iter->ts).count()) / range;
        double w0 = double((iter->ts - target_ts).count()) / range;

        value_type d0 = *prev_iter;
        value_type d1 = *iter;

        return interpolate_data(d0, w0, d1, w1);
      }
      else if (iter->ts < target_ts)
      {
        if (iter == _storage.end() - 1)
          return *iter;

        iterator next_iter = iter + 1;

        double range = (next_iter->ts - iter->ts).count();

        double w1 = double((target_ts - iter->ts).count()) / range;
        double w0 = double((next_iter->ts - target_ts).count()) / range;

        value_type d0 = *iter;
        value_type d1 = *next_iter;

        return interpolate_data(d0, w0, d1, w1);
      }
      else
        return *iter;
    }

  private:
    starageT& _storage;
  };

} // namespace daqu
