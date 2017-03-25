#include "stdafx.hpp"
#include <openssl/md5.h>

std::vector<std::uint8_t> der_encode( ripple::cryptoconditions::der::Preamble& p, const ripple::Slice& pMessage ) {

	std::vector<std::uint8_t> v;
	v.resize( 2 + pMessage.size() );

	// ?Not Primitive, Context Specific?
	v[0] = p.type;

	// Message Type
	v[0] |= (p.tag & 0x1F);

	// Message length
	if (p.length >= 0x7F) {

		// TODO

	}
	else
		v[1] = p.length;

	std::memcpy( &v[2], pMessage.data(), pMessage.size() );

	return v;
}

ripple::Buffer serializeFulFillment( size_t pType, ripple::Buffer const pMessage ) {
	ripple::cryptoconditions::der::Preamble p;

	p.type = 0x80;
	p.tag = 0x00;
	p.length = pMessage.size();

	std::vector<std::uint8_t> Message = der_encode( p, pMessage.operator ripple::Slice() );

	p.type = 0x20 | 0x80;
	p.tag = pType;
	p.length = Message.size();

	Message = der_encode( p, ripple::makeSlice( Message ) );

	return ripple::Buffer( Message.data(), Message.size() );
}

ripple::Buffer serializeCondition( ripple::cryptoconditions::Condition pCondition ) {
	ripple::cryptoconditions::der::Preamble p;

	p.type = 0x80;
	p.tag = 0x00;
	p.length = pCondition.fingerprint.size();
	std::vector<std::uint8_t> MessageA = der_encode( p, pCondition.fingerprint.operator ripple::Slice() );

	std::string A;
	A.push_back( pCondition.cost );
	p.type = 0x80;
	p.tag = 0x01;
	p.length = 0x01;
	std::vector<std::uint8_t> MessageB = der_encode( p, ripple::Slice( A.c_str(), A.size() ) );

	p.type = 0x20 | 0x80;
	p.tag = static_cast<size_t>(pCondition.type);

	std::string MessageC;
	MessageC.resize( MessageA.size() + MessageB.size() );
	std::memcpy( &MessageC[0], MessageA.data(), MessageA.size() );
	std::memcpy( &MessageC[MessageA.size()], MessageB.data(), MessageB.size() );

	p.length = MessageC.size();

	std::vector<std::uint8_t> MessageF = der_encode( p, ripple::Slice( MessageC.c_str(), MessageC.size() ) );

	return ripple::Buffer( MessageF.data(), MessageF.size() );

}
/**
 * Create a new wallet
 */
cAccount::cAccount( const std::string& pSeed, const uint32_t pSequence ) {

    mSequence = pSequence;

    mSeed = ripple::parseGenericSeed( pSeed );
    if (!mSeed)
        mSeed = ripple::randomSeed();

    mKeys = generateKeyPair( ripple::KeyType::secp256k1, *mSeed );
    mSignerQuorum = 0;

    mCreated = time( 0 );
    mXRP = 0;
    mTransferRate = 0;
}

/**
 * Creating a wallet from JSON
 */ 
cAccount::cAccount( const Json::Value& pJson ) {
    if (!ConvertFromJson( pJson )) {
        throw std::exception( "Loaded invalid JSON" );
    }
}

/**
 * Save this account as JSON
 */
Json::Value cAccount::ConvertToJson() const {
    Json::Value Result;

    Result["Seed"] = getSecret();
    Result["Sequence"] = (Json::UInt) getSequence();
    Result["Created_On"] = (Json::UInt) mCreated;
    Result["Currency"]["XRP"] = mXRP;
    Result["SignerQuorum"] = mSignerQuorum;

    for (const auto Signer : mSigners) {
        Json::Value Result2;
        Result2["Weight"] = Signer.weight;
        Result2["Account"] = ripple::toBase58( Signer.account );

        Result["Signers"].append( Result2 );
    }

    for (const auto Transaction : mTransactionHistory) {
        Result["Transactions"].append( Transaction.ConvertToJson() );
    }

    Result["DomainName"] = mDomain;
    Result["EmailAddress"] = mEmailAddress;
    Result["MessageKey"] = mMessageKey;
    Result["TransferRate"] = (Json::UInt) mTransferRate;

    for (const auto Flags : mFlags) {
        Result["Flags"].append( Flags );
    }
    return Result;
}

