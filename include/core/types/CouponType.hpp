#pragma once

namespace qris::core {
    enum class CouponType {
        Fixed, 
        Floating, 
        Capped, 
        Floored, 
        Collared
    };
}