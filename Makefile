#------------------------------------
# t_mem_L3_L2  	  = test_memory_ram_L3_L2
# t_mem_L2_L3  	  = test_memory_ram_L2_L3
# t_mem_L3_L3  	  = test_memory_ram_L3_L3
# t_mem_L3_L1_fc  = test_memory_ram_L3_L1_fc
# t_mem_L3_L1_cl  = test_memory_ram_L3_L1_cl
# t_mem_L3_Reg 	  = test_memory_ram_L3_Reg
TYPE = -Dt_mem_L3_L1_cl
APP              = test_mem
APP_SRCS        += test_mem_cp.c 
APP_CFLAGS      += -O2 $(TYPE)

# rules for old api 
#include $(GAP_SDK_HOME)/tools/rules/pmsis_rules.mk
# new api
include $(RULES_DIR)/pmsis_rules.mk

