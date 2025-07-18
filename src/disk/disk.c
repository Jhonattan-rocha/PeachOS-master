#include "disk.h"
#include "io/io.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"

struct disk disk;

int disk_read_sector(int lba, int total, void* buf)
{
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20);

    unsigned short* ptr = (unsigned short*) buf;
    for (int b = 0; b < total; b++)
    {
        // Wait for the buffer to be ready
        char c = insb(0x1F7);
        while(!(c & 0x08))
        {
            c = insb(0x1F7);
        }

        // Copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(0x1F0);
            ptr++;
        }

    }
    return 0;
}

void disk_search_and_init()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = PEACHOS_DISK_TYPE_REAL;
    disk.sector_size = PEACHOS_SECTOR_SIZE;
    disk.id = 0;
    disk.filesystem = fs_resolve(&disk);
}

struct disk* disk_get(int index)
{
    if (index != 0)
        return 0;
    
    return &disk;
}

int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf)
{
    if (idisk != &disk)
    {
        return -EIO;
    }

    return disk_read_sector(lba, total, buf);
}

int disk_write_sector(int lba, int total, void *buf) {
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xFF));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));

    unsigned char status = insb(0x1F7);
    while ((status & 0x80) || !(status & 0x40))  // bit 7: Busy (BSY), bit 6: Ready (DRDY)
    {
        status = insb(0x1F7);
    }

    outb(0x1F7, 0x30); // Comando write (0x30)

    unsigned short *ptr = (unsigned short *)buf;
    for (int b = 0; b < total; b++) {
        // Aguarde o disco ficar pronto
        char c = insb(0x1F7);
        while (!(c & 0x08)) {
            c = insb(0x1F7);
        }

        // Copie os dados da memória para o buffer do disco
        for (int i = 0; i < 256; i++) {
            outw(0x1F0, *ptr);
            ptr++;
        }

        // Aguarde a conclusão da operação de gravação
        int retry = 100000;
        c = insb(0x1F7);
        while (!(c & 0x08) && !(c & 0x01) && retry--) {
            c = insb(0x1F7);
        }
        
        if (retry <= 0 && c & 0x01) {
            return -EIO;
        }

        // Verifique se houve erro
        if (c & 0x01) {
            return -EIO;
        }
    }
    
    // Enviar comando FLUSH CACHE (depois de escrever tudo)
    outb(0x1F7, 0xE7);

    // Esperar flush
    int retry = 100000;
    status = insb(0x1F7);
    while ((status & 0x80) && retry--) {
        status = insb(0x1F7);
    }

    if (retry <= 0) {
        return -EIO;
    }

    return 0;
}

int disk_write_block(struct disk *idisk, unsigned int lba, int total, void *buf) {
    if (idisk != &disk) {
        return -EIO;
    }

    return disk_write_sector(lba, total, buf);
}