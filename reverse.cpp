 #include <openssl/rand.h>
#include <byteswap.h>
#include <limits>
#include <iostream>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <string>
#include <math.h>

using namespace std;

const unsigned int A = 1103515245;
const unsigned int C = 12345;

unsigned int xor_fun(unsigned int c, unsigned int p){
	return c ^ p;
}

unsigned int nextR(unsigned int r){
	unsigned int p = pow(2, 31);
	unsigned int ret =  ((unsigned long)(A * r) + C) % p;
	return ret;
}

unsigned int swapEndianness(unsigned int num){
	unsigned int swapped = ((num>>24)&0xff) | // move byte 3 to byte 0
                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
                    ((num>>8)&0xff00) | // move byte 2 to byte 1
                    ((num<<24)&0xff000000); // byte 0 to byte 3

    return swapped;
}

int main(){
	FILE* fd = fopen("ciphertext.txt", "r");
	char tmp[9];
	fread(tmp, sizeof(char), 8, fd);
	tmp[8] = '\0';
	unsigned int cold = stoul(tmp, NULL, 16);
	unsigned int pold = 0x44613a20;		//"Da: "
	cout << "Da: ";
	unsigned int cnew, pnew;

	unsigned int rold = cold ^ pold;
	unsigned int rnew = nextR(__builtin_bswap32(rold));

	for(int i=0; i<83; i++){

		int r = fread(tmp, sizeof(char), 8, fd);
		if(r <= 0)
			break;
		tmp[8] = '\0';
		cnew = stoul(tmp, NULL, 16);
		cnew = __builtin_bswap32(cnew);
		pnew = rnew ^ cnew;
//---------------
		//PRINT

		char ch[4];
		memcpy(ch, (char*)&pnew, sizeof(unsigned int)); 
		for(int i=0; i<4; i++)
			cout << ch[i];

//---------------		
		rold = rnew;
		rnew = nextR(rold);
	}


}

