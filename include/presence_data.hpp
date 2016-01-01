#ifndef XETHRU_PRESENCE_DATA_HPP
#define XETHRU_PRESENCE_DATA_HPP

#include <cstdint>

namespace xethru {

struct presence_data
{
    std::uint8_t presence;
    float reserved1;
    float reserved2;
    std::uint32_t signal_quality;
    bool ready;

    presence_data()
    : presence(0)
    , reserved1(0)
    , reserved2(0)
    , signal_quality(0)
    , ready(false)
    {}

    presence_data(
        const std::uint8_t presence
      , const float reserved1
      , const float reserved2
      , const std::uint32_t signal_quality
    )
    : presence(presence)
    , reserved1(reserved1)
    , reserved2(reserved2)
    , signal_quality(signal_quality)
    , ready(true)
    {}
};

}

#endif

