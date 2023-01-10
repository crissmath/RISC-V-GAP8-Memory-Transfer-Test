/* PMSIS includes */
#include "pmsis.h"
#include "bsp/ram.h"
#include "bsp/ram/hyperram.h"
#include "pulp.h"


#define mc                  ( 1 )
#define nc                  ( 2048 )
#define reps                ( 1000 )
#define BUFFER_SIZE_L2      ( mc*nc )
#define BUFFER_SIZE_L3      ( mc*nc )

// 0: L3 --> L2
// 1: L2---> L3
// 2: L3---> L3
// 3: L3---> L1
#define test    0         

char test_name[][30] = {"test_memory_ram_L3_L2", "test_memory_ram_L2_L3",
                         "test_memory_ram_L3_L3", "test_memory_ram_L3_L1",
                         "test_memory_ram_L3_Reg"};

signed char   *buff_L2;
signed char   *buff_L3;                         // pointer to variable where return the addres
signed char   *buff_aux;
signed char   *buff_L1;

static struct       pi_device ram;            // ram object 
static struct       pi_hyperram_conf conf;    //config object


void test_memory_ram_L3_L2(void)
{
    printf("ram init... L3 to L2\n");
/*------------Init memory----------------- */
    /* Init & open ram. */
    pi_hyperram_conf_init(&conf);
    pi_open_from_conf(&ram, &conf);
    if (pi_ram_open(&ram)){
        printf("Error ram open !\n");
        pmsis_exit(-3);
    }
printf("ram open...\n");

    printf("Entering main controller\n");
    float t1, t2, t, time ;
    uint32_t errors = 0;

printf("allocs...\n");
    buff_L2  = (signed char*) pmsis_l2_malloc((uint32_t) BUFFER_SIZE_L2);
    if(buff_L2 == NULL){
        printf("buff_L2 alloc failed !\n");
        pmsis_exit(-1);
    }

    // Buffer L3
    if(pi_ram_alloc(&ram, &buff_L3, (uint32_t) mc*nc*sizeof(signed char))){
        printf("Ram malloc failed A!\n");
        pmsis_exit(-4);          
    }
    printf("Copies...\n");
    // write n_cp blocks in external ram 
    for(int b=1; b<(mc*nc); b++){
        t1   = rt_time_get_us();
       for(int i=0;i<reps;i++){
            pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b,1);  
        }
        t2   = rt_time_get_us();
    
    t = (t2-t1)/1e6;
    time = (t/reps);
    printf("%d %f %f\n", b, time, b/time);
    }
  
    /*-------free memory----------------------*/
    pmsis_l2_malloc_free(buff_L2, (uint32_t) BUFFER_SIZE_L2);
    pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t) BUFFER_SIZE_L3);
    pi_ram_close(&ram);

}



void test_memory_ram_L2_L3(void)
{
    printf("ram init...L2 to L3\n");
/*------------Init memory----------------- */
    /* Init & open ram. */
    pi_hyperram_conf_init(&conf);
    pi_open_from_conf(&ram, &conf);
    if (pi_ram_open(&ram)){
        printf("Error ram open !\n");
        pmsis_exit(-3);
    }
printf("ram open...\n");

    printf("Entering main controller\n");

    float t1, t2, t, time ;
    uint32_t errors = 0;

printf("allocs...\n");
    buff_L2  = (signed char*) pmsis_l2_malloc((uint32_t) BUFFER_SIZE_L2);
    if(buff_L2 == NULL){
        printf("buff_L2 alloc failed !\n");
        pmsis_exit(-1);
    }

    // Buffer L3
    if(pi_ram_alloc(&ram, &buff_L3, (uint32_t) mc*nc*sizeof(signed char))){
        printf("Ram malloc failed A!\n");
        pmsis_exit(-4);          
    }
    printf("Copies...\n");
    // write n_cp blocks in external ram 
    for(int b=1; b<(mc*nc); b++){
        t1   = rt_time_get_us();
       for(int i=0;i<reps;i++){
            pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b,0);  
        }
        t2   = rt_time_get_us();
    
    t = (t2-t1)/1e6;
    time = (t/reps);
    printf("%d %f %f\n", b, time, b/time);
    }

    
    /*-------free memory----------------------*/
    pmsis_l2_malloc_free(buff_L2, (uint32_t) BUFFER_SIZE_L2);
    pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t) BUFFER_SIZE_L3);
    pi_ram_close(&ram);

}




