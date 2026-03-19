#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main() {
    const char *fb_path = "/dev/fb0";   // Changed from fb1 to fb0
    int fb = open(fb_path, O_RDWR);
    if (fb < 0) {
        perror("Error: cannot open /dev/fb0");
        return 1;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if (ioctl(fb, FBIOGET_FSCREENINFO, &finfo)) {
        perror("Error reading fixed info");
        close(fb);
        return 1;
    }

    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("Error reading variable info");
        close(fb);
        return 1;
    }

    printf("Framebuffer info:\n");
    printf("Resolution: %dx%d\n", vinfo.xres, vinfo.yres);
    printf("Bits per pixel: %d\n", vinfo.bits_per_pixel);

    long screensize = finfo.line_length * vinfo.yres;

    uint16_t *fbp = (uint16_t *)mmap(
        0,
        screensize,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fb,
        0
    );

    if ((intptr_t)fbp == -1) {
        perror("Error mapping framebuffer");
        close(fb);
        return 1;
    }

    // RGB565 colors
    uint16_t red   = 0xF800;
    uint16_t green = 0x07E0;
    uint16_t blue  = 0x001F;
    uint16_t white = 0xFFFF;
    uint16_t black = 0x0000;

    uint16_t colors[] = { red, green, blue, white };

    int width = vinfo.xres;
    int height = vinfo.yres;

    // Fill screen with solid colors
    for (int c = 0; c < 4; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                long location = x + y * width;
                fbp[location] = colors[c];
            }
        }
        sleep(1);
    }

    // Pixel-by-pixel test
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            long location = x + y * width;
            fbp[location] = red;
            usleep(1000);   // Faster for large HDMI screens
            fbp[location] = black;
        }
    }

    // Clear display
    for (int i = 0; i < width * height; i++) {
        fbp[i] = black;
    }

    munmap(fbp, screensize);
    close(fb);

    printf("Test complete.\n");
    return 0;
}