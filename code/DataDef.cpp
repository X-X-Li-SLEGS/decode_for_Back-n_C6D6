#include "DataDef.h"
#include "assert.h"
#include <stdio.h>
#include <iostream>

#define FDM_UNIT_TOF 8./174.
#define TCM_UNIT_TOF 8./128.

#ifdef MEMORY_POOL
using namespace memory;
#endif

Fragment::Fragment(ssize_t maxSize) {
#ifdef MEMORY_POOL
    m_buffer = new Buffer(maxSize);
    m_ptr = (uint8_t*)m_buffer->addr();
#else
    m_ptr = new uint8_t[maxSize];
#endif
    m_maxSize = maxSize;
    m_internalBuffer = true;
}

Fragment::Fragment(uint8_t *ptr) {
    m_ptr = ptr;
    m_internalBuffer = false;
}

Fragment::~Fragment() {
    if(m_internalBuffer) {
#ifdef MEMORY_POOL
        delete m_buffer;
#else
        delete m_ptr;
#endif
    }
}

uint8_t *Fragment::ptr() {
    return m_ptr;
}

ssize_t Fragment::smax() {
    return m_maxSize;
}

FullFragment::FullFragment(ssize_t maxSize)
    : Fragment(maxSize)
{
    head()->reserved1 = 0xbb00bb01;
    head()->reserved2 = 0xbb00bb02;
}

FullFragment::FullFragment(uint8_t *ptr)
    : Fragment(ptr)
{
}

FullFragment::head_t *FullFragment::head()
{
    return (head_t*)ptr();
}

uint32_t FullFragment::size()
{
    return head()->size;
}

uint32_t FullFragment::type()
{
    return head()->type;
}

uint32_t FullFragment::numberOfSubFragments()
{
    return head()->numberOfSubFragments;
}

uint8_t *FullFragment::firstFragment()
{
    return ptr()+sizeof(head_t);
}

const int FEECommonFragment::s_fee_head_size = 6;

FEECommonFragment::FEECommonFragment(ssize_t maxSize)
    : Fragment(maxSize)
{
}

FEECommonFragment::FEECommonFragment(uint8_t *ptr)
    : Fragment(ptr)
{
}

void FEECommonFragment::checkFeeHead()
{
    uint8_t *ptr = feePtr();
    if(!(*ptr == 0xeb && *(ptr+1) == 0x90)) {
        dump();
    }
    assert(*ptr == 0xeb && *(ptr+1) == 0x90);
}

