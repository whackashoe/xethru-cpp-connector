#include <iostream>
#include "../include/xethru.hpp"
#include "example_util.hpp"

int main(int argc, char ** argv)
{
    std::cout << "starting xethru sensor" << std::endl;
    xethru::xethru sensor(
        "/dev/ttyUSB0"
      , xethru::XTS_ID_APP_RESP
      , { 0.5, 0.7 }
      , xethru::XT_UI_LED_MODE_FULL
      , 30
      , xethru::verbosity::NONE
    );

    if(sensor.init()) {
        std::uint8_t state = 0;

        while(true) {
            const xethru::respiration_data status = sensor.check_respiration_status();
        
            if(status.ready) {
                switch(status.state_code) {
                case xethru::XTS_VAL_RESP_STATE_BREATHING:
                    std::cout << "breathing at " << status.state_data << "RPM (" << std::setprecision(2) << status.movement << "mm)." << " @ " << pretty_time() << std::endl;
                    state = xethru::XTS_VAL_RESP_STATE_BREATHING;
                    break;
                case xethru::XTS_VAL_RESP_STATE_MOVEMENT:
                    if(state != xethru::XTS_VAL_RESP_STATE_MOVEMENT) {
                        std::cout << "movement @ " << pretty_time() << std::endl;
                        state = xethru::XTS_VAL_RESP_STATE_MOVEMENT;
                    } break;
                case xethru::XTS_VAL_RESP_STATE_MOVEMENT_TRACKING:
                    if(state != xethru::XTS_VAL_RESP_STATE_MOVEMENT_TRACKING) {
                        std::cout << "movement tracking @ " << pretty_time() << std::endl;
                        state = xethru::XTS_VAL_RESP_STATE_MOVEMENT_TRACKING;
                    } break;
                case xethru::XTS_VAL_RESP_STATE_NO_MOVEMENT:
                    if(state != xethru::XTS_VAL_RESP_STATE_NO_MOVEMENT) {
                        std::cout << "no movement @ " << pretty_time() << std::endl;
                        state = xethru::XTS_VAL_RESP_STATE_NO_MOVEMENT;
                    } break;
                case xethru::XTS_VAL_RESP_STATE_INTIALIZING:
                    if(state != xethru::XTS_VAL_RESP_STATE_INTIALIZING) {
                        std::cout << "initializing..." << std::endl;
                        state = xethru::XTS_VAL_RESP_STATE_INTIALIZING;
                    } break;
                default:
                    if(state != xethru::XTS_VAL_RESP_STATE_UNKNOWN) {
                        std::cout << "unknown" << std::endl;
                        state = xethru::XTS_VAL_RESP_STATE_UNKNOWN;
                    } break;
                }
            }
        }
    }

    return 0;
}

