#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>


/*p and q are two large prime numbers, and n is computed by p*q. e is the public exponent, d is the multiplicative inverse of e mod (p-1)(q-1).
The private key is the pair (p, q). The public key is the pair (n, e)*/
/*struct rsa_st {

    int pad;
    int32_t version;
    const RSA_METHOD *meth;

    ENGINE *engine;
    BIGNUM *n;
    BIGNUM *e;
    BIGNUM *d;
    BIGNUM *p;
    BIGNUM *q;
    BIGNUM *dmp1;
    BIGNUM *dmq1;
    BIGNUM *iqmp;

    STACK_OF(RSA_PRIME_INFO) *prime_infos;

    RSA_PSS_PARAMS *pss;

    CRYPTO_EX_DATA ex_data;
    CRYPTO_REF_COUNT references;
    int flags;

    BN_MONT_CTX *_method_mod_n;
    BN_MONT_CTX *_method_mod_p;
    BN_MONT_CTX *_method_mod_q;

    char *bignum_data;
    BN_BLINDING *blinding;
    BN_BLINDING *mt_blinding;
    CRYPTO_RWLOCK *lock;
};
*/

typedef struct	s_rsa_entry
{
	RSA 		*keypair;
    int			encryption_len;
    char		*raw;
}	      		t_rsa_entry;

void print_keys(t_rsa_entry *rsa);

