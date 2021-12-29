#include <iostream>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <fstream>
using namespace std;

#define u32 uint32_t
#define ul unsigned int

#define ROTL(x, b) (((x) >> (32 - (b))) | ((x) << (b)))
#define ROTR(x, b) (((x) << (32 - (b))) | ((x) >> (b)))

#define myrand32 ((ul)(4294967296.0 * drand48()))

#define ull unsigned long long
#define mod 4294967296
#define MOD(x) ((x + mod) % mod)

ul GetRandomNumber32()
{
       ull temp;
       int i;
       if (drand48() < 0.5)
              temp = 0;
       else
              temp = 1;

       for (i = 1; i < 32; i++)
       {
              if (drand48() < 0.5)
                     temp = temp << 1;
              else
                     temp = (temp << 1) + 1;
       }
       return temp;
}
void print(ul *x)
{
       int i;
       for (i = 0; i < 4; i++)
       {
              printf("%8x ", x[i]);
              if (i > 0 && i % 4 == 3)
                     printf("\n");
       }
       printf("\n");
}
void inti(u32 *v)
{
       int i;

       for (i = 0; i < 4; i++)
       {
              v[i] = GetRandomNumber32();
       }
}
void initializerKey(ul *k)
{
       for (int i = 0; i < 4; i++)
       {
              k[i] = GetRandomNumber32();
       }
}
void diffinitializeR(ul *x, int w, int b)
{
       int i;
       int pattern = 0x00000001 << b;
       x[w] = x[w] ^ pattern;
}
void copystate(u32 *v1, u32 *v)
{
       int i;

       for (i = 0; i < 4; i++)
              v1[i] = v[i];
}
/*Forward Rounds*/
void E(u32 *v, u32 *k)
{ /*round function*/

       int i;

       for (i = 0; i < 3; i++)
       {
              v[0] += v[1];
              v[1] = ROTL(v[1], 5);
              v[1] ^= v[0];
              v[0] = ROTL(v[0], 16);
              v[2] += v[3];
              v[3] = ROTL(v[3], 8);
              v[3] ^= v[2];
              v[0] += v[3];
              v[3] = ROTL(v[3], 13);
              v[3] ^= v[0];
              v[2] += v[1];
              v[1] = ROTL(v[1], 7);
              v[1] ^= v[2];
              v[2] = ROTL(v[2], 16);
       }
}
void E1(u32 *v, u32 *k)
{ /*round function*/

       int i;
       // for (int i = 0; i < 4; i++)
       // {
       //  v[i] ^= k[i];
       // }

       for (i = 0; i < 1; i++)
       {
              v[0] += v[1];
              v[1] = ROTL(v[1], 5);
              v[1] ^= v[0];
              v[0] = ROTL(v[0], 16);
              v[2] += v[3];
              v[3] = ROTL(v[3], 8);
              v[3] ^= v[2];
              v[0] += v[3];
              v[3] = ROTL(v[3], 13);
              v[3] ^= v[0];
              v[2] += v[1];
              v[1] = ROTL(v[1], 7);
              v[1] ^= v[2];
              v[2] = ROTL(v[2], 16);
       }
}

void ffhalf(u32 *v)
{ /*1st forward half round*/

       v[0] += v[1];
       v[1] = ROTL(v[1], 5);
       v[1] ^= v[0];
       v[0] = ROTL(v[0], 16);
       v[2] += v[3];
       v[3] = ROTL(v[3], 8);
       v[3] ^= v[2];
}
void fshalf(u32 *v)
{ /*2nd forward half round*/
       v[0] += v[3];
       v[3] = ROTL(v[3], 13);
       v[3] ^= v[0];
       v[2] += v[1];
       v[1] = ROTL(v[1], 7);
       v[1] ^= v[2];
       v[2] = ROTL(v[2], 16);
}
/*Backward Rounds*/
void D(u32 *v, u32 *k)
{ /*round function*/

       int i;
       for (int i = 1; i > 0; --i)
       {
              v[2] = ROTR(v[2], 16);
              v[1] ^= v[2];
              v[1] = ROTR(v[1], 7);
              v[2] -= v[1];
              v[3] ^= v[0];
              v[3] = ROTR(v[3], 13);
              v[0] -= v[3];
              v[3] ^= v[2];
              v[3] = ROTR(v[3], 8);
              v[2] -= v[3];
              v[0] = ROTR(v[0], 16);
              v[1] ^= v[0];
              v[1] = ROTR(v[1], 5);
              v[0] -= v[1];
       }
       //  for (i = 0; i < 4; i++)
       //  {
       //   v[i] ^= k[i];
       //           }
}
void bshalf(u32 *v)
{ /*2nd backward half round*/
       v[2] = ROTR(v[2], 16);
       v[1] ^= v[2];
       v[1] = ROTR(v[1], 7);
       v[2] -= v[1];
       v[3] ^= v[0];
       v[3] = ROTR(v[3], 13);
       v[0] -= v[3];
}
ul *xordiff(ul *v, ul *v1, ul *z)
{
       for (int j = 0; j < 4; ++j)
       {
              z[j] = v[j] ^ v1[j];
       }
       return z;
}

