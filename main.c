#include <stdio.h>
#include <kernel.h>
#include <string.h>
#include <tamtypes.h>
#include <malloc.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <debug.h>
#include <iopcontrol.h>
#include <libcdvd.h>

int PrintTemperature() {
    unsigned char in_buffer[1], out_buffer[16];
    int result;
    int stat = 0;
    u16 inBuffSize = sizeof(in_buffer);

    memset(out_buffer, 0, sizeof(out_buffer));
    in_buffer[0] = 0xEF;

    result = sceCdApplySCmd(0x03, in_buffer, inBuffSize, out_buffer);
    if (result != 0) {
        stat = out_buffer[0];
    }

    if (!stat) {
        unsigned short temp = out_buffer[1] * 256 + out_buffer[2];
        scr_printf("Temperature: %2d.%2d °C\n", temp / 128, temp % 128);
        return 1;
    } else {
        scr_printf("Failed to execute command. Status=0x%x\n", stat);
        return 0;
    }
}

int main(int argc, char *argv[]) {
    SifInitRpc(0);
    SifIopReset("", 0);
    while (!SifIopSync()) {}
    SifInitRpc(0);
    SifLoadFileInit();

    init_scr();

    scr_printf("IOP Reset completed.\n");

    if (SifLoadModule("rom0:SIO2MAN", 0, NULL) < 0) {
        scr_printf("Failed to load SIO2MAN module.\n");
        return -1;
    }
    if (SifLoadModule("rom0:CDVDMAN", 0, NULL) < 0) {
        scr_printf("Failed to load CDVDMAN module.\n");
        return -1;
    }
    if (SifLoadModule("rom0:CDVDFSV", 0, NULL) < 0) {
        scr_printf("Failed to load CDVDFSV module.\n");
        return -1;
    }

    scr_printf("Modules loaded successfully.\n");

    for (int i = 0; i < 10; i++) {
        scr_printf("Reading temperature... (%d/10)\n", i + 1);
        if (!PrintTemperature()) {
            scr_printf("Error reading temperature.\n");
        }
        sleep(1);
    }

    scr_printf("Finished temperature readings.\n");

    SleepThread();
    return 0;
}
