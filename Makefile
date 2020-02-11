EE_BIN  = PS2Temps.elf
EE_OBJS_DIR = obj/

EE_OBJS = main.o
EE_INCS := -I$(PS2SDK)/ports/include -I$(PS2SDK)/ee/include -I$(PS2SDK)/common/include
EE_LDFLAGS := -L./lib -L$(PS2SDK)/ports/lib -s
EE_LIBS = -lxcdvd -lmc -ldebug -lm -lz  -lc -lkernel

$(EE_OBJS_DIR)%.o : $(EE_SRC_DIR)%.c
	$(EE_CC) $(EE_CFLAGS) $(EE_INCS) -c $< -o $@
		
$(EE_OBJS_DIR)%.o : %.s
	$(EE_AS) $(EE_ASFLAGS) $(EE_ASM_DIR)$< -o $@	

all: $(EE_BIN)	
	
	
clean:
	rm -f $(EE_OBJS) $(EE_BIN)

install:

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
