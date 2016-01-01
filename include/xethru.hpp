#ifndef XETHRU_XETHRU_HPP
#define XETHRU_XETHRU_HPP

#include <chrono>
#include <iostream>
#include <cstdint>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <cassert>
#include <memory>
#include <utility>

#include <serial/serial.h>

#include "xethru_const.hpp"
#include "presence_data.hpp"
#include "respiration_data.hpp"

namespace xethru {

struct xethru
{
    const std::string comport;
    const uint32_t app_id;
    const float detection_zone_min;
    const float detection_zone_max;
    const uint8_t led_mode;
    const uint32_t response_timout;
    const verbosity verbose;

    std::unique_ptr<serial::Serial> serial_connection;

    float range_min, range_max;
    float span_min, span_max;

    xethru(
        const std::string & comport
      , const uint32_t app_id
      , const std::pair<float, float> detection_zone
      , const uint8_t led_mode
      , const uint32_t response_timout
      , const verbosity verbose
    )
    : comport(comport)
    , app_id(app_id)
    , detection_zone_min(detection_zone.first)
    , detection_zone_max(detection_zone.second)
    , led_mode(led_mode)
    , response_timout(response_timout)
    , verbose(verbose)
    {}

    struct xlog
    {
        int lvl;
        std::string str;

        xlog(
            const int lvl
          , const std::string & str)
        : lvl(lvl)
        , str(str)
        {}

        friend std::ostream& operator<<(std::ostream & os, const xlog & mp)
        {
            if(mp.lvl <= 0) {
                os << mp.str;
            }

            return os;
        }
    };

    bool init()
    {
        try {
            serial_connection = std::unique_ptr<serial::Serial>(new serial::Serial(
                comport
              , XETHRU_BAUD
              , serial::Timeout::simpleTimeout(1000)
            ));
        } catch(serial::IOException & e) {
            std::cerr << xlog(verbosity::ERR - verbose, std::string(e.what()) + "\n");
            return false;
        }

        if(! serial_connection->isOpen()) {
            std::cerr << xlog(verbosity::ERR - verbose, "serial connection not opened\n");
            return false;
        }

        std::cout << xlog(verbosity::STATUS - verbose, "resetting module...\n");
        if(! reset_module()) {
            std::cerr << xlog(verbosity::ERR - verbose, "reset module failed\n");
            return false;
        } else xlog(verbosity::STATUS - verbose, "ok!\n");

        switch(app_id) {
            case XTS_ID_APP_PRESENCE:
                range_min = XETHRU_PRES_MIN;
                range_max = XETHRU_PRES_MAX;
                span_min  = XETHRU_PRES_SPAN_MIN;
                span_max  = XETHRU_PRES_SPAN_MAX;
                break;
            case XTS_ID_APP_RESP:
                range_min = XETHRU_RESP_MIN;
                range_max = XETHRU_RESP_MAX;
                span_min  = XETHRU_RESP_SPAN_MIN;
                span_max  = XETHRU_RESP_SPAN_MAX;
                break;
            default:
                std::cerr << xlog(verbosity::ERR - verbose, "unknown app_id\n");
                return false;
        }

        std::cout << xlog(verbosity::STATUS - verbose, "loading application...\n");
        if(! load_application(app_id)) {
            std::cerr << xlog(verbosity::ERR - verbose, "load application failed\n");
            return false;
        } else std::cout << xlog(verbosity::STATUS - verbose, "ok!\n");

        std::cout << xlog(verbosity::STATUS - verbose, "setting led control...\n");
        if(! set_led_control(led_mode)) {
            std::cerr << xlog(verbosity::ERR - verbose, "set led control failed\n");
            return false;
        } else std::cout << xlog(verbosity::STATUS - verbose, "ok!\n");

        std::cout << xlog(verbosity::STATUS - verbose, "setting detection zone...\n");
        if(! set_detection_zone(detection_zone_min, detection_zone_max)) {
            std::cerr << xlog(verbosity::ERR - verbose, "set detection zone failed\n");
            return false;
        } else std::cout << xlog(verbosity::STATUS - verbose, "ok!\n");

        std::cout << xlog(verbosity::STATUS - verbose, "setting run mode...\n");
        if(! set_mode(XTS_SM_RUN)) {
            std::cerr << xlog(verbosity::ERR - verbose, "set mode failed\n");
            return false;
        } else std::cout << xlog(verbosity::STATUS - verbose, "ok!\n");

        std::cout << xlog(verbosity::STATUS - verbose, "ready!\n");

        return true;
    }

