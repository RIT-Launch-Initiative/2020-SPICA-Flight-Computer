#include <stdlib.h>
#include <stdio.h>

#ifndef MTK_H
#define MTK_H

// GGA only
#define NMEA_OUTPUT_DEFAULT "314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
// Update rate 1 Hz
#define NMEA_RATE_DEFAULT "220,1000"

typedef unsigned char byte_t;

typedef struct angle // 8 bytes
{
	int degrees; // sign here; N,E positive
	float minutes;
} angle_t;

typedef struct gga // 26 bytes
{
	angle_t latitude;
	angle_t longitude;
	int time; // ms since midnight
	float altitude;
	short fix;
	short sat_count;
} gga_packet_t;


/* SETUP: (TODO: test send)
 * init(NMEA_OUTPUT_DEFAULT, NMEA_RATE_DEFAULT);
*/

void init_gga(char* output, char* rate);
	// generates and sends output commands

void gps_send(char* data);
	// null-terminated data string

int parse_gga(char* nmea_string, gga_packet_t* gga_packet, size_t n);
	// string containing GGA sentence (need not be null-terminated), read at most n characters
	// returns 1 if nmea_string contains less than 9 characters

byte_t get_checksum(char* command);
	// XOR of null-terminated string after first character

#endif
