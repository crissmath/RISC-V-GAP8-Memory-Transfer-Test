/* PMSIS includes */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Gap8.h"
#include "bsp/ram.h"
#include "bsp/ram/hyperram.h"
#include "pmsis.h"
#include "pulp.h"
#include "rt/rt_api.h"

#define mc (1)
#define nc (2048)
#define reps (1000)
#define BUFFER_SIZE_L2 (mc * nc)
#define BUFFER_SIZE_L3 (mc * nc)
//#define VERBOSE

// 0: L3 --> L2
// 1: L2---> L3
// 2: L3---> L3
// 3: L3---> L1

char test_name[][30] = {"test_memory_ram_L3_L2", "test_memory_ram_L2_L3",
                        "test_memory_ram_L3_L3", "test_memory_ram_L3_L1",
                        "test_memory_ram_L3_Reg"};

// Buffers for transfers
signed char *buff_L2;
signed char *buff_L3;
signed char *buff_aux;
signed char *buff_L1_fc;  // L1 FC
signed char *buff_L1_cl;  // L1 inside of cluster

// Ram config parameters
static struct pi_device ram;          // ram object
static struct pi_hyperram_conf conf;  // config object

// struct for arguments to transfers
typedef struct trans_cl_arg {
  signed char *L3_buff;
  signed char *L2_buff;
  signed char *L2_aux;
  signed char *L1_buff_cl;
  unsigned int ram;
  unsigned int b;
} trans_cl_arg_t;

trans_cl_arg_t cl_Arg;

/***************************************************
 *                 Transfers L3->L2
 **************************************************/
/*! \brief This function implement transfer L3->L2
 **************************************************/
void test_memory_ram_L3_L2(void) {
#ifdef VERBOSE
  printf("ram init... L3->L2\n");
#endif
  /*------------Init memory----------------- */
  /* Init & open ram. */
  pi_hyperram_conf_init(&conf);
  pi_open_from_conf(&ram, &conf);
  if (pi_ram_open(&ram)) {
    printf("Error ram open !\n");
    pmsis_exit(-3);
  }
#ifdef VERBOSE
  printf("ram open...\n");
#endif
  float t1, t2, t, time;
  uint32_t errors = 0;

#ifdef VERBOSE
  printf("start:alloc buff L2..\n");
#endif
  buff_L2 = (signed char *)pmsis_l2_malloc((uint32_t)BUFFER_SIZE_L2);
  if (buff_L2 == NULL) {
    printf("buff_L2 alloc failed !\n");
    pmsis_exit(-1);
  }
#ifdef VERBOSE
  printf("done:alloc buff L2..\n");
#endif

#ifdef VERBOSE
  printf("start:alloc buff L3..\n");
#endif
  if (pi_ram_alloc(&ram, &buff_L3, (uint32_t)mc * nc * sizeof(signed char))) {
    printf("Ram malloc failed A!\n");
    pmsis_exit(-4);
  }
#ifdef VERBOSE
  printf("done:alloc buff L3..\n");
#endif

#ifdef VERBOSE
  printf("start:transfers..\n");
#endif
  // write n_cp blocks in external ram
  for (int b = 1; b < (mc * nc); b++) {
    t1 = rt_time_get_us();
    for (int i = 0; i < reps; i++) {
      pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b, 1);
    }
    t2 = rt_time_get_us();

    t = (t2 - t1) / 1e6;
    time = (t / reps);
    printf("%d,%f,%f\n", b, time, b / time);
  }
#ifdef VERBOSE
  printf("done:Copy..\n");
#endif
  /*-------free memory----------------------*/
  pmsis_l2_malloc_free(buff_L2, (uint32_t)BUFFER_SIZE_L2);
  pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t)BUFFER_SIZE_L3);
  pi_ram_close(&ram);
#ifdef VERBOSE
  printf("done: Memfree..\n");
#endif
}

/***************************************************
 *                 Transfers L2->L3
 **************************************************/
