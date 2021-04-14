#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>


class Deck;

using std::vector;
using field = vector<Deck>;

const int empty = -1;
const int deck_num = 2;
const int suit_num = 4;
const int dif_values = 9;
const int field_deck_num = 9;
const int all_cards = deck_num * dif_values * suit_num;
const int cards_in_deck = all_cards / field_deck_num;

class Deck {
public:
    std::list<int> cards;
    int lowest_opened_idx = cards_in_deck - 1;
    int highest_opened_idx = cards_in_deck - 1;

    int lowest_opened_card = 0;
    int highest_opened_card = 0;


    Deck(int card_num) : cards(card_num, empty) {}
    

    void insert(Deck& deck, int count) {

        int deck_size = deck.cards.size();
        int size = cards.size();

        if (count > deck_size)
            throw "Error!";

        auto iter = deck.cards.end();
        
        for (int i = 0; i < count; ++i)
            --iter;

        cards.splice(cards.end(), deck.cards, iter, deck.cards.end());

        if (size == 0) {
            lowest_opened_idx = 0;
            lowest_opened_card = *cards.begin();
        }

        highest_opened_card = cards.back();
        highest_opened_idx += count;


        if (count == deck_size) {
            lowest_opened_idx = -1;
            highest_opened_idx = -1;
        }
        else {
            deck.highest_opened_card = deck.cards.back();
            deck.highest_opened_idx -= count;
        }
    }

    bool is_empty() {
        return lowest_opened_idx == -1;
    }

    bool operator==(const Deck& other) const {
        if (cards == other.cards &&
            lowest_opened_card == other.lowest_opened_card &&
            highest_opened_card == other.highest_opened_card &&
            lowest_opened_idx == other.lowest_opened_idx &&
            highest_opened_idx == other.highest_opened_idx)
            return true;
        return false;
    }
 
    bool is_completed() {
        for (auto i : cards) {
            if (lowest_opened_card != 0 or highest_opened_card != dif_values - 1)
                return false;
        }

        return true;
    }

    friend class State;
};



struct deck_hash {
    size_t operator()(const Deck& deck) {
        size_t result = 0;
        std::hash<int> hasher;
        for (auto card : deck.cards) {
            result ^= hasher(card);
        }
        result ^= hasher(deck.lowest_opened_card);
        result ^= hasher(deck.highest_opened_card);
        result ^= hasher(deck.lowest_opened_idx);
        result ^= hasher(deck.highest_opened_idx);
        return result;
    }
};

void read(field& cards);


class State {
public:
    State() : cards(field_deck_num, all_cards) {
        read(cards);
    }

private:
    field cards;

    State(const State& old_state) {
        cards = old_state.cards;
    }

    friend bool possible_win(State& cur_state, std::queue<State*>& states);
};

bool possible_win_caller() {

    std::queue<State*> states;
    State initial_state;

    bool result = 0;

    possible_win(initial_state, states);
    while (states.empty() == false) {
        State* cur_state = states.front();
        result = possible_win(*cur_state, states);
        if (result == true)
            return true;
        states.pop();
    }

    return result;

}

bool possible_win(State& cur_state, std::queue<State*>& states) {

    int counter = 0;
    for (auto& deck : cur_state.cards) {
        if (deck.is_completed() == false)
            break;
        else 
            ++counter;
    }
    if (counter == field_deck_num)
        return true;
        

    vector<int> highest_cards(field_deck_num);
    for (int i = 0; i < field_deck_num; ++i) {
        highest_cards[i] = cur_state.cards[i].highest_opened_card;
    }


    for (int i = 0; i < field_deck_num; ++i) {
        for (int deck_num = 0; deck_num < field_deck_num; ++deck_num) {

            if (i == deck_num)
                continue;

            if (cur_state.cards[deck_num].highest_opened_card >= cur_state.cards[i].lowest_opened_card && 
                cur_state.cards[deck_num].highest_opened_card <= cur_state.cards[i].highest_opened_card) {
                
                State* new_state = new State(cur_state);
                
                states.push(new_state);
                new_state->cards[deck_num].insert(new_state->cards[i],
                    new_state->cards[i].highest_opened_card - new_state->cards[deck_num].highest_opened_card - 1);
            }
        }
    }
}


void create_random_data() {
    vector<int> data(all_cards);
    const int suit_deck = suit_num * deck_num;

    for (int i = 0; i < dif_values; ++i) {
        for (int j = 0; j < suit_deck; ++j)
            data[i * suit_deck + j] = i;
    }

    std::random_shuffle(data.begin(), data.end());
    std::ofstream file("test", std::ios_base::out);

    for (auto i : data)
        file << i << " ";

    file.close();

}

inline bool are_valid(int less, int more) {
    if (less < more)
        return true;
    return false;
}

void read(field& cards) {

    std::fstream file("test");

    int value = 0;
    for (int i = 0; i < all_cards; ++i) {
        file >> value;
        if (value < 0 or value > dif_values - 1) {
            throw "Wrong input";
        }

        cards[i / 9].cards.emplace_back(value);

        if (i % 8 == 7) {
            cards[i / 9].highest_opened_card = value;
            cards[i / 9].lowest_opened_card = value;
        }

    }
    file.close();

}

int main() {
    std::cout << possible_win_caller();
}