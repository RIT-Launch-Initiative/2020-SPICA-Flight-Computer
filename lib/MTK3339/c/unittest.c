#include "MTK.h"
#include<string.h>
#define NMEA_OUTPUT_TEST "314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0" 
#define NMEA_RATE_TEST "220,1000"
// #define GGA_TEST "$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E\r\n"
#define GGA_TEST_BROKEN "$GPGGA,115739.00,4158.8441367,N,09147.4416929"

int main()
{
	printf("TESTING OUTPUT COMMAND\n");
	init_gga(NMEA_OUTPUT_TEST, NMEA_RATE_TEST);

	char* sim_sentence = sim_gga();
	char* test_sentence = malloc(sizeof(char) * strlen(GGA_TEST_BROKEN));
	strcpy(test_sentence, GGA_TEST_BROKEN);
	gga_packet_t* sim_packet_p = malloc(sizeof(gga_packet_t));
	gga_packet_t* test_packet_p = malloc(sizeof(gga_packet_t));

	printf("TESTING SENTENCE SIMULATION\n\n");
	printf("Test (broken) GGA sentence (length %ld):\n%s\n", strlen(test_sentence), test_sentence);

	int p = parse_gga(test_sentence, test_packet_p, 80);
	if (p) {
		printf("Invalid GGA sentence.\n");
	} else
	{
		gga_packet_t test_packet = *test_packet_p;

		printf("Time: %s\nLat: %+02ddeg %fmin\nLong: %+03ddeg %fmin\nAltitude: %fm\nFix: %d, Sattelites: %d\n\n",
			test_packet.time, // 115739.00
			test_packet.latitude.degrees, test_packet.latitude.minutes, // +41deg 58.8441367min
			test_packet.longitude.degrees, test_packet.longitude.minutes,// -191deg 47.4416929min
			test_packet.altitude, // 255.747
			test_packet.fix, test_packet.sat_count); // fix 4, sat 13

	}

	printf("Simulated GGA sentence (length %ld):\n%s\n", strlen(sim_sentence), sim_sentence);
	
	parse_gga(sim_sentence, sim_packet_p, 80);
	gga_packet_t sim_packet = *sim_packet_p;
	
	printf("Time: %s\nLat: %+02ddeg %fmin\nLong: %+03ddeg %fmin\nAltitude: %fm\nFix: %d, Sattelites: %d\n",
			sim_packet.time,
			sim_packet.latitude.degrees, sim_packet.latitude.minutes,
			sim_packet.longitude.degrees, sim_packet.longitude.minutes,
			sim_packet.altitude,
			sim_packet.fix, sim_packet.sat_count);
	
	free(sim_sentence);
	free(test_sentence);
	free(sim_packet_p);
	free(test_packet_p);
	
	return 0;
}
