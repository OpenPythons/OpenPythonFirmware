# Select the configuration to build with, defaulting to "minimal"

BUILD = build
FIRMWARE = $(BUILD)/firmware

include ../micropython/py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE = arm-none-eabi-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

CFLAGS_CORTEX_M4 = -mthumb -mtune=cortex-m4 -mcpu=cortex-m4 -mfloat-abi=soft -fsingle-precision-constant -Wdouble-promotion
CFLAGS = $(INC) -Wall -Werror -std=c99 -nostdlib $(CFLAGS_CORTEX_M4) $(COPT)
CFLAGS += -DMP_CONFIGFILE="<mpconfigport.h>"

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os -DNDEBUG
endif

LDFLAGS = -nostdlib -T umport.ld -Map=$@.map --cref
LIBS = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

# Remove uncalled code from the final image.
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += --gc-sections

MEMZIP_DIR = "memzip"

SRC_C = \
	main.c \
	help.c \
	mphalport.c \
	modmachine.c \
	moduos.c \
	modutime.c \

SRC_EXTMOD = $(addprefix extmod/,\
    vfs.c \
    )

SRC_LIB = $(addprefix lib/,\
	utils/stdout_helpers.c \
	utils/interrupt_char.c \
	utils/pyexec.c \
    memzip/memzip.c \
	libc/string0.c \
	mp-readline/readline.c \
	)

SRC_LIBM_DBL = $(addprefix lib/libm_dbl/,\
	__cos.c \
	__expo2.c \
	__fpclassify.c \
	__rem_pio2.c \
	__rem_pio2_large.c \
	__signbit.c \
	__sin.c \
	__tan.c \
	acos.c \
	acosh.c \
	asin.c \
	asinh.c \
	atan.c \
	atan2.c \
	atanh.c \
	ceil.c \
	cos.c \
	cosh.c \
	erf.c \
	exp.c \
	expm1.c \
	floor.c \
	fmod.c \
	frexp.c \
	ldexp.c \
	lgamma.c \
	log.c \
	log10.c \
	log1p.c \
	modf.c \
	nearbyint.c \
	pow.c \
	rint.c \
	scalbn.c \
	sin.c \
	sinh.c \
	sqrt.c \
	tan.c \
	tanh.c \
	tgamma.c \
	trunc.c \
	)

SRC_LIBM = $(addprefix lib/libm/,\
	math.c \
	acoshf.c \
	asinfacosf.c \
	asinhf.c \
	atan2f.c \
	atanf.c \
	atanhf.c \
	ef_rem_pio2.c \
	ef_sqrt.c \
	erf_lgamma.c \
	fmodf.c \
	kf_cos.c \
	kf_rem_pio2.c \
	kf_sin.c \
	kf_tan.c \
	log1pf.c \
	nearbyintf.c \
	sf_cos.c \
	sf_erf.c \
	sf_frexp.c \
	sf_ldexp.c \
	sf_modf.c \
	sf_sin.c \
	sf_tan.c \
	wf_lgamma.c \
	wf_tgamma.c \
	)

SRC_QSTR += $(SRC_C)

OBJ =
OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_EXTMOD:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_LIB:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_LIBM:.c=.o))
OBJ += $(BUILD)/memzip-files.o

MEMZIP_MPY_DIR = $(BUILD)/memzip
SRC_MEMZIP = $(shell find $(MEMZIP_DIR) -name '*.py')
SRC_MEMZIP_MPY = $(addprefix $(BUILD)/, $(SRC_MEMZIP:.py=.mpy))

MAKE_MEMZIP = $(TOP)/lib/memzip/make-memzip.py

TOOL_MPY_CROSS = $(TOP)/mpy-cross/mpy-cross

$(TOOL_MPY_CROSS):
	cd $(TOP)/mpy-cross && $(MAKE) mpy-cross

$(MEMZIP_MPY_DIR):
	mkdir -p $@

$(SRC_MEMZIP_MPY): $(SRC_MEMZIP) $(MEMZIP_MPY_DIR) $(TOOL_MPY_CROSS)
	$(TOOL_MPY_CROSS) $(SRC_MEMZIP) -o $(SRC_MEMZIP_MPY) -X emit=bytecode

$(BUILD)/memzip-files.o: $(BUILD)/memzip-files.c
	$(call compile_c)

$(BUILD)/memzip-files.c: $(SRC_MEMZIP_MPY)
	@$(ECHO) "Creating $@"
	$(Q)$(PYTHON) $(MAKE_MEMZIP) --zip-file $(BUILD)/memzip-files.zip --c-file $@ $(MEMZIP_MPY_DIR)

all: $(FIRMWARE).bin

$(FIRMWARE).elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(FIRMWARE).bin: $(FIRMWARE).elf
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $@

include $(TOP)/py/mkrules.mk
