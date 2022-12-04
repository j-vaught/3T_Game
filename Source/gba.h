//gba.h

//240x160
//ARM7TDMI 16.78 MHz
//Color word is 16bits
//0bXbbbbbgggggrrrrr
//0x00000000 -> 0x00003fff (System ROM, exe but not readable)
//0x02000000 -> 0x02020000 (External RAM, 256 KB)
//0x03000000 -> 0x03007fff (Internal RAM, 32 KB)
//0x04000000 -> 0x04003fff (IO registers)
//0x05000000 -> 0x05003fff (Color Palette)
//0x06000000 -> 0x06017fff (VRAM 96 KB)
//0x07000000 -> 0x070003ff (OAM RAM)
//0x08000000 -> 0x???????? (Gamepak ROM up to 32MB)
//0x0E000000 -> 0x???????? (Gamepak RAM)

#define SCREEN_WIDTH            240
#define SCREEN_HEIGHT           160

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;

#define MEMIO                   0x04000000
#define VRAM                    0x06000000

#define VIDEOMODE               0x0003
#define BGMODE                  0x0400

#define REG_DISPLAY             (*(volatile uint32 *) (MEMIO))
#define REG_DISPLAY_VCOUNT      (*(volatile uint32 *) (MEMIO + 0x0006))
#define REG_DISPLAY_INPUT       (*(volatile uint32 *) (MEMIO + 0x0130))

#define A       1
#define B       2
#define SELECT  4
#define START   8
#define RIGHT   16
#define LEFT    32
#define UP      64
#define DOWN    128
#define R       256
#define L       512

#define SCREENBUFFER    ((volatile uint16 *) VRAM)

struct Rect {
  int x, y, w, h;
} x1, x2, y1, y2, winner, pixel, p1, p2, p3, p4, p5, p6, p7, p8, p9;