#include"MTK.h"
#include<string.h>
#include<stdio.h>

#define COMMAND_HEAD "$PMTK"
#define COMMAND_TAIL_FORM "*%X\r\n"
#define COMMAND_MAX_LEN 64

#define GGA_HEAD "$GPGGA,"
#define SEC_COUNT 10
#define SEC_LENS {6,9,9,1,9,1,1,1,0,1}

#define MTK3339_DESC 2


#define NEXT seeker = strchr(seeker, '\0') + 1
#define PUTNEXT(x) *(seeker++) = (x)
#define RANDD '0' + rand() % 10
#define PUTND(n) for(int i=0; i<(n); i++) {PUTNEXT(RANDD);}

void init_gga(char* output, char* rate)
{  
	gps_send(output);
	gps_send(rate);
} 

void gps_send(char* data)
{ 
	char out[COMMAND_MAX_LEN];
	memset(out, '\0', COMMAND_MAX_LEN);

	strcpy(out, COMMAND_HEAD);
	strcat(out, data);

	byte_t checksum = get_checksum(out);
	sprintf(strchr(out, '\0'), COMMAND_TAIL_FORM, checksum); // writes checksum and tail to end

	// printf("Command data: %s\nFull command string (length %ld): %s\n", data, strlen(out), out);
	fprintf((FILE*)2, out, strlen(out));
}
 
int parse_gga(char* nmea_output, gga_packet_t* gga_packet, size_t n)
{
	// "$GPGGA,115739.00,4158.8441367,N,19147.4416929,W,4   ,13  ,0.9,255.747,M,,,,*"
	//         ^time     ^lat           ^lon            ^fix ^sat     ^alt
	if (strncmp(nmea_output, GGA_HEAD, 7)) 
	{
		// printf("Not a GGA sentence.\n");
		return -1; // wrong type of string
	}
	register int i = 0;
	register int sec = 0;
	register int cur = 0;
	int lens[] = SEC_LENS;
	// first pass: check each section against lens for sufficient length, set ',' to \0
	while (nmea_output[i] && (i < n) && (sec < SEC_COUNT))
	{ 
		if (nmea_output[i] == ',')
		{
			if (cur < lens[sec])
			{
				// printf("Section %d (length %d) less than required length %d.\n", sec, cur, lens[sec]);
				return -1; // section length non-compliant
	  	  	}
			sec++;
			nmea_output[i] = '\0';
			cur = 0;
	     	}
	 	cur++;
		i++;
	}   
	if (sec < SEC_COUNT)
	{ 
		// printf("Too few sections.\n");
		return -1; // not enough sections were read
	}
	char* seeker = nmea_output;
	// time; very annoying
	NEXT;
	gga_packet->time = 0;
	gga_packet->time += 10*(seeker[0] - '0') + seeker[1] - '0';
	gga_packet->time *= 60;
	gga_packet->time += 10*(seeker[2] - '0') + seeker[3] - '0';
	gga_packet->time *= 60;
	gga_packet->time += 10*(seeker[4] - '0') + seeker[5] - '0';
	gga_packet->time *= 1000;
	gga_packet->time += atoi(seeker + 7);
	// latitude
	NEXT;
	sscanf(seeker, "%2d%f", // copy angle
		&((gga_packet->latitude).degrees), 
		&((gga_packet->latitude).minutes));
	NEXT;
	// set sign of angle using cardinal direction
	gga_packet->latitude.degrees *= (*seeker == 'N') ? 1 : -1;
	// longitude
	NEXT;
	sscanf(seeker, "%3d%f",
		&(gga_packet->longitude.degrees), 
		&(gga_packet->longitude.minutes));
	NEXT;
	gga_packet->longitude.degrees *= (*seeker == 'E') ? 1 : -1;
	// fix
	NEXT;
	sscanf(seeker, "%1hd", &(gga_packet->fix));
	// sattelite count
	NEXT;
	sscanf(seeker, "%2hd", &(gga_packet->sat_count));
	// altitude
	NEXT; NEXT;
	sscanf(seeker, "%f", &(gga_packet->altitude));
	return 0;
}

byte_t get_checksum(char * command)
{
	byte_t checksum = 0;
	for(int i = 1; command[i] && (i < 64); i++)
	{
		checksum ^= command[i];
	}
	return checksum;
}
