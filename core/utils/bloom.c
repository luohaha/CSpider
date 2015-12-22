#include "utils.h"
/**
   create_bloom : create Bloom_t struct
   @size : size of bit array
   @nfuncs : size of hash function array

   return new Bloom_t
**/
Bloom_t *create_bloom(size_t size, size_t nfuncs, ...) {
  Bloom_t *bloom;
  va_list l;
  int n;

  if (!(bloom = (Bloom_t *)malloc(sizeof(Bloom_t)))) {
    return NULL;
  }
  if (!(bloom->a = calloc((size+CHAR_BIT-1)/CHAR_BIT, sizeof(char)))) {
    free(bloom);
    return NULL;
  }
  
  if (!(bloom->funcs=(hashfunc_t*)malloc(nfuncs*sizeof(hashfunc_t)))) {
    free(bloom->a);
    free(bloom);
    return NULL;
  }

  va_start(l, nfuncs);
  for(n=0; n<nfuncs; ++n) {
    bloom->funcs[n]=va_arg(l, hashfunc_t);
  }
  va_end(l);

  bloom->nfuncs=nfuncs;
  bloom->asize=size;

  return bloom;
}

/**
   bloom_destroy : destroy the bloom
   @bloom : ready to be destroyed
**/
int bloom_destroy(Bloom_t *bloom)
{
    free(bloom->a);
    free(bloom->funcs);
    free(bloom);

    return 0;
}

/**
   bloom_add : add string to filter
   @bloom : the bloom
   @s : ready to add
**/
int bloom_add(Bloom_t *bloom, char *s)
{
    size_t n;

    for(n=0; n<bloom->nfuncs; ++n) {
        SETBIT(bloom->a, bloom->funcs[n](s)%bloom->asize);
    }

    return 0;
}

/**
   bloom_check : check if string exits
   @bloom : the bloom
   @s : ready
**/
int bloom_check(Bloom_t *bloom, char *s)
{
    size_t n;

    for(n=0; n<bloom->nfuncs; ++n) {
        if(!(GETBIT(bloom->a, bloom->funcs[n](s)%bloom->asize))) return 0;
    }

    return 1;
}

/**
  init Bloom_t 
**/
Bloom_t *init_Bloom() {
  return create_bloom(2500000, 2, sax_hash, sdbm_hash);
}

/**
hash functions
**/

unsigned int sax_hash(char *key)
{
    unsigned int h=0;

    while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;

    return h;
}

unsigned int sdbm_hash(char *key)
{
    unsigned int h=0;
    while(*key) h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
    return h;
}