/**
 * Load an account from JSON
 */
bool cAccount::ConvertFromJson( const Json::Value& pJson ) {
    mSeed = ripple::parseGenericSeed( pJson["Seed"].asString() );
    if (!mSeed)
        return false;

    mSequence = pJson["Sequence"].asUInt();
    mCreated = pJson["Created_On"].asUInt();
    mXRP = pJson["Currency"]["XRP"].asDouble();
    mSignerQuorum = pJson["SignerQuorum"].asUInt();

    mKeys = generateKeyPair( ripple::KeyType::secp256k1, *mSeed );

    for (const auto Signer : pJson["Signers"]) {
        const std::string aa = Signer["Account"].asString();

        auto Account = ripple::parseBase58<ripple::AccountID>( aa );

        mSigners.emplace_back( *Account, Signer["Weight"].asUInt() );
    }

    for (const auto jsonTrans : pJson["Transactions"]) {

        mTransactionHistory.emplace_back( jsonTrans );
    }

    mDomain = pJson["DomainName"].asString();
    mEmailAddress = pJson["EmailAddress"].asString();
    mMessageKey = pJson["MessageKey"].asString();
    mTransferRate = pJson["TransferRate"].asUInt();

    for (const auto Flags : pJson["Flags"]) {
        mFlags.push_back( Flags.asUInt() );
    }

    return true;
}

/**
 * Get the address in base58 of this account
 */
std::string cAccount::getAddress() const {
    return ripple::toBase58( ripple::calcAccountID( mKeys.first ) );
}

/**
 * Get the secret as in 1751
 */
std::string cAccount::getSecretAs1751() const {
    return ripple::seedAs1751( *mSeed );
}

/**
 * Get the secret in base58 for this account
 */
std::string cAccount::getSecret() const {
    return ripple::toBase58( *mSeed );
}

/**
 * Get the amount of 'Currency' held by this account
 */
double cAccount::getCurrency( const std::string pCurrency ) const {
    return mXRP;
}

/**
 * Set the amount of 'Currency' held by this account
 */
void cAccount::setCurrency( const double pTotal, const std::string pCurrency ) {
    mXRP = pTotal;
}

/**
 * Reduce the amount of 'Currency' held by this account
 */
bool cAccount::reduceCurrency( const double pTotal, const std::string pCurrency ) {
    if (mXRP < pTotal)
        return false;

    mXRP -= pTotal;
    return true;
}

/**
 * Get the timestamp of the (internal) creation/import of this account
 */
uint64_t cAccount::getCreated() const {
    return mCreated;
}

/**
 * Is the seed valid
 */
bool cAccount::isValid() const {
    return mSeed.is_initialized();
}

/**
 *
 */
bool cAccount::operator==( const cAccount& pRight ) const {
    return (mSeed.get_ptr() == pRight.mSeed.get_ptr());
}

/**
 * Get the next sequence number
 */
uint64_t cAccount::getSequence() const {
    return mSequence;
}

/**
 * Set the next sequence number
 */
void cAccount::setSequence( const uint64_t& pSequence ) {
    mSequence = pSequence;
}

/**
 * Set the signer quorum (will clear the signers list)
 */
void cAccount::setSignerQuorum( const uint32_t& pQuorum ) {
    if (pQuorum == 0) {
        mSigners.clear();
    }
    mSignerQuorum = pQuorum;
}

/**
 * Get the signer quorum
 */
uint32_t cAccount::getSignerQuorum() const {
    return mSignerQuorum;
}

/**
 * Get the list of known transactions
 */
const std::list<sTransaction>& cAccount::getTransactions() const {
    return mTransactionHistory;
}

