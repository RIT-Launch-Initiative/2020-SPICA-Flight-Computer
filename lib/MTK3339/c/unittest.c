#include "MTK.h"

#define NMEA_OUTPUT_TEST "314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0" 
#define NMEA_RATE_TEST "220,1000"
#define GGA_TEST "$GPGGA,115739.00,4158.8441367,N,19147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E"

int main()
{
	printf("TESTING OUTPUT COMMAND\n");
	char* output_cmd = generate_command(NMEA_OUTPUT_TEST);
	printf("Output command: %s\n", output_cmd); // expected checksum 1F
	free(output_cmd);
	
	char* rate_cmd = generate_command(NMEA_RATE_TEST);
	printf("Rate command: %s\n", rate_cmd);
	free(rate_cmd);
	
	char* sim_sentence = sim_gga();
	gga_packet_t sim_packet = parse_gga(sim_sentence);
	gga_packet_t test_packet = parse_gga(GGA_TEST);

	printf("TESTING SENTENCE SIMULATION\n\n");
	printf("Test GGA sentence:\n%s\n", GGA_TEST);
	printf("Time: %s\nLat: %+02ddeg %fmin\nLong: %+03ddeg %fmin\nFix: %d, Sattelites: %d\n\n",
			test_packet.time, // 115739.00
			test_packet.latitude.degrees, test_packet.latitude.minutes, // +41deg 58.8441367min
			test_packet.longitude.degrees, test_packet.longitude.minutes,// -191deg 47.4416929min
			test_packet.fix, test_packet.sat_count); // fix 4, sat 13

	printf("Simulated GGA sentence:\n%s\n", sim_sentence);
	printf("Time: %s\nLat: %+02ddeg %fmin\nLong: %+03ddeg %fmin\nFix: %d, Sattelites: %d\n",
			sim_packet.time,
			sim_packet.latitude.degrees, sim_packet.latitude.minutes,
			sim_packet.longitude.degrees, sim_packet.longitude.minutes,
			sim_packet.fix, sim_packet.sat_count);
	
	free(sim_sentence);
	return 0;
}
