#include"MTK.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define NEXT data = strchr(data, ',') + 1
// MTK3339 commands take the form $PMTK<command id>,<data>*<checksum hex char>\r\n
#define COMMAND_HEAD "$PMTK"
#define COMMAND_TAIL_FORM "*%X\r\n"

// for debugging
#define NMEA_OUTPUT_TEST "314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0"
#define NMEA_RATE_TEST "220,1000"
#define GGA_TEST "$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E"
int main()
{
	char* output_cmd = generate_command(NMEA_OUTPUT_TEST);
	printf("Output command: %s\n", output_cmd); // expected checksum 1F
	free(output_cmd);

	char* rate_cmd = generate_command(NMEA_RATE_TEST);
	printf("Rate command: %s\n", rate_cmd);
	free(rate_cmd);

	struct gga test = parse_gga(GGA_TEST);
	printf("Time: %s\nLat: %+02ddeg %fmin\nLong: %+03ddeg %fmin\nFix: %d, Sattelites: %d\n",
			test.time, // 115739.00
			test.latitude.degrees, test.latitude.minutes, // +41deg 58.8441367min
			test.longitude.degrees, test.longitude.minutes,// -091deg 47.4416929min
			test.fix, test.sat_count); // fix 4, sat 13
	return 0;
}

char* generate_command(char* data)
{
	size_t data_len = strlen(data) + strlen(COMMAND_HEAD) + strlen(COMMAND_TAIL_FORM);
	char* out = malloc(data_len + 1);
	sprintf(out, "%s%s", COMMAND_HEAD, data); // writes head and data
	unsigned char checksum = get_checksum(out);
	sprintf(strchr(out, '\0'), COMMAND_TAIL_FORM, checksum); // writes tail to end
	return out;
}


struct gga parse_gga(char* nmea_output)
{
	struct gga parsed;

	char* data = strstr(nmea_output, "GPGGA,") + strlen("GPGGA,");

	memset(parsed.time, 9, '0');
	strncpy(parsed.time, data, 9);

	char degbuf[4] = "";
	char minbuf[20] = "";
	// latitude (DDMM.MM ...)
	NEXT;
	strncpy(degbuf, data, 2); // copies DD to degbuf
	data += 2;
	strncpy(minbuf, data, strchr(data, ',') - data); // copies MM.MM ... to minbuf
	NEXT; // advances to cardinal
	parsed.latitude.degrees = atoi(degbuf) * ((*data == 'S') ? -1 : 1);
	parsed.latitude.minutes = atof(minbuf);

	// longitude (DDDMM.MM ...)
	NEXT;
	strncpy(degbuf, data, 3); // copies DDD to degbuf
	data += 3;
	strncpy(minbuf, data, strchr(data, ',') - data); // copies MM.MM ... to minbuf
	NEXT; // advances to cardinal

	parsed.longitude.degrees = atoi(degbuf) * ((*data == 'W') ? -1 : 1);
	parsed.longitude.minutes = atof(minbuf);

	// fix
	NEXT;
	parsed.fix = *data - '0';
	// sat_count
	NEXT;
	parsed.sat_count = 10 * (data[0] - '0') + (data[1] - '0');

	return parsed;
}

unsigned char get_checksum(char * command)
{
	unsigned char checksum = 0;
	for(int i = 1; command[i]; i++)
	{
		// printf("XOR with \'%c\'", command[i]);
		checksum ^= command[i];
	}
	return checksum;
}


void send(char* command)
{
	// fprintf(2, command, strlen(command));
}

char* read_nmea()
{
	return NULL;
}
