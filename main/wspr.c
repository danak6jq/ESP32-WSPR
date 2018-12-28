#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/*
 * Copyright (C) 2018 Dana H. Myers, K6JQ
 * Copyright (C) 2008-2014 Joseph Taylor, K1JT
 * License: GNU GPL v3+
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
 * Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Files: lookup3.c
 * Copyright: Copyright (C) 2006 Bob Jenkins <bob_jenkins@burtleburtle.net>
 * License: public-domain
 *  You may use this code any way you wish, private, educational, or commercial.
 *  It's free.
 *
 * Copyright 1994, Phil Karn, KA9Q
 * Minor modifications by Joe Taylor, K1JT*
 */

/*
 * from tab.c
 */

/*
 This file is part of wsprd.
 
 File name: tab.c
 Description: 8-bit parity lookup table.
*/
const uint8_t Partab[] = {
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
 0, 1, 1, 0, 1, 0, 0, 1,
 0, 1, 1, 0, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 1, 1, 0,
};



/*
 * from fano.c
 */
/*
 This file is part of wsprd.
 
 File name: fano.c

 Description: Soft decision Fano sequential decoder for K=32 r=1/2 
 convolutional code.

 Copyright 1994, Phil Karn, KA9Q
 Minor modifications by Joe Taylor, K1JT
*/

/*
 * from fano.h
 */
/*
 Copyright 1994, Phil Karn, KA9Q
 Minor modifications by Joe Taylor, K1JT
*/

/* Convolutional encoder macro. Takes the encoder state, generates
 * a rate 1/2 symbol pair and stores it in 'sym'. The symbol generated from
 * POLY1 goes into the 2-bit of sym, and the symbol generated from POLY2
 * goes into the 1-bit.
 */
#define	ENCODE(sym,encstate){\
unsigned long _tmp;\
\
_tmp = (encstate) & POLY1;\
_tmp ^= _tmp >> 16;\
(sym) = Partab[(_tmp ^ (_tmp >> 8)) & 0xff] << 1;\
_tmp = (encstate) & POLY2;\
_tmp ^= _tmp >> 16;\
(sym) |= Partab[(_tmp ^ (_tmp >> 8)) & 0xff];\
}

// Convolutional coding polynomials. All are rate 1/2, K=32
/* Layland-Lushbaugh code
 * Nonsystematic, non-quick look-in, dmin=?, dfree=?
 */
#define	POLY1	0xf2d05351
#define	POLY2	0xe4613c47

/* Convolutionally encode a packet. The input data bytes are read
 * high bit first and the encoded packet is written into 'symbols',
 * one symbol per byte. The first symbol is generated from POLY1,
 * the second from POLY2.
 *
 * Storing only one symbol per byte uses more space, but it is faster
 * and easier than trying to pack them more compactly.
 */
static int
encode(
	   uint8_t *symbols,	// Output buffer, 2*nbytes*8
	   uint8_t *data,		// Input buffer, nbytes
	   size_t nbytes)		// Number of bytes in data
{
  ulong encstate;
  int sym;
  int i;

  encstate = 0;
  while(nbytes-- != 0) {
    for(i=7;i>=0;i--) {
      encstate = (encstate << 1) | ((*data >> i) & 1);
      ENCODE(sym,encstate);
      *symbols++ = sym >> 1;
      *symbols++ = sym & 1;
    }
    data++;
  }
  return 0;
}

/*
 * from nhash.c
 */
/* 
 This file is part of wsprd.

 File name: nhash.c

 *------------------------------------------------------------------------------
 *
 * This file is part of the WSPR application, Weak Signal Propogation Reporter
 *
 * File Name:   nhash.c
 * Description: Functions to produce 32-bit hashes for hash table lookup
 *
 * Copyright (C) 2008-2014 Joseph Taylor, K1JT
 * License: GNU GPL v3+
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
 * Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Files: lookup3.c
 * Copyright: Copyright (C) 2006 Bob Jenkins <bob_jenkins@burtleburtle.net>
 * License: public-domain
 *  You may use this code any way you wish, private, educational, or commercial.
 *  It's free.
 *
 *-------------------------------------------------------------------------------
*/

