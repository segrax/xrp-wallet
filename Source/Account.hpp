struct sTransaction {
    uint64_t mTimestamp;
    std::string mDestination;
    std::string mAmount;
    std::string mType;

    ripple::TxSeq mSequence;

    sTransaction( const std::string& pType, const std::string& pDestination, const std::string& pAmount, const ripple::TxSeq& pSequence ) {
        mTimestamp = time( 0 );
        mDestination = pDestination;
        mType = pType;
        mAmount = pAmount;
        mSequence = pSequence;
    }

    sTransaction( const Json::Value& pJson ) {
        if (!ConvertFromJson( pJson )) {
            throw std::exception( "Loaded Invalid Transaction JSON" );
        }
    }

    Json::Value ConvertToJson() const {
        Json::Value Result;

        Result["Timestamp"] = (Json::UInt) mTimestamp;
        Result["Type"] = mType;
        Result["Destination"] = mDestination;
        Result["Sequence"] = (Json::UInt) mSequence;
        Result["Amount"]["XRP"] = mAmount;

        return Result;
    }

    bool ConvertFromJson( const Json::Value& pJson ) {
        mTimestamp = pJson["Timestamp"].asUInt();
        mDestination = pJson["Destination"].asString();
        mSequence = pJson["Sequence"].asUInt();
        mAmount = pJson["Amount"]["XRP"].asString();
        mType = pJson["Type"].asString();

        return true;
    }
};

class cAccount {
    typedef std::pair<ripple::PublicKey, ripple::SecretKey> tKeyPair;

    uint64_t mCreated;
    tKeyPair mKeys;
    boost::optional<ripple::Seed> mSeed;
    ripple::TxSeq mSequence;
    uint32_t mSignerQuorum;
    std::list<ripple::SignerEntries::SignerEntry> mSigners;

    std::list<sTransaction> mTransactionHistory;

    std::string mDomain;
    std::string mEmailAddress;
    std::string mMessageKey;
    uint64_t mTransferRate;
    std::list<uint32_t> mFlags;
    std::list<uint32_t> mFlagsNew;
    double mXRP;

    private:

    void txAppendPubKey( ripple::STObject& pTx, const bool& pForceSingleSign = false ) const;
    void addTransaction( const std::string& pType, const std::string& pDestination, const uint64_t& pAmountDrops );

    public:
    cAccount( const std::string& pSeed = "", const uint32_t pSequence = 1 );
    cAccount( const Json::Value& pJson );
    cAccount( const cAccount& ) = delete;

    Json::Value ConvertToJson() const;
    bool ConvertFromJson( const Json::Value& pJson );

    std::string getAddress() const;
    std::string getSecretAs1751() const;
    std::string getSecret() const;

    uint64_t calculateFeeDrops() const;
    double calculateFeeXrp() const;

    double DropsToXrp( const uint64_t& pDrops ) const {
        return ((double)pDrops) / 1000000;
    }

    double getCurrency( const std::string pCurrency = std::string( "XRP" ) ) const;
    void setCurrency( const double pTotal = 0.0, const std::string pCurrency = std::string( "XRP" ) );
    bool reduceCurrency( const double pTotal, const std::string pCurrency = std::string( "XRP" ) );

    const std::list<ripple::SignerEntries::SignerEntry>& getSigners() const;
    void addSigner( const ripple::SignerEntries::SignerEntry pSignerEntry );
    void removeSigner( const ripple::SignerEntries::SignerEntry pSignerEntry );
    void removeSigners();
    
    uint32_t getSignerQuorum() const;
    void setSignerQuorum( const uint32_t& pQuorum );


    const std::list<sTransaction>& getTransactions() const;

    bool isValid() const;
    bool operator==( const cAccount& pRight ) const;

    void clearFlags();
    void setNewFlag( const uint32_t& pFlag );
    void setFlagsFromLedger( const uint32_t& pFlags );

    uint64_t getCreated() const;

    const std::list<uint32_t>& getFlags() const;
    bool getFlagIsSet( const uint32_t& pFlag );
    
    std::string getDomain() const;
    void setDomain( const std::string& pDomain );

    std::string getEmailAddress() const; 
    void setEmailAddress( const std::string& pEmail );
    
    std::string getMessageKey() const;
    void setMessageKey( const std::string& pMessageKey );
    
	uint64_t getSequenceNext() { if (!mSequence) ++mSequence; return mSequence++; }
    uint64_t getSequence() const;
    void setSequence( const uint64_t& pSequence );
    
    uint64_t getTransferRate() const;
    void setTransferRate( const uint64_t pTransferRate );

    bool SignTx( ripple::STTx& pTx );
    ripple::STTx CreateAccountSet( );
    ripple::STTx CreatePayment( const std::string& pDestination, const uint64_t pAmountDrops, const uint64_t pDestinationTag );
    ripple::STTx CreateSignerListSet( const std::list<ripple::SignerEntries::SignerEntry>& pSigners, const uint32_t& pNewQuorum );
    ripple::STTx CreateEscrow( const std::string& pDestination, const uint64_t pAmountDrops, const uint64_t pCancelTime, const uint64_t pExecuteTime, const uint64_t pDestinationTag, const std::string& pProofText );
    ripple::STTx CreateEscrowFinish( const std::string& pOwner, const uint32_t pSequence,  const std::string& pProofText );
    ripple::STTx CreateEscrowCancel( const std::string& pOwner, const uint32_t pSequence );

};
