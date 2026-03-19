#include "sensehat.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

static uint16_t *fbp = NULL;
static int fb = -1;
static int width = 0;
static int height = 0;
static long screensize = 0;

int sense_init(const char *fb_device) {

    fb = open(fb_device, O_RDWR);
    if (fb < 0) {
        perror("Cannot open framebuffer");
        return -1;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);

    width = vinfo.xres;
    height = vinfo.yres;
    screensize = finfo.line_length * vinfo.yres;

    fbp = (uint16_t *)mmap(
        0, screensize,
        PROT_READ | PROT_WRITE,
        MAP_SHARED, fb, 0
    );

    if ((intptr_t)fbp == -1) {
        perror("mmap failed");
        close(fb);
        return -1;
    }

    return 0;
}

void sense_clear() {
    for (int i = 0; i < width * height; i++)
        fbp[i] = BLACK;
}

void sense_light_led(int index, uint16_t color) {
    int x = index % width;
    int y = index / width;
    int location = x + y * width;
    fbp[location] = color;
}

void sense_close() {
    if (fbp)
        munmap(fbp, screensize);
    if (fb >= 0)
        close(fb);
}