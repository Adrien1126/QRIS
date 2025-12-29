#pragma once

namespace qris::time {
    enum class DayCountConvention {
        ACT_365F, 
        ACT_360, 
        THIRTY_360_US,
        THIRTY_E_360
};
}