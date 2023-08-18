#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csignal>
using namespace std;

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <syslog.h>

#define READ_BUFFER_LEN 256
#define MAX_DATA 20

int main(int argc, char** argv)
{
  char read_buffer[READ_BUFFER_LEN+1];
  char write_buffer[READ_BUFFER_LEN+1];
  char *p, *pData[MAX_DATA];
  int iData;
  char ch;
  int  rc, read_len;
  FILE* of;
  char filename[FILENAME_MAX+1];
  time_t t;
  struct tm *stm;

  signal(SIGALRM, SIG_IGN);
	alarm(1);
  read_len = 0;
  do {
    if((rc = read(STDIN_FILENO, &ch, 1)) > 0)
    {
      read_buffer[read_len] = ch;
      read_len++;
    }
    else
    {
      break;
    }
  } while(read_len < READ_BUFFER_LEN);
	alarm(0);
  read_buffer[read_len] = 0;
  write_buffer[0] = 0;
	
//  printf("[DEBUG] %s\n", read_buffer);

  t = time(&t);
  stm = localtime(&t);

  /* 0   1          2  3      4 5         6 7          8 9    10 11     12       13  14 15   16     */
  /* *HQ,9172783634,V1,000115,V,3434.2058,S,05838.4060,W,0.00,33,010104,fbf7fbff,722,07,4486,27701# */
  /*
    1.- ID Cliente
    5.- Latitud
    6.- Emisferio
    7.- Longitud
    8.- Emisferio
    9.- Velocidad
  */
  if( !memcmp(read_buffer, "*HQ,", 4))
  {
    /* GPS Tracker XS-002 */
    iData = 0;
    p = &read_buffer[0];
    while(*p && iData < MAX_DATA)
    {
      pData[iData++] = p;
      while(*p && *p != ',') p++;
      while(*p && *p == ',')
      {
        *p = 0;
        p++;
      }
    }
    /* pData tiene los datos parseados */
    sprintf(write_buffer, "HORA:%02i%02i%02i,LAT:%s,LAT_EMI:%s,LONG:%s,LONG_EMI:%s,VEL:%s",
      stm->tm_hour, stm->tm_min, stm->tm_sec, pData[5], pData[6], pData[7], pData[8], pData[9]);
    sprintf(filename, "%s/TR-%04i%02i%02i-%s.log", 
      LOGDIR, stm->tm_year+1900, stm->tm_mon+1, stm->tm_mday, pData[1]);

//    printf("[DEBUG] %s\n", write_buffer);
//    printf("[DEBUG] %s\n", filename);


  }





  /* Registro */
  if(strlen(write_buffer))
  {
    of = fopen(filename, "ab");
    if(of)
    {
      strcat(write_buffer, "\n");
      fwrite(write_buffer, sizeof(char), strlen(write_buffer), of);
      fclose(of);
    }
  }
  return 0;
}

