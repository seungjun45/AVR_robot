#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main (void)
 
{
	char str[80];
	char rstr[80];
	char buff[256];
 
	FILE *comport;
	if ((comport = fopen("COM6", "r+")) == NULL)
		{
			printf("Failed to open the communication port COM6\n");
			printf("The port may be disabled or in use\n");
			printf("enter \"quit\" to exit \n");
			getch();
		}
	else
		{
		printf("COM6 opended successfully\n");
		
			for(;;)
			{
				printf ("enter a string \n");
				fgets (str , 80 , stdin);						
				printf ("\n%s\n",str);
				fprintf (comport, "%s\n",str);
				fflush(comport);
 
				fgets(buff, sizeof buff, comport);  
				
				strcpy(rstr, buff );
				printf("s%\n",rstr);
											
			}
		fclose(comport);
		
 
		}
	return 0;
}