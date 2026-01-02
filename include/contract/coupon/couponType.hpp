#pragma once

namespace qris::contract {
    enum class CouponType {
        Fixed, 
        Floating, 
        Capped, 
        Floored, 
        Collared
    };
}