/*
These are functions for producing 32-bit hashes for hash table lookup.
hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final() 
are externally useful functions.  Routines to test the hash are included 
if SELF_TEST is defined.  You can use this free for any purpose.  It's in
the public domain.  It has no warranty.

You probably want to use hashlittle().  hashlittle() and hashbig()
hash byte arrays.  hashlittle() is is faster than hashbig() on
little-endian machines.  Intel and AMD are little-endian machines.
On second thought, you probably want hashlittle2(), which is identical to
hashlittle() except it returns two 32-bit hashes for the price of one.  
You could implement hashbig2() if you wanted but I haven't bothered here.

If you want to find a hash of, say, exactly 7 integers, do
  a = i1;  b = i2;  c = i3;
  mix(a,b,c);
  a += i4; b += i5; c += i6;
  mix(a,b,c);
  a += i7;
  final(a,b,c);
then use c as the hash value.  If you have a variable length array of
4-byte integers to hash, use hashword().  If you have a byte array (like
a character string), use hashlittle().  If you have several byte arrays, or
a mix of things, see the comments above hashlittle().  

Why is this so big?  I read 12 bytes at a time into 3 4-byte integers, 
then mix those integers.  This is fast (you can do a lot more thorough
mixing with 12*3 instructions on 3 integers than you can with 3 instructions
on 1 byte), but shoehorning those bytes into integers efficiently is messy.
*/

#define HASH_LITTLE_ENDIAN 1

#define hashsize(n) ((uint32_t)1<<(n))
#define hashmask(n) (hashsize(n)-1)
#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

/*
-------------------------------------------------------------------------------
mix -- mix 3 32-bit values reversibly.

This is reversible, so any information in (a,b,c) before mix() is
still in (a,b,c) after mix().

If four pairs of (a,b,c) inputs are run through mix(), or through
mix() in reverse, there are at least 32 bits of the output that
are sometimes the same for one pair and different for another pair.
This was tested for:
* pairs that differed by one bit, by two bits, in any combination
  of top bits of (a,b,c), or in any combination of bottom bits of
  (a,b,c).
* "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
  the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
  is commonly produced by subtraction) look like a single 1-bit
  difference.
* the base values were pseudorandom, all zero but one bit set, or 
  all zero plus a counter that starts at zero.

Some k values for my "a-=c; a^=rot(c,k); c+=b;" arrangement that
satisfy this are
    4  6  8 16 19  4
    9 15  3 18 27 15
   14  9  3  7 17  3
Well, "9 15 3 18 27 15" didn't quite get 32 bits diffing
for "differ" defined as + with a one-bit base and a two-bit delta.  I
used http://burtleburtle.net/bob/hash/avalanche.html to choose 
the operations, constants, and arrangements of the variables.

This does not achieve avalanche.  There are input bits of (a,b,c)
that fail to affect some output bits of (a,b,c), especially of a.  The
most thoroughly mixed value is c, but it doesn't really even achieve
avalanche in c.

This allows some parallelism.  Read-after-writes are good at doubling
the number of bits affected, so the goal of mixing pulls in the opposite
direction as the goal of parallelism.  I did what I could.  Rotates
seem to cost as much as shifts on every machine I could lay my hands
on, and rotates are much kinder to the top and bottom bits, so I used
rotates.
-------------------------------------------------------------------------------
*/
#define mix(a,b,c) \
{ \
  a -= c;  a ^= rot(c, 4);  c += b; \
  b -= a;  b ^= rot(a, 6);  a += c; \
  c -= b;  c ^= rot(b, 8);  b += a; \
  a -= c;  a ^= rot(c,16);  c += b; \
  b -= a;  b ^= rot(a,19);  a += c; \
  c -= b;  c ^= rot(b, 4);  b += a; \
}

/*
-------------------------------------------------------------------------------
final -- final mixing of 3 32-bit values (a,b,c) into c

Pairs of (a,b,c) values differing in only a few bits will usually
produce values of c that look totally different.  This was tested for
* pairs that differed by one bit, by two bits, in any combination
  of top bits of (a,b,c), or in any combination of bottom bits of
  (a,b,c).
* "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
  the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
  is commonly produced by subtraction) look like a single 1-bit
  difference.
* the base values were pseudorandom, all zero but one bit set, or 
  all zero plus a counter that starts at zero.

These constants passed:
 14 11 25 16 4 14 24
 12 14 25 16 4 14 24
and these came close:
  4  8 15 26 3 22 24
 10  8 15 26 3 22 24
 11  8 15 26 3 22 24
-------------------------------------------------------------------------------
*/
#define final(a,b,c) \
{ \
  c ^= b; c -= rot(b,14); \
  a ^= c; a -= rot(c,11); \
  b ^= a; b -= rot(a,25); \
  c ^= b; c -= rot(b,16); \
  a ^= c; a -= rot(c,4);  \
  b ^= a; b -= rot(a,14); \
  c ^= b; c -= rot(b,24); \
}