void sha256(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

void load_keys(t_rsa_entry *rsa)
{
	char public_key[] = "-----BEGIN RSA PUBLIC KEY-----\n" \
"MIICCAKCAgEAt+qskUX3WVUDvrjEKf1bNtoK9uYVWR496hu/UVAPn/epGQy8P9Zc\n" \
"Q6M06NS3yyFRHo0SvPI2FElx8PZKEtV11XP1pHfeUw8z95LNuk/fmO24xBHRsqfs\n" \
"T7IiAAo3W1DYcLl7QtwM8YhRczK1dfGxjbBlrA3MRv7ZNppPpFrkla4KISZQTabA\n" \
"eQgEA1HOXxdqjWSO7FabJEwSxOP2Yx+fnn8XRuzvLwZVgfJ8hl+G7rvCjb3qTBUW\n" \
"54H9V/lFSEaiQsQYH7hluaJkbbPn7xYCS5EiMOrmEZgZwwA5DtG7Ym+v3R7v5mBa\n" \
"1wBurDab+f1wtsa+vEsdCMstZE2Pebvp4QVi/nWiTkgzYq8JxWhFYp57+VHdvZro\n" \
"X/H8l0OyQ2umzTbtDVCv9zXQWJenxTDMZY4Ixs1AJnfD3ZZp++P+DnzCsWCG/tyE\n" \
"xsOG2s746jq7nu6e3OxdvcwzqxpIGGDXuq9HYjE3xqHdpVRNR8DLyPjF7UOTGdiE\n" \
"3BcdK5FDytKorMTWH5Duiy8ASmqwN6HxqaB6kzUW3rhX56FDo+kq2/ljxeYe2N8w\n" \
"wecGs1/HcPv4pvpzeQsPVvIVQ4w7VexV+bT3VLGPIPqXBxdSOLyqdg161REk6dtS\n" \
"csyBy2cHQsaua7CMlZV8jfOlI2ErQRigGhVuGQjc7R/L0Iqj/G7uLA8CAQM=:\n"	   \
"-----END RSA PUBLIC KEY-----\n";

	char private_key[] = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIIJKAIBAAKCAgEAt+qskUX3WVUDvrjEKf1bNtoK9uYVWR496hu/UVAPn/epGQy8\n" \
"P9ZcQ6M06NS3yyFRHo0SvPI2FElx8PZKEtV11XP1pHfeUw8z95LNuk/fmO24xBHR\n" \
"sqfsT7IiAAo3W1DYcLl7QtwM8YhRczK1dfGxjbBlrA3MRv7ZNppPpFrkla4KISZQ\n" \
"TabAeQgEA1HOXxdqjWSO7FabJEwSxOP2Yx+fnn8XRuzvLwZVgfJ8hl+G7rvCjb3q\n" \
"TBUW54H9V/lFSEaiQsQYH7hluaJkbbPn7xYCS5EiMOrmEZgZwwA5DtG7Ym+v3R7v\n" \
"5mBa1wBurDab+f1wtsa+vEsdCMstZE2Pebvp4QVi/nWiTkgzYq8JxWhFYp57+VHd\n" \
"vZroX/H8l0OyQ2umzTbtDVCv9zXQWJenxTDMZY4Ixs1AJnfD3ZZp++P+DnzCsWCG\n" \
"/tyExsOG2s746jq7nu6e3OxdvcwzqxpIGGDXuq9HYjE3xqHdpVRNR8DLyPjF7UOT\n" \
"GdiE3BcdK5FDytKorMTWH5Duiy8ASmqwN6HxqaB6kzUW3rhX56FDo+kq2/ljxeYe\n" \
"2N8wwecGs1/HcPv4pvpzeQsPVvIVQ4w7VexV+bT3VLGPIPqXBxdSOLyqdg161REk\n" \
"6dtScsyBy2cHQsaua7CMlZV8jfOlI2ErQRigGhVuGQjc7R/L0Iqj/G7uLA8CAQMC\n" \
"ggIAepxzC4P6O44CfyXYG/48zzwHT0QOO2l+nBJ/i4q1FU/GELMoKo7oLRd4mzh6\n" \
"h2uLabNh00wkDYZL9fmGtzj5OPf5GE/pjLTNT7cz0YqVEJ57LWE2dxqdinbBVVwk\n" \
"54s69dD81z1d9lrg93cjo/Z2XnWZHV6IL1SQzxGKbZHtuR6xa27gM8SAULACrOE0\n" \
"P2Txs5hfSDm8wt1h2Jf5l2pqaaoPhJ30ygQ5AUxTBD+vSdKBs9PxiA4PRQFTj/uD\n" \
"hYRsLIK6v9BD0Rbtnnfv9LlW3QtsIJyZYRARLKrQtIvSQZ/Kk2n1REA8j1WfHXm9\n" \
"UVOgednUfYdosIdzmDO0+9Kb6jd9+WvsaaFl+//9zwn7/b+WxS6zE8uoEJvSiRc2\n" \
"SNKoLlu4PUawFCzqHkKo2Y86BH/rWgpqor4ATPMJR1gAmgQY+n99yX3cb4mn25xm\n" \
"2456m7wCNVBneK1WCukXS62yfxRIGym/34c5yk4HrNSJZ/rUsycCY4boEdQZvJEB\n" \
"HJlSqS2C84kmIpA+7grWF6X8URG69MyfIhu5ADBK3eQLvLcEzo4PjOVKcBPMNj7d\n" \
"tBedUBqsLOB7Ph+lC56XUu/p5gAedw+ZEF+dhVQNty2ygDu3hnb7knxhz5CkzuAZ\n" \
"fucZCiAIo4TSFERrHTEGflUBZ+lEgjpLawJnco+zzoXHjEsCggEBAN9I5YSIGjpp\n" \
"PBpP0woZUhmf/EuqSafCdtEan8mB7fI1Qn065luhLuKkLhiWy/j3URnEg+7ByK8Y\n" \
"BXIHNgQZWrv0b0C4JAFnAx6ne6OAnYwHUzDpTMi998HIC6fvN8gXMXhOrUWK45hi\n" \
"Q0b9CoPXSlXehHQV4gFR+reK0evEHykANTcaMaaFXOd7RGJcR4ta5wC3IAHXdQND\n" \
"EWToGAneZiCfO512gDcViuqkoF3Lqjaq17f0TyF42DmeNYT9aDhmvZWZtP0KP5e0\n" \
"B0Od4bGRGiXtAKqiT3ccDAs5MsFn/VI9a7d9z/xhqRVXFneDH1/YV+HA/0TvsZwQ\n" \
"AE0umNeTZXcCggEBANLdIs83lZuxLJS9O88yE+V51UAm1kGp0DcmKdde6D11RTAd\n" \
"ywsGqg/M/RsTsuD+DbRjO87eaaurZLfU2tvjzLqoymCSLJ5THFZjlcDeA1j8YiOy\n" \
"QCsEkQZqjxS17xQ0ypiMjC0NE76ksphEuf4mYqqoXBT5oozWxqFrvsv+AMOsecl3\n" \
"gJyv+m8moPgSzJ2cSQUrBABQtot/VfPrPwk62sY9VHORBVArjt6vwaOvOKHh1xp8\n" \
"5pjdoYj/fJla8UwQYx8v7R0fSNN2t9WfxEaNm436irQJPqmjr2hRYlV5BxnLcMkB\n" \
"0FHfxnetYSAeZDlFplXGVmx8GwAgpGXnrc6vdCkCggEBAJTbQ62wEXxGKBGKjLFm\n" \
"Nru//YfG28UsTzYRv9ur8/bOLFN8mZJrdJcYHrsPMqX6NhEtrUnWhcoQA6FaJAK7\n" \
"kdKi9NXQGADvV2nE/RerE7KvjMtGMzB+pSvasm/0z9q6IPrfHi5cl7rsLNn+BwKP\n" \
"huPpraK5QVY2pyUHNp0tahtVeM9my8RY6JpSLZboL7I8mgB6FVaPo1eCC5iaurE+\n" \
"7sBqJ75PACS5B0cYaukycXnHOnqi32ul5Xu+zlio8CWZ07kRI1NcKmUir4JpQSEL\n" \
"ZsPzVccW36S9XVzQzIDv/jbTnST+iqhBG2OPZE+sv5U65UErVNify71gADN0ZeUM\n" \
"7k8CggEBAIyTbIolDmfLcw3TfTTMDUOmjirEjtZxNXoZcTo/RX5OLiAT3LIEcV/d\n" \
"/hINIetUCSLs0onpm8fHmHqN5z1CiHxwhusMHb7iEuRCY9XpV5CoQW0hgByttgRH\n" \
"Cg3On2LN3GWzCB4It9RtzGWDJqlu7HHFkrimbF3khGudKd1UAIJy+9ulAGh1UZ9v\n" \
"Ffq3MxO9hgNyAqrgebJU4/fyKgYnPIQo4ve2A4rHtJR1K8J00Gvr5LxTRGXpFltU\n" \
"/buR9jK1l2of82i/heJPJTkVLYReZ7P8XHgGKcZtH5rg7Dj7WhEySzCr4DaVLvpz\n" \
"lhVpmCYubuPZjvL9Z1VrGEPvyTR0+BsCggEBANnhDi9tLiUwQ55O7bV19a9JFR5W\n" \
"2KoVIENSPgPwMoFAprQLwqN1a6L1toN2/csxLBbbM2jvRT6mnbw6se67WwW+OPTX\n" \
"A7avYe76yv8jWHJqnb0QN5rHejDIF4A1wbN2IcWg2ZGWx5LJ+ViiPG7XwPyrUUsf\n" \
"mo+Z4Z6g2ygy7cdGvbj13n8WovgaN5J0jukQDNjM2sX2uTN2ya01Q84bClN50tDp\n" \
"RaO29CUwHKGcgWGdDoy3f52Q7KqXHlhP8qxc/wSQ6Ro3SWmLU513xeAcQvuzLrZ7\n" \
"oElS/yqgHIYFEWfyoaHYkP/kgY7xH57U1JFmNKDhyqiU7kU6CImSeokFhwQ=\n" \
"-----END RSA PRIVATE KEY-----\n";


	 BIO *bufio_public = BIO_new_mem_buf((void*)&public_key, sizeof(public_key));
	 BIO *bufio_private = BIO_new_mem_buf((void*)&private_key, sizeof(private_key));

	PEM_read_bio_RSA_PUBKEY(bufio_public, &rsa->keypair, 0, 0);
	PEM_read_bio_RSAPrivateKey(bufio_private, &rsa->keypair, 0, 0);

	// BIO* bio = BIO_new(BIO_s_mem());
	// int len = BIO_write(bio, (void*)&public_key, sizeof(public_key));
	// EVP_PKEY* evp_key = malloc(sizeof(EVP_PKEY*));
	// PEM_read_bio_PUBKEY(bio, evp_key, NULL, NULL);
	// EVP_PKEY_get1_RSA(evp_key, rsa->keypair);


	// size_t pri_len = BIO_pending(bufio_private);
    //
	// char *pri_key = malloc(pri_len + 1);
    //
	// BIO_read(bufio_private, pri_key, pri_len);
    //
	// pri_key[pri_len] = '\0';
}

t_rsa_entry *hash_rsa(char *buffer, int len)
{
	t_rsa_entry		*rsa_entry = NULL;
	char			error[256];

	memset((char*)&error, 0, 255);
	if (!(rsa_entry = (t_rsa_entry*)malloc(sizeof(struct s_rsa_entry))))
		return (NULL);
	rsa_entry->encryption_len = 0;
	  if (!(rsa_entry->keypair = RSA_generate_key(4096, 3, NULL, NULL)))
	  	return (NULL);

	load_keys(rsa_entry);
	 if (!(rsa_entry->raw = malloc(RSA_size(rsa_entry->keypair))))
	 	return (NULL);
	if ((rsa_entry->encryption_len = RSA_public_encrypt(len + 1, (unsigned char*)buffer,
   (unsigned char*)rsa_entry->raw, rsa_entry->keypair, RSA_PKCS1_OAEP_PADDING)) == -1) {
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), (char*)&error);
    fprintf(stderr, "Error encrypting message: %s\n", (char*)&error);
	return (NULL);
	}
	return rsa_entry;
}

