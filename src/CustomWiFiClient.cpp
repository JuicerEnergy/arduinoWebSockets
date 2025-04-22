/**
 * Juicer Energy Inc. changed
 */
// Wifi Client custom to fix issue with connected() function
#include <Arduino.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <errno.h>
#include <CustomWiFiClient.h>

/**
 * Overridden to fix bug in WiFiClient
 */
uint8_t CustomWiFiClient::connected() {
    if(_connected) {
        uint8_t dummy;
        // this is more reliable, since we will get a 0 or -1 even if there is no data available
        // and hence the errno will be properly set
        int res = recv(fd(), &dummy, 1, MSG_DONTWAIT|MSG_PEEK);
        // int res = recv(fd(), &dummy, 0, MSG_DONTWAIT);
        // avoid unused var warning by gcc
        (void)res;
        // recv only sets errno if res is <= 0
        if(res <= 0) {
            switch(errno) {
                case EWOULDBLOCK:
                case ENOENT:    // caused by vfs
                    _connected = true;
                    break;
                case ENOTCONN:
                case EPIPE:
                case ECONNRESET:
                case ECONNREFUSED:
                case ECONNABORTED:
                    _connected = false;
                    log_d("Disconnected: RES: %d, ERR: %d", res, errno);
                    break;
                default:
                    log_i("Unexpected: RES: %d, ERR: %d", res, errno);
                    _connected = true;
                    break;
            }
        } else {
            _connected = true;
        }
    }
    return _connected;
}