/**
 * Get the lsit of signers
 */
const std::list<ripple::SignerEntries::SignerEntry>& cAccount::getSigners() const {
    return mSigners;
}

/**
 * Add a signer
 */
void cAccount::addSigner( const ripple::SignerEntries::SignerEntry pSignerEntry ) {
    mSigners.emplace_back( pSignerEntry );
}

/**
 * Remove a signer
 */
void cAccount::removeSigner( const ripple::SignerEntries::SignerEntry pSignerEntry ) {
    const auto Signer = std::find( mSigners.begin(), mSigners.end(), pSignerEntry );

    if (Signer == mSigners.end())
        return;

    mSigners.erase( Signer );
}

/**
 * Remove all signers and reset the Quorum
 */
void cAccount::removeSigners() {
    mSigners.clear();
    mSignerQuorum = 0;
}

/**
 * Get the list of flags
 */
const std::list<uint32_t>& cAccount::getFlags() const {
    return mFlags;
}

/**
 * Clear all flags
 */
void cAccount::clearFlags() {
    mFlags.clear();
}

/**
 * Set a flag
 */
void cAccount::setNewFlag( const uint32_t& pFlag ) {

    mFlagsNew.push_back( pFlag );
}

/**
 * Load the account flags
 */
void cAccount::setFlagsFromLedger( const uint32_t& pFlags ) {
    using namespace ripple;

    mFlags.clear();

    if(pFlags & lsfDisableMaster)
        mFlags.push_back( asfDisableMaster );

    if(pFlags & lsfNoFreeze)
        mFlags.push_back( asfNoFreeze );
    
    if (pFlags & lsfRequireDestTag)
        mFlags.push_back( asfRequireDest );

    if (pFlags & lsfRequireAuth)
        mFlags.push_back( asfRequireAuth );

    if (pFlags & lsfDisallowXRP)
        mFlags.push_back( asfDisallowXRP );

    if (pFlags & lsfDisableMaster)
        mFlags.push_back( asfDisableMaster );

    if (pFlags & lsfNoFreeze)
        mFlags.push_back( asfNoFreeze );

    if (pFlags & lsfGlobalFreeze)
        mFlags.push_back( asfGlobalFreeze );

    if (pFlags & lsfDefaultRipple)
        mFlags.push_back( asfDefaultRipple );
}

/**
 * Is a flag set
 */
bool cAccount::getFlagIsSet( const uint32_t& pFlag ) {
    return std::find( mFlags.begin(), mFlags.end(), pFlag ) != mFlags.end();
}

/**
 * Get the domain name associated to this account
 */
std::string cAccount::getDomain() const {
    return mDomain;
}

/**
 * Set the domain name associated to this account
 */
void cAccount::setDomain( const std::string& pDomain ) {
    mDomain = pDomain;
}

/**
 * Get the email address associated to this account
 */
std::string cAccount::getEmailAddress() const {
    return mEmailAddress;
}

/**
 * Set the email address associated to this account
 */ 
void cAccount::setEmailAddress( const std::string& pEmail ) {
    mEmailAddress = pEmail;
}

/**
 * Get the message key
 */
std::string cAccount::getMessageKey() const {
    return mMessageKey;
}

/** 
 * Set the message key
 */
void cAccount::setMessageKey( const std::string& pMessageKey ) {
    mMessageKey = pMessageKey;
}

/**
 * Get the transfer rate
 */
uint64_t cAccount::getTransferRate() const {
    return mTransferRate;
}

/**
 * Set the transfer rate
 */
void cAccount::setTransferRate( const uint64_t pTransferRate ) {
    mTransferRate = pTransferRate;
}

/**
 * Calculate the fee in drops, based on the number of signers
 */
uint64_t cAccount::calculateFeeDrops() const {

    return (g_WalletManager.GetCurrentFee() * (mSigners.size() + 1));
}

/**
* Calculate the fee in XRP, based on the number of signers
*/
double cAccount::calculateFeeXrp() const {

    return DropsToXrp( calculateFeeDrops() );
}

