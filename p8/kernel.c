#include "hype.h"


//TEXT INIT
unsigned char start_kernel[50] = "C0:Start...";
unsigned char c1_int[50] = "C0:Int Core 1...";

void
kernel()
{
	unsigned int then, now, delta;

	init_video_code();
	#include "initf.auto"
 
    //PRINT TEXT
    print_text(start_kernel);

    while (1) {
		now = now_usec();
		delta = usec_diff( now, then );
		if (delta > 5 * ONE_SEC) {
			then = now;
            print_text(c1_int);
			while (1) continue;
		}
	}
}

