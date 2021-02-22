#include"MTK.h"
#include<string.h>
#define NEXT data = strchr(data, ',') + 1

// MTK3339 commands take the form $PMTK<command id>,<data>*<checksum byte as hex>\r\n
#define COMMAND_HEAD "$PMTK"
#define COMMAND_TAIL_FORM "*%X\r\n"
// The GGA data is preceded by $GPGGA,
#define GGA_HEAD "$GPGGA,"
#define MTK3339_DESC 2

using namespace MTK3339;


void MTK3339::init(char* output, char* rate)
{
	char* output_c = generate_command(output);
	char* rate_c = generate_command(rate);
	send(output_c);
	send(rate_c);
	free(output_c);
	free(rate_c);
}

gga_packet_t MTK3339::read_packet()
{
	char* gga_sentence = read_gga();
	gga_packet_t data = parse_gga(gga_sentence);
	free(gga_sentence);
	return data;
}

char* MTK3339::generate_command(char* data)
{
	size_t data_len = strlen(data) + strlen(COMMAND_HEAD) + strlen(COMMAND_TAIL_FORM);
	char* out = (char*)malloc(data_len + 1);
	sprintf(out, "%s%s", COMMAND_HEAD, data); // writes head and data
	byte_t checksum = get_checksum(out);
	sprintf(strchr(out, '\0'), COMMAND_TAIL_FORM, checksum); // writes tail to end
	return out;
}


gga_packet_t MTK3339::parse_gga(char* nmea_output)
{
	gga_packet_t parsed;

	char* data = strstr(nmea_output, GGA_HEAD) + strlen(GGA_HEAD);

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

byte_t MTK3339::get_checksum(char * command)
{
	byte_t checksum = 0;
	for(int i = 1; command[i]; i++)
	{
		// printf("XOR with \'%c\'", command[i]);
		checksum ^= command[i];
	}
	return checksum;
}


void MTK3339::send(char* command)
{
	fprintf((FILE*)2, command, strlen(command));
}

char* MTK3339::read_gga()
{
	FILE* nmea = fdopen(MTK3339_DESC, "r");
	char* packet = (char*)malloc(256);
	strcpy(packet, GGA_HEAD);
	char* data = packet + strlen(GGA_HEAD);
	fscanf(nmea, "$GPGGA,%s\r\n", data);

	return packet;
}
