/**
 *   @file  MATRIX_cholesky_dat.c
 *
 *   @brief
 *      Tables used for Cholesky Decomposition
 *
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifdef _TMS320C64PLUS
/* Include files for CSL ................................................... */
#include <csl.h>
#endif

#define __MATRIX_CHOLESKY_DAT_C
#include <common/src/dpu/capon3d/modules/postProcessing/matrixFunc/api/MATRIX_cholesky_dat.h>

const uint16_t recipSqtTable_hcn[256] = {
          0x5a82, 0x5a55, 0x5a28, 0x59fb, 0x59cf, 0x59a3, 0x5977, 0x594c,
          0x5920, 0x58f5, 0x58ca, 0x58a0, 0x5875, 0x584b, 0x5821, 0x57f8,
          0x57ce, 0x57a5, 0x577c, 0x5753, 0x572b, 0x5702, 0x56da, 0x56b2,
          0x568b, 0x5663, 0x563c, 0x5615, 0x55ee, 0x55c8, 0x55a1, 0x557b,
          0x5555, 0x552f, 0x5509, 0x54e4, 0x54bf, 0x549a, 0x5475, 0x5450,
          0x542c, 0x5407, 0x53e3, 0x53bf, 0x539b, 0x5378, 0x5354, 0x5331,
          0x530e, 0x52eb, 0x52c9, 0x52a6, 0x5284, 0x5261, 0x523f, 0x521e,
          0x51fc, 0x51da, 0x51b9, 0x5198, 0x5177, 0x5156, 0x5135, 0x5114,
          0x50f4, 0x50d3, 0x50b3, 0x5093, 0x5073, 0x5054, 0x5034, 0x5015,
          0x4ff6, 0x4fd6, 0x4fb7, 0x4f99, 0x4f7a, 0x4f5b, 0x4f3d, 0x4f1f,
          0x4f00, 0x4ee2, 0x4ec4, 0x4ea7, 0x4e89, 0x4e6c, 0x4e4e, 0x4e31,
          0x4e14, 0x4df7, 0x4dda, 0x4dbd, 0x4da1, 0x4d84, 0x4d68, 0x4d4b,
          0x4d2f, 0x4d13, 0x4cf7, 0x4cdc, 0x4cc0, 0x4ca4, 0x4c89, 0x4c6e,
          0x4c53, 0x4c37, 0x4c1d, 0x4c02, 0x4be7, 0x4bcc, 0x4bb2, 0x4b97,
          0x4b7d, 0x4b63, 0x4b49, 0x4b2f, 0x4b15, 0x4afb, 0x4ae1, 0x4ac8,
          0x4aae, 0x4a95, 0x4a7c, 0x4a62, 0x4a49, 0x4a30, 0x4a18, 0x49ff,
          0x49e6, 0x49ce, 0x49b5, 0x499d, 0x4984, 0x496c, 0x4954, 0x493c,
          0x4924, 0x490c, 0x48f4, 0x48dd, 0x48c5, 0x48ae, 0x4896, 0x487f,
          0x4868, 0x4851, 0x483a, 0x4823, 0x480c, 0x47f5, 0x47de, 0x47c8,
          0x47b1, 0x479b, 0x4784, 0x476e, 0x4758, 0x4742, 0x472c, 0x4716,
          0x4700, 0x46ea, 0x46d4, 0x46bf, 0x46a9, 0x4694, 0x467e, 0x4669,
          0x4654, 0x463e, 0x4629, 0x4614, 0x45ff, 0x45ea, 0x45d6, 0x45c1,
          0x45ac, 0x4598, 0x4583, 0x456f, 0x455a, 0x4546, 0x4532, 0x451d,
          0x4509, 0x44f5, 0x44e1, 0x44cd, 0x44b9, 0x44a6, 0x4492, 0x447e,
          0x446b, 0x4457, 0x4444, 0x4430, 0x441d, 0x440a, 0x43f7, 0x43e3,
          0x43d0, 0x43bd, 0x43aa, 0x4398, 0x4385, 0x4372, 0x435f, 0x434d,
          0x433a, 0x4328, 0x4315, 0x4303, 0x42f0, 0x42de, 0x42cc, 0x42ba,
          0x42a8, 0x4296, 0x4284, 0x4272, 0x4260, 0x424e, 0x423c, 0x422a,
          0x4219, 0x4207, 0x41f6, 0x41e4, 0x41d3, 0x41c1, 0x41b0, 0x419f,
          0x418e, 0x417c, 0x416b, 0x415a, 0x4149, 0x4138, 0x4127, 0x4116,
          0x4106, 0x40f5, 0x40e4, 0x40d4, 0x40c3, 0x40b2, 0x40a2, 0x4091,
          0x4081, 0x4071, 0x4060, 0x4050, 0x4040, 0x4030, 0x4020, 0x4010
};  

