/*
 *  linux/include/linux/mtd/onenand.h
 *
 *  Copyright (C) 2005 Samsung Electronics
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __LINUX_MTD_ONENAND_H
#define __LINUX_MTD_ONENAND_H

#include <linux/mtd/onenand_regs.h>

/* Note: The header order is impoertant */
#include <onenand_uboot.h>

#include <linux/mtd/bbm.h>

#define MAX_BUFFERRAM		2
#define MAX_ONENAND_PAGESIZE	(2048 + 64)

/* Scan and identify a OneNAND device */
extern int onenand_scan(struct mtd_info *mtd, int max_chips);
/* Free resources held by the OneNAND device */
extern void onenand_release(struct mtd_info *mtd);

/**
 * onenand_state_t - chip states
 * Enumeration for OneNAND flash chip state
 */
typedef enum {
	FL_READY,
	FL_READING,
	FL_WRITING,
	FL_ERASING,
	FL_SYNCING,
	FL_UNLOCKING,
	FL_LOCKING,
} onenand_state_t;

/**
 * struct onenand_bufferram - OneNAND BufferRAM Data
 * @param block		block address in BufferRAM
 * @param page		page address in BufferRAM
 * @param valid		valid flag
 */
struct onenand_bufferram {
	int block;
	int page;
	int valid;
};

/**
 * struct onenand_chip - OneNAND Private Flash Chip Data
 * @param base		[BOARDSPECIFIC] address to access OneNAND
 * @param chipsize	[INTERN] the size of one chip for multichip arrays
 * @param device_id	[INTERN] device ID
 * @param verstion_id	[INTERN] version ID
 * @param options	[BOARDSPECIFIC] various chip options. They can partly be set to inform onenand_scan about
 * @param erase_shift	[INTERN] number of address bits in a block
 * @param page_shift	[INTERN] number of address bits in a page
 * @param ppb_shift	[INTERN] number of address bits in a pages per block
 * @param page_mask	[INTERN] a page per block mask
 * @param bufferam_index	[INTERN] BufferRAM index
 * @param bufferam	[INTERN] BufferRAM info
 * @param readw		[REPLACEABLE] hardware specific function for read short
 * @param writew	[REPLACEABLE] hardware specific function for write short
 * @param command	[REPLACEABLE] hardware specific function for writing commands to the chip
 * @param wait		[REPLACEABLE] hardware specific function for wait on ready
 * @param read_bufferram	[REPLACEABLE] hardware specific function for BufferRAM Area
 * @param write_bufferram	[REPLACEABLE] hardware specific function for BufferRAM Area
 * @param chip_lock	[INTERN] spinlock used to protect access to this structure and the chip
 * @param wq		[INTERN] wait queue to sleep on if a OneNAND operation is in progress
 * @param state		[INTERN] the current state of the OneNAND device
 * @param autooob	[REPLACEABLE] the default (auto)placement scheme
 * @param priv		[OPTIONAL] pointer to private chip date
 */
struct onenand_chip {
	void __iomem		*base;
	unsigned int		chipsize;
	unsigned int		device_id;
	unsigned int		options;

	unsigned int		erase_shift;
	unsigned int		page_shift;
	unsigned int		ppb_shift;	/* Pages per block shift */
	unsigned int		page_mask;

	unsigned int		bufferram_index;
	struct onenand_bufferram	bufferram[MAX_BUFFERRAM];

	int (*command)(struct mtd_info *mtd, int cmd, loff_t address, size_t len);
	int (*wait)(struct mtd_info *mtd, int state);
	int (*read_bufferram)(struct mtd_info *mtd, int area,
			unsigned char *buffer, int offset, size_t count);
	int (*write_bufferram)(struct mtd_info *mtd, int area,
			const unsigned char *buffer, int offset, size_t count);
	unsigned short (*read_word)(void __iomem *addr);
	void (*write_word)(unsigned short value, void __iomem *addr);
	void (*mmcontrol)(struct mtd_info *mtd, int sync_read);

	spinlock_t		chip_lock;
	wait_queue_head_t	wq;
	onenand_state_t		state;

	struct nand_oobinfo	*autooob;

	void			*bbm;

	void			*priv;
};

#define ONENAND_CURRENT_BUFFERRAM(this)		(this->bufferram_index)
#define ONENAND_NEXT_BUFFERRAM(this)		(this->bufferram_index ^ 1)
#define ONENAND_SET_NEXT_BUFFERRAM(this)	(this->bufferram_index ^= 1)

/*
 * Options bits
 */
#define ONENAND_CONT_LOCK		(0x0001)


/*
 * OneNAND Flash Manufacturer ID Codes
 */
#define ONENAND_MFR_SAMSUNG	0xec
#define ONENAND_MFR_UNKNOWN	0x00

/**
 * struct nand_manufacturers - NAND Flash Manufacturer ID Structure
 * @param name:		Manufacturer name
 * @param id:		manufacturer ID code of device.
*/
struct onenand_manufacturers {
        int id;
        char *name;
};

#endif	/* __LINUX_MTD_ONENAND_H */
