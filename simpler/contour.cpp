/*

      1st & 2nd price auction model smart contract for EOS
      By James William Fletcher (~2018)
      http://github.com/mrbid

*/
#include <eosiolib/eosio.hpp>

class auction : public eosio::contract
{

public:

      //Constructor
      auction(account_name s): contract(s), _auctioneer(NULL), _winner(NULL), _done(false) {}

      //Places a bid
      void placebid(account_name owner, int64_t bid)
      {
            require_auth(owner);

            //Is this bid high enough? 
            if(_hb1 > bid)
            {
                  eosio::print("Your bid is too low, it's outbid.\n\nHighest Bid: ", _hb1, "\nSecond Highest Bid: ", _hb2, "\n");
                  return;
            }

            //Set current winner (highest bidder)
            _winner = owner;

            //Set new highest bid (and last highest)
            _hb2 = _hb1;
            _hb1 = bid;
      }

      ///Start a new Auction  
      void initauction(account_name owner)
      {
            require_auth(owner);
            eosio_assert(_auctioneer != NULL, "This auction has already been initialized.");
            _auctioneer = owner;
      }

      //Finish the auction and set the winner  
      void endauction(account_name owner)
      {
            require_auth(owner);
            _auctioneer = NULL;
            _done = true;
            getwinner();
      }

      //Who was the winner / who is the current winner?
      void getwinner()
      {
            eosio::print("The winning address: ", _winner, "\nHighest Bid: ", _hb1, "\nSecond Highest Bid: ", _hb2, "\n\n");
            if(_done == false)
                  eosio::print("The auction has not finished yet.\n");
            else
                  eosio::print("This auction has finished.\n");
      }

private:

      account_name _auctioneer; //Auctioneer Address (Makes sure only the Auction `initiater` can end the Auction)
      account_name _winner; //Winning Address
      uint64_t _hb2; //Highest Bid (2nd price)
      uint64_t _hb1; //Highest Bid (1st price)
      bool _done;

};

EOSIO_ABI(auction, (placebid)(initauction)(endauction)(getwinner))
