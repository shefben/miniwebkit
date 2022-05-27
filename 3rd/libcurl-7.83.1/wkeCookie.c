#include "lib/urldata.h"
#include "lib/cookie.h"

/*wke++++++*/
long curl_cookies_count(struct Curl_easy *data)
{
    if (data->cookies == NULL)
        return 0;

    return data->cookies->numcookies;
}

struct Cookie* curl_first_cookie(struct Curl_easy *data)
{
    if (data->cookies)
        return data->cookies->cookies;

    return NULL;
}

struct Cookie* curl_next_cookie(struct Cookie *c)
{
    return c->next;
}

const char* curl_cookie_name(struct Cookie *c)
{
    return c->name;
}

const char* curl_cookie_value(struct Cookie *c)
{
    return c->value;
}

const char* curl_cookie_domain(struct Cookie *c)
{
    return c->domain;
}

const char* curl_cookie_path(struct Cookie *c)
{
    return c->path;
}

bool curl_cookie_secure(struct Cookie *c)
{
    return c->secure;
}

curl_off_t curl_cookie_expires(struct Cookie *c)
{
    return c->expires;
}

struct Cookie* curl_cookie_add(
    struct Curl_easy *data,
    const char* value,
    bool httpheader, /* TRUE if HTTP header-style line */
    bool noexpire, /* if TRUE, skip remove_expired() */
    const char* domain,
    const char* path,
    bool secure)
{
    if(!data->cookies)
        data->cookies = Curl_cookie_init(data, NULL, NULL, TRUE);

    return Curl_cookie_add(data, data->cookies, httpheader, noexpire, (char*)value, domain, path, secure);
}
/*wke++++++*/
