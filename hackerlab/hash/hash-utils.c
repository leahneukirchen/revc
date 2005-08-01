/* hash-utils.c - computing hash values
 *
 ****************************************************************
 * Copyright (C) 2000 Thomas Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */



#include "hackerlab/machine/alignment.h"
#include "hackerlab/hash/hash-utils.h"


/************************************************************************
 *(h1 "Hash Utilities"
 *    :includes ("hackerlab/hash/hash-utils.h"))
 * 
 * The functions in this section provide tools useful for computing 
 * hash values.
 */




static unsigned long shuffled_bytes[] =
{
  245, 184, 171, 36, 93, 194, 192, 143, 207, 89, 63, 175, 203, 231, 47, 238,
  103, 67, 176, 102, 80, 133, 24, 155, 91, 141, 234, 58, 44, 191, 218, 157,
  13, 168, 160, 113, 211, 213, 252, 236, 2, 19, 21, 148, 111, 251, 165, 74,
  124, 25, 181, 210, 250, 195, 235, 97, 185, 1, 179, 198, 105, 101, 5, 220,
  35, 162, 142, 41, 200, 209, 224, 71, 201, 134, 69, 48, 65, 170, 72, 167,
  145, 205, 28, 88, 215, 81, 214, 78, 118, 26, 123, 84, 140, 49, 45, 8,
  7, 107, 227, 60, 59, 32, 30, 82, 31, 189, 131, 17, 66, 239, 64, 10,
  149, 40, 130, 146, 54, 147, 9, 114, 4, 254, 241, 116, 110, 249, 57, 233,
  37, 55, 206, 100, 177, 119, 139, 158, 108, 75, 94, 23, 186, 152, 244, 27,
  38, 33, 188, 87, 76, 166, 228, 52, 120, 99, 247, 174, 51, 183, 3, 161,
  246, 135, 14, 178, 11, 216, 77, 172, 122, 154, 39, 253, 104, 34, 164, 230,
  219, 242, 68, 151, 180, 115, 173, 73, 212, 90, 125, 29, 22, 221, 56, 121,
  255, 204, 83, 169, 182, 112, 96, 187, 20, 106, 79, 15, 61, 223, 70, 85,
  53, 197, 217, 232, 196, 95, 136, 150, 243, 109, 129, 202, 208, 237, 144, 156,
  86, 127, 62, 248, 138, 229, 153, 226, 240, 199, 50, 12, 193, 98, 137, 126,
  0, 159, 222, 18, 163, 117, 190, 46, 225, 132, 16, 43, 128, 42, 92, 6
};



/*(c hash_ul)
 * unsigned long hash_ul (unsigned long n);
 * 
 * Generate a hash value from an integer.
 * 
 * This function is slow, but attempts to give a good distribution of
 * hash values even for a series of `n' which are not particularly
 * random.
 */
unsigned long
hash_ul (unsigned long n)
{
  int rounds;
  t_ulong answer = 0;

  for (rounds = 0; rounds < 5; ++rounds)
    {
      int byte;

      for (byte = 0;  byte < sizeof (t_ulong); ++byte)
        {
          t_ulong new_byte;

          new_byte = shuffled_bytes[0xff & (n >> byte)];
          answer ^= (new_byte << byte);
        }

      n = ((n >> 3) ^ (n << 7));
    }

  return answer;
}


/*(c hash_double)
 * unsigned long hash_double (double n);
 * 
 * Generate a hash value from a double precision float.
 * 
 * This function is slow, but attempts to give a good distribution of
 * hash values even for a series of `n' which are not particularly
 * random.
 */
unsigned long
hash_double (double d)
{
  union
  {
    t_uchar raw[sizeof (double)];
    double d;
  } data;

  data.d = d;
  return hash_mem (data.raw, sizeof (double));
}






/*(c hash_pointers)
 * unsigned long hash_pointers (void * elts, size_t n_elts);
 * 
 * Compute a hash value from an array of pointers.
 * 
 * This function is slow, but attempts to give a good distribution of
 * hash values even for a series of pointers which are not
 * particularly random.  Usually, pointers are not particularly
 * random.
 * 
 * `slow' means that the function does roughly `3 * sizeof (n)' array
 * look-ups and lots of bit twiddling, per pointer.
 * 
 */
unsigned long
hash_pointers (void * elts, size_t n_elts)
{
  size_t x;
  unsigned long hash;

  hash = 0xdec0ded;
  for (x = 0; x < n_elts; ++x)
    {
      hash ^= hash_ul (((unsigned long *)elts)[x]);
    }
  return hash;
}




/*(c hash_mem)
 * unsigned long hash_mem (const t_uchar * elts, size_t n_bytes);
 * 
 * Compute a hash value from an array of bytes.
 * 
 * This function is slow, but attempts to give a good distribution of
 * hash values even for a series of bytes which are not particularly
 * random.
 * 
 * `slow' means that the function does roughly `3 * sizeof (n)' array
 * look-ups and lots of bit twiddling, per `sizeof (unsigned long)'
 * bytes.
 * 
 */
unsigned long
hash_mem (const t_uchar * elts, size_t n_elts)
{
  size_t x;
  unsigned long hash;

  hash = 0xde7a115;

  for (x = 0; (x < n_elts) && (MACHINE_ALIGNMENT - 1); ++x)
    hash ^= hash_ul ((unsigned long)elts[x]);

  while ((n_elts - x) >= sizeof (unsigned long))
    {
      hash ^= hash_ul (*(unsigned long *)(elts + x));
      x += sizeof (unsigned long);
    }
  
  while (x < n_elts)
    {
      hash ^= hash_ul ((unsigned long)elts[x]);
      ++x;
    }
  return hash;
}


