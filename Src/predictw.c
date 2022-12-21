#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "stdio.h"
#include "string.h"
#include "dma.h"
#include "hts221.h"
#include "lps22hb.h"
#include <math.h>
#include "ili9163.h"
#include "spi.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

unsigned long delayTime;
   int t_hour=0;
   int t_minute=0;

   int station2sealevel(int p, int height, int t){
     return (double) p*pow(1-0.0065*(double)height/(t+0.0065*(double)height+273.15),-5.275);
   }

   int myabs(int val){
     if (val<0)
       return -val;
     return val;
   }

   int calc_zambretti(int curr_pressure, int prev_pressure, int mon){
           if (curr_pressure<prev_pressure){
             //FALLING
             if (mon>=4 && mon<=9)
             //summer
             {
               if (curr_pressure>=1030)
                 return 2;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 8;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 18;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 21;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 24;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 24;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 26;
               else if(curr_pressure<970)
                 return 26;
             }
             else{
             //winter
               if (curr_pressure>=1030)
                 return 2;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 8;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 15;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 21;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 22;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 24;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 26;
               else if(curr_pressure<970)
                 return 26;
             }
           }
           else if (curr_pressure>prev_pressure){
             //RAISING
             if (mon>=4 && mon<=9){
               //summer
               if (curr_pressure>=1030)
                 return 1;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 2;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 3;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 7;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 9;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 12;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 17;
               else if(curr_pressure<970)
                 return 17;
             }
             else
               //winter
              {
               if (curr_pressure>=1030)
                 return 1;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 2;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 6;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 7;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 10;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 13;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 17;
               else if(curr_pressure<970)
                 return 17;
              }
           }
           else{
               if (curr_pressure>=1030)
                 return 1;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 2;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 11;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 14;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 19;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 23;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 24;
               else if(curr_pressure<970)
                 return 26;

           }

   }

   int pressureArray[10]={0};
   int counter=0;
   int delta_time=0;

   //lsm6ds0_init();
   //hts221_init();
   //lps22hb_init();

   int weather() {
     char tStr[21];
     char pStr[22];
     char hStr[20];
     char pseaStr[26];
     char timeStr[6];
     char dateStr[12];
     char zambretti[10]="N/A";
     char pressureHistory[57];
     int temperature=(int)lps22hb_get_temp();
     //int temperature=(int)hts221_get_temp();
     int humidity=hts221_get_humid();
     int pressure=(int)(lps22hb_get_pressure()/100.0F);
     int altitude=560;//(int)bme.readAltitude(SEALEVELPRESSURE_HPA);
     int seapressure = station2sealevel(pressure,altitude,temperature);
     int weather; //1-sunny 2-sunny_cloudy 3-cloudy 4-worsening 5-raining
     //int seapressure = station2sealevel(944,595,27);

     //DateTime now = rtc.now();
     int t_hour2=1;
     int t_minute2=1;
     int Z=0;

     if (t_hour2!=t_hour || t_minute2!=t_minute){
       delta_time++;
       if (delta_time>10){
         delta_time=0;

         if (counter==10)
         {
           for (int i=0; i<9;i++){
             pressureArray[i]=pressureArray[i+1];
           }
           pressureArray[counter-1]=seapressure;
         }
         else{
           pressureArray[counter]=seapressure;
           counter++;
         }
       }
     Z=calc_zambretti((pressureArray[9]+pressureArray[8]+pressureArray[7])/3,(pressureArray[0]+pressureArray[1]+pressureArray[2])/3, 12);
     sprintf(zambretti, "Z=%d", Z);
        sprintf(tStr, "%d C", temperature);
        sprintf(hStr, "%d %%", humidity);
        sprintf(pStr, "%d hPa", pressure);
        sprintf(pseaStr, "%d hPa", seapressure);
        //sprintf(dateStr, "%02d.%02d.%d", now.day(), now.month(),now.year());
        //sprintf(timeStr, "%02d:%02d", now.hour(), now.minute());


      //  DEBUG, REMOVED
   //     pressureArray[9]=1020;
   //     pressureArray[8]=1020;
   //     pressureArray[7]=1020;
   //     pressureArray[6]=1020;
   //     pressureArray[5]=1020;
   //     pressureArray[4]=1020;
   //     pressureArray[3]=1020;
   //     pressureArray[2]=1020;
   //     pressureArray[1]=1020;
   //     pressureArray[0]= 1021;
   //     Z=6;

        if (pressureArray[9]>0 && pressureArray[0]>0){
           if (pressureArray[9]+pressureArray[8]+pressureArray[7]-pressureArray[0]-pressureArray[1]-pressureArray[2]>=3){
         //RAISING
           if (Z<3){
             weather = 1;//sunny(500,350);
           }
           else if (Z>=3 && Z<=9){
        	   weather = 2;//sunny_cloudy(400,350);
           }
           else if (Z>9 && Z<=17)
        	   weather = 3;//cloudy(300,300);
           else if (Z>17){
        	   weather = 5;//rainy(300,300);
             }
         }

           else if (pressureArray[0]+pressureArray[1]+pressureArray[2]-pressureArray[9]-pressureArray[8]-pressureArray[7]>=3){
         //FALLING

           if (Z<4)
        	   weather = 1;//sunny(400,350);
           else if (Z>=4 && Z<14){
        	   weather = 2;//sunny_cloudy(400,350);
           }
           else if (Z>=14 && Z<19){
        	   weather = 4;//worsening(400,350);
           }
           else if (Z>=19 && Z<21)
        	   weather = 3;//cloudy(300,300);
           else if (Z>=21){
        	   weather = 5;//rainy(300,300);
             }
         }
         else{
          //STEADY
           //draw_arrow(670,450,0);
          if (Z<5)
        	  weather = 1;//sunny(400,350);
           else if (Z>=5 && Z<=11){
        	   weather = 2;//sunny_cloudy(400,350);
           }
           else if (Z>11 && Z<14)
        	   weather = 3;//cloudy(300,300);
           else if (Z>=14 && Z<19){
        	   weather = 4;//worsening(400,350);
           }
           else if (Z>19){
        	   weather = 5;//rainy(300,300);
           }
         }
        }
        else{
         if (seapressure<1005)
        	 weather = 5;//rainy(300,300);
         else if (seapressure>=1005 && seapressure<=1015)
        	 weather = 3;//cloudy(300,300);
         else if (seapressure>1015 && seapressure<1025)
        	 weather = 2;//sunny_cloudy(400,350);
         else
        	 weather = 5;//rainy(300,300);
        }


       t_hour=t_hour2;
       t_minute=t_minute2;


     }

       return weather;
   }
