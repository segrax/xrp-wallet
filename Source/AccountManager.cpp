#include "stdafx.hpp"
#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/aes.h>

cAccountManager::cAccountManager( const std::string& pKeyDatabase, const std::string& pDatabasePassword ) {
    mFeeDropsCurrent = 20;
    mFailedLoad = false;
    mKeyDatabase = pKeyDatabase;
    SetDatabasePassword( pDatabasePassword );
}

cAccountManager::~cAccountManager() {
    if (!mFailedLoad) {
        if (!Save( mKeyDatabase )) {
            // TODO:
        }
    }
}

cAccount* cAccountManager::AccountAdd( const std::string& pSeed, const uint32_t pSequence ) {

    mAccounts.emplace_back( pSeed, pSequence );
    
    if (!Save())
        return 0;

    return AccountGetByAddress( mAccounts.back().getAddress() );
}

cAccount* cAccountManager::AccountGenerate( const std::string& pSeed ) {
    return AccountAdd( pSeed );
}

cAccount* cAccountManager::AccountGetByAddress( const std::string& pAddress ) {
    for (cAccount& Wallet : mAccounts) {
        if (Wallet.getAddress().compare( pAddress ) == 0)
            return &Wallet;
    }

    return 0;
}

cAccount* cAccountManager::AccountImport( const std::string& pSeed, const uint32_t pSequence ) {
    return AccountAdd( pSeed, pSequence );
}

bool cAccountManager::AccountRemove( const cAccount& pWallet ) {
    mAccounts.remove( pWallet );

    return Save();
}

void cAccountManager::SetDatabasePassword( const std::string& pDatabasePassword ) {
    static const unsigned char salt[8] = "XRPWall";

    // Get the Key/IV buffers ready
    mKeyDatabaseKey.resize( EVP_MAX_KEY_LENGTH );
    mKeyDatabaseIv.resize( EVP_MAX_IV_LENGTH );

    // Prepare the Key and IV
    EVP_BytesToKey( EVP_aes_256_cbc(), EVP_sha1(), salt,
        (const unsigned char*)pDatabasePassword.c_str(), pDatabasePassword.length(),
                    5, (unsigned char*)mKeyDatabaseKey.c_str(), (unsigned char*)mKeyDatabaseIv.c_str() );
}

std::string cAccountManager::EnDeCrypt( bool pEncrypt, const std::string& pData ) {
    int OutputLen, FinalLen;
    EVP_CIPHER_CTX ctx;
    std::string ResultBuffer;

    // Prepare EVP
    EVP_CipherInit( &ctx, EVP_aes_256_cbc(), (const unsigned char*)mKeyDatabaseKey.c_str(), (const unsigned char*)mKeyDatabaseIv.c_str(), pEncrypt );

    // Prepare the output buffer
    ResultBuffer.resize( pData.length() + EVP_CIPHER_CTX_block_size( &ctx ) );

    // Crypt the data
    EVP_CipherUpdate( &ctx, (unsigned char*)&ResultBuffer[0], &FinalLen, (const unsigned char*)&pData[0], pData.length() );
    EVP_CipherFinal( &ctx, (unsigned char*)&ResultBuffer[FinalLen], &OutputLen );

    // Resize buffer to remove extras
    ResultBuffer.resize( FinalLen + OutputLen );

    return ResultBuffer;
}

bool cAccountManager::AccountDataProcess( const Json::Value& pAccountData ) {
    
    cAccount* Account = AccountGetByAddress( pAccountData["Account"].asString() );
    if (!Account)
        return false;

    uint64_t Balance = std::stoull( pAccountData["Balance"].asString() );

    Account->setCurrency( (double) Balance / 1000000 );
    Account->setSequence( pAccountData["Sequence"].asUInt() );

    auto Domain = ripple::strUnHex( pAccountData["Domain"].asString() ).first;

    Account->setDomain( std::string((const char*)Domain.data(), Domain.size() ));
    Account->setFlagsFromLedger( pAccountData["Flags"].asUInt() );
    Account->removeSigners();

    if (pAccountData["signer_lists"].size()) {
        const Json::Value& Signers = *pAccountData["signer_lists"].begin();

        Account->setSignerQuorum( Signers["SignerQuorum"].asUInt() );

        for (const auto& SignerEntry : Signers["SignerEntries"]) {
            const Json::Value& SE = *SignerEntry.begin();

            auto SignerAccount = ripple::parseBase58<ripple::AccountID>( SE["Account"].asString() );
            if (!SignerAccount)
                continue;

            Account->addSigner( ripple::SignerEntries::SignerEntry( *SignerAccount, SE["SignerWeight"].asUInt() ) );
        }
    } else {
        Account->setSignerQuorum( 0 );
    }
    
    return true;
}

cAddressBookEntry* cAccountManager::AddressBookAdd( const std::string& pName, const std::string& pAddress ) {
    if (!AddressBookGet( pAddress )) {
        mAddressBook.emplace_back( pName, pAddress );
        Save();
    }

    return AddressBookGet( pAddress );
}

cAddressBookEntry* cAccountManager::AddressBookGet( const std::string& pAddress ) {
    for (auto& AddressBookEntry : mAddressBook) {
        if (AddressBookEntry.mAddress.compare( pAddress ) == 0)
            return &AddressBookEntry;
    }

    return 0;
}

void cAccountManager::AddressBookRemove( const cAddressBookEntry& pEntry ) {
    mAddressBook.remove( pEntry );
}

uint64_t cAccountManager::GetCurrentFee() const {
    return mFeeDropsCurrent;
}

void cAccountManager::SetCurrentFee( const uint64_t pFeeDrops ) {
    mFeeDropsCurrent = pFeeDrops;
}

bool cAccountManager::Load( const std::string& pFilename ) {
    std::ifstream KeyFile( pFilename.empty() ? mKeyDatabase : pFilename, std::ios::binary );
    if (KeyFile.is_open()) {
        Json::Value json;

        // Read the file
        std::string KeyContents( (std::istreambuf_iterator<char>( KeyFile )),
            (std::istreambuf_iterator<char>()) );

        // Close the file
        KeyFile.close();

        // Parse the JSON
        if (!Json::Reader().parse( EnDeCrypt( false, KeyContents ), json )) {
            mFailedLoad = true;
            return false;
        }

        mAccounts.clear();
        mAddressBook.clear();

        // Load each wallet in the loaded JSON
        for (const auto& jsonWallet : json["Wallets"]) {

            mAccounts.emplace_back( jsonWallet );
        }

        // Load the address book
        for (const auto& jsonAddressBookEntry : json["AddressBook"]) {

            mAddressBook.emplace_back( jsonAddressBookEntry );
        }

        mFeeDropsCurrent = json["FeeDropsCurrent"].asUInt();
    }

    return true;
}

bool cAccountManager::Save( const std::string& pFilename ) {
    Json::Value json;

    // Append wallets
    for (const auto& Wallet : mAccounts) {
        json["Wallets"].append( Wallet.ConvertToJson() );
    }

    // Append the address book
    for (const auto& AddressBookEntry : mAddressBook) {
        json["AddressBook"].append( AddressBookEntry.ConvertToJson() );
    }

    json["FeeDropsCurrent"] = (Json::UInt) mFeeDropsCurrent;

    // Encrypt and save
    std::ofstream KeyFile( pFilename.empty() ? mKeyDatabase : pFilename, std::ios::binary );
    if (KeyFile.is_open()) {
        KeyFile << EnDeCrypt( true, json.toStyledString() );
        KeyFile.close();
    } else {
        return false;
    }

    return true;
}