const int16_t recipTable[64] = {
          0x7FFF, 0x7E07, 0x7C1E, 0x7A44, 0x7877, 0x76B9, 0x7506, 0x7360,
          0x71C6, 0x7037, 0x6EB3, 0x6D39, 0x6BC9, 0x6A63, 0x6906, 0x67B1,
          0x6665, 0x6522, 0x63E6, 0x62B2, 0x6185, 0x605F, 0x5F40, 0x5E28,
          0x5D16, 0x5C0B, 0x5B05, 0x5A05, 0x590A, 0x5815, 0x5725, 0x563A,
          0x5554, 0x5473, 0x5397, 0x52BE, 0x51EB, 0x511B, 0x504F, 0x4F88,
          0x4EC4, 0x4E04, 0x4D47, 0x4C8F, 0x4BD9, 0x4B27, 0x4A78, 0x49CC,
          0x4924, 0x487E, 0x47DB, 0x473B, 0x469E, 0x4603, 0x456B, 0x44D6,
          0x4443, 0x43B3, 0x4325, 0x4299, 0x4210, 0x4188, 0x4103, 0x4080 
        };

const uint8_t recipSqtTable_asm[256] = {
        0x6a, 0x69, 0x68, 0x67, 0x67, 0x66, 0x65, 0x65, 
        0x64, 0x63, 0x63, 0x62, 0x61, 0x61, 0x60, 0x5f, 
        0x5f, 0x5e, 0x5d, 0x5d, 0x5c, 0x5c, 0x5b, 0x5a, 
        0x5a, 0x59, 0x58, 0x58, 0x57, 0x57, 0x56, 0x55, 
        0x55, 0x54, 0x54, 0x53, 0x52, 0x52, 0x51, 0x51, 
        0x50, 0x50, 0x4f, 0x4e, 0x4e, 0x4d, 0x4d, 0x4c, 
        0x4c, 0x4b, 0x4b, 0x4a, 0x4a, 0x49, 0x48, 0x48, 
        0x47, 0x47, 0x46, 0x46, 0x45, 0x45, 0x44, 0x44, 
        0x43, 0x43, 0x42, 0x42, 0x41, 0x41, 0x40, 0x40, 
        0x3f, 0x3f, 0x3e, 0x3e, 0x3d, 0x3d, 0x3c, 0x3c, 
        0x3c, 0x3b, 0x3b, 0x3a, 0x3a, 0x39, 0x39, 0x38, 
        0x38, 0x37, 0x37, 0x36, 0x36, 0x36, 0x35, 0x35, 
        0x34, 0x34, 0x33, 0x33, 0x33, 0x32, 0x32, 0x31, 
        0x31, 0x30, 0x30, 0x30, 0x2f, 0x2f, 0x2e, 0x2e, 
        0x2d, 0x2d, 0x2d, 0x2c, 0x2c, 0x2b, 0x2b, 0x2b, 
        0x2a, 0x2a, 0x29, 0x29, 0x29, 0x28, 0x28, 0x27, 
        0x27, 0x27, 0x26, 0x26, 0x26, 0x25, 0x25, 0x24, 
        0x24, 0x24, 0x23, 0x23, 0x23, 0x22, 0x22, 0x21, 
        0x21, 0x21, 0x20, 0x20, 0x20, 0x1f, 0x1f, 0x1f, 
        0x1e, 0x1e, 0x1e, 0x1d, 0x1d, 0x1d, 0x1c, 0x1c, 
        0x1c, 0x1b, 0x1b, 0x1a, 0x1a, 0x1a, 0x19, 0x19, 
        0x19, 0x18, 0x18, 0x18, 0x17, 0x17, 0x17, 0x17, 
        0x16, 0x16, 0x16, 0x15, 0x15, 0x15, 0x14, 0x14, 
        0x14, 0x13, 0x13, 0x13, 0x12, 0x12, 0x12, 0x11, 
        0x11, 0x11, 0x11, 0x10, 0x10, 0x10, 0x0f, 0x0f, 
        0x0f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0d, 0x0d, 0x0d, 
        0x0c, 0x0c, 0x0c, 0x0c, 0x0b, 0x0b, 0x0b, 0x0a, 
        0x0a, 0x0a, 0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 
        0x08, 0x08, 0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 
        0x06, 0x05, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 
        0x04, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 
        0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 
   };

 


