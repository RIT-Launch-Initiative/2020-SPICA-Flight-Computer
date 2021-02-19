// #include "main.h"

#include<stdlib.h>
#include<stdio.h>

#define NMEA_OUTPUT_DEFAULT "314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
#define NMEA_RATE_DEFAULT "220,1000"

typedef unsigned char byte_t;

typedef struct angle
{
	int degrees; // N positive
	double minutes; // E positive
} angle_t;

typedef struct gga 
{
	char time[10]; // as HH:mm:ss
	angle_t latitude;
	angle_t longitude;
	int fix;
	int sat_count;
} gga_packet_t;

/* SETUP: (SEND NOT IMPLEMENTED)
 * char* output_set = generate_command(NMEA_OUTPUT_DEFAULT);
 * send(output);
 * free(output);
 * char* rate_set = generate_command(NMEA_RATE_DEFAULT);
 * send(rate);
 * free(rate); 
*/

/* READ STRING CONTAINING GGA SENTENCE: (READ NOT IMPLEMENTED)
 * char* output = read_nmea();
 * struct gga data = parse_gga(output); 
*/

char* generate_command(char* data); // null-terminated data string (use defaults)
	// Calculates checksum, formats
	// ALLOCATES HEAP MEMORY
void send(char* command); // null-terminated command string
	// sends command using fprintf(2, command, n)
char* read_nmea();
	// reads from chipset into buffer using HAL_UART_Receive_IT(handle, return char, length)
	// TODO: figure out what the handle is
	// TODO: figure out how long the return string is
struct gga parse_gga(char* nmea); // null-terminated output from read_nmea
unsigned char get_checksum(char* command); // command is a null-terminated string
