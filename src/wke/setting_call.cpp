#include "setting_call.h"

namespace wke {
    static wkeSettings* __settings = nullptr;
    wkeSettings* wkeSettingsManeger::Instance() {
        return __settings;
    }
    void wkeSettingsManeger::SetInstance(wkeSettings* i) {
        __settings = i;
    }
}