/**
 * Append the signing public key to the provided object, or an empty entry for multi-signed wallets
 */
void cAccount::txAppendPubKey( ripple::STObject& pTx, const bool& pForceSingleSign ) const {
    using namespace ripple;

    if (mSignerQuorum && !pForceSingleSign)
        pTx[sfSigningPubKey] = Slice{ nullptr, 0 };
    else
        pTx[sfSigningPubKey] = mKeys.first;
}

/**
 * Add a transaction to the history, and reduce the available currency
 */
void cAccount::addTransaction( const std::string& pType, const std::string& pDestination, const uint64_t& pAmountDrops ) {

    reduceCurrency(  DropsToXrp( pAmountDrops ), "XRP" );

    mTransactionHistory.emplace_back( pType, pDestination, std::to_string( DropsToXrp( pAmountDrops ) ), mSequence );

    mTransactionHistory.sort( 
        []( const sTransaction& a, const sTransaction& b ) { return a.mTimestamp < b.mTimestamp; } );
}

/**
* Sign a transaction for someone using this wallets key
*/
bool cAccount::SignTx( ripple::STTx& pTx ) {
    using namespace ripple;

    // Are we signing our own transaction, and the master key isn't disabled?
    if (pTx.getAccountID( sfAccount ) == calcAccountID( mKeys.first ) && !getFlagIsSet( ripple::asfDisableMaster )) {

        txAppendPubKey( pTx, true );
        pTx.sign( mKeys.first, mKeys.second );

        // Return if the signature is valid.
        return pTx.checkSign( false ).first;
    } else {
        // Get the signature for this transaction
        Serializer s = buildMultiSigningData( pTx, calcAccountID( mKeys.first ) );
        auto const multisig = ripple::sign( mKeys.first, mKeys.second, s.slice() );

        // Make the signer object that we'll inject into the array.
        STObject element( sfSigner );
        element[sfAccount] = calcAccountID( mKeys.first );
        element[sfSigningPubKey] = mKeys.first;
        element[sfTxnSignature] = multisig;

        // If a Signers array does not yet exist make one.
        if (!pTx.isFieldPresent( sfSigners ))
            pTx.setFieldArray( sfSigners, {} );

        // Insert the signer into the array.
        STArray& signers{ pTx.peekFieldArray( sfSigners ) };
        signers.emplace_back( std::move( element ) );

        // Sort the Signers array by Account.  If it is not sorted when submitted
        // to the network then it will be rejected.
        std::sort( signers.begin(), signers.end(),
                   []( STObject const& a, STObject const& b ) {
            return (a[sfAccount] < b[sfAccount]);
        } );

        // Return if the signature is valid.
        return pTx.checkSign( true ).first;
    }
}

/**
 * Create an Account Set transaction
 */
ripple::STTx cAccount::CreateAccountSet() {
    using namespace ripple;
    bool isMasterDisabled = getFlagIsSet( ripple::asfDisableMaster );

    addTransaction( "AccountSet", getAddress(), calculateFeeDrops() );

    STTx Tx( ttACCOUNT_SET,
             [&]( auto& obj ) {
        obj[sfAccount] = calcAccountID( mKeys.first );
        obj[sfFee] = STAmount{ calculateFeeDrops() };

        // Email address hash
        {
            unsigned char MD[17];
            MD5( (const unsigned char*)mEmailAddress.c_str(), mEmailAddress.size(), MD );

            uint128 EmailHash;
            std::memcpy( EmailHash.data(), MD, 16 );

            obj[sfEmailHash] = EmailHash;
        }

        obj[sfDomain] = makeSlice( mDomain );
        obj[sfMessageKey] = makeSlice( mMessageKey );

        obj[sfSequence] = getSequenceNext();
        obj[sfTransferRate] = mTransferRate;

        std::list<uint32_t> FlagsOld = mFlags;

        for (std::list<uint32_t>::iterator Flag = mFlagsNew.begin(); Flag != mFlagsNew.end();) {
            
            if (getFlagIsSet( *Flag )) {

                FlagsOld.remove( *Flag );
                Flag = mFlagsNew.erase( Flag );
            } else {
                ++Flag;
            }
        }

        // Any remaining flags must be set
        if (mFlagsNew.size()) {
            obj[sfSetFlag] = *mFlagsNew.begin();
            mFlags.push_back( *mFlagsNew.begin() );
        }
        // or cleared
        if (FlagsOld.size()) {
            obj[sfClearFlag] = *FlagsOld.begin();
            mFlags.remove( *FlagsOld.begin() );
        }

        mFlagsNew.clear();

        // Apply the public key, if the master key is enabled
        txAppendPubKey( obj, !isMasterDisabled );
    } );

    // Sign the transaction with the master key, if enabled
    if (!isMasterDisabled)
        Tx.sign( mKeys.first, mKeys.second );

    return Tx;
}