/*
-------------------------------------------------------------------------------
hashlittle() -- hash a variable-length key into a 32-bit value
  k       : the key (the unaligned variable-length array of bytes)
  length  : the length of the key, counting by bytes
  initval : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Two keys differing by one or two bits will have
totally different hash values.

The best hash table sizes are powers of 2.  There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.  For example, if you need only 10 bits, do
  h = (h & hashmask(10));
In which case, the hash table should have hashsize(10) elements.

If you are hashing n strings (uint8_t **)k, do it like this:
  for (i=0, h=0; i<n; ++i) h = hashlittle( k[i], len[i], h);

By Bob Jenkins, 2006.  bob_jenkins@burtleburtle.net.  You may use this
code any way you wish, private, educational, or commercial.  It's free.

Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
-------------------------------------------------------------------------------
*/

static uint32_t
nhash( const void *key, size_t length, uint32_t initval)
{
  uint32_t a,b,c;                                          /* internal state */
  union { const void *ptr; size_t i; } u;     /* needed for Mac Powerbook G4 */

  /* Set up the internal state */
  a = b = c = 0xdeadbeef + ((uint32_t)length) + initval;

  u.ptr = key;
  if (HASH_LITTLE_ENDIAN && ((u.i & 0x3) == 0)) {
    const uint32_t *k = (const uint32_t *)key;         /* read 32-bit chunks */

    /*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
    while (length > 12)
    {
      a += k[0];
      b += k[1];
      c += k[2];
      mix(a,b,c);
      length -= 12;
      k += 3;
    }

    /*----------------------------- handle the last (probably partial) block */
    /* 
     * "k[2]&0xffffff" actually reads beyond the end of the string, but
     * then masks off the part it's not allowed to read.  Because the
     * string is aligned, the masked-off tail is in the same word as the
     * rest of the string.  Every machine with memory protection I've seen
     * does it on word boundaries, so is OK with this.  But VALGRIND will
     * still catch it and complain.  The masking trick does make the hash
     * noticably faster for short strings (like English words).
     */
#ifndef VALGRIND

    switch(length)
    {
    case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
    case 11: c+=k[2]&0xffffff; b+=k[1]; a+=k[0]; break;
    case 10: c+=k[2]&0xffff; b+=k[1]; a+=k[0]; break;
    case 9 : c+=k[2]&0xff; b+=k[1]; a+=k[0]; break;
    case 8 : b+=k[1]; a+=k[0]; break;
    case 7 : b+=k[1]&0xffffff; a+=k[0]; break;
    case 6 : b+=k[1]&0xffff; a+=k[0]; break;
    case 5 : b+=k[1]&0xff; a+=k[0]; break;
    case 4 : a+=k[0]; break;
    case 3 : a+=k[0]&0xffffff; break;
    case 2 : a+=k[0]&0xffff; break;
    case 1 : a+=k[0]&0xff; break;
    case 0 : return c;              /* zero length strings require no mixing */
    }

#else /* make valgrind happy */

    k8 = (const uint8_t *)k;
    switch(length)
    {
    case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
    case 11: c+=((uint32_t)k8[10])<<16;  /* fall through */
    case 10: c+=((uint32_t)k8[9])<<8;    /* fall through */
    case 9 : c+=k8[8];                   /* fall through */
    case 8 : b+=k[1]; a+=k[0]; break;
    case 7 : b+=((uint32_t)k8[6])<<16;   /* fall through */
    case 6 : b+=((uint32_t)k8[5])<<8;    /* fall through */
    case 5 : b+=k8[4];                   /* fall through */
    case 4 : a+=k[0]; break;
    case 3 : a+=((uint32_t)k8[2])<<16;   /* fall through */
    case 2 : a+=((uint32_t)k8[1])<<8;    /* fall through */
    case 1 : a+=k8[0]; break;
    case 0 : return c;
    }

#endif /* !valgrind */

  } else if (HASH_LITTLE_ENDIAN && ((u.i & 0x1) == 0)) {
    const uint16_t *k = (const uint16_t *)key;         /* read 16-bit chunks */
    const uint8_t  *k8;

    /*--------------- all but last block: aligned reads and different mixing */
    while (length > 12)
    {
      a += k[0] + (((uint32_t)k[1])<<16);
      b += k[2] + (((uint32_t)k[3])<<16);
      c += k[4] + (((uint32_t)k[5])<<16);
      mix(a,b,c);
      length -= 12;
      k += 6;
    }

    /*----------------------------- handle the last (probably partial) block */
    k8 = (const uint8_t *)k;
    switch(length)
    {
    case 12: c+=k[4]+(((uint32_t)k[5])<<16);
             b+=k[2]+(((uint32_t)k[3])<<16);
             a+=k[0]+(((uint32_t)k[1])<<16);
             break;
    case 11: c+=((uint32_t)k8[10])<<16;     /* fall through */
    case 10: c+=k[4];
             b+=k[2]+(((uint32_t)k[3])<<16);
             a+=k[0]+(((uint32_t)k[1])<<16);
             break;
    case 9 : c+=k8[8];                      /* fall through */
    case 8 : b+=k[2]+(((uint32_t)k[3])<<16);
             a+=k[0]+(((uint32_t)k[1])<<16);
             break;
    case 7 : b+=((uint32_t)k8[6])<<16;      /* fall through */
    case 6 : b+=k[2];
             a+=k[0]+(((uint32_t)k[1])<<16);
             break;
    case 5 : b+=k8[4];                      /* fall through */
    case 4 : a+=k[0]+(((uint32_t)k[1])<<16);
             break;
    case 3 : a+=((uint32_t)k8[2])<<16;      /* fall through */
    case 2 : a+=k[0];
             break;
    case 1 : a+=k8[0];
             break;
    case 0 : return c;                     /* zero length requires no mixing */
    }

  } else {                        /* need to read the key one byte at a time */
    const uint8_t *k = (const uint8_t *)key;

    /*--------------- all but the last block: affect some 32 bits of (a,b,c) */
    while (length > 12)
    {
      a += k[0];
      a += ((uint32_t)k[1])<<8;
      a += ((uint32_t)k[2])<<16;
      a += ((uint32_t)k[3])<<24;
      b += k[4];
      b += ((uint32_t)k[5])<<8;
      b += ((uint32_t)k[6])<<16;
      b += ((uint32_t)k[7])<<24;
      c += k[8];
      c += ((uint32_t)k[9])<<8;
      c += ((uint32_t)k[10])<<16;
      c += ((uint32_t)k[11])<<24;
      mix(a,b,c);
      length -= 12;
      k += 12;
    }

    /*-------------------------------- last block: affect all 32 bits of (c) */
    switch(length)                   /* all the case statements fall through */
    {
    case 12: c+=((uint32_t)k[11])<<24;
    case 11: c+=((uint32_t)k[10])<<16;
    case 10: c+=((uint32_t)k[9])<<8;
    case 9 : c+=k[8];
    case 8 : b+=((uint32_t)k[7])<<24;
    case 7 : b+=((uint32_t)k[6])<<16;
    case 6 : b+=((uint32_t)k[5])<<8;
    case 5 : b+=k[4];
    case 4 : a+=((uint32_t)k[3])<<24;
    case 3 : a+=((uint32_t)k[2])<<16;
    case 2 : a+=((uint32_t)k[1])<<8;
    case 1 : a+=k[0];
             break;
    case 0 : return c;
    }
  }

  final(a,b,c);
  c=(32767&c);

  return c;
}