int main()
{
       srand48(time(NULL));
       u32 v[4], v1[4], k[4], v2[4], y[4], y1[4], y2[4], z[4], z1[4];
       // diffForward, diffBack;
       ul d[4], r[4];
       int PNB_SET[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 17, 18, 19, 20, 21, 22, 25, 32, 33, 34, 35, 36, 39, 40, 43, 44, 45, 46, 51, 52, 53, 58, 59, 60, 61, 62, 63, 67, 68, 69, 74, 75, 76, 77, 78, 79, 80, 81, 84, 87, 92, 93, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118};
       //121
       int ll = 79;
       ul FORWARD_DIFF, BACK_DIFF, pattern;
       int i, j, WORD, BIT;
       double cnt = 0.0;

       int kcounter[256];
       int kwrdcount[128];
       int kbitcounter[128];
       double temp[4][32];
       double bias[128];
       int pnb = 0;
       int bit_no = 0;
       int l = 0;
       int e = 0;
       double round = 1024 * 64;
       ull loop = 0;
       ull LOOP = 0;

       while (LOOP < 1024 * 1024 * 1024)
       {

              inti(v);
              initializerKey(k);
              for (int i = 0; i < 4; i++)
              {
                     v[i] ^= k[i];
              }
              copystate(v1, v);
              copystate(y, v);

              ul pattern = (0x00000001 << 31);
              v1[0] = v[0] ^ pattern;
              copystate(y1, v1);
              fshalf(v);
              fshalf(v1);
              E(v, k);
              E(v1, k);
              ffhalf(v);
              ffhalf(v1);
              xordiff(v, v1, r);

              FORWARD_DIFF = (r[2]);

              fshalf(v);
              fshalf(v1);

              E1(v, k);
              E1(v1, k);

              for (int i = 0; i < 4; i++)
              {
                     v[i] ^= k[i];
                     v1[i] ^= k[i];
              }

              for (int i = 0; i < 4; i++)
              {
                     v[i] ^= k[i];
                     v1[i] ^= k[i];
              }
              for (j = 0; j < ll; j++)
              {

                     WORD = (PNB_SET[j] / 32);
                     BIT = PNB_SET[j] % 32;

                     if (drand48() < 0.5)
                     {
                            y[WORD] = y[WORD] ^ (0x00000001 << BIT);
                            y1[WORD] = y1[WORD] ^ (0x00000001 << BIT);
                     }
              }
              for (int round = 6; round > 5; --round)
              {
                     D(v, k);
                     D(v1, k);
              }
              bshalf(v);
              bshalf(v1);
              xordiff(v, v1, d);

              BACK_DIFF = (d[2]);

              ul pattern0 = (0x00000001 << 20);
              if (((FORWARD_DIFF ^ BACK_DIFF) & pattern0) == 0)
              {
                     pattern0 = pattern0 << 1;
                     if (((FORWARD_DIFF ^ BACK_DIFF) & pattern0) == 0)
                            cnt = cnt + 1.0;

                     loop++;
              }
              if (loop > 0 && loop % (2) == 0)
                     printf("%llu   %0.10f   \n", loop / (1024 * 256), 2 * ((cnt / loop) - 0.5));
       }
}
//                      cnt = cnt + 1.0;

//               loop++;
//               perloop++;
//               if (loop > 0 && loop % (1024 * 256) == 0)
//                      printf("#PNBs: %d #SAMPLES:2^20*%llu    Bias: %0.10f   \n", ll, LOOP, 2 * ((cnt / loop) - 0.5));
//        }
//               LOOP++;
//        }
// }