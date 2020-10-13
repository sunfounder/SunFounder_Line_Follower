/* C module with equivalen functionality like the original Line_Follwer.py */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/* Globals */
#define NUM_REF 5
int references[NUM_REF] = {300,300,300,300,300};
int address = 0x11;
int bus = 1;

/* Function Prototypes */
char * read_raw();
int * read_analog(int trys);
int * read_digital();
float * get_average(int mount);
int * found_line_in(float timeout);
void wait_tile_status(int *status);
void wait_tile_center();


char * read_raw(){
   int flag = 1;
   static char raw_result[NUM_REF*2+1];
   int i;  
   for(i=0;i<NUM_REF;i++){
      /* Do an i2c read and if successful break from the loop */
      break;
   }
   if(flag){ 
      return raw_result;
   }
   else{
      return NULL;
   }
}

int * read_analog(int trys){
   int i,j;
   char *raw_result;
   char high_byte, low_byte;
   static int analog_result[NUM_REF];
   if(trys <= 0){
      trys = NUM_REF;
   }
   for(j=0;j<trys;j++){
      raw_result = read_raw();
      if(raw_result != NULL){
          for(i=0;i<NUM_REF;i++){
             high_byte = raw_result[i*2] << 8;  
             low_byte = raw_result[i*2+1];  
	     analog_result[i] = high_byte + low_byte;
	     if(analog_result[i] > 1024){
		continue;
	     }
	  }
	  return analog_result;
      }
   }
   printf("Line follower read error. Please check the wiring.\n");
   return NULL; 
}

int * read_digital(){
   int * lt;
   int i;
   static int digital_list[NUM_REF];
   lt = read_analog(NUM_REF);
   for(i=0;i<NUM_REF;i++){
      if(lt[i] > references[i]){
         digital_list[i] = 0;
      }
      else if(lt[i] < references[i]){
         digital_list[i] = 1;
      }
      else{
         digital_list[i] = -1;
      }
   }
   return digital_list;
}

float * get_average(int mount){
   static float average[NUM_REF] = {0.0,0.0,0.0,0.0,0.0};
   float *lt_list [NUM_REF];
   int i,lt_id;
   int * lt;
   float sum;
   for(i=0;i<NUM_REF;i++){
      lt_list[i] = malloc(sizeof(float)*mount);
   }
   for(i=0;i<mount;i++){
      lt = read_analog(NUM_REF);
      for(lt_id=0;lt_id<NUM_REF;lt_id++){
         lt_list[lt_id][i] = lt[lt_id];
      } 
   }
   for(lt_id=0;lt_id < NUM_REF;lt_id++){
      sum = 0;
      for(i=0;i<mount;i++){
	  sum += lt_list[lt_id][i];
      }
      average[lt_id] = sum/mount;
   }
   for(i=0;i<NUM_REF;i++){
      free(lt_list[i]);
   }
return average;
}

int * found_line_in(float timeout){
   int * lt_status;
   float time_during;
   clock_t time_start;
   clock_t time_now;
   time_start = clock();
   time_during = 0;
   while(time_during < timeout){   
       lt_status = read_digital();
       for(int i=0;i<NUM_REF;i++){
	  if(lt_status[i] == 1){
             return lt_status;
	  }
       }
       time_now = clock();
       time_during = (time_now - time_start)/1000000; /* Convert from micro to seconds */
   }
   return NULL;

}

void wait_tile_status(int * status){
   int * lt_status;
   int i;
   int flag = 1;
   while(1){
       flag = 1;
       lt_status = read_digital();
       for(i=0;i<NUM_REF;i++){
          if(lt_status[i] != status[i]){
              flag = 0; 
          }
       }
       if(flag == 1){
	  break;
       }
   }
}

void wait_tile_center(){
   int * lt_status;
   while(1){
       lt_status = read_digital();
       if(lt_status[2] == 1){
          break;
       }
   }
}

