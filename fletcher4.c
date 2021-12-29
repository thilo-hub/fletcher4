#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
typedef unsigned int uint32_t;

struct scalar_t {
	uint64_t  zc_word[4];
};
struct fl_ctx {
	struct scalar_t scalar;
};

#define ZIO_SET_CHECKSUM(ptr, a, b, c, d) do { (ptr)->zc_word[0]=a;(ptr)->zc_word[1]=b;(ptr)->zc_word[2]=c;(ptr)->zc_word[3]=d;} while(0)
  
typedef struct fl_ctx fletcher_4_ctx_t;
static void
fletcher_4_scalar_native(fletcher_4_ctx_t *ctx, const void *buf,
    uint64_t size)
{
	const uint32_t *ip = buf;
	const uint32_t *ipend = ip + (size / sizeof (uint32_t));
	uint64_t a, b, c, d;

	a = ctx->scalar.zc_word[0];
	b = ctx->scalar.zc_word[1];
	c = ctx->scalar.zc_word[2];
	d = ctx->scalar.zc_word[3];

	for (; ip < ipend; ip++) {
		a += ip[0];
		b += a;
		c += b;
		d += c;
	}

	ZIO_SET_CHECKSUM(&ctx->scalar, a, b, c, d);
}

#define BLKSIZE 4096
int main(int argc, char *argv[])
{
  fletcher_4_ctx_t ctx;
  uint64_t offset=0;

  size_t blksize;
  unsigned char *buff;

  // ARG1 block-size
  if ( argc > 1 && (blksize=strtoul(argv[1],0,0)) > 0) {
	fprintf(stderr,"BLKSIZE: 0x%lx\n",blksize);
  } else {
	blksize = 4096;
  }
  buff = malloc(blksize);
  // ARG2 seek to byte
  if ( argc > 2 && (offset=strtoul(argv[2],0,0)) > 0) {
	lseek(0,offset,SEEK_SET);
  }

  while( read(0,buff,blksize) == blksize) {
	  ZIO_SET_CHECKSUM(&ctx.scalar, 0, 0, 0, 0);
	  fletcher_4_scalar_native(&ctx,buff,blksize);
	  // fprintf(stderr,"cksum_%6ld = 0x%lx 0x%lx 0x%lx 0x%lx\n",offset,ctx.scalar.zc_word[0],ctx.scalar.zc_word[1],ctx.scalar.zc_word[2],ctx.scalar.zc_word[3]);
	  fprintf(stderr,"cksum_%6lx = %lx:%lx:%lx:%lx\n",offset,ctx.scalar.zc_word[0],ctx.scalar.zc_word[1],ctx.scalar.zc_word[2],ctx.scalar.zc_word[3]);
	  offset += blksize;
	  write(1,&ctx.scalar,sizeof(ctx.scalar));
	}
 }
	 

