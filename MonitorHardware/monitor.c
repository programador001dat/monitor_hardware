#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sensors/sensors.h>
#include <unistd.h>

// funcao para mostrar a temperatura usando o libsensors.
void view_temp()
{
   const sensors_chip_name *chip;
   int c = 0;

   while((chip = sensors_get_detected_chips(NULL, &c)) != NULL)
   {
      const sensors_feature *feature;
      int f = 0;

      while((feature = sensors_get_features(chip, &f)) != NULL)
      {
      if(feature->type == SENSORS_FEATURE_TEMP)
         {
            const sensors_subfeature *sub;
            sub = sensors_get_subfeature(chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
            if(sub)
            {
               double temp;
               sensors_get_value(chip, sub->number, &temp);
               printf("Temp:\t%1.f ºC\n", temp);
            }
         }
      }
   }
}


// função para mostrar Memória RAM.
void view_memory()
{
   FILE *fp = fopen("/proc/meminfo", "r");
   char line[256];
   long total = 0, free = 0, space = 0;

   while(fgets(line, sizeof(line), fp))
   {
      if(sscanf(line, "MemTotal: %ld kB", &total) == 1)
      {
         continue;
      }
      if(sscanf(line, "MemAvailable: %ld kB", &free) == 1)
      {
         break;
      }
   }
   fclose(fp);

   if(total > 0)
   {
      printf("RAM:\t%.2f%% usada\n", 100.0 * (total - free) / total);
   } else {
      printf("Erro ao ler /proc/meminfo\n");
   }
}


// função para mostrar carga do OS.
void view_load()
{
   double medium_load[3];
   getloadavg(medium_load, 3);
   printf("Carga Média\t(1m/5m/15m):\t%2.f\t%2.f\t%.2f\n",
      medium_load[0] < 0 ? 0.0 : medium_load[0],
      medium_load[1] < 0 ? 0.0 : medium_load[1],
      medium_load[2] < 0 ? 0.0 : medium_load[2]);
}


// função principal
int main()
{
   sensors_init(NULL);

   while(1)
   {
      system("clear");
      printf("[]=== Monitor de Hardware ===[]\n");
      view_temp();
      view_memory();
      view_load();
      sleep(2);
   }

   sensors_cleanup();

   return 0;
}



