#include <iostream>
#include "../include/xethru.hpp"
#include "example_util.hpp"

int main(int argc, char ** argv)
{
    std::cout << "starting xethru sensor" << std::endl;
    xethru::xethru sensor(
        "/dev/ttyUSB0"
      , xethru::XTS_ID_APP_PRESENCE
      , {0.5, 1.3}
      , xethru::XT_UI_LED_MODE_FULL
      , 30
      , xethru::verbosity::DEBUG);

    if(sensor.init()) {
        std::uint8_t state = 0;

        while(true) {
            const xethru::presence_data status = sensor.check_presence_status();
        
            if(status.ready) {
                switch(status.presence) {
                case xethru::XETHRU_PRES_NODETECT:
                    if(state != xethru::XETHRU_PRES_NODETECT) {
                        std::cout << "no presence detected @ " << pretty_time() << std::endl;
                        state = xethru::XETHRU_PRES_NODETECT;
                    } break;
                case xethru::XETHRU_PRES_DETECTED:
                    if(state != xethru::XETHRU_PRES_DETECTED) {
                        std::cout << "presence detected @ " << pretty_time() << std::endl;
                        state = xethru::XETHRU_PRES_DETECTED;
                    } break;
                default:
                    if(state != xethru::XETHRU_PRES_UNKNOWN) {
                        std::cout << "unknown" << std::endl;
                        state = xethru::XETHRU_PRES_UNKNOWN;
                    } break;
                }
            }
        }
    }

    return 0;
}