/*! \brief This function implement transfer L2->L3
**************************************************/
void test_memory_ram_L2_L3(void) {
#ifdef VERBOSE
  printf("ram init L2->L3..\n");
#endif
  /*------------Init memory----------------- */
  /* Init & open ram. */
  pi_hyperram_conf_init(&conf);
  pi_open_from_conf(&ram, &conf);
  if (pi_ram_open(&ram)) {
    printf("Error ram open !\n");
    pmsis_exit(-3);
  }

#ifdef VERBOSE
  printf("done:ram open...\n");
#endif
  float t1, t2, t, time;
  uint32_t errors = 0;

#ifdef VERBOSE
  printf("start:alloc buffs..\n");
#endif
  buff_L2 = (signed char *)pmsis_l2_malloc((uint32_t)BUFFER_SIZE_L2);
  if (buff_L2 == NULL) {
    printf("buff_L2 alloc failed !\n");
    pmsis_exit(-1);
  }
  // Buffer L3
  if (pi_ram_alloc(&ram, &buff_L3, (uint32_t)mc * nc * sizeof(signed char))) {
    printf("Ram malloc failed A!\n");
    pmsis_exit(-4);
  }
#ifdef VERBOSE
  printf("done:alloc buffs..\n");
#endif

#ifdef VERBOSE
  printf("start:transfers..\n");
#endif
  // write n_cp blocks in external ram
  for (int b = 1; b < (mc * nc); b++) {
    t1 = rt_time_get_us();
    for (int i = 0; i < reps; i++) {
      pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b, 0);
    }
    t2 = rt_time_get_us();
    t = (t2 - t1) / 1e6;
    time = (t / reps);
    printf("%d %f %f\n", b, time, b / time);
  }

  /*-------free memory----------------------*/
  pmsis_l2_malloc_free(buff_L2, (uint32_t)BUFFER_SIZE_L2);
  pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t)BUFFER_SIZE_L3);
  pi_ram_close(&ram);
#ifdef VERBOSE
  printf("done: Memfree..\n");
#endif
}

/***************************************************
 *                 Transfers L3->L3
 **************************************************/
/*! \brief This function implement transfer L3->L3
**************************************************/
void test_memory_ram_L3_L3(void) {
#ifdef VERBOSE
  printf("start:ram init..\n");
#endif
  /*------------Init memory----------------- */
  /* Init & open ram. */
  pi_hyperram_conf_init(&conf);
  pi_open_from_conf(&ram, &conf);
  if (pi_ram_open(&ram)) {
    printf("Error ram open !\n");
    pmsis_exit(-3);
  }
#ifdef VERBOSE
  printf("done:ram init..\n");
#endif
  float t1, t2, t, time;
  uint32_t errors = 0;

#ifdef VERBOSE
  printf("start:alloc buffs..\n");
#endif
  buff_L2 = (signed char *)pmsis_l2_malloc((uint32_t)BUFFER_SIZE_L2);
  if (buff_L2 == NULL) {
    printf("buff_L2 alloc failed !\n");
    pmsis_exit(-1);
  }
  // Buffer L3
  if (pi_ram_alloc(&ram, &buff_L3, (uint32_t)mc * nc * sizeof(signed char))) {
    printf("Ram malloc failed A!\n");
    pmsis_exit(-4);
  }
#ifdef VERBOSE
  printf("done:alloc buffs..\n");
#endif

#ifdef VERBOSE
  printf("start:transfers..\n");
#endif
  // write n_cp blocks in external ram
  for (int b = 1; b < (mc * nc); b++) {
    t1 = rt_time_get_us();
    for (int i = 0; i < reps; i++) {
      pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b, 1);
      pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b, 0);
    }
    t2 = rt_time_get_us();
    t = (t2 - t1) / 1e6;
    time = (t / reps);
    printf("%d %f %f\n", b, time, b / time);
  }
#ifdef VERBOSE
  printf("done:transfers..\n");
#endif

  /*-------free memory----------------------*/
  pmsis_l2_malloc_free(buff_L2, (uint32_t)BUFFER_SIZE_L2);
  pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t)BUFFER_SIZE_L3);
  pi_ram_close(&ram);
#ifdef VERBOSE
  printf("done:Mem-free..\n");
#endif
}

/***************************************************
 *                 Transfers L3->L1_fc
 **************************************************/
/*! \brief This function implement transfer L2->L1
 * L1 in the fabric controller
 **************************************************/
