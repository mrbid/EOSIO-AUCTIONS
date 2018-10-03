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
      auction(account_name s): contract(s), _bids(s, s) {}

      //Get 1st and 2nd bids
      void ghbs()
      {
            _hb1 = 0;
            _hb2 = 0;
            _winner = 0;
            for(auto it = _bids.begin(); it != _bids.end(); ++it)
            {
                  if(it->bid > _hb1)
                  {
                        _hb2 = _hb1;
                        _hb1 = it->bid;
                        _winner = it->owner;
                  }
            }
      }

      //Places a bid
      void placebid(account_name owner, int64_t bid)
      {
            require_auth(owner);
            
            //
            ghbs();

            //Is this bid high enough? 
            if(_hb1 > bid)
            {
                  eosio::print("Your bid is too low, it's outbid.\n\nHighest Bid: ", _hb1, "\nSecond Highest Bid: ", _hb2, "\n");
                  return;
            }

            //Set new highest bid (and last highest)
            _hb2 = _hb1;
            _hb1 = bid;

            //If record exists delete it
            for(auto it = _bids.begin(); it != _bids.end();)
            {
                  if(it->owner == owner)
                  {   
                        _bids.erase(it);
                        break;
                  }
                  else
                  {
                        ++it;
                  }       
            }    

            //Create new bid record
            _bids.emplace(owner, [&](auto& rcrd)
            {
                  rcrd.owner = owner;
                  rcrd.bid = bid;
            });
      }

      //Who was the winner / who is the current winner?
      void getwinner(account_name owner)
      {
            require_auth(owner);
            ghbs();
            eosio::print("The winning address: ", _winner, "\nHighest Bid: ", _hb1, "\nSecond Highest Bid: ", _hb2, "\n\n");
      }
      
      //Dump memory (all bids and addresses)
      void getbids(account_name owner)
      {
            require_auth(owner);
            for(auto it = _bids.begin(); it != _bids.end(); ++it)
                  eosio::print("Address: ", it->owner, " - Bid:", it->bid, "\n");
      }

private:

      uint64_t _hb2;
      uint64_t _hb1;
      account_name _winner;
      
      struct record
      {
            account_name owner;
            int64_t bid;
            uint64_t primary_key() const{return owner;}
      };

      typedef eosio::multi_index<N(records), record> bids_table;
      bids_table _bids;

};

EOSIO_ABI(auction, (placebid)(getwinner)(getbids))
