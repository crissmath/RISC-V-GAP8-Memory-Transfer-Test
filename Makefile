# User Test
#------------------------------------


APP              = test_mem
APP_SRCS        += test_mem_cp.c 
APP_INC         +=
APP_CFLAGS      += 


# rules for old api 
#include $(GAP_SDK_HOME)/tools/rules/pmsis_rules.mk

# new api
include $(RULES_DIR)/pmsis_rules.mk