/*
 * from wsprsim_utils.c
 */
/*
 Functions used by wsprsim
 */

static char
get_locator_character_code(char ch)
{
    if( ch >=48 && ch <=57 ) { //0-9
        return ch-48;
    }
    if( ch == 32 ) {  //space
        return 36;
    }
    if( ch >= 65 && ch <= 82 ) { //A-Z
        return ch-65;
    }
    return -1;
}

static char
get_callsign_character_code(char ch)
{
    if( ch >=48 && ch <=57 ) { //0-9
        return ch-48;
    }
    if( ch == 32 ) {  //space
        return 36;
    }
    if( ch >= 65 && ch <= 90 ) { //A-Z
        return ch-55;
    }
    return -1;
}

long unsigned int
pack_grid4_power(char const *grid4, int power)
{
    long unsigned int m;
    
    m=(179-10*grid4[0]-grid4[2])*180+10*grid4[1]+grid4[3];
    m=m*128+power+64;
    return m;
}

static long
unsigned int pack_call(char const *callsign)
{
    unsigned int i;
    long unsigned int n;
    char call6[6];
    memset(call6,' ',sizeof(call6));
    // callsign is 6 characters in length. Exactly.
    size_t call_len = strlen(callsign);
    if( call_len > 6 ) {
        return 0;
    }
    if( isdigit((int) callsign[2]) ) {
        for (i=0; i<call_len; i++) {
            call6[i]=callsign[i];
        }
    } else if( isdigit((int) callsign[1]) ) {
        for (i=1; i<call_len+1; i++) {
            call6[i]=callsign[i-1];
        }
    }
    for (i=0; i<6; i++) {
        call6[i]=get_callsign_character_code(call6[i]);
    }
    n = call6[0];
    n = n*36+call6[1];
    n = n*10+call6[2];
    n = n*27+call6[3]-10;
    n = n*27+call6[4]-10;
    n = n*27+call6[5]-10;
    return n;
}

