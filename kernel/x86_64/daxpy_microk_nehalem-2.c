/***************************************************************************
Copyright (c) 2014, The OpenBLAS Project
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.
3. Neither the name of the OpenBLAS project nor the names of
its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE OPENBLAS PROJECT OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#define HAVE_KERNEL_8 1
static void daxpy_kernel_8( BLASLONG n, FLOAT *x, FLOAT *y , FLOAT *alpha) __attribute__ ((noinline));

static void daxpy_kernel_8( BLASLONG n, FLOAT *x, FLOAT *y, FLOAT *alpha)
{


	BLASLONG register i = 0;

	__asm__  __volatile__
	(
	"movsd			(%4), %%xmm0		    \n\t"  // alpha	
	"shufpd          $0,  %%xmm0, %%xmm0                \n\t"

	".p2align 4				            \n\t"
	"1:				            \n\t"
        // "prefetcht0      192(%2,%0,8)                       \n\t"
        // "prefetcht0      192(%3,%0,8)                       \n\t"

        "movups                  (%2,%0,8), %%xmm12         \n\t"  // 2 * x
        "movups                16(%2,%0,8), %%xmm13         \n\t"  // 2 * x
        "movups                32(%2,%0,8), %%xmm14         \n\t"  // 2 * x
        "movups                48(%2,%0,8), %%xmm15         \n\t"  // 2 * x
        "movups                  (%3,%0,8), %%xmm8          \n\t"  // 2 * y
        "movups                16(%3,%0,8), %%xmm9          \n\t"  // 2 * y
        "movups                32(%3,%0,8), %%xmm10         \n\t"  // 2 * y
        "movups                48(%3,%0,8), %%xmm11         \n\t"  // 2 * y

	"mulpd          %%xmm0 , %%xmm12 		    \n\t"  // alpha * x
	"mulpd          %%xmm0 , %%xmm13 		    \n\t"  
	"mulpd          %%xmm0 , %%xmm14 		    \n\t"  
	"mulpd          %%xmm0 , %%xmm15 		    \n\t"  

	"addpd		%%xmm12, %%xmm8   \n\t" // y += alpha *x
	"addpd		%%xmm13, %%xmm9   \n\t"
	"addpd		%%xmm14, %%xmm10  \n\t"
	"addpd		%%xmm15, %%xmm11  \n\t"

	"movups		%%xmm8 ,   (%3,%0,8)                 \n\t"
	"movups		%%xmm9 , 16(%3,%0,8)                 \n\t"
	"movups		%%xmm10, 32(%3,%0,8)                 \n\t"
	"movups		%%xmm11, 48(%3,%0,8)                 \n\t"

	"addq		$8 , %0	  	 	             \n\t"
	"subq	        $8 , %1			             \n\t"		
	"jnz		1b		             \n\t"

	:
        : 
          "r" (i),	// 0	
	  "r" (n),  	// 1
          "r" (x),      // 2
          "r" (y),      // 3
          "r" (alpha)   // 4
	: "cc", 
	  "%xmm0", 
	  "%xmm8", "%xmm9", "%xmm10", "%xmm11", 
	  "%xmm12", "%xmm13", "%xmm14", "%xmm15",
	  "memory"
	);

} 


