#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char name[8];
	int  offset;
	int  length;
} listing;

const char zeroes[] =
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int main(int argc, char **argv)
{
	char *buf;
	char fileName[9] = {0};
	int pos;
	FILE *in, *out;
	listing l;
	
	if (argc != 2)
	{
		printf("Usage: %s input.sbin\n", argv[0]);
		return 1;
	}
	
	if ((in = fopen(argv[1], "rb")) == 0)
	{
		perror("Error");
		return 1;
	}
	
	for (;;)
	{
		fread(&l, sizeof(listing), 1, in);
		
		if (memcmp(&l, zeroes, sizeof(listing)) == 0)
			break;
		
		strncpy(fileName, l.name, 8);
		out = fopen(fileName, "wb");
		
		printf("Extracting %s...\n", fileName);

		buf = malloc(l.length);
		pos = ftell(in);
		
		fseek(in, l.offset, SEEK_SET);
		fread(buf, 1, l.length, in);
		fwrite(buf, 1, l.length, out);
		free(buf);

		fseek(in, pos, SEEK_SET);
	}
	
	fclose(in);
	fclose(out);

	puts("\nDone!");
	
	return 0;
}