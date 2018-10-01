This is a EOS smart contract for private or public 1st amd 2nd price auctions of real or digital inventory where an auction master (auctioneer) initialises the auction and users can submit bids.

ABI Interface:
- placebid(user, bid) //Users can place bids
- initauction(owner) //The auctioneer initialises the smart contract from here
- endauction(owner) //The auctioneer ends the auction
- getwinner() //Get's the current status of the running or ended contract
- dumpmem() //Only for public auction contract, dumps list of all bids from x addresses

Start with initauction,

User's can now use placebid() to place bids

When it's time to finish the auction call endauction()

Now call getwinner() to get the auction winner.

.. Maybe getwinner should be called at the end of endauction execution? personal preference?

/private - this directory contains a simplified and private version with no multi_array, because frankly, it's not really needed unless you want to log everyone that took part in the auction, only the winner really matters.

