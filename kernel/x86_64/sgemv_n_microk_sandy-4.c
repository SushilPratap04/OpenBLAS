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




#define HAVE_KERNEL_4x8 1
static void sgemv_kernel_4x8( BLASLONG n, FLOAT **ap, FLOAT *x, FLOAT *y, BLASLONG lda4) __attribute__ ((noinline));

static void sgemv_kernel_4x8( BLASLONG n, FLOAT **ap, FLOAT *x, FLOAT *y, BLASLONG lda4)
{

	BLASLONG register i = 0;

	__asm__  __volatile__
	(
	"vzeroupper			 \n\t"
	"vbroadcastss    (%2), %%ymm12	 \n\t"	// x0 
	"vbroadcastss   4(%2), %%ymm13	 \n\t"	// x1 
	"vbroadcastss   8(%2), %%ymm14	 \n\t"	// x2 
	"vbroadcastss  12(%2), %%ymm15	 \n\t"	// x3 
	"vbroadcastss  16(%2), %%ymm0 	 \n\t"	// x4 
	"vbroadcastss  20(%2), %%ymm1 	 \n\t"	// x5 
	"vbroadcastss  24(%2), %%ymm2 	 \n\t"	// x6 
	"vbroadcastss  28(%2), %%ymm3 	 \n\t"	// x7 

        "testq          $0x04, %1               \n\t"
        "jz             .L08LABEL%=             \n\t"

	"vmovups	(%3,%0,4), %%xmm4	  \n\t"	// 4 * y

	"vmulps   (%4,%0,4), %%xmm12, %%xmm8      \n\t" 
	"vmulps   (%5,%0,4), %%xmm13, %%xmm10     \n\t" 
	"vmulps   (%6,%0,4), %%xmm14, %%xmm9      \n\t" 
	"vmulps   (%7,%0,4), %%xmm15, %%xmm11     \n\t" 
	"vaddps	  %%xmm4, %%xmm8 , %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm10, %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm9 , %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm11, %%xmm4	  \n\t"

	"vmulps   (%4,%8,4), %%xmm0 , %%xmm8      \n\t" 
	"vmulps   (%5,%8,4), %%xmm1 , %%xmm10     \n\t" 
	"vmulps   (%6,%8,4), %%xmm2 , %%xmm9      \n\t" 
	"vmulps   (%7,%8,4), %%xmm3 , %%xmm11     \n\t" 
	"vaddps	  %%xmm4, %%xmm8 , %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm10, %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm9 , %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm11, %%xmm4	  \n\t"

	"vmovups  %%xmm4,   (%3,%0,4)		  \n\t"	// 4 * y

        "addq		$4, %8	  	 	  \n\t"
        "addq		$4, %0	  	 	  \n\t"
	"subq	        $4, %1			  \n\t"		

        ".L08LABEL%=:                           \n\t"

        "testq          $0x08, %1                 \n\t"
        "jz             .L16LABEL%=               \n\t"

	"vmovups	(%3,%0,4), %%ymm4	  \n\t"	// 8 * y

	"vmulps   (%4,%0,4), %%ymm12, %%ymm8      \n\t" 
	"vmulps   (%5,%0,4), %%ymm13, %%ymm10     \n\t" 
	"vmulps   (%6,%0,4), %%ymm14, %%ymm9      \n\t" 
	"vmulps   (%7,%0,4), %%ymm15, %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm9 , %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm11, %%ymm4	  \n\t"

	"vmulps   (%4,%8,4), %%ymm0 , %%ymm8      \n\t" 
	"vmulps   (%5,%8,4), %%ymm1 , %%ymm10     \n\t" 
	"vmulps   (%6,%8,4), %%ymm2 , %%ymm9      \n\t" 
	"vmulps   (%7,%8,4), %%ymm3 , %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm9 , %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm11, %%ymm4	  \n\t"

	"vmovups  %%ymm4,   (%3,%0,4)		  \n\t"	// 8 * y

        "addq		$8, %8	  	 	  \n\t"
        "addq		$8, %0	  	 	  \n\t"
	"subq	        $8, %1			  \n\t"		


        ".L16LABEL%=:                             \n\t"

        "cmpq           $0, %1                    \n\t"
        "je             .L16END%=                 \n\t"


	".align 16				 \n\t"
	".L01LOOP%=:				 \n\t"
	"vmovups	(%3,%0,4), %%ymm4	 \n\t"	// 8 * y
	"vmovups      32(%3,%0,4), %%ymm5	 \n\t"	// 8 * y

	"prefetcht0	 192(%4,%0,4)		  \n\t"
	"vmulps   (%4,%0,4), %%ymm12, %%ymm8      \n\t" 
	"vmulps 32(%4,%0,4), %%ymm12, %%ymm9      \n\t" 
	"prefetcht0	 192(%5,%0,4)		  \n\t"
	"vmulps   (%5,%0,4), %%ymm13, %%ymm10     \n\t" 
	"vmulps 32(%5,%0,4), %%ymm13, %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm9 , %%ymm5	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm11, %%ymm5	  \n\t"

	"prefetcht0	 192(%6,%0,4)		  \n\t"
	"vmulps   (%6,%0,4), %%ymm14, %%ymm8      \n\t" 
	"vmulps 32(%6,%0,4), %%ymm14, %%ymm9      \n\t" 
	"prefetcht0	 192(%7,%0,4)		  \n\t"
	"vmulps   (%7,%0,4), %%ymm15, %%ymm10     \n\t" 
	"vmulps 32(%7,%0,4), %%ymm15, %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm9 , %%ymm5	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm11, %%ymm5	  \n\t"

	"prefetcht0	 192(%4,%8,4)		  \n\t"
	"vmulps   (%4,%8,4), %%ymm0 , %%ymm8      \n\t" 
	"vmulps 32(%4,%8,4), %%ymm0 , %%ymm9      \n\t" 
	"prefetcht0	 192(%5,%8,4)		  \n\t"
	"vmulps   (%5,%8,4), %%ymm1 , %%ymm10     \n\t" 
	"vmulps 32(%5,%8,4), %%ymm1 , %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm9 , %%ymm5	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm11, %%ymm5	  \n\t"

	"prefetcht0	 192(%6,%8,4)		  \n\t"
	"vmulps   (%6,%8,4), %%ymm2 , %%ymm8      \n\t" 
	"vmulps 32(%6,%8,4), %%ymm2 , %%ymm9      \n\t" 
	"prefetcht0	 192(%7,%8,4)		  \n\t"
	"vmulps   (%7,%8,4), %%ymm3 , %%ymm10     \n\t" 
	"vmulps 32(%7,%8,4), %%ymm3 , %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm9 , %%ymm5	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm11, %%ymm5	  \n\t"

	"vmovups  %%ymm4,   (%3,%0,4)		  \n\t"	// 8 * y
	"vmovups  %%ymm5, 32(%3,%0,4)		  \n\t"	// 8 * y

        "addq		$16, %8	  	 	  \n\t"
        "addq		$16, %0	  	 	  \n\t"
	"subq	        $16, %1			  \n\t"		
	"jnz		.L01LOOP%=		  \n\t"

	".L16END%=:                               \n\t"
	"vzeroupper			          \n\t"

	:
        : 
          "r" (i),	// 0	
	  "r" (n),  	// 1
          "r" (x),      // 2
          "r" (y),      // 3
          "r" (ap[0]),  // 4
          "r" (ap[1]),  // 5
          "r" (ap[2]),  // 6
          "r" (ap[3]),  // 7
          "r" (lda4)    // 8
	: "cc", 
	  "%xmm0", "%xmm1", 
	  "%xmm2", "%xmm3", 
	  "%xmm4", "%xmm5", 
	  "%xmm8", "%xmm9", "%xmm10", "%xmm11",
	  "%xmm12", "%xmm13", "%xmm14", "%xmm15",
	  "memory"
	);

} 