static void
pack_prefix(char *callsign, int32_t *n, int32_t *m, int32_t *nadd )
{
    size_t i;
    char call6[7];
    size_t i1 = strcspn(callsign,"/");
    
    if( callsign[i1+2] == 0 ) { 
        //single char suffix
        for (i=0; i<i1; i++) {
            call6[i]=callsign[i];
        }
        call6[i] = '\0';
        *n=pack_call(call6);
        *nadd=1;
        int nc = callsign[i1+1];
        if( nc >= 48 && nc <= 57 ) {
            *m=nc-48;
        } else if ( nc >= 65 && nc <= 90 ) {
            *m=nc-65+10;
        } else {
            *m=38;
        }
        *m=60000-32768+*m;
    } else if( callsign[i1+3]==0 ) {
        //two char suffix
        for (i=0; i<i1; i++) {
            call6[i]=callsign[i];
        }
        *n=pack_call(call6);
        *nadd=1;
        *m=10*(callsign[i1+1]-48)+(callsign[i1+2]-48);
        *m=60000 + 26 + *m;
    } else {
        char const * pfx = strtok (callsign,"/");
        char const * call = strtok(NULL," ");
        *n = pack_call (call);
        size_t plen=strlen (pfx);
        if( plen ==1 ) {
            *m=36;
            *m=37*(*m)+36;
        } else if( plen == 2 ) {
            *m=36;
        } else {
            *m=0;
        }
        for (i=0; i<plen; i++) {
            int nc = callsign[i];
            if( nc >= 48 && nc <= 57 ) {
                nc=nc-48;
            } else if ( nc >= 65 && nc <= 90 ) {
                nc=nc-65+10;
            } else {
                nc=36;
            }
            *m=37*(*m)+nc;
        }
        *nadd=0;
        if( *m > 32768 ) {
            *m=*m-32768;
            *nadd=1;
        }
    }
}