char *unhash_rsa(t_rsa_entry *rsa_entry)
{
	char	*decrypted = NULL;
	char	err[256];

	memset((char*)&err, 0, 255);
	if (!(decrypted = (char*)malloc(RSA_size(rsa_entry->keypair))))
		return (NULL);
	if (RSA_private_decrypt(rsa_entry->encryption_len, (unsigned char*)rsa_entry->raw, (unsigned char*)decrypted,
	                       rsa_entry->keypair, RSA_PKCS1_OAEP_PADDING) == -1) {
	   ERR_load_crypto_strings();
	   ERR_error_string(ERR_get_error(), (char*)&err);
	   fprintf(stderr, "Error decrypting message: %s\n", (char*)&err);
	   return (NULL);
	}
	return (decrypted);
}

void print_encrypted_bytecode(char *raw, int len)
{
	int n=len, i =0;
	unsigned char* byte_array = raw;

	while (i < n)
	{
		printf("%02X\n",(unsigned)byte_array[i]);
		i++;
	}
}

void print_keys(t_rsa_entry *rsa)
{
	BIO *pri = BIO_new(BIO_s_mem());
	BIO *pub = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(pri, rsa->keypair, NULL, NULL, 0, NULL, NULL);
	PEM_write_bio_RSAPublicKey(pub, rsa->keypair);

	size_t pri_len = BIO_pending(pri);
	size_t pub_len = BIO_pending(pub);

	char *pri_key = malloc(pri_len + 1);
	char *pub_key = malloc(pub_len + 1);

	BIO_read(pri, pri_key, pri_len);
	BIO_read(pub, pub_key, pub_len);

	pri_key[pri_len] = '\0';
	pub_key[pub_len] = '\0';

	printf("\n%s\n%s\n", pri_key, pub_key);
}