void test_memory_ram_L3_L3(void)
{
    printf("ram init...L3 to L3\n");
/*------------Init memory----------------- */
    /* Init & open ram. */
    pi_hyperram_conf_init(&conf);
    pi_open_from_conf(&ram, &conf);
    if (pi_ram_open(&ram)){
        printf("Error ram open !\n");
        pmsis_exit(-3);
    }
printf("ram open...\n");


    printf("Entering main controller\n");

    float t1, t2, t, time ;
    uint32_t errors = 0;

printf("allocs...\n");
    buff_L2  = (signed char*) pmsis_l2_malloc((uint32_t) BUFFER_SIZE_L2);
    if(buff_L2 == NULL){
        printf("buff_L2 alloc failed !\n");
        pmsis_exit(-1);
    }

    // Buffer L3
    if(pi_ram_alloc(&ram, &buff_L3, (uint32_t) mc*nc*sizeof(signed char))){
        printf("Ram malloc failed A!\n");
        pmsis_exit(-4);          
    }
    printf("Copies...\n");
    // write n_cp blocks in external ram 
    for(int b=1; b<(mc*nc); b++){
        t1   = rt_time_get_us();
       for(int i=0;i<reps;i++){
            pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b,1);  
            pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_L2, (uint32_t)b,0);  
        }
        t2   = rt_time_get_us();
    
    t = (t2-t1)/1e6;
    time = (t/reps);
    printf("%d %f %f\n", b, time, b/time);
    }

    
    /*-------free memory----------------------*/
    pmsis_l2_malloc_free(buff_L2, (uint32_t) BUFFER_SIZE_L2);
    pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t) BUFFER_SIZE_L3);
    pi_ram_close(&ram);


}


void test_memory_ram_L3_L1(void)
{
    printf("ram init...L3 to L1\n");
/*------------Init memory----------------- */
    /* Init & open ram. */
    pi_hyperram_conf_init(&conf);
    pi_open_from_conf(&ram, &conf);
    if (pi_ram_open(&ram)){
        printf("Error ram open !\n");
        pmsis_exit(-3);
    }
printf("ram open...\n");

    printf("Entering main controller\n");

    float t1, t2, t, time ;
    uint32_t errors = 0;

printf("allocs...\n");
    buff_L2  = (signed char*) pmsis_l2_malloc((uint32_t) BUFFER_SIZE_L2);
    if(buff_L2 == NULL){
        printf("buff_L2 alloc failed !\n");
        pmsis_exit(-1);
    }
    buff_aux = (signed char*) pmsis_l2_malloc((uint32_t) BUFFER_SIZE_L2);
    if(buff_aux == NULL){
        printf("buff_L2 alloc failed !\n");
        pmsis_exit(-2);
    }

    // Buffer L3
    if(pi_ram_alloc(&ram, &buff_L3, (uint32_t) mc*nc*sizeof(signed char))){
        printf("Ram malloc failed A!\n");
        pmsis_exit(-4);          
    }

    buff_L1 = (signed char *)pmsis_fc_tcdm_malloc((uint32_t)BUFFER_SIZE_L2);

    printf("Copies...\n");
    // write n_cp blocks in external ram 
    for(int b=1; b<(mc*nc); b++){
        t1   = rt_time_get_us();
       for(int i=0;i<reps;i++){
            pi_ram_copy(&ram, (uint32_t)(buff_L3), buff_aux, (uint32_t)b,1);  
            for(int j=0; j<b;j++){
                buff_L1[j] = buff_aux[j];  
            } 
       }
        t2   = rt_time_get_us();
    
    t = (t2-t1)/1e6;
    time = (t/reps);
    printf("%d %f %f\n", b, time, b/time);
    }

    /*-------free memory----------------------*/
    pmsis_l2_malloc_free(buff_L2, (uint32_t) BUFFER_SIZE_L2);
    pmsis_l2_malloc_free(buff_aux, (uint32_t) BUFFER_SIZE_L2);

    pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t) BUFFER_SIZE_L3);
    pi_ram_close(&ram);
}


void test_memory_ram_L3_Reg(void)
{
    printf("ram init...L3 to Reg\n");
/*------------Init memory----------------- */
    /* Init & open ram. */
    pi_hyperram_conf_init(&conf);
    pi_open_from_conf(&ram, &conf);
    if (pi_ram_open(&ram)){
        printf("Error ram open !\n");
        pmsis_exit(-3);
    }
    printf("ram open...\n");
    printf("Entering main controller\n");

    float t1, t2, t, time ;
    uint32_t errors = 0;
    // registro temporal
    signed char Reg[16];

    printf("allocs...\n");
    // Buffer L3
    if(pi_ram_alloc(&ram, &buff_L3, (uint32_t) 16*sizeof(signed char))){
        printf("Ram malloc failed A!\n");
        pmsis_exit(-4);          
    }

    printf("Copies...\n");
    // write n_cp blocks in external ram 
    for(int b=1; b<16; b++){
        t1   = rt_time_get_us();
       for(int i=0;i<reps;i++){
            pi_ram_copy(&ram, (uint32_t)(buff_L3), Reg, (uint32_t)b,1);  
       }
        t2   = rt_time_get_us();
    
    t = (t2-t1)/1e6;
    time = (t/reps);
    printf("%d %f %f\n", b, time, b/time);
    }

    /*-------free memory----------------------*/
    pi_ram_free(&ram, (uint32_t)buff_L3, (uint32_t) BUFFER_SIZE_L3);
    pi_ram_close(&ram);
}



/* Program Entry. */
int main(void)
{   
    printf("\n\n\t ***Memory Test ***\n\n");
    //printf("\n\n\t test name %s \n:", test_name[test]);



    return pmsis_kickoff((void *) test_memory_ram_L3_Reg);
}
