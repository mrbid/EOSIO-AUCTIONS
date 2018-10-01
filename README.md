This is a smart contract for 1st and 2nd price auction models where an auction master (auctioneer) initialises the auction and users can submit bids.

ABI Interface:
- placebid(user, bid)
- initauction(owner)
- endauction(owner)
- getwinner()
- dumpmem() ~ eh might ad this at some point to dump the multi_array memory

Start with initauction,

User's can now use placebid() to place bids

When it's time to finish the auction call endauction()

Now call getwinner() to get the auction winner.

.. Maybe getwinner should be called at the end of endauction execution? personal preference?

/simpler - this directory contains a simplified version with no multi_array, because frankly, it's not really needed unless you want to log everyone that took part in the auction, only the winner really matters.

