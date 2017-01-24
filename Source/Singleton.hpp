#include <cassert>

template <typename tBase> class cSingleton {
    static tBase* mSingleton;

    public:

    cSingleton() {
        assert( !mSingleton );
        mSingleton = static_cast<tBase*> (this);
    }

    ~cSingleton() {
        assert( mSingleton );
        mSingleton = 0;
    }

    cSingleton( const cSingleton<tBase>& ) = delete;

    static tBase& GetSingleton() {
        assert( mSingleton );
        return *mSingleton;
    }

    static tBase* GetSingletonPtr() {
        return mSingleton;
    }
};

template <typename tBase> tBase* cSingleton<tBase>::mSingleton = 0;