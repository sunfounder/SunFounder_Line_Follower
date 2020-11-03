/* C module with equivalen functionality like the original Line_Follwer.py */

#include <stdio.h>
#include "Line_Follower.h" 

int main(){
   printf("Line Follower C Test Application Running\n");
   int *dig_list; 
   int i;
   dig_list = read_digital();
   for(i=0;i<NUM_REF;i++){
      printf("%d\n",dig_list[i]);
   }
   return 0;
}
