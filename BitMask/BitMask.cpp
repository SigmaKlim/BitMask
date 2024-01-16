#include "BitMask.h"
#include <iostream>
#include <cassert>

BitMask::BitMask(const BitMask& other)
{
    _numBits = 0;
    *this = other;
}

BitMask::BitMask()
{
    _numBits = 0;
}

BitMask BitMask::MakeAllZeros(unsigned numBits)
{
    BitMask mask;
    mask._numBits = numBits;
    const unsigned NUM_CELLS = numBits / CELL_BITS + (bool)(numBits % CELL_BITS);
    mask._cells.resize(NUM_CELLS);
    return mask;
}

BitMask BitMask::MakeAllOnes(unsigned numBits)
{
    BitMask mask;
    if (numBits != 0)
    {
        mask._numBits = numBits;
        const unsigned RESIDUAL_BITS = numBits % CELL_BITS;
        const unsigned NUM_CELLS = numBits / CELL_BITS + (bool)(RESIDUAL_BITS);
        mask._cells.resize(NUM_CELLS);
        for (auto& cell : mask._cells)
            cell = ALL_ONES;
        mask.KillInsignificantBits();
    }
    return mask;
}

unsigned BitMask::MakeRightOnes(unsigned numOnes)
{
    if (numOnes > std::numeric_limits<unsigned>::digits)
    {
        std::cout <<"Error\n";
        return 0;
    }
    if (numOnes == 0)
        return 0;
    return ((1 << (numOnes - 1)) - 1) * 2 + 1;
}

BitMask BitMask::MakeRightOnes(unsigned numBits, unsigned numOnes)
{
    BitMask mask;
    assert(numOnes <= numBits);
    mask._numBits = numBits;
    const unsigned NUM_CELLS = numBits / CELL_BITS + (bool)(numBits % CELL_BITS);
    mask._cells.resize(NUM_CELLS);
    const unsigned ALL_ONES_NUM_CELLS = numOnes / CELL_BITS;
    const unsigned ALL_ONES_NUM_BITS = numOnes % CELL_BITS;
    for (unsigned i = 0; i < ALL_ONES_NUM_CELLS; i++)
        mask._cells[i] = ALL_ONES;
    if (ALL_ONES_NUM_BITS != 0)
            mask._cells[ALL_ONES_NUM_CELLS] = MakeRightOnes(ALL_ONES_NUM_BITS);
    return mask;
}

BitMask BitMask::MakeOne(unsigned numBits)
{
    BitMask mask = MakeAllZeros(numBits);
    mask._cells[0] = 1;
    return mask;
}

unsigned BitMask::MakeLeftOnes(unsigned numOnes)
{
    return ALL_ONES - MakeRightOnes(std::numeric_limits<unsigned>::digits - numOnes);
}

BitMask BitMask::MakeLeftOnes(unsigned numBits, unsigned numOnes)
{
    BitMask mask;
    assert(numOnes <= numBits);
    mask._numBits = numBits;
    const unsigned BIT_RESIDUE = numBits % CELL_BITS;
    const unsigned NUM_CELLS = numBits / CELL_BITS + (bool)(BIT_RESIDUE);
    mask._cells.resize(NUM_CELLS);
    mask._cells[NUM_CELLS - 1] = MakeRightOnes(BIT_RESIDUE);
    const unsigned ALL_ONES_NUM_CELLS = (numOnes - BIT_RESIDUE) / CELL_BITS;
    const unsigned ONES_RESIDUE =  (numOnes - BIT_RESIDUE) % CELL_BITS;
    for (unsigned i = 0; i < ALL_ONES_NUM_CELLS; i++)
        mask._cells[NUM_CELLS - 1 - (bool)(BIT_RESIDUE) - i] = ALL_ONES;
    if (ONES_RESIDUE != 0)
        mask._cells[NUM_CELLS - 1 - (bool)(BIT_RESIDUE) - ALL_ONES_NUM_CELLS] = MakeLeftOnes(ONES_RESIDUE);
    return mask;
}

BitMask::~BitMask() = default;

BitMask& BitMask::operator=(const BitMask& other)
{
    if (other._numBits != _numBits)
    {
        _numBits = other._numBits;
        _cells.resize(other._cells.size());
    }
    for (unsigned i = 0; i < _cells.size(); i++)
        _cells[i] = other._cells[i];
    return *this;
}

BitMask& BitMask::operator&=(const BitMask& other)
{
    assert(_numBits == other._numBits);
    for (unsigned i = 0; i < _cells.size(); i++)
        _cells[i] &= other._cells[i];
    return *this;
}

BitMask& BitMask::operator|=(const BitMask& other)
{
    assert(_numBits == other._numBits);
    for (unsigned i = 0; i < _cells.size(); i++)
        _cells[i] |= other._cells[i];
    return *this;
}

BitMask& BitMask::operator>>=(unsigned shift)
{
    const unsigned CELL_SHIFT = shift / CELL_BITS;
    const unsigned BIT_SHIFT = shift % CELL_BITS;
    if (CELL_SHIFT > 0)
        for (unsigned i = 0; i < _cells.size(); i++)
            _cells[i] = (i + CELL_SHIFT < _cells.size()) ? _cells[i + CELL_SHIFT] : 0;
    if (BIT_SHIFT > 0)
    {
        unsigned prevCellLeftover = 0;
        for (unsigned i = 0; i < _cells.size(); i++)
        {
            const unsigned THIS_CELL_LEFTOVER = (_cells[_cells.size() - 1 - i] & MakeRightOnes(BIT_SHIFT)) << (CELL_BITS - BIT_SHIFT);
            _cells[_cells.size() - 1 - i] >>= shift;
            _cells[_cells.size() - 1 - i] |= prevCellLeftover;
            prevCellLeftover = THIS_CELL_LEFTOVER;
        }
    }
    return *this;
}

