#pragma once

namespace daqu
{
  struct stamped_data_category_tag {};

  template <typename dataT, typename timeT>
  struct stamped_data
  {
    typedef stamped_data_category_tag stamped_data_category;

    typedef dataT                     data_value_type;
    typedef timeT                     time_value_type;

    stamped_data() : data(dataT()), ts(timeT()) {}
    stamped_data(const dataT& d, const timeT& t) : data(d), ts(t) {}

    data_value_type data;
    time_value_type ts;
  };

} // namespace daqu
