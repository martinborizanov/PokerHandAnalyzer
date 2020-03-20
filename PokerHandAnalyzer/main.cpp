//
//  main.cpp
//  PokerHandAnalyzer
//
//  Created by Martin Borizanov on 18/03/2020.
//  Copyright © 2020 Martin Borizanov. All rights reserved.
//

#include <iostream>
#include "Suit.hpp"
#include "Ordinal.hpp"
#include <vector>
#include <unordered_map>

enum PokerHand
{
    ROYAL_FLUSH, //
    STRAIGHT_FLUSH, //
    FOUR_OF_A_KIND, //
    FULL_HOUSE, //
    FLUSH,
    STRAIGHT,
    THREE_OF_A_KIND, //
    TWO_PAIR, //
    ONE_PAIR, //
    HIGH_CARD //
};

class Card
{
    
private:
    Suit suit;
    Ordinal ordinal;
        
public:
    Card() = default;
    Card( Suit given_suit, Ordinal given_ordinal ) : suit(given_suit ), ordinal( given_ordinal ) {};
    Suit getSuit() { return suit; };
    Ordinal getOrdinal() { return ordinal; };
    bool operator < ( const Card& another_card ) const { return ( ordinal < another_card.ordinal ); };

};

class Hand{
    
private:
    std::vector<Card> cards;
    PokerHand poker_hand;
    std::unordered_map<Ordinal, int> ordinal_occurrences;
    Card highest_card ;
    bool same_suit;
    bool sequence;
    
    void analyzePokerHand()
    {
        
        // Royal Flush, Straight Flush and Flush
        if( same_suit )
        {
            if( sequence ) poker_hand = (highest_card.getOrdinal() == ACE ) ? ROYAL_FLUSH : STRAIGHT_FLUSH;
            else poker_hand = FLUSH;
        }
        else
        {
            // Straight
            if( sequence ) poker_hand = STRAIGHT;
            else
            {
                switch (ordinal_occurrences.size() )
                {
                    case 2:
                        
                        poker_hand = ( ordinal_occurrences.begin()->second == 4 || ordinal_occurrences.begin()->second == 1 ) ? FOUR_OF_A_KIND : FULL_HOUSE;
                        break;
                        
                    case 3:
                        
                        poker_hand = TWO_PAIR;
                        for(std::unordered_map<Ordinal, int>::iterator it = ordinal_occurrences.begin(); it != ordinal_occurrences.end(); ++it )
                        {
                            if( it->second == 3 ) poker_hand = THREE_OF_A_KIND;
                        }
                        break;
                        
                    case 4:
                        
                        poker_hand = ONE_PAIR;
                        break;
                        
                    case 5:
                        
                        poker_hand = HIGH_CARD;
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    
public:
    Hand( std::vector<Card> givenCards ) : cards( givenCards )
    {
        same_suit = true;
        sequence = true;
        
        std::sort( cards.begin(), cards.end() );
        highest_card = cards.back();
              
        for( std::vector<Card>::iterator it = cards.begin(); it != cards.end(); ++it )
        {
            
            if( it != cards.begin())
            {
                if( it->getSuit() != ( it-1 )->getSuit() ) same_suit = false;
                if( it->getOrdinal() != ( it-1 )->getOrdinal()+1  ) sequence = false;
            }
            
            if( ordinal_occurrences.find( it->getOrdinal() ) == ordinal_occurrences.end() )
            {
                ordinal_occurrences.insert( std::make_pair( it->getOrdinal(), 1 ) );
            }
            else
            {
                ordinal_occurrences[it->getOrdinal()]++;
            }
        }
        analyzePokerHand();
    }
    
    PokerHand getPokerHand() { return poker_hand; };
};




static void runTest( const std::vector<Card> &given_cards, PokerHand expected)
{
    Hand given_hand = Hand( given_cards );
    std::string result = given_hand.getPokerHand() == expected ? "Passed" : "Failed";
    std::cout<< "Test Result: " << result << std::endl;
}

int main(int argc, const char * argv[])
{
    /// Royal Flush
    std::vector<Card> royal_flush_cards = { Card( HEARTS, ACE ), Card( HEARTS, KING ), Card( HEARTS, QUEEN ), Card( HEARTS, JACK ), Card( HEARTS, TEN ), };
    runTest( royal_flush_cards, ROYAL_FLUSH);
    
    /// Straight Flush
    std::vector<Card> straight_flush_cards = { Card( HEARTS, NINE ), Card( HEARTS, KING ), Card( HEARTS, QUEEN ), Card( HEARTS, JACK ), Card( HEARTS, TEN ), };
    runTest( straight_flush_cards, STRAIGHT_FLUSH);
    
    /// Four of a kind
    std::vector<Card> four_of_a_kind_cards = { Card( HEARTS, TWO ), Card( SPADES, TWO ), Card( CLUBS, TWO ), Card( DIAMONDS, TWO ), Card( HEARTS, SEVEN ), };
    runTest( four_of_a_kind_cards, FOUR_OF_A_KIND);

    // Full House
    std::vector<Card> full_house_cards = { Card( HEARTS, TWO ), Card( SPADES, TWO ), Card( CLUBS, TWO ), Card( DIAMONDS, THREE ), Card( HEARTS, THREE ), };
    runTest( full_house_cards, FULL_HOUSE);
    
    /// Flush
    std::vector<Card> flush_cards = { Card( HEARTS, TWO ), Card( HEARTS, NINE ), Card( HEARTS, SIX ), Card( HEARTS, FOUR ), Card( HEARTS, SEVEN ), };
    runTest( flush_cards, FLUSH );
    
    /// Straight
    std::vector<Card> straight_cards = { Card( HEARTS, NINE ), Card( CLUBS, KING ), Card( HEARTS, QUEEN ), Card( HEARTS, JACK ), Card( HEARTS, TEN ), };
    runTest( straight_cards, STRAIGHT);
    
    // Three of a kind
    std::vector<Card> three_of_a_kind_cards = { Card( HEARTS, TWO ), Card( SPADES, TWO ), Card( CLUBS, TWO ), Card( DIAMONDS, THREE ), Card( HEARTS, SEVEN ), };
    runTest( three_of_a_kind_cards, THREE_OF_A_KIND);
    
    /// Two Pair
    std::vector<Card> two_pair_cards = { Card( HEARTS, TWO ), Card( SPADES, TWO ), Card( CLUBS, SIX ), Card( DIAMONDS, SIX ), Card( HEARTS, SEVEN ), };
    runTest( two_pair_cards, TWO_PAIR);

    /// One Pair
    std::vector<Card> one_pair_cards = { Card( HEARTS, TWO ), Card( SPADES, TWO ), Card( CLUBS, SIX ), Card( DIAMONDS, FOUR ), Card( HEARTS, SEVEN ), };
    runTest( one_pair_cards, ONE_PAIR);
    
    /// High Card
    std::vector<Card> high_card_cards = { Card( HEARTS, TWO ), Card( SPADES, NINE ), Card( CLUBS, SIX ), Card( DIAMONDS, FOUR ), Card( HEARTS, SEVEN ), };
    runTest( high_card_cards, HIGH_CARD);
    
    return 0;
}
