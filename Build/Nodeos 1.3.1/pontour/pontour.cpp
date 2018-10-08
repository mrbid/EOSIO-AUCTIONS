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
      auction(account_name s): contract(s), _bids(s, 0) {}

      //Get 1st and 2nd bids
      void sync()
      {
            _hb1 = 0;
            _hb2 = 0;
            _winner = 0;

            //Get highest
            for(auto it = _bids.begin(); it != _bids.end(); ++it)
            {
                  if(it->bid > _hb1)
                  {
                        _hb1 = it->bid;
                        _winner = it->owner;
                  }
            }

            //Get second highest
            for(auto it = _bids.begin(); it != _bids.end(); ++it)
            {
                  if(it->bid > _hb2 && it->bid != _hb1)
                  {
                        _hb2 = it->bid;
                        _winner = it->owner;
                  }
            }
      }

      //Places a bid
      __attribute__((eosio_action))
      void placebid(account_name owner, int64_t bid)
      {
            require_auth(owner);
            
            //Sync local variables from persistent storage
            sync();

            //Is this bid high enough? 
            if(_hb1 >= bid)
            {
                  eosio::print("Your bid is too low, it's outbid. : Highest Bid: ", _hb1, " : Second Highest Bid: ", _hb2);
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
      __attribute__((eosio_action))
      void getwinner(account_name owner)
      {
            require_auth(owner);
            sync();
            eosio::print("The winning address: ", _winner, " : Highest Bid: ", _hb1, " : Second Highest Bid: ", _hb2);
      }
      
      //Dump memory (all bids and addresses)
      __attribute__((eosio_action))
      void getbids(account_name owner)
      {
            require_auth(owner);
            for(auto it = _bids.begin(); it != _bids.end(); ++it)
                  eosio::print("Address: ", it->owner, " - Bid:", it->bid, " : ");
      }

private:

      uint64_t _hb2;
      uint64_t _hb1;
      account_name _winner;
      
      struct record
      {
            account_name owner;
            int64_t bid;
            uint64_t primary_key() const{return bid;}
      };

      typedef eosio::multi_index<N(records), record> bids_table;
      bids_table _bids;

};

EOSIO_ABI(auction, (placebid)(getwinner)(getbids))
