/* PMSIS includes */
#include "bsp/ram.h"
#include "bsp/ram/hyperram.h"
#include "pmsis.h"
#include "pulp.h"

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
signed char *buff_L1_FC;  // L1 FC
signed char *buff_L1_cl;  // L1 inside of cluster

// Ram config parameters
static struct pi_device ram;          // ram object
static struct pi_hyperram_conf conf;  // config object

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
 *                 Transfers L3->L1
 **************************************************/
/*! \brief This function implement transfer L2->L3
**************************************************/
void test_memory_ram_L3_L1(void) {
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
  buff_L1 = (signed char *)pmsis_fc_tcdm_malloc((uint32_t)BUFFER_SIZE_L2);
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
        buff_L1[j] = buff_aux[j];
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
  pmsis_l2_malloc_free(buff_L2, (uint32_t)BUFFER_SIZE_L2);
  pmsis_l2_malloc_free(buff_aux, (uint32_t)BUFFER_SIZE_L2);
  pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t)BUFFER_SIZE_L3);
  pi_ram_close(&ram);
#ifdef VERBOSE
  printf("done:Memfree..\n");
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
  for (int b = 1; b < 16; b++) {
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

/* Program Entry. */
int main(void) {
  // printf("\n\n\t ***Memory Test ***\n\n");
  // printf("\n\n\t test name %s \n:", test_name[test]);

  return pmsis_kickoff((void *)test_memory_ram_L3_L2);
}
