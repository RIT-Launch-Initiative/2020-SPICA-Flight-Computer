#include"MTK.h"
#include<string.h>

// MTK3339 commands take the form $PMTK<command id>,<data>*<checksum byte as hex>\r\n
#define COMMAND_HEAD "$PMTK"
#define COMMAND_TAIL_FORM "*%X\r\n"
#define COMMAND_MAX_LEN 64
// The GGA data is preceded by $GPGGA,
#define GGA_HEAD "$GPGGA,"

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
	sprintf(strchr(out, '\0'), COMMAND_TAIL_FORM, checksum); // writes tail to end
	printf("Command data: %s\nFull command string (length %zu): %s\n", data, strlen(out), out);
	// fprintf(2, out, strlen(out));
}

void parse_gga(char* nmea_output, gga_packet_t* gga_packet)
{
	// "$GPGGA,115739.00,4158.8441367,N,19147.4416929,W,4   ,13  ,0.9,255.747,M,,,,*"
	//         ^time     ^lat           ^lon            ^fix ^sat     ^alt
	// sets all ',' to null bytes
	for (int i = 0; nmea_output[i] != '*'; i++)
	{
		if (nmea_output[i] == ',')
		{
			nmea_output[i] = '\0';
		}
	}
	char* seeker = nmea_output;
	// time
	NEXT;
	printf("Time section %s:\n", seeker);
	char* time_s = gga_packet->time;
	time_s[11] = '\0';
	time_s[2] = time_s[5] = ':';
	sscanf(seeker, "%2c%2c%5s", time_s, time_s + 3, time_s + 6);
	// latitude
	NEXT;
	printf("Latitude section: %s\n", seeker);
	angle_t* lat = &(gga_packet->latitude);
	sscanf(seeker, "%2d%f", &(lat->degrees), &(lat->minutes));
	NEXT;
	lat->degrees *= (*seeker == 'N') ? 1 : -1;
	// longitude
	NEXT;
	printf("Longitude section: %s\n", seeker);
	angle_t* lon = &(gga_packet->longitude);
	sscanf(seeker, "%3d%f", &(lon->degrees), &(lon->minutes));
	NEXT;
	lon->degrees *= (*seeker == 'E') ? 1 : -1;
	// fix
	NEXT;
	printf("Fix section: %s\n", seeker);
	int* fix = &(gga_packet->fix);
	sscanf(seeker, "%d", fix);

	// sattelite count
	NEXT;
	printf("Sat count section: %s\n", seeker);
	int* sat = &(gga_packet->sat_count);
	sscanf(seeker, "%d", sat);

	// altitude
	NEXT; NEXT;
	float* alt = &(gga_packet->alt);
	sscanf(seeker, "%f", alt);
}

char* sim_gga()
{
	char* sentence = malloc(256);
	strcpy(sentence, "$GPGGA,");
	char* seeker = strchr(sentence, '\0');

	// time section
	PUTND(6);
	PUTNEXT('.');
	PUTND(2);
	PUTNEXT(',');
	// lat section
	PUTND(4);
	PUTNEXT('.');
	PUTND(7);
	PUTNEXT(',');
	// lat direction
	PUTNEXT((rand() % 2) ? 'N' : 'S');
	PUTNEXT(',');
	// longe section
	PUTND(5);
	PUTNEXT('.');
	PUTND(7);
	PUTNEXT(',');
	// longe direction
	PUTNEXT((rand() % 2) ? 'W' : 'E');
	PUTNEXT(',');
	// fix
	PUTNEXT(RANDD);
	PUTNEXT(',');
	// sat count
	PUTND(2);
	PUTNEXT(',');
	// the rest we don't care about
	strcpy(seeker, "X.X,XXX.XXX,C,SXX.XX,C,XX,XXXX*XX\r\n");
	return sentence;
}

byte_t get_checksum(char * command)
{
	byte_t checksum = 0;
	for(int i = 1; command[i]; i++)
	{
		checksum ^= command[i];
	}
	return checksum;
}
