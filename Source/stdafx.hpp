#include <string>
#include <list>
#include <algorithm>

#include <ripple/conditions/Condition.h>
#include <ripple/conditions/impl/PreimageSha256.h>
#include <ripple/conditions/Fulfillment.h>
#include <ripple/protocol/digest.h>
#include <ripple/protocol/HashPrefix.h>
#include <ripple/protocol/JsonFields.h>
#include <ripple/protocol/st.h>
#include <ripple/protocol/Sign.h>
#include <ripple/protocol/TxFlags.h>
#include <ripple/basics/StringUtilities.h>
#include <ripple/json/to_string.h>
#include <ripple/json/json_reader.h>
#include <ripple/json/json_writer.h>

#include <ripple/app/tx/impl/SignerEntries.h>

#include <boost/version.hpp>

#include "Singleton.hpp"
#include "Account.hpp"
#include "AccountManager.hpp"

#include "Xrp_WalletApp.h"

#define g_WalletManager cAccountManager::GetSingleton()