#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
unsigned char S[ 256 ];
unsigned int i, j;
void rc4_init( unsigned const char* key, unsigned int key_length )
{
	unsigned char temp;

	for( i = 0; i != 256; ++i )
		S[ i ] = i;

	for( i = j = 0; i != 256; ++i ){
		j = ( j + key[ i % key_length ] + S[ i ] ) % 256;
			temp = S[ i ];
		S[ i ] = S[ j ];
		S[ j ] = temp;
	}

	i = j = 0;
}
unsigned char rc4_output()
{
	unsigned char temp;

	i = ( i + 1 ) % 256;
	j = ( j + S[ i ] ) % 256;

	temp = S[ j ];
	S[ j ] = S[ i ];
	S[ i ] = temp;

	return S[ ( temp + S[ j ] ) % 256 ];
}



int main(int argc, char * argv[]){
	if (argc != 2){
		return 1;
	}
	int len = strlen(argv[1]);
	if (len != 11){
		return 2;
	}
	unsigned char qq [10];
	srand(31337);
	for (int i=0; i<10; i++){
		qq[i] = rand();
	}
	qq[10] = 0;
	rc4_init(qq,8);
	int valid = 0;
	int idx = 5;
//	char buf[12];
//	buf[11]=0;
	do {
		unsigned char res = rc4_output();
		res = res % 26;
		res = 'A' + res;
//		printf("%d\n",valid);
		valid = valid + (res - argv[1][idx])* (res - argv[1][idx]);
//		buf[idx] = res;
		idx = (idx + 43) % 11;
	} while (idx != 5);
//	printf("%s\n",buf);
	// ZVDYAPSRERB
	if (valid == 0){
		printf("Congrats, flag is STCTF#%s#\n",argv[1]);
	}
	return 0;
}
