#ifndef PAGE_SETTINGS_H
#define PAGE_SETTINGS_H

typedef enum
{
    WKE_PROXY_NONE,
    WKE_PROXY_HTTP,
    WKE_PROXY_SOCKS4,
    WKE_PROXY_SOCKS4A,
    WKE_PROXY_SOCKS5,
    WKE_PROXY_SOCKS5HOSTNAME

} wkeProxyType;
typedef struct 
{
    wkeProxyType type;
    char hostname[100];
    unsigned short port;
    char username[50];
    char password[50];
} wkeProxy;
enum wkeSettingMask 
{
    WKE_SETTING_PROXY = 1,
    WKE_SETTING_COOKIE_FILE_PATH = 1<<1
};
namespace wke {
    class wkeSettings
    {
        public:
            wkeSettings(): proxy(nullptr),
                cookieFilePath(nullptr),
                mask(0),
                pageScaleFactor(1.0f) {};
        public:
            wkeProxy* proxy;
            char* cookieFilePath;
            unsigned int mask;
            float pageScaleFactor;
    };
    class wkeSettingsManeger {
        public:
            static wkeSettings* Instance();
            static void SetInstance(wkeSettings* i);
    };
}

#endif