    respiration_data check_respiration_status()
    {
        assert(app_id == XTS_ID_APP_RESP);

        const std::vector<std::uint8_t> res = receive_response();
        if(    res.size() > 0
            && res[0] == XTS_SPR_APPDATA
            && get_int(std::vector<std::uint8_t>(std::begin(res) + 1, std::begin(res) + 5)) == XTS_ID_RESP_STATUS
        ) {
           return parse_respiration(std::vector<std::uint8_t>(std::begin(res) + 5, std::end(res)));
        }

        return respiration_data();
    }

    presence_data check_presence_status()
    {
        assert(app_id == XTS_ID_APP_PRESENCE);

        const std::vector<std::uint8_t> res = receive_response();
        if(    res.size() > 0
            && res[0] == XTS_SPR_APPDATA
            && get_int(std::vector<std::uint8_t>(std::begin(res) + 1, std::begin(res) + 5)) == XTS_ID_PRESENCE_STATUS
        ) {
           return parse_presence(std::vector<std::uint8_t>(std::begin(res) + 5, std::end(res)));
        }

        return presence_data();
    }

    respiration_data parse_respiration(const std::vector<std::uint8_t> & data) const
    {
        assert(data.size() == 24);

        return respiration_data(
            get_int(  std::vector<std::uint8_t>(std::begin(data) +  0, std::begin(data) +  4))
          , data[4]
          , get_int  (std::vector<std::uint8_t>(std::begin(data) +  8, std::begin(data) + 12))
          , get_float(std::vector<std::uint8_t>(std::begin(data) + 12, std::begin(data) + 16))
          , get_float(std::vector<std::uint8_t>(std::begin(data) + 16, std::begin(data) + 20))
          , get_int  (std::vector<std::uint8_t>(std::begin(data) + 20, std::begin(data) + 24))
        );
    }

    presence_data parse_presence(const std::vector<std::uint8_t> & data) const
    {
        assert(data.size() == 16);

        return presence_data(
            data[0]
          , get_float(std::vector<std::uint8_t>(std::begin(data) +  4, std::begin(data) +  8))
          , get_float(std::vector<std::uint8_t>(std::begin(data) +  8, std::begin(data) + 12))
          , get_int(  std::vector<std::uint8_t>(std::begin(data) + 12, std::begin(data) + 16))
        );
    }

    bool reset_module()
    {
        transmit_command({ XTS_SPC_MOD_RESET });

        while(true) {
            const std::vector<std::uint8_t> res = receive_response();

            if(res.size() == 5) {
                if(res[0] == XTS_SPR_SYSTEM && res[1] == XTS_SPRS_READY) {
                    return true;
                } else continue;
            } else return false;
        }
    }

    bool check_ack() 
    {
        const std::vector<std::uint8_t> res = receive_response();
        return res.size() > 0 && res[0] == XTS_SPR_ACK;
    }

    bool load_application(const std::uint32_t app_id)
    {
        std::vector<std::uint8_t> data = { XTS_SPC_MOD_LOADAPP };
        append_int(data, app_id);
        transmit_command(data);

        return check_ack();
    }

    bool set_mode(const std::uint8_t mode)
    {
        transmit_command({ XTS_SPC_MOD_SETMODE, mode });

        // weird dummy data
        if(check_ack()) {
            return true;
        }

        // actual response
        return check_ack();
    }

    bool set_led_control(const std::uint8_t mode)
    {
        transmit_command({ XTS_SPC_MOD_SETLEDCONTROL, mode });
        return check_ack();
    }

    bool set_detection_zone(float min, float max)
    {
        if(min < range_min) {
            min = range_min;
        } else if(min > (range_max - span_min)) {
            min = range_max - span_min;
        }

        if(max > (min + range_max)) {
            max = min + range_max;
        }

        if(max > range_max) {
            max = range_max;
        } else if(max < (min + span_min)) {
            max = min + span_min;
        }

        std::vector<std::uint8_t> data = { XTS_SPC_APPCOMMAND, XTS_SPCA_SET };
        append_int(data, XTS_ID_DETECTION_ZONE);
        append_float(data, min);
        append_float(data, max);

        transmit_command(data);

        return check_ack();
    }

