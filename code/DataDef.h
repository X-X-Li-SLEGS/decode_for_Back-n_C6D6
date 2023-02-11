#ifndef DataDef_H
#define DataDef_H 1

#include <stdint.h>
#include <stdlib.h>
#include <iostream>

//#define MEMORY_POOL

#ifdef MEMORY_POOL
#include <memory/Buffer.h>
#endif

#define DEFAULT_MAX_SIZE (1*1024*1024)

struct file_head_t {
    uint32_t sz;
    uint32_t version;
    uint32_t run_number;
    uint32_t run_start_time;
    uint32_t file_open_time;
};
#define CUR_DATA_VERSION 0x00010002  // 1.0

class Fragment {
public:
    enum fragment_type_t {
        FDM = 1,
        STATUS = 2,
        TCM = 3,
    };

    Fragment(ssize_t maxSize = DEFAULT_MAX_SIZE);
    Fragment(uint8_t *ptr);
    virtual ~Fragment();
    uint8_t *ptr();
    ssize_t smax();

private:
    uint8_t* m_ptr;
    ssize_t m_maxSize;
    bool m_internalBuffer;

#ifdef MEMORY_POOL
    memory::Buffer *m_buffer;
#endif
};

class FullFragment : public Fragment 
{
public:
    enum type_t { T0_PACK=1, FIX_LEN_PACK=20 };

#pragma pack(1)
    struct head_t { 
        uint32_t type;
        uint32_t size;
        uint32_t id;
        uint32_t numberOfSubFragments;
        uint32_t reserved1;
        uint32_t reserved2;
    };
#pragma pack()

    FullFragment(ssize_t maxSize = DEFAULT_MAX_SIZE);
    FullFragment(uint8_t *ptr);
    //virtual ~FullFragment();

    uint32_t type();
    uint32_t size();
    uint32_t numberOfSubFragments();

    head_t *head();
    uint8_t *firstFragment();
};

class FEECommonFragment : public Fragment {
public:
    const static int s_fee_head_size;
    #pragma pack(1)
    struct head_t { 
        uint32_t boardId;
        uint32_t size;
        uint32_t reserved1;
        uint32_t reserved2;
    };
    #pragma pack()

    /**
     * for writing fragment data
     */
    FEECommonFragment(ssize_t maxSize = DEFAULT_MAX_SIZE);

    /**
     * for reading fragment data
     */
    FEECommonFragment(uint8_t *ptr);

    void checkFeeHead();

    ///FOLLOWING: can be called after set ptr to HEAD
    ssize_t length();
    uint8_t packType();

    void setBoardId(uint32_t boardId);
    uint32_t boardId();

    void updateSize();
    uint32_t size();

    head_t *head();
    uint8_t *feePtr();

    void updateHead(uint32_t boardId);

    void dump();
};

struct FDMDataHead{
    int Length;//2B
    int ID;//1B
    int DetID;//1B
    int Trigger;//2B
    char TODTime[5];
    char CoaTime[4];
    char TOF[4];
    char Reserved[10];
    int Chn;//1B
} ;

class FDMFragment : public FEECommonFragment {
public:
    /**
     * for writing fragment data
     */
    FDMFragment(ssize_t maxSize = DEFAULT_MAX_SIZE);

    /**
     * for reading fragment data
     */
    FDMFragment(uint8_t *ptr);
    
    ///FOLLOWING: can be called after packege content has been read to ptr
    uint16_t triggerId();
    uint8_t channel();

    ssize_t adcDataSize();
    uint8_t *adcDataPtr();

    int adcCount();
    //uint16_t adcValue(int index);
    uint16_t adcValue(int index);

    uint32_t t0id();
    double tof();
};

class StatusFragment : public FEECommonFragment {
public:
    /**
     * for writing fragment data
     */
    StatusFragment(ssize_t maxSize = DEFAULT_MAX_SIZE);

    /**
     * for reading fragment data
     */
    StatusFragment(uint8_t *ptr);
    
    ///FOLLOWING: can be called after packege content has been read to ptr
    uint8_t crateId();
    uint8_t boardId();
    uint8_t slot();
};

class TCMFragment : public FEECommonFragment {
public:
    /**
     * for writing fragment data
     */
    TCMFragment(ssize_t maxSize = DEFAULT_MAX_SIZE);

    /**
     * for reading fragment data
     */
    TCMFragment(uint8_t *ptr);
    
    ///FOLLOWING: can be called after packege content has been read to ptr
    uint32_t t0id();
    uint64_t t0sec();
    uint64_t t0ns();
    double fixTof();
};

inline uint32_t getT0Id(FEECommonFragment *f)
{
    if(f->packType() ==  Fragment::FDM) {
        FDMFragment f1(f->ptr());
        return f1.t0id();
    }
    else if(f->packType() == Fragment::TCM) {
        TCMFragment f1(f->ptr());
        return f1.t0id();
    }
    else {
        std::cout << "unknown pack type" << std::endl;
        return -1;
    }
}


#endif