int main ()
{
  /*
        mov rax, 60          ; sys_exit
        mov rdi, 2
        syscall
  */
 /*char code[] = {
      0x48, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x48, 0xbf, 0x02, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x0f, 0x05
  };*/

 //char code[] = { 0x48, 0xC7, 0xC0, 0x3C, 0x00, 0x00, 0x00, 0x48, 0xC7, 0xC7, 0x02, 0x00, 0x00, 0x00, 0x0F, 0x05 };

// char code[] = { 0x55, 0x48, 0x83, 0xFF, 0x20, 0x7C, 0x0F, 0x48, 0x83, 0xFF, 0x7E, 0x7F, 0x09, 0x48, 0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x5D, 0xC3, 0x48, 0xC7, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x5D, 0xC3 };

char BYTE[] = "";

char code[] = "\x48\xc7\xc0\x01\x00\x00\x00\x48\x89\xfe\x48\xc7\xc2\x17\x00\x00\x00\x48\xc7\xc7\x01\x00\x00\x00\x0f\x05\xc3";
int len = sizeof(code);
  void *buf = NULL;
  if ((buf = mmap (0,sizeof(code),PROT_READ|PROT_WRITE|PROT_EXEC,
              MAP_PRIVATE|MAP_ANON,-1,0)) == NULL)
	return -1;
  memcpy (buf, code, sizeof(code));

	t_rsa_entry *rsa = NULL;
	if ((rsa = hash_rsa((char*)&code, len)) != NULL) {
		//print_keys(rsa);
		print_encrypted_bytecode(rsa->raw, rsa->encryption_len);

		printf("\n ___________ \n");
		char *result = unhash_rsa(rsa);
		if (result) {
			buf = mmap (0,len,PROT_READ|PROT_WRITE|PROT_EXEC,
		                MAP_PRIVATE|MAP_ANON,-1,0);
		    memcpy (buf, result, len);
			((void (*) (char*))buf)("hack3d by darkanonymous");
		}
	}

  return 0;
}
