#ifndef MTK_H
#define MTK_H

#include<stdlib.h>
#include<stdio.h>

// GGA only
#define NMEA_OUTPUT_DEFAULT "314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
// Update rate 1 Hz
#define NMEA_RATE_DEFAULT "220,1000"

typedef unsigned char byte_t;

typedef struct angle // 8 bytes
{
	int degrees; // N positive
	float minutes; // E positive
} angle_t;

typedef struct gga // 40 bytes
{
	char time[12]; // as HH:mm:ss.ss
	angle_t latitude;
	angle_t longitude;
	float alt;
	int fix;
	int sat_count;
} gga_packet_t;


/* SETUP: (TODO: test send)
 * init(NMEA_OUTPUT_DEFAULT, NMEA_RATE_DEFAULT);
*/

void init_gga(char* output, char* rate);
	// generates and sends output commands
	// frees memory allocated by generate_command()

char* sim_gga();
	// ALLOCATES HEAP MEMORY

void gps_send(char* data);
	// null-terminated data string

void parse_gga(char* nmea_string, gga_packet_t* gga_packet);
	// string containing GGA sentence (need not be null-terminated)

byte_t get_checksum(char* command);
	// null-terminated string

#endif
