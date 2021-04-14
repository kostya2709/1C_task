#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>


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
};


class State {
    field cards;
};




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
    create_random_data();
    vector<Deck> cards(field_deck_num, all_cards);
    read(cards);
}