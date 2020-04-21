#pragma once

namespace daqu
{
  template <typename dataT, typename timeT>
  struct stamped_data
  {
    stamped_data(const dataT& d, const timeT& t) : data(d), ts(t) {}
    stamped_data() : data(dataT()), ts(timeT()) {}

    dataT data;
    timeT ts;
  };

} // namespace daqu
