//
// VESA BIOS Extension definitions
//
#ifndef VGA_H
#define VGA_H

#define VESA_ADDR(ptr) (((ptr) & 0xFFFF) | ((ptr) >> 12))

#pragma pack(push, 1)

// VGA driver ioctl codes

#define IOCTL_VGA_GET_VIDEO_MODE   1024
#define IOCTL_VGA_GET_FRAME_BUFFER 1025

// VESA BIOS extension information

struct vesa_info {
    unsigned char vesa_signature[4];
    unsigned short vesa_version;
    unsigned long oem_string_ptr;
    unsigned char capabilities[4];
    unsigned long video_mode_ptr;
    unsigned short total_memory;
    unsigned short oem_software_rev;
    unsigned long oem_vendor_name_ptr;
    unsigned long oem_product_name_ptr;
    unsigned long oem_product_rev_ptr;
    unsigned char reserved[222];
    unsigned char oem_data[256];
};

// VESA mode information

struct vesa_mode_info {
    unsigned short attributes;
    unsigned char wina_attributes;
    unsigned char winb_attributes;
    unsigned short win_granularity;
    unsigned short win_size;
    unsigned short wina_segment;
    unsigned short winb_segment;
    unsigned long win_func_ptr;
    unsigned short bytes_per_scanline;

    unsigned short x_resolution;
    unsigned short y_resolution;
    unsigned char x_char_size;
    unsigned char y_char_size;
    unsigned char number_of_planes;
    unsigned char bits_per_pixel;
    unsigned char number_of_banks;
    unsigned char memory_model;
    unsigned char bank_size;
    unsigned char number_of_image_pages;
    unsigned char reserved_page;

    unsigned char red_mask_size;
    unsigned char red_mask_pos;
    unsigned char green_mask_size;
    unsigned char green_mask_pos;
    unsigned char blue_mask_size;
    unsigned char blue_mask_pos;
    unsigned char reserved_mask_size;
    unsigned char reserved_mask_pos;
    unsigned char direct_color_mode_info;

    unsigned long phys_base_ptr;
    unsigned long off_screen_mem_offset;
    unsigned short off_screen_mem_size;

    unsigned char reserved[206];
};

#pragma pack(pop)

#endif