    void transmit_command(std::vector<std::uint8_t> data)
    {
        data = add_break_characters(data, XETHRU_ESC);
        data = add_break_characters(data, XETHRU_START);
        data = add_break_characters(data, XETHRU_END);

        data.insert(std::begin(data), XETHRU_START);
        data.push_back(calculate_checksum(data));
        data.push_back(XETHRU_END);

        if(verbose >= verbosity::DEBUG) {
            std::cout << "transmitting: ";
            for(std::uint8_t c : data) {
                std::cout << "0x" << hexify(c >> 4) << hexify(c & 0x0f) << " ";
            }
            std::cout << std::endl;
        }

        serial_connection->write(std::string(std::begin(data), std::end(data)));
    }

    std::vector<std::uint8_t> receive_response()
    {
        auto seconds_since_epoch = []() -> std::uint64_t
        {
            return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count();
        };

        const std::uint64_t packet_timeout = seconds_since_epoch() + response_timout;
        std::vector<std::uint8_t> data;

        while(true) {
            if(seconds_since_epoch() > packet_timeout) {
                std::cerr << "timed out" << std::endl;
                return {};
            }

            std::string res = serial_connection->read(1);
            if(res.length() == 0) {
                continue;
            }

            const std::uint8_t byte = static_cast<uint8_t>(res[0]);

            if(byte == XETHRU_START) {
                data.push_back(byte);
                break;
            }
        }

        bool break_received = false;
        while(true) {
            if(seconds_since_epoch() > packet_timeout) {
                std::cerr << "timed out" << std::endl;
                return {};
            }

            std::string res = serial_connection->read(1);
            if(res.length() == 0) {
                continue;
            }

            const std::uint8_t byte = static_cast<uint8_t>(res[0]);

            if(break_received) {
                data.push_back(byte);
                break_received = false;
            } else {
                switch(byte) {
                    case XETHRU_ESC:
                        break_received = true;
                        break;
                    case XETHRU_END:
                        goto packet_end;
                        break;
                    default:
                        data.push_back(byte);
                        break;
                }
            }
        } packet_end:

        if(calculate_checksum(data)) {
            return {}; // checksum doesn't match
        }

        if(verbose >= verbosity::DEBUG) {
            std::cout << "received: ";

            for(std::uint8_t c : data) {
                std::cout << "0x" << hexify(c >> 4) << hexify(c & 0x0f) << " ";
            }

            std::cout << std::endl;
        }

        return std::vector<std::uint8_t>(std::begin(data) + 1, std::end(data) - 1);
    }

    void append_int(std::vector<std::uint8_t> & data, const std::uint32_t val) const
    {
        data.push_back((val >> 0)  & 0xff);
        data.push_back((val >> 8)  & 0xff);
        data.push_back((val >> 16) & 0xff);
        data.push_back((val >> 24) & 0xff);
    }

    void append_float(std::vector<std::uint8_t> & data, const float val) const
    {
        std::uint32_t v;
        memcpy(&v, &val, sizeof(v));

        data.push_back((v >> 0)  & 0xff);
        data.push_back((v >> 8)  & 0xff);
        data.push_back((v >> 16) & 0xff);
        data.push_back((v >> 24) & 0xff);
    }

    std::uint32_t get_int(const std::vector<std::uint8_t> & data) const
    {
        assert(data.size() == sizeof(std::uint32_t));

        return (data[0] << 0)
             + (data[1] << 8)
             + (data[2] << 16)
             + (data[3] << 24);
    }

    float get_float(const std::vector<std::uint8_t> & data) const
    {
        assert(data.size() == sizeof(float));

        float ret;
        memcpy(&ret, &data, sizeof(ret));
        return ret;
    }

    std::vector<std::uint8_t> add_break_characters(const std::vector<uint8_t> & data, const uint8_t flag) const
    {
        std::vector<uint8_t> ret = data;

        for(std::size_t i=0; i<ret.size(); ++i) {
            const auto it = std::find(std::begin(ret) + i, std::end(ret), flag);

            if(it != ret.end()) {
                i = it - std::begin(ret); // get index of iterator
                ret.insert(it, XETHRU_ESC);
                ++i;
            } else {
                break;
            }
        }

        return ret;
    }

    uint8_t calculate_checksum(const std::vector<uint8_t> & data) const
    {
        return std::accumulate(std::begin(data), std::end(data), 0, std::bit_xor<uint8_t>());
    }

    char hexify(const uint8_t v) const
    {
        return (v < 10) ? ('0' + v) : ('a' + v - 10);
    };

};

}

#endif