BitMask& BitMask::operator<<=(unsigned shift)
{
    const unsigned CELL_SHIFT = shift / CELL_BITS;
    const unsigned BIT_SHIFT = shift % CELL_BITS;
    if (CELL_SHIFT > 0)
        for (unsigned i = 0; i < _cells.size(); i++)
            _cells[_cells.size() - 1 - i] = (_cells.size() - 1 - i - CELL_SHIFT < _cells.size())
            ? _cells[_cells.size() - 1 - i - CELL_SHIFT] : 0;
    if (BIT_SHIFT > 0)
    {
        unsigned prevCellLeftover = 0;
        for (unsigned i = 0; i < _cells.size(); i++)
        {
            const unsigned THIS_CELL_LEFTOVER = (_cells[i] & MakeLeftOnes(BIT_SHIFT)) >> (CELL_BITS - BIT_SHIFT);
            _cells[i] <<= shift;
            _cells[i] |= prevCellLeftover;
            prevCellLeftover = THIS_CELL_LEFTOVER;
        }
    }
    KillInsignificantBits();
    return *this;
}

BitMask BitMask::operator~() const
{
    BitMask inversed = *this;
    for (unsigned i = 0; i < _cells.size(); i++)
        inversed._cells[i] = ~_cells[i];
    inversed.KillInsignificantBits();
    return inversed;
}

BitMask& BitMask::Invert()
{
    for (auto& cell : _cells)
        cell = ~cell;
    KillInsignificantBits();
    return *this;
}

BitMask& BitMask::SetBitValue(unsigned bitPosition, bool bitValue)
{
    const unsigned CELL_OFFSET = bitPosition / CELL_BITS;
    const unsigned BIT_OFFSET = bitPosition % CELL_BITS;
    assert(bitPosition < _numBits);
    unsigned cellValue = 1 << BIT_OFFSET;
    if (bitValue == 0)
    {
        cellValue = ~cellValue;
        _cells[CELL_OFFSET] &= cellValue;
    }
    else
    {
        _cells[CELL_OFFSET] |= cellValue;
    }
    return *this;
}

bool BitMask::GetBitValue(unsigned bitPosition) const
{
    const unsigned CELL_OFFSET = bitPosition / CELL_BITS;
    const unsigned BIT_OFFSET = bitPosition % CELL_BITS;
    assert(bitPosition < _numBits);
    const unsigned MASKED_VALUE = _cells[CELL_OFFSET] & (1 << BIT_OFFSET);
    return MASKED_VALUE != 0;    
}

unsigned BitMask::GetNumBits() const
{
    return _numBits;
}

unsigned BitMask::GetNumCells() const
{
    return _cells.size();
}

std::vector<std::string> BitMask::GetBinary() const
{
    std::vector <std::string> binaryStrings(_cells.size());
    for (unsigned i = 0; i < _cells.size(); i++)
        binaryStrings[i] = std::bitset<CELL_BITS>(_cells[i]).to_string();
    return binaryStrings;
}

unsigned BitMask::operator[](unsigned index) const
{
    assert(index < _cells.size());
    return _cells[index];
}

void BitMask::KillInsignificantBits()
{
    const unsigned BITS_RESIDUE = _numBits % CELL_BITS;
    auto b = GetBinary();
    if (BITS_RESIDUE != 0)
    {
        const unsigned KILLER_MASK = MakeRightOnes(BITS_RESIDUE);
        _cells[_cells.size() - 1] &= KILLER_MASK; 
    }
    b = GetBinary();
}

BitMask operator&(const BitMask& left, const BitMask& right)
{
    BitMask res(left);
    return res &= right;
}

BitMask operator|(const BitMask& left, const BitMask& right)
{
    BitMask res(left);
    return res |= right;
}

BitMask operator>>(const BitMask& mask, unsigned shift)
{
    BitMask res(mask);
    return res >>= shift;
}

BitMask operator<<(const BitMask& mask, unsigned shift)
{
    BitMask res(mask);
    return res <<= shift;
}

bool operator==(const BitMask& left, const BitMask& right)
{
    if (left._numBits == 0 && right._numBits == 0)
        return true;
    else if (left._numBits == 0 || right._numBits == 0) //we enable comparison with default value BitMask()
        return false;
    assert(left._numBits == right._numBits);
    for (unsigned i = 0; i < left._cells.size(); i++)
        if (left._cells[i] != right._cells[i])
            return false;
    return true;
}

bool operator!=(const BitMask& left, const BitMask& right)
{
    return !(left == right);
}

bool operator>(const BitMask& left, const BitMask& right)
{
    assert(left._numBits == right._numBits);
    for (unsigned i = 0; i < left._cells.size(); i++)
        if (left._cells[i] <= right._cells[i])
            return false;
    return true;
}

bool operator<(const BitMask& left, const BitMask& right)
{
    assert(left._numBits == right._numBits);
    for (unsigned i = 0; i < left._cells.size(); i++)
        if (left._cells[i] >= right._cells[i])
            return false;
    return true;
}

bool operator>=(const BitMask& left, const BitMask& right)
{
    return !(left < right);
}

bool operator<=(const BitMask& left, const BitMask& right)
{
    return !(right > left);
}


