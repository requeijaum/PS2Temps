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
	
	// Based on PS2Ident libxcdvd from SP193
	unsigned char in_buffer[1], out_buffer[16];
	int result;
	int stat = 0;

    memset(&out_buffer, 0, 16);
	
	in_buffer[0]= 0xEF;
	if((result=sceCdApplySCmd(0x03, in_buffer, sizeof(in_buffer), out_buffer, sizeof(out_buffer)))!=0)
	{
		stat=out_buffer[0];
	}
    
	if( !stat) {
		unsigned short temp = out_buffer[1] * 256 + out_buffer[2];
		scr_printf("  Temp: %2d.%2d  C \n",  (temp - (temp%128) ) / 128  , (temp%128)      );
		return 1;
		
	}else{
		
		scr_printf("Failed 0x03 0xEF command. stat=%x \n", stat);
		return 0;
		
	}

}


int main(int argc, char *argv[]) {
   

    SifInitRpc(0);
	SifIopReset("", 0);
	while(!SifIopSync()){}
	SifInitRpc(0);
	SifLoadFileInit();
	fioInit(); 
	
   init_scr();
 
   scr_printf("Reset IOP done. ");
    
   int ret;
   
   ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
   ret = SifLoadModule("rom0:CDVDMAN", 0, NULL);
   ret = SifLoadModule("rom0:CDVDFSV", 0, NULL);
 
   scr_printf("All modules loaded.\n");
   
   for(ret = 0; ret < 0x10; ret++) 
	        PrintTemperature();
   

   SleepThread();
  
  return 0;
 
}
