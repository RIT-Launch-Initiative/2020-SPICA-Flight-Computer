#include<stdlib.h>
#include<stdio.h>

// GGA only
#define NMEA_OUTPUT_DEFAULT "314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
// Update rate 1 Hz
#define NMEA_RATE_DEFAULT "220,1000"

typedef unsigned char byte_t;

typedef struct angle
{
	int degrees; // N positive
	double minutes; // E positive
} angle_t;

typedef struct gga 
{
	char time[12]; // as HH:mm:ss.ss
	angle_t latitude;
	angle_t longitude;
	int fix;
	int sat_count;
} gga_packet_t;


/* SETUP: (TODO: test send)
 * init(NMEA_OUTPUT_DEFAULT, NMEA_RATE_DEFAULT);
*/

/* READ STRING CONTAINING GGA SENTENCE: (TODO: test read)
 * gga_packet_t data = read_packet();
*/

void init_gga(char* output, char* rate);
	// generates and sends output commands
	// frees memory allocated by generate_command()

gga_packet_t read_packet();
	// reads gga packet (unsafely), generates packet
	// frees memory allocated by read_nmea();

char* sim_gga();
	// ALLOCATES HEAP MEMORY
char* generate_command(char* data); // null-terminated data string (use defaults)
	// Calculates checksum, formats
	// ALLOCATES HEAP MEMORY
void send(char* command); // null-terminated command string
	// sends command using fprintf(2, command, strlen(command))
char* read_gga();
	// reads using fscanf(char*, "$GPGGA,%s\r\n", char*);
	// ALLOCATES HEAP MEMORY
struct gga parse_gga(char* nmea); // null-terminated output from read_nmea
byte_t get_checksum(char* command); // command is a null-terminated string
