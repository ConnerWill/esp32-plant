/*
 * File: ./bitmap.h
 */

#pragma once

#ifndef BITMAP_H
#define BITMAP_H

// BAYER COLORS
// Bitmap data for the mustang icon (128x64px)
const unsigned char bitmap_image[] PROGMEM = {
  // 'mustang-resized', 128x64px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xef, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x80, 0x12, 0xab, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x95, 0x40, 0x00, 0x0b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf5, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xda, 0x40, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 
  0xdd, 0xdd, 0xdd, 0xdd, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x68, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfb, 0xdd, 0xdf, 
  0xff, 0xff, 0xff, 0xfe, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xf7, 0x77, 0x77, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf9, 0x91, 0x00, 0x10, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf7, 0x22, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x7f, 0xff, 0xff, 0xd6, 0x60, 0x00, 0x10, 0x00, 0x00, 0x01, 0xff, 0xef, 0xff, 0xff, 0xff, 0xfd, 
  0xfd, 0x03, 0x77, 0x5c, 0x84, 0x00, 0x20, 0x00, 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfa, 0x78, 0xbf, 0xbd, 0x88, 0x00, 0x20, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x04, 0x06, 0x71, 0x00, 0x00, 0x30, 0x00, 0x00, 0x01, 0x3f, 0xef, 0xff, 0xff, 0xff, 0xef, 
  0xf8, 0x00, 0x5b, 0xf6, 0x10, 0x00, 0x20, 0x00, 0x00, 0x00, 0x9d, 0x7f, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x00, 0x00, 0x04, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x49, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf0, 0x00, 0x00, 0x08, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x2a, 0x0b, 0xff, 0xff, 0xff, 0x7f, 
  0xe0, 0x00, 0x00, 0x18, 0x41, 0x29, 0x41, 0x00, 0x80, 0x00, 0x00, 0x00, 0x17, 0xff, 0xff, 0xfd, 
  0xf0, 0x00, 0x88, 0x50, 0x00, 0x00, 0x00, 0x00, 0x09, 0x24, 0x10, 0x04, 0x40, 0xbf, 0xff, 0xff, 
  0xe5, 0xff, 0xff, 0xff, 0xda, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2a, 0x05, 0xfb, 0xff, 
  0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x00, 0x00, 0x02, 0x00, 0x00, 0xf4, 0xbf, 0xef, 
  0xc1, 0xff, 0xff, 0x81, 0x7f, 0xff, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x90, 0x00, 0x05, 0x5f, 0xff, 
  0xc1, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xe0, 0xff, 0xc9, 0x55, 0xba, 0x4b, 0x48, 0x00, 0x2b, 0xff, 
  0xc1, 0x5d, 0xf8, 0x28, 0x0f, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0x03, 0xfe, 
  0xc0, 0x00, 0x11, 0x96, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0x3f, 
  0xe0, 0x00, 0x86, 0x00, 0x3e, 0x20, 0xad, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xfd, 0x1f, 
  0xe0, 0x00, 0x08, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x17, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xef, 
  0xf5, 0x00, 0x10, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xbf, 0xee, 0xf1, 0xff, 0xef, 
  0xff, 0xff, 0xe0, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0xf0, 0x0d, 0xbf, 0xc7, 
  0x0a, 0xdf, 0xe0, 0x16, 0x01, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0xc0, 0x02, 0x8f, 0x87, 
  0x00, 0x00, 0x00, 0x3f, 0x80, 0xff, 0xff, 0xaa, 0x80, 0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x07, 
  0x00, 0x00, 0x00, 0xff, 0xc0, 0xbf, 0xff, 0xff, 0xff, 0xfe, 0xea, 0xa1, 0x00, 0x00, 0x80, 0x0f, 
  0x80, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x55, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x80, 0x6a, 0xff, 
  0x80, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xab, 0x7e, 0x03, 0xf0, 0x7f, 0xff, 
  0x80, 0x00, 0x01, 0xf5, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x82, 
  0xc0, 0x00, 0x03, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x01, 
  0xe5, 0x80, 0x03, 0xe0, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x01, 
  0xe0, 0x00, 0x03, 0xc0, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x03, 
  0xf0, 0x00, 0x03, 0xc0, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x3e, 0x00, 0x03, 
  0xff, 0xc0, 0x03, 0xe0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1e, 0x00, 0x07, 
  0xff, 0xfd, 0x23, 0xe0, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1e, 0x00, 0x1f, 
  0xff, 0xff, 0xf3, 0xf1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1e, 0x00, 0x3f, 
  0xff, 0xff, 0xf1, 0xff, 0xf0, 0x5f, 0xff, 0xf7, 0x6d, 0xaa, 0x80, 0x00, 0x3c, 0x3e, 0x00, 0x3f, 
  0xff, 0xff, 0xf9, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf5, 0x55, 0x1e, 0x3c, 0x55, 0x7f, 
  0xff, 0xff, 0xf8, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x7f, 0x83, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xfc, 0xff, 0xff, 
  0x77, 0x7f, 0xfe, 0x3f, 0x47, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xf8, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf1, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xe3, 0xff, 0xfb, 
  0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbd, 0xf0, 0x0f, 0xff, 0xff, 
  0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x2f, 0xff, 0xdf, 
  0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x77, 0x77, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff
};

#endif
//vim: filetype=arduino:shiftwidth=2:softtabstop=2:expandtab:nowrap:cursorline:cursorcolumn:number:relativenumber