void test_memory_ram_L3_L1_fc(void) {
#ifdef VERBOSE
  printf("start:ram init..\n");
#endif
  /*------------Init memory----------------- */
  /* Init & open ram. */
  pi_hyperram_conf_init(&conf);
  pi_open_from_conf(&ram, &conf);
  if (pi_ram_open(&ram)) {
    printf("Error ram open !\n");
    pmsis_exit(-3);
  }
#ifdef VERBOSE
  printf("done:ram init..\n");
#endif

  float t1, t2, t, time;
  uint32_t errors = 0;

#ifdef VERBOSE
  printf("start:alloc buffs..\n");
#endif
  // Buff L2
  buff_L2 = (signed char *)pmsis_l2_malloc((uint32_t)BUFFER_SIZE_L2);
  if (buff_L2 == NULL) {
    printf("buff_L2 alloc failed !\n");
    pmsis_exit(-1);
  }
  // Buff aux(L2)
  buff_aux = (signed char *)pmsis_l2_malloc((uint32_t)BUFFER_SIZE_L2);
  if (buff_aux == NULL) {
    printf("buff_L2 alloc failed !\n");
    pmsis_exit(-2);
  }
  // Buffer L3
  if (pi_ram_alloc(&ram, &buff_L3, (uint32_t)mc * nc * sizeof(signed char))) {
    printf("Ram malloc failed A!\n");
    pmsis_exit(-4);
  }
  // Buffer L1
  buff_L1_fc = (signed char *)pi_fc_l1_malloc((uint32_t)BUFFER_SIZE_L2);
#ifdef VERBOSE
  printf("done:alloc buffs..\n");
#endif

#ifdef VERBOSE
  printf("start:transfers..\n");
#endif
  // write n_cp blocks in external ram
  for (int b = 1; b < (mc * nc); b++) {
    t1 = rt_time_get_us();
    for (int i = 0; i < reps; i++) {
      pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_aux, (uint32_t)b, 1);
      for (int j = 0; j < b; j++) {
        buff_L1_fc[j] = buff_aux[j];
      }
    }
    t2 = rt_time_get_us();

    t = (t2 - t1) / 1e6;
    time = (t / reps);
    printf("%d %f %f\n", b, time, b / time);
  }
#ifdef VERBOSE
  printf("done:transfers..\n");
#endif

  /*-------free memory----------------------*/
  // pi_fc_l1_malloc_free(buff_L1_fc, (uint32_t)BUFFER_SIZE_L2);
  pmsis_l2_malloc_free(buff_L2, (uint32_t)BUFFER_SIZE_L2);
  pmsis_l2_malloc_free(buff_aux, (uint32_t)BUFFER_SIZE_L2);
  pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t)BUFFER_SIZE_L3);
  pi_ram_close(&ram);
#ifdef VERBOSE
  printf("done:Memfree..\n");
#endif
}

/***************************************************
 *                 Transfers L3->L1_cl
 **************************************************/
/*! \brief This function implement transfer L3->L1
 * L1 in the cluster
 **************************************************/
void test_memory_ram_L3_L1_cl(trans_cl_arg_t *cl_Arg) {
#ifdef VERBOSE_buf_cl
  printf("start:transfers..\n");
#endif
  // write n_cp blocks from external ram to L1_cl

  signed char *buff_L3_cl = cl_Arg->L3_buff;
  signed char *buff_L2_cl = cl_Arg->L2_buff;
  signed char *buff_aux_cl = cl_Arg->L2_aux;
  signed char *buff_L1_cl = cl_Arg->L1_buff_cl;
  unsigned int ram = cl_Arg->ram;
  unsigned int b = cl_Arg->b;

#ifdef VERBOSE_buff_cl
  printf("L3 = %x\n", buff_L3_cl);
  printf("L2 = %x\n", buff_L2_cl);
  printf("aux = %x\n", buff_aux_cl);
  printf("L1_cl = %x\n", buff_L1_cl);
  printf("ram = %x\n", ram);
#endif

  pi_cl_ram_req_t req_cp;
  pi_cl_ram_copy(ram, (uint32_t)(buff_L3_cl), buff_aux_cl, (uint32_t)b, 1,
                 &req_cp);
  pi_cl_ram_copy_wait(&req_cp);
  // for (int j = 0; j < b; j++) {
  //  buff_L1_cl[j] = buff_aux[j];
  //}
#ifdef VERBOSE_buff_cl
  printf("done:transfers..\n");
#endif
}

/***************************************************
 *                 Transfers L3->Reg
 **************************************************/
