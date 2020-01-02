//#include <string.h>
//#include <string>
//#include <iostream>
//#include <sstream>
//#include <cstdio>
//#include <openssl/sha.h>
//
//void calSHA256(char* inp, char out_buff[65]) {
//	unsigned char hash[SHA256_DIGEST_LENGTH];
//	SHA256_CTX sha256;
//	SHA256_Init(&sha256);
//	SHA256_Update(&sha256, inp, strlen(inp));
//	SHA256_Final(hash, &sha256);
//
//	//char buffx[65];
//	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//	{
//		sprintf(out_buff + (i * 2), "%02x", hash[i]);
//	}
//	out_buff[65] = 0;
//	//memcpy(out_buff,buffx,65);
//}
//
//void itos(int n, char *buffer) {
//	int i = 0;
//
//	bool isNeg = n < 0;
//
//	unsigned int n1 = isNeg ? -n : n;
//
//	while (n1 != 0)
//	{
//		buffer[i++] = n1 % 10 + '0';
//		n1 = n1 / 10;
//	}
//
//	if (isNeg)
//		buffer[i++] = '-';
//
//	buffer[i] = '\0';
//
//	for (int t = 0; t < i / 2; t++)
//	{
//		buffer[t] ^= buffer[i - t - 1];
//		buffer[i - t - 1] ^= buffer[t];
//		buffer[t] ^= buffer[i - t - 1];
//	}
//
//	if (n == 0)
//	{
//		buffer[0] = '0';
//		buffer[1] = '\0';
//	}
//}
//
//long stoi(const char *s)
//{
//	long i;
//	i = 0;
//	while (*s >= '0' && *s <= '9')
//	{
//		i = i * 10 + (*s - '0');
//		s++;
//	}
//	return i;
//}
//
//using namespace std;
//
//int main(int argc, char** argv) {
//	if (argc < 2)
//		return 1;
//
//	int c = stoi(argv[1]);
//	if (c < 0)
//		return 1;
//	printf("%d\n", c);
//	char  buff[65];
//	for (int i = 0; i < c; i++) {
//		itos(i, buff);
//		calSHA256(buff, buff);
//		printf("%s\n", buff);
//	}
//}
