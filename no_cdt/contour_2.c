void require_auth(unsigned long long owner);
void prints(const char* cstr, ...);
unsigned long long _auctioneer=0; //Auctioneer Address (Makes sure only the Auction `initiater` can end the Auction)
unsigned long long _winner=0; //Winning Address
unsigned long long _hb2=0; //Highest Bid (2nd price)
unsigned long long _hb1=0; //Highest Bid (1st price)
unsigned long long _done=0;

//Places a bid
void placebid(unsigned long long owner, unsigned long long bid)
{
      require_auth(owner);

      //Is this bid high enough? 
      if(_hb1 > bid)
      {
            prints("Your bid is too low, it's outbid.\n\nHighest Bid: ", _hb1, "\nSecond Highest Bid: ", _hb2, "\n");
            return;
      }

      //Set current winner (highest bidder)
      _winner = owner;

      //Set new highest bid (and last highest)
      _hb2 = _hb1;
      _hb1 = bid;
}

///Start a new Auction  
void initauction(unsigned long long owner)
{
      require_auth(owner);
      if(_auctioneer != 0)
      {
        prints("This auction has already been initialized.\n");
        return;
      }
      _auctioneer = owner;
}

//Who was the winner / who is the current winner?
void getwinner(unsigned long long owner)
{
      require_auth(owner);
      if(_auctioneer == owner)
      {
            prints("The winning address: ", _winner, "\nHighest Bid: ", _hb1, "\nSecond Highest Bid: ", _hb2, "\n\n");
            if(_done == 0)
                  prints("The auction has not finished yet.\n");
            else
                  prints("This auction has finished.\n");
      }
      else
      {
            prints("You need to be the Auctioneer to see the winner.\n");
      }
}

//Finish the auction and set the winner  
void endauction(unsigned long long owner)
{
      require_auth(owner);
      _auctioneer = 0;
      _done = 1;
      getwinner(owner);
}
