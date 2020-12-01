/* C module with equivalen functionality like the original Line_Follwer.py */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "Line_Follower.h" 

/* Globals */
#define RAW_LEN (2*NUM_REF)
int references[NUM_REF] = {300,300,300,300,300};
const int SLAVE_ADDRESS = 0x11;
int bus = 1;



int read_i2c(char *buffer,int length){
   int file_i2c;
   //----- OPEN THE I2C BUS -----
   char *filename = (char*)"/dev/i2c-1";
   printf("Line_Follower.c :: read_i2c() called\n");
   if ((file_i2c = open(filename, O_RDWR)) < 0)
   {
	//ERROR HANDLING: you can check errno to see what went wrong
	printf("Failed to open the i2c bus");
	return 0;
   }
	
   int addr = SLAVE_ADDRESS;          //<<<<<The I2C address of the slave
   if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
   {
	printf("Failed to acquire bus access and/or talk to slave.\n");
	//ERROR HANDLING; you can check errno to see what went wrong
	return 0;
   }
	
	
   //----- READ BYTES -----
   if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
   {
	//ERROR HANDLING: i2c transaction failed
	printf("Failed to read from the i2c bus.\n");
   }
   else
   {
	printf("Line_Follower.c :: read_i2c() :: Data read: %s\n", buffer);
   }
   close(file_i2c);
}

char * read_raw(){
   int flag = 0;
   static char raw_result[RAW_LEN+1];
   int i;  
   printf("Line_Follower.c :: read_raw() called\n");
   for(i=0;i<NUM_REF;i++){
      /* Do an i2c read and if successful break from the loop */
      if(read_i2c(raw_result,RAW_LEN)){
	 flag = 1;
         break;
      }
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
   printf("Line_Follower.c :: read_analog() called\n");
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
      else{
	 break;
      }
   }
   printf("Line follower read error. Please check the wiring.\n");
   return NULL; 
}

int * read_digital(){
   int * lt;
   int i;
   static int digital_list[NUM_REF] = {0};
   printf("Line_Follower.c :: read_digital() called\n");
   lt = read_analog(NUM_REF);
   if(lt != NULL){
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
   int i;
   time_start = clock();
   time_during = 0;
   while(time_during < timeout){   
       lt_status = read_digital();
       for(i=0;i<NUM_REF;i++){
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

