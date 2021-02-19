#include "MTK.h"

// for debugging
#define NMEA_OUTPUT_TEST "314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0" 
#define NMEA_RATE_TEST "220,1000"
#define GGA_TEST "$GPGGA,115739.00,4158.8441367,N,19147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E"

int main()
{
	char* output_cmd = generate_command(NMEA_OUTPUT_TEST);
	printf("Output command: %s\n", output_cmd); // expected checksum 1F
	free(output_cmd);
	
	char* rate_cmd = generate_command(NMEA_RATE_TEST);
	printf("Rate command: %s\n", rate_cmd);
	free(rate_cmd);

	gga_packet_t test = parse_gga(GGA_TEST);
	printf("Time: %s\nLat: %+02ddeg %fmin\nLong: %+03ddeg %fmin\nFix: %d, Sattelites: %d\n",
			test.time, // 115739.00
			test.latitude.degrees, test.latitude.minutes, // +41deg 58.8441367min
			test.longitude.degrees, test.longitude.minutes,// -191deg 47.4416929min
			test.fix, test.sat_count); // fix 4, sat 13
	return 0;
}