void FEECommonFragment::dump()
{
    uint8_t *ptr = feePtr();
    int len = (128 > length() ? length() : 128);
    for(int i=0; i<len; i++) {
        printf("%02x ", (uint32_t)ptr[i]);
        if((i+1)%16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

void FEECommonFragment::setBoardId(uint32_t boardId)
{
    head()->boardId = boardId;
}

void FEECommonFragment::updateHead(uint32_t boardId)
{
    setBoardId(boardId);
    updateSize();
    head()->reserved1 = 0xaa00aa01;
    head()->reserved2 = 0xaa00aa02;
}

uint32_t FEECommonFragment::boardId()
{
    return head()->boardId;
}

void FEECommonFragment::updateSize()
{
    head()->size = length()+sizeof(FEECommonFragment::head_t);
}

uint32_t FEECommonFragment::size()
{
    return head()->size;
}

ssize_t FEECommonFragment::length()
{
    uint8_t *p = feePtr();
    return (uint32_t)(*(p+2)) << 16
        | (uint32_t)(*(p+3)) << 8
        | (uint32_t)(*(p+4)) << 0;
}

uint8_t FEECommonFragment::packType()
{
    uint8_t *p = feePtr();
    return (*(p+5));
}

FEECommonFragment::head_t *FEECommonFragment::head()
{
    return (head_t*)ptr();
}

uint8_t *FEECommonFragment::feePtr()
{
    return ptr()+sizeof(FEECommonFragment::head_t);
}

///
/// FDMFragment
///
FDMFragment::FDMFragment(ssize_t maxSize)
    : FEECommonFragment(maxSize)
{
}

FDMFragment::FDMFragment(uint8_t *ptr)
    : FEECommonFragment(ptr)
{
}

uint16_t FDMFragment::triggerId()
{
    uint8_t *p = feePtr();
    return (uint32_t)(*(p+0x15)) << 8 
        | (uint32_t)(*(p+0x16)) << 0;
}

uint8_t FDMFragment::channel()
{
    uint8_t *p = feePtr();
    return *(p+0x2f);
}

ssize_t FDMFragment::adcDataSize()
{
    return length() - 0x30;
}

uint8_t *FDMFragment::adcDataPtr()
{
    return feePtr() + 0x30;
}

int FDMFragment::adcCount()
{
    return adcDataSize() / 2;
}

uint16_t FDMFragment::adcValue(int index)
{
    assert(index < adcCount());
    uint8_t *p = adcDataPtr() + index*2;
    return (uint32_t)(*(p+0)) << 8 
        | (uint32_t)(*(p+1)) << 0;
}

uint32_t FDMFragment::t0id()
{
    uint8_t *p = feePtr();
    uint32_t t0id = (uint32_t)(*(p+6)) << 24
        | (uint32_t)(*(p+7)) << 16
        | (uint32_t)(*(p+8)) << 8
        | (uint32_t)(*(p+9)) << 0;
    return t0id;
}

double FDMFragment::tof()
{
    uint8_t *p = feePtr();
    uint32_t byte0 = (uint32_t)(*(p+0x1a));
    double ps = (double)byte0*FDM_UNIT_TOF;
    
    uint8_t flag = (uint32_t)(*(p+0x17)) & 0x80 ;
    uint32_t ns = (uint32_t)(*(p+0x17)) << 16
        | (uint32_t)(*(p+0x18)) << 8
        | (uint32_t)(*(p+0x19)) << 0;
    if(flag) ns = ns | (0xff << 24) ;
    
    return (double)ps+(int32_t)ns*8 ;
}

/**
 * for StatusFragment
 */

StatusFragment::StatusFragment(ssize_t maxSize)
    : FEECommonFragment(maxSize)
{
}

StatusFragment::StatusFragment(uint8_t *ptr)
    : FEECommonFragment(ptr)
{
}

uint8_t StatusFragment::crateId()
{
    uint8_t *p = feePtr();
    return *(p+6);
}

uint8_t StatusFragment::boardId()
{
    std::cout << "status fragment board id" << std::endl;
    uint8_t *p = feePtr();
    return *(p+7);
}

uint8_t StatusFragment::slot()
{
    uint8_t *p = feePtr();
    return *(p+8);
}

/**
 * for TCMFragment
 */

TCMFragment::TCMFragment(ssize_t maxSize)
    : FEECommonFragment(maxSize)
{
}

TCMFragment::TCMFragment(uint8_t *ptr)
    : FEECommonFragment(ptr)
{
}

uint64_t TCMFragment::t0sec()
{
    uint8_t *p = feePtr();
    uint64_t utc = (uint64_t)(*(p+10)) << 32
        | (uint64_t)(*(p+11)) << 24
        | (uint64_t)(*(p+12)) << 16
        | (uint64_t)(*(p+13)) << 8
        | (uint64_t)(*(p+14)) << 0;
    bool utcOk = ((*(p+15) >> 5 & 0x07) == 0x07);
    if(utcOk) {
        return utc;
    }
    else {
        return 0;
    }
}

uint64_t TCMFragment::t0ns()
{
    uint8_t *p = feePtr();
    uint64_t ns = (uint64_t)(*(p+15)&0x0f) << 24
        | (uint64_t)(*(p+16)) << 16
        | (uint64_t)(*(p+17)) << 8
        | (uint64_t)(*(p+18)) << 0;

    return ns*8;
}


uint32_t TCMFragment::t0id()
{
    uint8_t *p = feePtr();
    uint32_t t0id = (uint32_t)(*(p+6)) << 24
        | (uint32_t)(*(p+7)) << 16
        | (uint32_t)(*(p+8)) << 8
        | (uint32_t)(*(p+9)) << 0;
    return t0id;
}

double TCMFragment::fixTof()
{
    uint8_t *p = feePtr();
    uint8_t flag = (uint8_t)(*(p+0x0f));
    double val = (double)(*(p+0x13))*TCM_UNIT_TOF;
    if(flag >> 4 & 0x01) {
        val += 8;
    }
    return val;
}

