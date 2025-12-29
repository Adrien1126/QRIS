#include "time/dayCountConvention.hpp"

namespace qris::time {

double yearFraction(const Date& start,
                    const Date& end,
                    DayCountConvention convention)
{
    const int days = end.diff(start);

    switch (convention) {

    case DayCountConvention::ACT_365F:
        return static_cast<double>(days) / 365.0;

    case DayCountConvention::ACT_360:
        return static_cast<double>(days) / 360.0;

    case DayCountConvention::THIRTY_E_360: {
        int d1 = std::min(start.day(), 30);
        int d2 = std::min(end.day(),   30);

        int days360 =
              360 * (end.year()  - start.year())
            +  30 * (end.month() - start.month())
            + (d2 - d1);

        return static_cast<double>(days360) / 360.0;
    }

    case DayCountConvention::THIRTY_360_US: {
        int d1 = start.day();
        int d2 = end.day();

        int dayPart =
            std::max(30 - d1, 0)
            + std::min(d2, 30);

        int monthPart = 30 * (end.month() - start.month() - 1);
        int yearPart  = 360 * (end.year()  - start.year());

        return static_cast<double>(dayPart + monthPart + yearPart) / 360.0;
    }
    default:
        throw std::invalid_argument("Unsupported DayCountConvention");
    }
}

} // namespace qris::time