/**
 * Create the signer list set transaction for this address
 */
ripple::STTx cAccount::CreateSignerListSet( const std::list<ripple::SignerEntries::SignerEntry>& pSigners, const uint32_t& pNewQuorum ) {
    using namespace ripple;

    addTransaction( "SignerListSet", getAddress(), calculateFeeDrops() );

    STTx Tx( ttSIGNER_LIST_SET,
             [&]( auto& obj ) {
        obj[sfAccount] = calcAccountID( mKeys.first );
        obj[sfFee] = STAmount{ calculateFeeDrops() };
        obj[sfSignerQuorum] = pNewQuorum;

        obj[sfFlags] = 0;
        obj[sfSequence] = getSequenceNext();

        txAppendPubKey( obj );

        // If we are deleting, don't add the signers array
        if (pNewQuorum) {
            // If a Signers array does not yet exist make one.
            if (!obj.isFieldPresent( sfSignerEntries ))
                obj.setFieldArray( sfSignerEntries, {} );

            STArray& signers{ obj.peekFieldArray( sfSignerEntries ) };

            for (const auto& Signer : pSigners) {
                STObject element( sfSignerEntry );
                element[sfAccount] = Signer.account;
                element[sfSignerWeight] = Signer.weight;

                signers.emplace_back( std::move( element ) );
            }

            // Sort the Signers array by Account
            std::sort( signers.begin(), signers.end(),
                       []( STObject const& a, STObject const& b ) {
                return (a[sfAccount] < b[sfAccount]);
            } );
        }
    } );

    if (!mSignerQuorum)
        Tx.sign( mKeys.first, mKeys.second );

    mSigners = pSigners;
    setSignerQuorum( pNewQuorum );

    return Tx;
}

/**
 * Create a payment
 */
ripple::STTx cAccount::CreatePayment( const std::string& pDestination, const uint64_t pAmountDrops, const uint64_t pDestinationTag ) {
    using namespace ripple;

    auto const destination = parseBase58<AccountID>( pDestination );
    assert( destination );

    // Drops: 1000 = 0.001 XRP ; 1000000 = 1.0 XRP
    addTransaction( "Payment", pDestination, calculateFeeDrops() + pAmountDrops );

    STTx Tx( ripple::ttPAYMENT,
             [&]( auto& obj ) {
        obj[sfAccount] = calcAccountID( mKeys.first );
        obj[sfFee] = STAmount{ calculateFeeDrops() };

        obj[sfFlags] = tfFullyCanonicalSig;
        obj[sfSequence] = getSequenceNext();

        obj[sfAmount] = STAmount{ pAmountDrops };
        obj[sfDestination] = *destination;

		if (pDestinationTag)
			obj[sfDestinationTag] = pDestinationTag;

        txAppendPubKey( obj );
    } );

    if (!mSignerQuorum)
        Tx.sign( mKeys.first, mKeys.second );

    return Tx;
}

/**
 * Create a suspended payment
 */