/*! \brief This function implement transfer L2->Reg
**************************************************/
void test_memory_ram_L3_Reg(void) {
#ifdef VERBOSE
  printf("start:ram init..\n");
#endif
  /*------------Init memory----------------- */
  /* Init & open ram. */
  pi_hyperram_conf_init(&conf);
  pi_open_from_conf(&ram, &conf);
  if (pi_ram_open(&ram)) {
    printf("Error ram open !\n");
    pmsis_exit(-3);
  }
#ifdef VERBOSE
  printf("done:ram init..\n");
#endif

  float t1, t2, t, time;
  uint32_t errors = 0;
  // registro temporal
  signed char Reg[16];

#ifdef VERBOSE
  printf("start:alloc buffs..\n");
#endif
  // Buffer L3
  if (pi_ram_alloc(&ram, &buff_L3, (uint32_t)16 * sizeof(signed char))) {
    printf("Ram malloc failed A!\n");
    pmsis_exit(-4);
  }

#ifdef VERBOSE
  printf("done:alloc buffs..\n");
#endif

#ifdef VERBOSE
  printf("start:transfers..\n");
#endif
  // write n_cp blocks in external ram
  for (int b = 1; b < (mc * nc); b++) {
    t1 = rt_time_get_us();
    for (int i = 0; i < reps; i++) {
      pi_ram_copy(&ram, (uint32_t)(buff_L3), Reg, (uint32_t)b, 1);
    }
    t2 = rt_time_get_us();

    t = (t2 - t1) / 1e6;
    time = (t / reps);
    printf("%d %f %f\n", b, time, b / time);
  }
#ifdef VERBOSE
  printf("done:transfers..\n");
#endif
  /*-------free memory----------------------*/
  pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t)BUFFER_SIZE_L3);
  pi_ram_close(&ram);
#ifdef VERBOSE
  printf("done:Memfree..\n");
#endif
}

/************************************************************
 *               Delegate function
 *  In this function we are in the core0(master) of the cluster
 *  The cluster controller is core 0 of the cluster.
 *  Core 1 to 7 are the slave cores.
 *************************************************************/
void cluster_delegate(void *arg) {
  /*In this function we are in the core 0 of the cluster*/
#ifdef VERBOSE_buff_cl
  printf("Cluster master core entry\n");
#endif
  int nb_cores = 1;
  pi_cl_team_fork(nb_cores, (void *)test_memory_ram_L3_L1_cl, arg);
#ifdef VERBOSE_buf_cl
  printf("Cluster master core exit\n");
#endif
}

/*************************************************
 *                 FC MAIN
 **************************************************/
/*! \brief This function is the fabric controller
 * entry program.
 **************************************************/
