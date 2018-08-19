include ../micropython/py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

FROZEN_MPY_DIR = modules

BUILD = build
FIRMWARE = $(BUILD)/firmware

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE = arm-none-eabi-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)


CFLAGS_CORTEX_M = -mthumb -fsingle-precision-constant -Wdouble-promotion
CFLAGS_MCU_m0 = $(CFLAGS_CORTEX_M) -mtune=cortex-m0 -mcpu=cortex-m0 -mfloat-abi=soft

CFLAGS = $(INC) -Wall -Werror -std=c99 -nostdlib $(CFLAGS_MCU_m0) $(COPT)
CFLAGS += -DMP_CONFIGFILE="<mpconfigport.h>"

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os -DNDEBUG
endif

LDFLAGS = -nostdlib -T openpie.ld -Map=$@.map --cref
LIBS = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

# Remove uncalled code from the final image.
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += --gc-sections


SRC_C = \
	main.c \
	help.c \
	mphalport.c \
	modmachine.c \
	moduos.c \
	modutime.c \
	modusystem.c \
	svcfunc.c \
	gccollect.c \

SRC_S = \
	gchelper_m0.s \

SRC_LIB = $(addprefix lib/,\
	utils/stdout_helpers.c \
	utils/interrupt_char.c \
	utils/pyexec.c \
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
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.s=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_LIB:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_LIBM:.c=.o))


all: $(FIRMWARE).bin

$(FIRMWARE).elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(FIRMWARE).bin: $(FIRMWARE).elf
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $@

include $(TOP)/py/mkrules.mk