ripple::STTx cAccount::CreateEscrow( const std::string& pDestination, const uint64_t pAmountDrops, const uint64_t pCancelTime, const uint64_t pExecuteTime, const uint64_t pDestinationTag, const std::string& pProofText ) {
    using namespace ripple;

    auto const destination = parseBase58<AccountID>( pDestination );
    assert( destination );

    addTransaction( "EscrowCreate", pDestination, calculateFeeDrops() + pAmountDrops );

    STTx Tx( ripple::ttESCROW_CREATE,
             [&]( auto& obj ) {
        obj[sfAccount] = calcAccountID( mKeys.first );
        obj[sfFee] = STAmount{ calculateFeeDrops() };

        obj[sfFlags] = tfFullyCanonicalSig;
        obj[sfSequence] = getSequenceNext();

        obj[sfAmount] = STAmount{ pAmountDrops };
        obj[sfDestination] = *destination;

		if (pDestinationTag)
			obj[sfDestinationTag] = pDestinationTag;

		txAppendPubKey( obj );

        if (pProofText.size()) {
			ripple::cryptoconditions::PreimageSha256 shaCondition( makeSlice( pProofText ));

            obj[sfCondition] = serializeCondition( shaCondition.condition() );
        }

        if (pCancelTime)
            obj[sfCancelAfter] = pCancelTime - 946684800;

        if (pExecuteTime)
            obj[sfFinishAfter] = pExecuteTime - 946684800;
    } );

    if (!mSignerQuorum)
        Tx.sign( mKeys.first, mKeys.second );


    return Tx;
}

/**
 * Create the finalisation of a suspended payment
 */
ripple::STTx cAccount::CreateEscrowFinish( const std::string& pOwner, const uint32_t pSequence, const std::string& pProofText ) {
    using namespace ripple;

    auto const OwnerAccount = parseBase58<AccountID>( pOwner );
    assert( OwnerAccount );

    //From SusPayFinish::calculateBaseFee
    const uint64_t extraFee = (32 + static_cast<std::uint64_t> (pProofText.size() / 16)) * 10;

    addTransaction( "EscrowFinish", getAddress(), calculateFeeDrops() + extraFee );

    STTx Tx( ripple::ttESCROW_FINISH,
                [&]( auto& obj ) {

        obj[sfAccount] = calcAccountID( mKeys.first );
        txAppendPubKey( obj );
        obj[sfFee] = STAmount{ calculateFeeDrops() + extraFee};
        obj[sfSequence] = getSequenceNext();

        obj[sfOwner] = *OwnerAccount;
        obj[sfOfferSequence] = pSequence;
        if (pProofText.size()) 
		{
			ripple::Buffer ProofBuffer( pProofText.data(), pProofText.size() );

			ripple::cryptoconditions::PreimageSha256 shaCondition( makeSlice( pProofText ) );

			obj[sfCondition] = serializeCondition( shaCondition.condition() );
			obj[sfFulfillment] = serializeFulFillment( static_cast<size_t>(shaCondition.type()), ProofBuffer );
        }
    } );

    if (!mSignerQuorum)
        Tx.sign( mKeys.first, mKeys.second );

    return Tx;
}

/**
 * Create the cancellation of a suspended payment
 **/
ripple::STTx cAccount::CreateEscrowCancel( const std::string& pOwner, const uint32_t pSequence ) {
    using namespace ripple;
    auto const OwnerAccount = parseBase58<AccountID>( pOwner );

    addTransaction( "EscrowCancel", getAddress(), calculateFeeDrops() );

    STTx Tx( ripple::ttESCROW_CANCEL,
             [&]( auto& obj ) {

        obj[sfAccount] = calcAccountID( mKeys.first );
        obj[sfSequence] = getSequenceNext();
        obj[sfFee] = STAmount{ calculateFeeDrops() };
        obj[sfOwner] = *OwnerAccount;

        txAppendPubKey( obj );
        obj[sfOfferSequence] = pSequence;
    } );

    if (!mSignerQuorum)
        Tx.sign( mKeys.first, mKeys.second );

    return Tx;
}