void fc_main() {
#ifdef VERBOSE
  printf("Entering Fabric Controller\n");
#endif
  //----- Choose the type of transfer you need to test
  // t_mem_L3_L2      = test_memory_ram_L3_L2
  // t_mem_L2_L3      = test_memory_ram_L2_L3
  // t_mem_L3_L3      = test_memory_ram_L3_L3
  // t_mem_L3_L1_fc   = test_memory_ram_L3_L1_fc
  // t_mem_L3_L1_cl   = test_memory_ram_L3_L1_cl
  // t_mem_L3_Reg     = test_memory_ram_L3_Reg
#ifdef t_mem_L3_L2
  test_memory_ram_L3_L2();

#elif defined(t_mem_L2_L3)
  test_memory_ram_L2_L3();

#elif defined(t_mem_L3_L3)
  test_memory_ram_L3_L3();

#elif defined(t_mem_L3_L1_fc)
  test_memory_ram_L3_L1_fc();

#elif defined(t_mem_L3_L1_cl)

  // in this function we need to configure the cluster
  // cluster_config();
  // Cluster-configuration----
  uint32_t core_id = pi_core_id();
  uint32_t cluster_id = pi_cluster_id();
  //-------- Set Voltage & Frecuency ------------
  // Set FC Freq at 250 MHz
  uint32_t fc_freq_in_Hz = 250 * 1000 * 1000;
  pi_freq_set(PI_FREQ_DOMAIN_FC, fc_freq_in_Hz);
  printf("Fabric Controller Frequency %d Hz\n",
         (int)pi_freq_get(PI_FREQ_DOMAIN_FC));
  // --------Configure & open cluster.
  struct pi_device cluster_dev = {0};
  struct pi_cluster_conf cl_conf = {0};
  // Init cluster configuration structure.
  pi_cluster_conf_init(&cl_conf);
  cl_conf.id = 0;  // Set cluster ID
  // Configure and Open Cluster*/
  pi_open_from_conf(&cluster_dev, &cl_conf);
  if (pi_cluster_open(&cluster_dev)) {
    printf("Cluster open failed !\n");
    pmsis_exit(-1);
  }
  // Set the max freq for the cluster @1.2V
  uint32_t cl_freq_in_Hz = 175 * 1000 * 1000;
  pi_freq_set(PI_FREQ_DOMAIN_CL, cl_freq_in_Hz);

#ifdef VERBOSE
  printf("start:ram init..\n");
#endif
  //------------Init memory-----------------
  // Init & open ram
  pi_hyperram_conf_init(&conf);
  pi_open_from_conf(&ram, &conf);
  if (pi_ram_open(&ram)) {
    printf("Error ram open !\n");
    pmsis_exit(-3);
  }
#ifdef VERBOSE
  printf("done:ram init..\n");
#endif
// ---------- Alloc Buffers ---------------
#ifdef VERBOSE
  printf("start:alloc buffs..\n");
#endif
  // Buff L3
  if (pi_ram_alloc(&ram, &buff_L3, (uint32_t)mc * nc)) {
    printf("Ram malloc failed A!\n");
    pmsis_exit(-4);
  }
  // Buff L2
  buff_L2 = (signed char *)pmsis_l2_malloc((uint32_t)BUFFER_SIZE_L2);
  if (buff_L2 == NULL) {
    printf("buff_L2 alloc failed !\n");
    pmsis_exit(-1);
  }
  // Buff aux(L2)
  buff_aux = (signed char *)pmsis_l2_malloc((uint32_t)BUFFER_SIZE_L2);
  if (buff_aux == NULL) {
    printf("buff_L2 alloc failed !\n");
    pmsis_exit(-2);
  }
  // Buffer L1_cl
  buff_L1_cl = (signed char *)pmsis_l1_malloc((uint32_t)BUFFER_SIZE_L2);
#ifdef VERBOSE
  printf("done:alloc buffs..\n");
#endif

#ifdef VERBOSE_buf_cl
  printf("L3 = %x\n", buff_L3);
  printf("L2 = %x\n", buff_L2);
  printf("aux = %x\n", buff_aux);
  printf("L1_cl = %x\n", buff_L1_cl);
  printf("ram = %x\n", ram);
#endif

  // assig arguments
  cl_Arg.L3_buff = buff_L3;
  cl_Arg.L2_buff = buff_L2;
  cl_Arg.L2_aux = buff_aux;
  cl_Arg.L1_buff_cl = buff_L1_cl;
  cl_Arg.ram = &ram;

  //------- Task setup -------------
  struct pi_cluster_task cl_task = {0};
  cl_task.entry = cluster_delegate;
  //---send task to cluster------
  // timer variables
  float t1, t2, t, time;
  uint32_t errors = 0;
#ifdef VERBOSE
  printf("send task to cluster..\n");
#endif
  float t_cl = 8e-06;
  for (int b = 1; b < (mc * nc); b++) {
    t1 = rt_time_get_us();
    for (int i = 0; i < reps; i++) {
      cl_Arg.b = b;
      cl_task.arg = (void *)&cl_Arg;
      pi_cluster_send_task_to_cl(&cluster_dev, &cl_task);
    }
    t2 = rt_time_get_us();
    t = (t2 - t1) / 1e6;
    time = (t / reps) - t_cl;
    printf("%d %f %f\n", b, time, b / time);
  }
  printf("Close cluster after end of computation.\n");
  pi_cluster_close(&cluster_dev);

  /*-------free memory----------------------*/
  pmsis_l1_malloc_free(buff_L1_cl, (uint32_t)BUFFER_SIZE_L2);
  pmsis_l2_malloc_free(buff_L2, (uint32_t)BUFFER_SIZE_L2);
  pmsis_l2_malloc_free(buff_aux, (uint32_t)BUFFER_SIZE_L2);
  pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t)BUFFER_SIZE_L3);
  pi_ram_close(&ram);
#ifdef VERBOSE
  printf("done:Memfree..\n");
#endif

#elif defined(t_mem_L3_Reg)
  test_memory_ram_L3_L3();
#else
  continue;
#endif
  pmsis_exit(0);
}

int main() {
  // printf("\n\n\t ***Memory Test ***\n\n");
  // printf("\n\n\t test name %s \n:", test_name[test]);
#ifdef VERBOSE
  printf("\n\n");
  printf("  #############################################\n");
  printf("  #                                           #\n");
  printf("  #          Mem transfers test               #\n");
  printf("  #                                           #\n");
  printf("  #############################################\n\n\n");
#endif
  return pmsis_kickoff((void *)fc_main);
}
