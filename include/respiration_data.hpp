#ifndef XETHRU_RESPIRATION_DATA_HPP
#define XETHRU_RESPIRATION_DATA_HPP

#include <cstdint>

namespace xethru {

struct respiration_data
{
    std::uint32_t counter;
    std::uint8_t state_code;
    std::uint32_t state_data;
    float distance;
    float movement;
    uint32_t signal_quality;
    bool ready;

    respiration_data()
    : counter(0)
    , state_code(0)
    , state_data(0)
    , distance(0)
    , movement(0)
    , signal_quality(0)
    , ready(false)
    {}

    respiration_data(
        std::uint32_t counter
      , std::uint8_t state_code
      , std::uint32_t state_data
      , float distance
      , float movement
      , uint32_t signal_quality
    )
    : counter(counter)
    , state_code(state_code)
    , state_data(state_data)
    , distance(distance)
    , movement(movement)
    , signal_quality(signal_quality)
    , ready(true)
    {}
};

}

#endif

