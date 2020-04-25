#include "circular_queue.h"

void circular_queue8_init(struct circular_queue8* queue, uint8_t* buf, size_t size)
{
    queue->buf = buf;
    queue->first = buf;
    queue->last = buf;
    queue->size = size;
    queue->empty = true;
}

bool circular_queue8_put(struct circular_queue8* queue, uint8_t data)
{
    if (queue->empty || queue->first != queue->last)
    {
        *queue->last++ = data;
        if (queue->last >= queue->buf + queue->size)
            queue->last = queue->buf;

        queue->empty = false;
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t circular_queue8_get(struct circular_queue8* queue, bool peek, bool* success)
{
    if (!queue->empty)
    {
        uint8_t data = *queue->first;
        if (!peek)
        {
            if (++queue->first >= queue->buf + queue->size)
                queue->first = queue->buf;
            if (queue->first == queue->last)
                queue->empty = true;
        }
        if (success)
            *success = true;
        return data;
    }
    else
    {
        if (success)
            *success = false;
        return 0;
    }
}

bool circular_queue8_remove(struct circular_queue8* queue)
{
    if (!queue->empty)
    {
        if (++queue->first >= queue->buf + queue->size)
            queue->first = queue->buf;
        if (queue->first == queue->last)
            queue->empty = true;

        return true;
    }
    else
    {
        return false;
    }
}
