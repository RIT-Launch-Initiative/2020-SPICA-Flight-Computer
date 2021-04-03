#include "MTK.h"
#include<string.h>
#define NMEA_OUTPUT_TEST "314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0" 
#define NMEA_RATE_TEST "220,1000"
#define GGA_TEST "$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E\r\n"
#define GGA_TEST_BROKEN "$GPRMC,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255,M,-32.00,M,01,0000*6E\r\n"

int packet_test(char* str, gga_packet_t* packetp) 
{
	if (parse_gga(str, packetp, 80))
 	{
		printf("Invalid GGA sentence\n");
		return -1;
	}
	else 
 	{
		gga_packet_t packet = *packetp;
		printf("Epoch time: %d\nLat: %+02ddeg %fmin\nLong: %+03ddeg %fmin\nAltitude: %fm\nFix: %d, Sattelites: %d\n",
			packet.time,
			packet.latitude.degrees, packet.latitude.minutes,
			packet.longitude.degrees, packet.longitude.minutes,
			packet.altitude,
			packet.fix, packet.sat_count);
	}
}

int main()
{
	printf("TESTING OUTPUT COMMAND\n");
	// init_gga(NMEA_OUTPUT_TEST, NMEA_RATE_TEST);

	char* sim_sentence = sim_gga();
	char* test_sentence = malloc(sizeof(char) * strlen(GGA_TEST));
	strcpy(test_sentence, GGA_TEST);
	gga_packet_t* sim_packet_p = malloc(sizeof(gga_packet_t));
	gga_packet_t* test_packet_p = malloc(sizeof(gga_packet_t));

	printf("TESTING SENTENCE SIMULATION\n\n");

	printf("Test GGA sentence (length %ld):\n%s\n", strlen(test_sentence), test_sentence);
	packet_test(test_sentence, test_packet_p);

	printf("Simulated GGA sentence (length %ld):\n%s\n", strlen(sim_sentence), sim_sentence);
	packet_test(sim_sentence, sim_packet_p);
	
	free(sim_sentence);
	free(test_sentence);
	free(sim_packet_p);
	free(test_packet_p);
	
	return 0;
}
