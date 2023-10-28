#pragma once
#include <vector>
#include <limits>
#include <bitset>
class BitMask
{
public:

                        BitMask         ();
                        BitMask         (const BitMask& other);
                        ~BitMask        ();
    //returns a bitmask with all bits set to 0
    static  BitMask     MakeAllZeros    (unsigned numBits);
    //returns a bitmask with all bits set to 1
    static  BitMask     MakeAllOnes     (unsigned numBits);
    //returns unsigned 11..100..0
    static  unsigned    MakeLeftOnes    (unsigned numOnes);
    //returns mask 11..100..0
    static  BitMask     MakeLeftOnes    (unsigned numBits, unsigned numOnes);
    //returns unsigned 00..011..1
    static  unsigned    MakeRightOnes   (unsigned numOnes);
    //returns mask 00..011..1
    static  BitMask     MakeRightOnes   (unsigned numBits, unsigned numOnes);
    //return a bitmask, where first bit is 1 and the rest are 0;
    static  BitMask     MakeOne         (unsigned numBits);
                        
            BitMask&    operator=       (const BitMask& other);
            BitMask&    operator&=      (const BitMask& other);
            BitMask&    operator|=      (const BitMask& other);
            BitMask&    operator>>=     (unsigned       shift);
            BitMask&    operator<<=     (unsigned       shift);
            BitMask     operator~       () const;
            BitMask&    Invert          ();

            //sets the bit value of bitPosition-th bit, starting from 0
            BitMask&    SetBitValue     (unsigned bitPosition, bool bitValue);
            //returns the bit value of bitPosition-th bit, starting from 0
            bool        GetBitValue     (unsigned        bitPosition) const;
            //returns number of bits stored in the mask
            unsigned    GetNumBits      () const;
            //debug method used to view the mask as a vector of 1/0 strings
            std::vector<std::string> GetBinary() const;
    
    friend  bool        operator==      (const BitMask& left, const BitMask& right);
    friend  bool        operator!=      (const BitMask& left, const BitMask& right);
    friend  bool        operator>       (const BitMask& left, const BitMask& right);
    friend  bool        operator<       (const BitMask& left, const BitMask& right);
    friend  bool        operator>=      (const BitMask& left, const BitMask& right);
    friend  bool        operator<=      (const BitMask& left, const BitMask& right);

    static  const unsigned  CELL_BITS   = std::numeric_limits<unsigned>::digits;
    static  const unsigned  ALL_ONES    = std::numeric_limits<unsigned>::max(); //a cell consisting of all ones

private:
            //Used to nullify all bits to the left of _numBits-1, which might appear as a result of left-shift or inversion
            void        KillInsignificantBits();
    
            //it is guaranteed that all bits to the right of _numBits-1 will not affect bitwise operation nor decimal value of the mask
            unsigned                _numBits;
            std::vector<unsigned>   _cells;
};



BitMask  operator&   (const BitMask& left, const BitMask& right);
BitMask  operator|   (const BitMask& left, const BitMask& right);
BitMask  operator>>  (const BitMask& mask, unsigned shift);
BitMask  operator<<  (const BitMask& mask, unsigned shift);
bool     operator==  (const BitMask& left, const BitMask& right);
bool     operator!=  (const BitMask& left, const BitMask& right);
bool     operator>   (const BitMask& left, const BitMask& right);
bool     operator<   (const BitMask& left, const BitMask& right);
bool     operator>=  (const BitMask& left, const BitMask& right);
bool     operator<=  (const BitMask& left, const BitMask& right);
    