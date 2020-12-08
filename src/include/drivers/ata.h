//  based on
//      https://wiki.osdev.org/ATA_PIO_Mode#IDENTIFY_command
#pragma once
#include "io_bus.h"
#include "ints.h"

#define ide_write(ata_bus, val) out(ata_bus, val)
#define ide_read(ata_bus) in(ata_bus)

#define __PRIMARY_ATA 0x1F0
#define __SECONDARY_ATA 0x170


#define ATA_REG_DATA_RW(__ata)          __ata + 0
#define ATA_REG_ERROR_R(__ata)          __ata + 1
#define ATA_REG_FEAT_W(__ata)           __ata + 1
#define ATA_REG_SECTOR_COUNT_RW(__ata)  __ata + 2
#define ATA_REG_LBA_LO_RW(__ata)        __ata + 3
#define ATA_REG_LBA_MID_RW(__ata)       __ata + 4
#define ATA_REG_LBA_HI_RW(__ata)        __ata + 5
#define ATA_REG_DRIVER_RW(__ata)        __ata + 6
#define ATA_REG_STATUS_R(__ata)         __ata + 7
#define ATA_REG_COMMAND_W(__ata)        __ata + 7

#define ATA_SC_BSY       0x80
#define ATA_SC_DRDY      0x40
#define ATA_SC_DF        0x20
#define ATA_SC_DSC       0x10
#define ATA_SC_DRQ       0x08
#define ATA_SC_CORR      0x04
#define ATA_SC_IDX       0x02
#define ATA_SC_ERR       0x01

#define ATA_ER_BBK       0x80
#define ATA_ER_UNC       0x40
#define ATA_ER_MC        0x20
#define ATA_ER_IDNF      0x10
#define ATA_ER_MCR       0x08
#define ATA_ER_ABRT      0x04
#define ATA_ER_TK0NF     0x02
#define ATA_ER_AMNF      0x01


typedef enum ata_bus {
    PRIMARY_ATA = __PRIMARY_ATA,
    SECONDARY_ATA = __SECONDARY_ATA,
} ata_bus;

static inline u32 ata_select(ata_bus ata)
{
    if (ata == PRIMARY_ATA)
        ide_write(ATA_REG_DRIVER_RW(ata), 0xA0); //
    else if (ata == SECONDARY_ATA)
        ide_write(ATA_REG_DRIVER_RW(ata), 0xB0); //
    else return 1; // wrong target drive

    return 0; // no Error
}
static inline void ide_polling(ata_bus ata)
{
    ide_read(ATA_REG_STATUS_R(ata)); // 100 ns
    ide_read(ATA_REG_STATUS_R(ata)); // 100 ns
    ide_read(ATA_REG_STATUS_R(ata)); // 100 ns
    ide_read(ATA_REG_STATUS_R(ata));  // 100 ns

    while (ide_read(ATA_REG_STATUS_R(ata)) & ATA_SC_BSY); // Wait for BSY to be zero.
}
static inline u32 ata_identify(ata_bus ata)
{
    if (ata_select(ata))
        return 1;

    ide_write(ATA_REG_LBA_LO_RW(ata), 0);
    ide_write(ATA_REG_LBA_MID_RW(ata), 0);
    ide_write(ATA_REG_LBA_HI_RW(ata), 0);
    ide_write(ATA_REG_SECTOR_COUNT_RW(ata), 0);

    ide_write(ATA_REG_COMMAND_W(ata), 0xEC); // send the IDENTIFY DEVICE command

    ide_polling(ata);

    u8 state = ide_read(ATA_REG_STATUS_R(ata));

    if (state & ATA_SC_ERR) return 2; // Error.
    else if (state & ATA_SC_DF) return 3; // Device fault.
    else if (!(state & ATA_SC_DRQ)) return 4; // DRQ should be set

    return 0;
}