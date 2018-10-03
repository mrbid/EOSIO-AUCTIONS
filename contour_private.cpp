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
      auction(account_name s): contract(s), _mem(s, s)
      {
            //Init only memory block required, make contract owner pay for the ram
            _mem.emplace(_self, [&](auto& r)
            {
                  r.auctioneer = _self;
                  r.winner = NULL;
                  r.hb1 = 0;
                  r.hb2 = 0;
                  r.done = false;
            });
      }

      //Places a bid
      void placebid(account_name owner, int64_t bid)
      {
            require_auth(owner);

            //Get memory
            auto r = _mem.begin();

            //Is this bid high enough? 
            if(r->hb1 > bid)
            {
                  eosio::print("Your bid is too low, it's outbid.\n\nHighest Bid: ", r->hb1, "\nSecond Highest Bid: ", r->hb2, "\n");
                  return;
            }

            //Write mem
            _mem.modify(r, _self, [&](auto& w)
            {
                  //Set current winner (highest bidder)
                  w.winner = owner;

                  //Set new highest bid (and last highest)
                  w.hb2 = r->hb1;
                  w.hb1 = bid;
            });
      }

      //Finish the auction and set the winner  
      void endauction(account_name owner)
      {
            require_auth(owner);
            auto r = _mem.begin();
            _mem.modify(r, _self, [&](auto& w)
            {
                  w.auctioneer = NULL;
                  w.done = true;
            });
            getwinner(owner);
      }

      //Who was the winner / who is the current winner?
      void getwinner(account_name owner)
      {
            require_auth(owner);
            auto r = _mem.begin();
            if(r->auctioneer == owner)
            {
                  eosio::print("The winning address: ", r->winner, "\nHighest Bid: ", r->hb1, "\nSecond Highest Bid: ", r->hb2, "\n\n");
                  if(r->done == false)
                        eosio::print("The auction has not finished yet.\n");
                  else
                        eosio::print("This auction has finished.\n");
            }
            else
            {
                  eosio::print("You need to be the Auctioneer to see the winner.\n");
            }
      }

private:

      struct record
      {
            account_name auctioneer; //Auctioneer Address (Makes sure only the Auction `initiater` can end the Auction)
            account_name winner; //Winning Address
            uint64_t hb2; //Highest Bid (2nd price)
            uint64_t hb1; //Highest Bid (1st price)
            bool done;

            uint64_t primary_key() const{return winner;}
      };

      typedef eosio::multi_index<N(records), record> mem_table;
      mem_table _mem;

};

EOSIO_ABI(auction, (placebid)(initauction)(endauction)(getwinner))
