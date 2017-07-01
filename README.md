# Xrp-Wallet

#### About

![Image](https://cloud.githubusercontent.com/assets/1327406/23770057/a87ecda8-0565-11e7-9886-094020fae4e4.png "Image")

Cold Wallet for XRP.

* Payments
* Escrow Payments
* Multisigning
* Account Settings
* Set Signer List


#### How to Use


##### First Launch

On first launch, you will be asked for a password. 
This password is used to generate an AES256 Key, which is used to encrypt your account database when stored to disk.

##### Submitting your Transactions via QR Code

You may use your phone to capture the QR Code using [XRP Ledger Submit](https://binz.io/), which will allow you to immediately submit your transaction to the network and view the result.

##### Creating an account

1. Browse to the 'Accounts' tab
2. Press 'Create Account'
3. You will be shown the secret which has been generated (this can be shown again later)
4. Double click the new account
5. You can now take the new account address via two methods  
  a. Click 'Show QR Code'  
  b. Click 'Copy Address'  
6. Send the new account the minimum XRP required to activate it (20 at the time of writting)


##### Sending a payment

1. Browse to the 'Send' tab
2. Select the account which you wish to send from
3. Enter the desetination account addres
4. Enter an amount
5. If you've never used the account outside the wallet, and have always submitted transactions generated, the Sequence Number should not need touching
6. Click 'Sign'
7. You can now take the sign transaction blob and submit it to a Ripple validator


##### Setting up a multisigned wallet

1. Browse to the 'Accounts' tab
2. Double click the account you wish to setup Multisign on
3. Click 'Set Signer List'
4. Set a 'Signer Target Weight'. This should be based on your target configuration.
5. Click 'Add'
6. Enter an account address that can sign for this account
7. Enter the 'weight' this account has
8. Repeat 5-7 for each account
9. Click 'Sign'
10. You can now take the sign transaction blob and submit it to a Ripple validator


#### Compiling

##### Requires

* WxWidgets 3.1
* OpenSSL
* rippled

##### Install some packages
$ git submodule init
$ git submodule update
