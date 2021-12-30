#include <iostream>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <fstream>
using namespace std;

#define ROTL(x, b) (uint32_t)(((x) >> (32 - (b))) | ((x) << (b)))
#define u32 uint32_t
#define ul unsigned int
#define myrand32 ((ul)(4294967296.0 * drand48()))
#define ROTR32(x, b) ((x) << (32 - (b))) | ((x) >> (b))
#define ull unsigned long long
#define mod 4294967295
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
    // for (int i = 0; i < 4; i++)
    // {
    //  v[i] ^= k[i];
    // }

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
        v[2] = ROTR32(v[2], 16);
        v[1] ^= v[2];
        v[1] = ROTR32(v[1], 7);
        v[2] -= v[1];
        v[3] ^= v[0];
        v[3] = ROTR32(v[3], 13);
        v[0] -= v[3];
        v[3] ^= v[2];
        v[3] = ROTR32(v[3], 8);
        v[2] -= v[3];
        v[0] = ROTR32(v[0], 16);
        v[1] ^= v[0];
        v[1] = ROTR32(v[1], 5);
        v[0] -= v[1];
    }
    //  for (i = 0; i < 4; i++)
    //  {
    //   v[i] ^= k[i];
    //           }
}
void bshalf(u32 *v)
{ /*2nd backward half round*/
    v[2] = ROTR32(v[2], 16);
    v[1] ^= v[2];
    v[1] = ROTR32(v[1], 7);
    v[2] -= v[1];
    v[3] ^= v[0];
    v[3] = ROTR32(v[3], 13);
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
    std::ofstream myfile;
    myfile.open("pnblist.txt");
    cout << endl;
    cout << endl;
    cout << endl;
    double limit = 0.3;
    cout << "               PNBs in 6 round Chaskey with bias limit " << limit << "                                  " << endl;
    myfile << "             PNBs in 6 round Chaskey with bias limit " << limit << "                                  " << endl;

    cout << "************************************************************************************************" << endl;
    myfile << "************************************************************************************************" << endl;
    cout << endl;
    float ctr = 0.0;
    srand48(time(NULL));
    u32 v[4], v1[4], k[4], v2[4], y[4], y1[4], y2[4], z[4], z1[4];
    // diffForward, diffBack;
    ul d[4], r[4];

    ul pattern;
    int count1 = 0;
    int count2 = 0;

    double num = 0.0;

    int kwrd, kbit;
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

    printf("   KeyBit                    Bias   \n");
    myfile << "     KeyBit                      Bias   \n"
           << endl;
    for (kwrd = 0; kwrd < 4; ++kwrd)
    {
        for (kbit = 0; kbit < 32; ++kbit)
        {
            num = 0.0;
            for (int cnt = 0; cnt < round; ++cnt)
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
                // diffinitializeR(v1, w, b);
                copystate(y1, v1);
                fshalf(v);
                fshalf(v1);
                //  for (int round = 1; round <= 4; ++round)
                // {
                E(v, k);
                E(v1, k);
                ffhalf(v);
                ffhalf(v1);
                xordiff(v, v1, r);
                ul pattern0 = (0x00000001 << 20);
                if (((r[2]) & pattern0) == 0)
                {
                    count1 = 1;
                }
                else
                {
                    count1 = 0;
                }
                // }
                fshalf(v);
                fshalf(v1);
                // for (int round = 5; round < 7; ++round)
                // {
                E1(v, k);
                E1(v1, k);
                // }
                for (int i = 0; i < 4; i++)
                {
                    v[i] ^= k[i];
                    v1[i] ^= k[i];
                }

                diffinitializeR(k, kwrd, kbit);
                for (int i = 0; i < 4; i++)
                {
                    v[i] ^= k[i];
                    v1[i] ^= k[i];
                }

                for (int round = 6; round > 5; --round)
                {
                    D(v, k);
                    D(v1, k);
                }
                bshalf(v);
                bshalf(v1);
                xordiff(v, v1, d);
                ul pattern1 = (0x00000001 << 20);
                if (((d[2]) & pattern0) == 0)

                {
                    count2 = 1;
                }
                else
                {
                    count2 = 0;
                }
                if (count1 == count2)
                {
                    num++;
                }
            }

            temp[kwrd][kbit] = (2.0 * num) / round - 1.0;
            // temp[kwrd][kbit] =num/round;
            if ((temp[kwrd][kbit]) >= limit)
            {
                bias[bit_no] = temp[kwrd][kbit];
                printf("    %d       %7lf   \n", (kwrd * 32) + kbit, bias[bit_no]);
                myfile << "    " << (kwrd * 32) + kbit << "                         " << bias[bit_no] << "      \n"
                       << endl;
                //printf("%d", (kwrd* 32) + kbit);
                pnb++;
                kbitcounter[l] = (kwrd * 32) + kbit;
                l++;
                bit_no++;
            }
        }
    }
    printf("The PNB set is - {");
    // myfile<<"The PNB set is - {";
    for (int g = 0; g < l; ++g)
    {
        if (g == l - 1)
        {
            printf("%d", kbitcounter[g]);
            myfile << kbitcounter[g];
        }
        else
            printf("%d,", kbitcounter[g]);
        myfile << kbitcounter[g] << ", ";
    }
    printf("}\n");
    myfile << "}\n"
           << endl;
    printf("\nTotal number of pnb = %d\n", pnb);
    // myfile<<"\nTotal number of pnb "<< pnb << endl;
}