// syscall_table.h : opmod/src/main/java/kr/pe/ecmaxp/openpie/arch/consts/OpenPieSystemCallTable.kt

#define SYS_DEBUG (0x010000)

#define SYS_CONTROL (0x020000)
#define SYS_CONTROL_SHUTDOWN (SYS_CONTROL | 1)
#define SYS_CONTROL_REBOOT (SYS_CONTROL | 2)
#define SYS_CONTROL_CRASH (SYS_CONTROL | 3)
#define SYS_CONTROL_RETURN (SYS_CONTROL | 4)
#define SYS_CONTROL_INIT_COPY (SYS_CONTROL | 16)
#define SYS_CONTROL_INIT_ZERO (SYS_CONTROL | 17)

#define SYS_SIGNAL (0x030000)
#define SYS_SIGNAL_POP (SYS_SIGNAL | 1)
#define SYS_SIGNAL_POP_WAKEUP (SYS_SIGNAL | 2)
#define SYS_SIGNAL_PUSH (SYS_SIGNAL | 3)

#define SYS_COMPONENT (0x040000)
#define SYS_COMPONENT_INVOKE (SYS_COMPONENT | 1)
#define SYS_COMPONENT_LIST (SYS_COMPONENT | 8)
#define SYS_COMPONENT_TYPE (SYS_COMPONENT | 9)
#define SYS_COMPONENT_SLOT (SYS_COMPONENT | 10)
#define SYS_COMPONENT_METHODS (SYS_COMPONENT | 11)
#define SYS_COMPONENT_DOC (SYS_COMPONENT | 12)

#define SYS_VALUE (0x050000)
#define SYS_VALUE_INVOKE (SYS_VALUE | 1)
#define SYS_VALUE_CALL (SYS_VALUE | 2)
#define SYS_VALUE_APPLY (SYS_VALUE | 3)
#define SYS_VALUE_UNAPPLY (SYS_VALUE | 4)
#define SYS_VALUE_DISPOSE (SYS_VALUE | 5)
#define SYS_VALUE_METHODS (SYS_VALUE | 11)
#define SYS_VALUE_DOC (SYS_VALUE | 12)

#define SYS_COMPUTER (0x060000)
#define SYS_COMPUTER_BEEP_1 (SYS_COMPUTER | 1)
#define SYS_COMPUTER_BEEP_2 (SYS_COMPUTER | 2)
#define SYS_COMPUTER_LAST_ERROR (SYS_COMPUTER | 8)
#define SYS_COMPUTER_USERS (SYS_COMPUTER | 16)
#define SYS_COMPUTER_ADD_USER (SYS_COMPUTER | 17)
#define SYS_COMPUTER_REMOVE_USER (SYS_COMPUTER | 18)
#define SYS_COMPUTER_COMPUTER_ADDRESS (SYS_COMPUTER | 24)
#define SYS_COMPUTER_TMP_ADDRESS (SYS_COMPUTER | 25)
#define SYS_COMPUTER_ENERGY (SYS_COMPUTER | 32)
#define SYS_COMPUTER_MAX_ENERGY (SYS_COMPUTER | 33)
#define SYS_COMPUTER_GET_ARCHITECTURES (SYS_COMPUTER | 40)
#define SYS_COMPUTER_GET_ARCHITECTURE (SYS_COMPUTER | 41)
#define SYS_COMPUTER_SET_ARCHITECTURE (SYS_COMPUTER | 42)

#define SYS_INFO (0x070000)
#define SYS_INFO_VERSION (SYS_INFO | 1)
#define SYS_INFO_RAM_SIZE (SYS_INFO | 2)

#define SYS_TIMER (0x080000)
#define SYS_TIMER_TICKS_MS (SYS_TIMER | 1)
#define SYS_TIMER_TICKS_US (SYS_TIMER | 2)
#define SYS_TIMER_TICKS_CPU (SYS_TIMER | 3)
#define SYS_TIMER_REAL_TIME (SYS_TIMER | 8)
#define SYS_TIMER_WORLD_TIME (SYS_TIMER | 9)
#define SYS_TIMER_UP_TIME (SYS_TIMER | 10)
#define SYS_TIMER_CPU_TIME (SYS_TIMER | 11)

#define SYS_VFS (0x090000)
#define SYS_VFS_OPEN (SYS_VFS | 1)
#define SYS_VFS_VALID (SYS_VFS | 2)
#define SYS_VFS_REPR (SYS_VFS | 3)
#define SYS_VFS_CLOSE (SYS_VFS | 4)
#define SYS_VFS_READ (SYS_VFS | 5)
#define SYS_VFS_WRITE (SYS_VFS | 6)
#define SYS_VFS_SEEK (SYS_VFS | 7)
#define SYS_VFS_FLUSH (SYS_VFS | 8)
