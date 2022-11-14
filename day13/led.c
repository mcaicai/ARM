void Delay(unsigned int time)
{
	while(time--);
}
typedef struct 
{
	unsigned int CON;
	unsigned int DAT;
	unsigned int PUD;
	unsigned int DRV;
}gpx2;

#define GPX2 (*(gpx2 *)0x11000c40)

int main()
{
	GPX2.CON = GPX2.CON & (~(0xF << 28)) | (0x1 << 28);
	
	while(1)
	{
		GPX2.DAT = GPX2.DAT | (1<<7);
		Delay(1000000);
		GPX2.DAT = GPX2.DAT & (~(1<<7));
		Delay(1000000);
	}
	return 0;

}
