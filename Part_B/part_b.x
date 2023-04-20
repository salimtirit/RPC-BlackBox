struct numbers {
	char name[256];
	char path[256];
	int a;
	int b;
};

program PART_B_PROG {
	version PART_B_VERS {
		string blackbox(numbers) = 1;
	}=1; 
}=0x11111111;
