#include "array.h"
#include "string.h"
#include "assert.h"

void array8_init(struct array8* ar, uint8_t* buf, uint8_t* end)
{
    ar->buf = ar->now = buf;
    ar->end = end;
}

size_t array8_max_size(struct array8* ar)
{
    return ar->end - ar->buf;
}

size_t array8_size(struct array8* ar)
{
    return ar->now - ar->buf;
}

bool array8_insert(struct array8* ar, uint8_t* pos, uint8_t data)
{
    assert(ar->buf <= pos && pos < ar->now);

    if (ar->now == ar->end)
        return false;

    memmove(pos + 1, pos, (ar->now - pos) * sizeof(*pos));
    ar->now += 1;
    *pos = data;
    return true;
}

void array8_erase(struct array8* ar, uint8_t* pos)
{
    assert(ar->buf <= pos && pos < ar->now);

    memmove(pos, pos + 1, (ar->now - pos - 1) * sizeof(pos));
    ar->now -= 1;
}

void array8_append(struct array8* dest, const struct array8* src)
{
    uint8_t* d = dest->now;
    uint8_t* de = dest->end;
    uint8_t* s = src->buf;
    uint8_t* se = src->now;
    size_t len = min(se - s, de - d);

    memcpy(d, s, len);
    dest->now = d + len;
}

bool array8_pushback(struct array8* ar, uint8_t data)
{
    return array8_insert(ar, ar->now, data);
}

uint8_t array8_popback(struct array8* ar)
{
    assert(ar->now > ar->buf);
    uint8_t ret = *(ar->now - 1);
    array8_erase(ar, ar->now - 1);
    return ret;
}
