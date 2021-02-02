## This is an EOS smart contract for private or public 1st and 2nd price auctions of real or digital inventory where an auction master (auctioneer) initialises the auction and users can submit bids.

**ABI Interface** [[`auction_public.cpp`](https://github.com/mrbid/EOSIO-AUCTIONS/blob/master/auction_private.cpp)]: *(Action caller pays for used ram)*
- `placebid(user, bid)` - Users can place bids
- `getwinner()` - Get's the current status of the running or ended contract
- `getbids()` - Dumps list of all bids from x addresses

**ABI Interface** [[`auction_private.cpp`](https://github.com/mrbid/EOSIO-AUCTIONS/blob/master/auction_private.cpp)]: *(Owner pays for all ram, only once when setting up the contract)*
- `placebid(user, bid)` - Users can place bids
- `getwinner()` - Get's the current status of the running or ended contract`
- `endauction()` - Ends the auction, can only be called by contract initiator`
- `reset()` - Reset's the auction so that the contract can be re-used for another auction.`

**auction_private is more efficient on ram usage than auction_public.**

**:: Thoughts for improvement**

It's notable that a public auction cannot be reset, and that possibly a public auction should identify its purpose with some kind of queryauctionpurpose() function, but this is optional.

As for private auctions, bidding is currently open to anyone aware of its existence, maybe bidding access could be password protected? Although, only distributing the awareness of the auction may be secure enough in some instances.