static void
interleave(uint8_t *sym)
{
    /* XXX: */
    uint8_t tmp[162];
    uint8_t p, i, j;
    
    i = p = 0;
    while (p < 162) {
        j=((i * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
        if (j < 162 ) {
            tmp[j] = sym[p];
            p++;
        }
        i++;
    }

    memcpy(sym, tmp, sizeof(tmp));
}

int
get_wspr_channel_symbols(char* rawmessage, uint8_t* symbols)
{
    int m=0, ntype=0;
    long unsigned int n=0;
    int i, j, ihash;
    static const uint8_t pr3[162] = {
        1,1,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,
        0,1,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,0,1,
        0,0,0,0,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,1,
        1,0,1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,0,1,
        0,0,1,0,1,1,0,0,0,1,1,0,1,0,1,0,0,0,1,0,
        0,0,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,0,1,1,
        0,1,0,0,0,1,1,1,0,0,0,0,0,1,0,1,0,0,1,1,
        0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,1,1,0,
        0,0
    };
    int nu[10]={0,-1,1,0,-1,2,1,0,-1,1};
    char *callsign, *grid, *powstr;
    char grid4[5], message[23];
    
    memset(message,0,sizeof(char)*23);
    i=0;
    while ( rawmessage[i] != 0 && i<23 ) {
        message[i]=rawmessage[i];
        i++;
    }
    
    size_t i1=strcspn(message," ");
    size_t i2=strcspn(message,"/");
    size_t i3=strcspn(message,"<");
    size_t i4=strcspn(message,">");
    size_t mlen=strlen(message);
    
    // Use the presence and/or absence of "<" and "/" to decide what
    // type of message. No sanity checks! Beware!
    
    if( i1 > 3 && i1 < 7 && i2 == mlen && i3 == mlen ) {
        // Type 1 message: K9AN EN50 33
        //                 xxnxxxx xxnn nn
        callsign = strtok(message," ");
        grid = strtok(NULL," ");
        powstr = strtok(NULL," ");
        int power = atoi(powstr);
        n = pack_call(callsign);
        
        for (i=0; i<4; i++) {
            grid4[i]=get_locator_character_code(*(grid+i));
        }
        m = pack_grid4_power(grid4,power);
        
    } else if ( i3 == 0 && i4 < mlen ) {
        // Type 3:      <K1ABC> EN50WC 33
        //          <PJ4/K1ABC> FK52UD 37
        // send hash instead of callsign to make room for 6 char grid.
        // if 4-digit locator is specified, 2 spaces are added to the end.
        callsign=strtok(message,"<> ");
        grid=strtok(NULL," ");
        powstr=strtok(NULL," ");
        int power = atoi(powstr);
        if( power < 0 ) power=0;
        if( power > 60 ) power=60;
        power=power+nu[power%10];
        ntype=-(power+1);
        ihash=nhash(callsign,strlen(callsign),(uint32_t)146);
        m=128*ihash + ntype + 64;
        
        char grid6[7];
        memset(grid6,0,sizeof(char)*7);
        j=strlen(grid);
        for(i=0; i<j-1; i++) {
            grid6[i]=grid[i+1];
        }
        grid6[5]=grid[0];
        n = pack_call(grid6);
    } else if ( i2 < mlen ) {  // just looks for a right slash
        // Type 2: PJ4/K1ABC 37
        callsign = strtok (message," ");
        if( i2==0 || i2>strlen(callsign) ) return 0; //guards against pathological case
        powstr = strtok (NULL," ");
        int power = atoi (powstr);
        if( power < 0 ) power=0;
        if( power > 60 ) power=60;
        power=power+nu[power%10];
        int n1, ng, nadd;
        pack_prefix(callsign, &n1, &ng, &nadd);
        ntype=power + 1 + nadd;
        m=128*ng+ntype+64;
        n=n1;
    } else {
        return 0;
    }

    // pack 50 bits + 31 (0) tail bits into 11 bytes
    unsigned char it, data[11];
    memset(data,0,sizeof(data));
    it=0xFF & (n>>20);
    data[0]=it;
    it=0xFF & (n>>12);
    data[1]=it;
    it=0xFF & (n>>4);
    data[2]=it;
    it= ((n&(0x0F))<<4) + ((m>>18)&(0x0F));
    data[3]=it;
    it=0xFF & (m>>10);
    data[4]=it;
    it=0xFF & (m>>2);
    data[5]=it;
    it=(m & 0x03)<<6 ;
    data[6]=it;
    data[7]=0;
    data[8]=0;
    data[9]=0;
    data[10]=0;
    
    size_t nbytes=11; // The message with tail is packed into almost 11 bytes.
    uint8_t channelbits[nbytes*8*2]; /* 162 rounded up */
    memset(channelbits, 0, sizeof(channelbits[0])*nbytes*8*2);
    
    encode(channelbits,data,nbytes);
    
    interleave(channelbits);

    for (i=0; i < 162; i++) {
        symbols[i] = 2 * channelbits[i] + pr3[i];
    }
    return 1;
}

#if 0

uint8_t syms[162];

static void
sendFrame(SNDFILE *f2, char *msg)
{
	float v = 0.0;
	int i;

    i = get_wspr_channel_symbols(msg, syms);

    printf("getsym: %d\n", i);

    sendFrame(f2, "<K6JQ> CM88WE 37");
    //sendFrame(f2, "WA6ZGB CM88 20");
}
#endif
