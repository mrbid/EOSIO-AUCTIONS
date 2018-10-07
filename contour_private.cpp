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
      auction(account_name s): contract(s), _mem(s, 0){}
      
      //Inits Auction
      void initauction(account_name owner)
      {
            require_auth(owner);

            //Init only memory block required, make contract owner pay for the ram
            _mem.emplace(owner, [&](auto& r)
            {
                  r.auctioneer = owner;
                  r.winner = NULL;
                  r.hb1 = 0;
                  r.hb2 = 0;
                  r.done = false;
            });
      }

      //Resets the auction
      void reset(account_name owner)
      {
            require_auth(owner);
            auto r = _mem.cbegin();
            if(r->auctioneer == owner)
            {
                  _mem.modify(r, _self, [&](auto& w)
                  {
                        w.winner = NULL;
                        w.hb1 = 0;
                        w.hb2 = 0;
                        w.done = false;
                  });  
            }
      }

      //Places a bid
      void placebid(account_name owner, int64_t bid)
      {
            require_auth(owner);

            //Get memory
            auto r = _mem.cbegin();
            
            //Is the auction done?
            if(r->done == true)
            {
                  eosio::print("Sorry this auction has finished.");
                  return;
            }

            //Is this bid high enough? 
            if(r->hb1 >= bid)
            {
                  eosio::print("Your bid is too low, it's outbid. : Highest Bid: ", r->hb1, " : Second Highest Bid: ", r->hb2);
                  return;
            }

            //Write mem
            _mem.modify(r, r->auctioneer, [&](auto& w)
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
            auto r = _mem.cbegin();
            _mem.modify(r, _self, [&](auto& w)
            {
                  w.done = true;
            });
            getwinner(owner);
      }

      //Who was the winner / who is the current winner?
      void getwinner(account_name owner)
      {
            require_auth(owner);
            auto r = _mem.cbegin();
            if(r->auctioneer == owner)
            {
                  eosio::print("The winning address: ", r->winner, " : Highest Bid: ", r->hb1, " : Second Highest Bid: ", r->hb2);
                  if(r->done == false)
                        eosio::print(" : The auction has not finished yet.");
                  else
                        eosio::print(" : This auction has finished.");
            }
            else
            {
                  eosio::print("You need to be the Auctioneer to see the winner.");
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

            uint64_t primary_key() const{return auctioneer;}
      };

      typedef eosio::multi_index<N(records), record> mem_table;
      mem_table _mem;

};

EOSIO_ABI(auction, (placebid)(initauction)(endauction)(getwinner)(reset))
