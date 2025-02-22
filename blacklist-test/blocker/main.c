#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define _NR_blacklist 666

static const int hash_len = 32;

unsigned char conv(char c)
{
	if(c >= '0' && c <= '9')
	{
		return (unsigned char)c - (unsigned char)'0';
	}

	if(c >= 'a' && c <= 'f')
	{
		return ((unsigned char)c - (unsigned char)'a') + (unsigned char)10;
	}

	return 0;
}

static void parse_hash(char *hash_str, unsigned char *hash)
{
	for(int i = 0; i < 64; i += 2)
	{
		char c = conv(hash_str[i]) << 4 | conv(hash_str[i+1]);
		hash[i >> 1] = c;
	}
}

int main(int argc, char** argv)
{
	char hash_str[(hash_len << 1) + 1];
	unsigned char hash[hash_len];

	// Read hash from user
	printf("Please type in hash: \n");
	scanf("%64s", hash_str);

	parse_hash(hash_str, hash);

	int ret = syscall(_NR_blacklist, hash);

	if(ret < 0)
	{
		printf("Error: %d\n", ret);
		return 1;
	}

	return 0;
}
