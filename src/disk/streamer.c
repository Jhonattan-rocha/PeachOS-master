#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

#include <stdbool.h>
struct disk_stream* diskstreamer_new(int disk_id)
{
    struct disk* disk = disk_get(disk_id);
    if (!disk)
    {
        return 0;
    }

    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
}

int diskstreamer_seek(struct disk_stream* stream, int pos)
{
    stream->pos = pos;
    return 0;
}

int diskstreamer_read(struct disk_stream* stream, void* out, int total)
{
    int sector = stream->pos / PEACHOS_SECTOR_SIZE;
    int offset = stream->pos % PEACHOS_SECTOR_SIZE;
    int total_to_read = total;
    bool overflow = (offset+total_to_read) >= PEACHOS_SECTOR_SIZE;
    char buf[PEACHOS_SECTOR_SIZE];

    if (overflow)
    {
        total_to_read -= (offset+total_to_read) - PEACHOS_SECTOR_SIZE;
    }

    int res = disk_read_block(stream->disk, sector, 1, buf);
    if (res < 0)
    {
        goto out;
    }

   
    for (int i = 0; i < total_to_read; i++)
    {
        *(char*)out++ = buf[offset+i];
    }

    // Adjust the stream
    stream->pos += total_to_read;
    if (overflow)
    {
        res = diskstreamer_read(stream, out, total-total_to_read);
    }
out:
    return res;
}

int diskstreamer_write(struct disk_stream* stream, void* in, int total)
{
    int sector = stream->pos / PEACHOS_SECTOR_SIZE;
    int offset = stream->pos % PEACHOS_SECTOR_SIZE;

    int total_to_write = total;
    bool overflow = (offset + total_to_write) >= PEACHOS_SECTOR_SIZE;
    char buf[PEACHOS_SECTOR_SIZE];

    if (overflow) {
        total_to_write -= (offset + total_to_write) - PEACHOS_SECTOR_SIZE;
    }

    // 1. Read the current sector data if necessary
    int res = 0;
    if (offset > 0 || total_to_write < PEACHOS_SECTOR_SIZE) {
        res = disk_read_block(stream->disk, sector, 1, buf);
        if (res < 0) {
            goto out;
        }
    }
    
    // 2. Modify the buffer with the data to write
    for (int i = 0; i < total_to_write; i++) {
        buf[offset + i] = *(char*)in++;
    }

    // 3. Write the modified sector back to disk
    res = disk_write_block(stream->disk, sector, 1, buf);
    if (res < 0) {
        goto out;
    }

    // 4. Update the stream position
    stream->pos += total_to_write;

    // 5. Handle overflow (write to the next sector)
    if (overflow) {
        res = diskstreamer_write(stream, in, total - total_to_write);
    }
out:
    return res;
}

void diskstreamer_close(struct disk_stream* stream)
{
    kfree(stream);
}