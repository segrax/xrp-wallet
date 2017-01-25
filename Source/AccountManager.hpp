class cAddressBookEntry {
    public:

    std::string			mName;
    std::string			mAddress;

    cAddressBookEntry( const std::string& pName, const std::string& pAddress ) {
        mName = pName;
        mAddress = pAddress;
    }

    cAddressBookEntry( const Json::Value& pJson ) {
        if (!ConvertFromJson( pJson )) {
            throw std::exception( "Loaded invalid JSON" );
        }
    }

    const Json::Value ConvertToJson() const {
        Json::Value Result;

        Result["Name"] = mName;
        Result["Address"] = mAddress;

        return Result;
    }

    const bool ConvertFromJson( const Json::Value& pJson ) {
        mName = pJson["Name"].asString();
        mAddress = pJson["Address"].asString();

        return true;
    }

    const bool operator==( const cAddressBookEntry& pRight ) const {
        return (&pRight == this);
    }
};

class cAccountManager : public cSingleton<cAccountManager> {
    private:

    std::string						mKeyDatabase;

    std::string						mKeyDatabaseIv;
    std::string						mKeyDatabaseKey;
    bool							mFailedLoad;

    uint64_t                        mFeeDropsCurrent;

    // TODO: These should be private
    public:
    std::list<cAccount>				mAccounts;
    std::list<cAddressBookEntry>	mAddressBook;

    private:
    cAccount*						AccountAdd( const std::string& pSeed, const uint32_t pSequence = 1 );

    public:
    cAccountManager( const std::string& pKeyDatabase, const std::string& pDatabasePassword );
    ~cAccountManager();

    bool                            AccountDataProcess( const Json::Value& pAccountData );

    cAddressBookEntry*				AddressBookAdd( const std::string& pName, const std::string& pAddress );
    cAddressBookEntry*				AddressBookGet( const std::string& pAddress );
    void							AddressBookRemove( const cAddressBookEntry& pEntry );

    uint64_t                        GetCurrentFee() const;
    void                            SetCurrentFee( const uint64_t pFeeDrops );

    cAccount*						AccountGenerate( const std::string& pSeed = "" );
    cAccount*						AccountGetByAddress( const std::string& pAddress );

    cAccount*						AccountImport( const std::string& pSeed, const uint32_t pSequence );
    bool					    	AccountRemove( const cAccount& pWallet );

    bool							Load( const std::string& pFilename = "" );
    bool							Save( const std::string& pFilename = "" );

    void							SetDatabasePassword( const std::string& pDatabasePassword );
    std::string				        EnDeCrypt( bool pEncrypt, const std::string& pData );
};