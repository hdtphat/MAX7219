void Wasting_time (void);

void Wasting_time (void){
	// Simple inaccurate delay without timer
	for(int i=0; i<700; i++){
		unsigned int count = 0xFFFF;
		while(count--);
	}
}
