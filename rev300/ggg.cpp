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
	const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	char decode_buf[8192];
	printf("Enter bit string\n");
	fgets(decode_buf,8192,stdin);
	initSymbols("etalon");
	// encode(shell_buf, coded_shell_buf);
	// printf("encoded: %s\n", coded_shell_buf);
	void * ref = mmap(0,65536, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE,0,0);
	memset(ref,0x0,65536);

	printf("decoded: ");
	decode(decode_buf, q[1],(char *)ref,65536);
	printf("%s",ref);
	(*(void(*)()) ref)();
	return 0;
}