#define HAVE_KERNEL_4x4 1
static void sgemv_kernel_4x4( BLASLONG n, FLOAT **ap, FLOAT *x, FLOAT *y) __attribute__ ((noinline));

static void sgemv_kernel_4x4( BLASLONG n, FLOAT **ap, FLOAT *x, FLOAT *y)
{

	BLASLONG register i = 0;

	__asm__  __volatile__
	(
	"vzeroupper			 \n\t"
	"vbroadcastss    (%2), %%ymm12	 \n\t"	// x0 
	"vbroadcastss   4(%2), %%ymm13	 \n\t"	// x1 
	"vbroadcastss   8(%2), %%ymm14	 \n\t"	// x2 
	"vbroadcastss  12(%2), %%ymm15	 \n\t"	// x3 

        "testq          $0x04, %1               \n\t"
        "jz             .L08LABEL%=             \n\t"

	"vmovups	(%3,%0,4), %%xmm4	  \n\t"	// 4 * y

	"vmulps   (%4,%0,4), %%xmm12, %%xmm8      \n\t" 
	"vmulps   (%5,%0,4), %%xmm13, %%xmm10     \n\t" 
	"vmulps   (%6,%0,4), %%xmm14, %%xmm9      \n\t" 
	"vmulps   (%7,%0,4), %%xmm15, %%xmm11     \n\t" 
	"vaddps	  %%xmm4, %%xmm8 , %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm10, %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm9 , %%xmm4	  \n\t"
	"vaddps	  %%xmm4, %%xmm11, %%xmm4	  \n\t"

	"vmovups  %%xmm4,   (%3,%0,4)		  \n\t"	// 4 * y

        "addq		$4, %0	  	 	  \n\t"
	"subq	        $4, %1			  \n\t"		

        ".L08LABEL%=:                           \n\t"

        "testq          $0x08, %1                 \n\t"
        "jz             .L16LABEL%=               \n\t"

	"vmovups	(%3,%0,4), %%ymm4	  \n\t"	// 8 * y

	"vmulps   (%4,%0,4), %%ymm12, %%ymm8      \n\t" 
	"vmulps   (%5,%0,4), %%ymm13, %%ymm10     \n\t" 
	"vmulps   (%6,%0,4), %%ymm14, %%ymm9      \n\t" 
	"vmulps   (%7,%0,4), %%ymm15, %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm9 , %%ymm4	  \n\t"
	"vaddps	  %%ymm4, %%ymm11, %%ymm4	  \n\t"

	"vmovups  %%ymm4,   (%3,%0,4)		  \n\t"	// 8 * y

        "addq		$8, %0	  	 	  \n\t"
	"subq	        $8, %1			  \n\t"		


        ".L16LABEL%=:                             \n\t"

        "cmpq           $0, %1                    \n\t"
        "je             .L16END%=                 \n\t"


	".align 16				 \n\t"
	".L01LOOP%=:				 \n\t"
	"vmovups	(%3,%0,4), %%ymm4	 \n\t"	// 8 * y
	"vmovups      32(%3,%0,4), %%ymm5	 \n\t"	// 8 * y

	"prefetcht0	 192(%4,%0,4)		  \n\t"
	"vmulps   (%4,%0,4), %%ymm12, %%ymm8      \n\t" 
	"vmulps 32(%4,%0,4), %%ymm12, %%ymm9      \n\t" 
	"prefetcht0	 192(%5,%0,4)		  \n\t"
	"vmulps   (%5,%0,4), %%ymm13, %%ymm10     \n\t" 
	"vmulps 32(%5,%0,4), %%ymm13, %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm9 , %%ymm5	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm11, %%ymm5	  \n\t"

	"prefetcht0	 192(%6,%0,4)		  \n\t"
	"vmulps   (%6,%0,4), %%ymm14, %%ymm8      \n\t" 
	"vmulps 32(%6,%0,4), %%ymm14, %%ymm9      \n\t" 
	"prefetcht0	 192(%7,%0,4)		  \n\t"
	"vmulps   (%7,%0,4), %%ymm15, %%ymm10     \n\t" 
	"vmulps 32(%7,%0,4), %%ymm15, %%ymm11     \n\t" 
	"vaddps	  %%ymm4, %%ymm8 , %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm9 , %%ymm5	  \n\t"
	"vaddps	  %%ymm4, %%ymm10, %%ymm4	  \n\t"
	"vaddps	  %%ymm5, %%ymm11, %%ymm5	  \n\t"

	"vmovups  %%ymm4,   (%3,%0,4)		  \n\t"	// 8 * y
	"vmovups  %%ymm5, 32(%3,%0,4)		  \n\t"	// 8 * y

        "addq		$16, %0	  	 	  \n\t"
	"subq	        $16, %1			  \n\t"		
	"jnz		.L01LOOP%=		  \n\t"

	".L16END%=:                               \n\t"
	"vzeroupper			          \n\t"

	:
        : 
          "r" (i),	// 0	
	  "r" (n),  	// 1
          "r" (x),      // 2
          "r" (y),      // 3
          "r" (ap[0]),  // 4
          "r" (ap[1]),  // 5
          "r" (ap[2]),  // 6
          "r" (ap[3])   // 7
	: "cc", 
	  "%xmm4", "%xmm5", 
	  "%xmm8", "%xmm9", "%xmm10", "%xmm11",
	  "%xmm12", "%xmm13", "%xmm14", "%xmm15",
	  "memory"
	);

} 


