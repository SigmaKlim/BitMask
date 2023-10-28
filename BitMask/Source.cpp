#include "BitMask.h"

int main()
{
    // BitMask m1 = BitMask::MakeOne(100);
    // BitMask m2(m1);
    // bool areSame = (m1 == m2); //true
    //
    // BitMask m3 = m1 << 30;
    // BitMask m4 = BitMask::MakeAllZeros(100);
    // m4.SetBitValue(30, 1);
    // areSame = (m3 == m4); //true
    //
    // BitMask m5 = BitMask::MakeAllOnes(50);
    // m5.SetBitValue(25, 0);
    // BitMask m6 = ~m5;
    // areSame = (m6 == (BitMask::MakeOne(50) << 25)); //true
    //
    // BitMask m7 = BitMask::MakeAllZeros(1000);
    // m7.Invert();
    // BitMask m8 = m7;
    // m7.SetBitValue(0, 0);
    // bool is7LargerThan8 = (m7 > m8); //false

    BitMask m9 = BitMask::MakeRightOnes(10, 11);
}