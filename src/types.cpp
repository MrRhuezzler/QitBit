#include <types.h>

breep::serializer& operator<<(breep::serializer& s, Request value) {
    s << static_cast<unsigned char>(value);
    return s;
}

breep::deserializer& operator>>(breep::deserializer& d, Request& value) {
    unsigned char extracted;
    d >> extracted;
    value = static_cast<Request>(extracted);
    return d;
}
