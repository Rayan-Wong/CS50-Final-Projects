#include <iostream>
#include <cstdlib>
#include <limits>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

//array of possible suits
string suits[] = {"Club", "Diamond", "Heart", "Spade"};
//array of possible values
string cardValues[] = {"1","2","3","4","5","6","7","8","9","10","King", "Queen", "Jack", "A"};

//struct to hold card type
typedef struct
{
    string suit;
    string value;
}
cardType;

int startGame(int money);
int getUserInput(int money);
cardType giveCard(void);
int getValue(vector<cardType> vect);
bool duplicateCheck(string suit, string value);
void showHands(void);

//vectors to store cards in dealer and player's hands respectively
vector<cardType> dealer;
vector<cardType> player;
//drawnCards to track cards that are already drawn to prevent duplicate cards
vector<cardType> drawnCards;

int main()
{
    cout << "Welcome to Blackjack!" << endl
    << "If you win, you gain twice the amount betted." << endl
    << "If you and the dealer tie, you get back your betted amount." << endl
    << "If you fold, you lose half your betted amount." << endl
    << "And if you lose, you lose the entire betted amount." << endl;
    int money;
    string buffer;
    //read save file
    ifstream save ("save.txt");
    save >> buffer;
    //if no data in save file, create new balance
    if (buffer == "")
    {
        cout << "There is currently no record of your balance." << endl
        << "Creating one now with $1000" << endl;
        money = 1000;
    }
    //otherwise, retrieve it
    else
    {
        cout << "Records found. Retrieving them." << endl;
        money = stoi(buffer);
    }
    //loop to allow quick restart
    while (true)
    {
        cout << "Currently, you have $" << money << "." << endl;
        //call game function
        money = startGame(money);
        cout << "Would you like to play again? Yes (y) or no (n)?" << endl;
        //get user input
        char choice;
        bool input_ok = false;
        do
        {
            cin >> choice;
            if (cin.fail())
            {
                cout << "Invalid input entered. Please type y or n." << endl;
            }
            else if (choice == 'y' || choice == 'n')
            {
                //breaking do-while loop
                input_ok = true;
            }
            //who knows what the player typed
            else
            {
                cout << "Invalid input entered. Please type y or n." << endl;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        while (!input_ok);
        if (choice == 'y')
        {
            //restart
            continue;
        }
        else if (choice == 'n')
        {
            //exit loop and quit
            break;
        }
    }
    //write balance into save file
    ofstream tosave ("save.txt");
    tosave << money;
}

//main game function
int startGame(int money)
{
    //reset dealer, player and drawnCard's vectors
    dealer.clear();
    player.clear();
    drawnCards.clear();
    bool gameRunning = true;
    bool dealerBlackjack = false;
    bool playerBlackjack = false;
    cout << "How much are you betting?" << endl;
    int bet = getUserInput(money);
    int dealerValue = 0;
    do
    {
        dealer.push_back(giveCard());
        dealerValue = getValue(dealer);
        //check if dealer blackjack
        if (dealerValue == 21)
        {
            dealerBlackjack = true;
        }
    }
    //by the rules, dealer must hit until at least 17
    while (dealerValue < 17);
    //draw 2 cards for player
    player.push_back(giveCard());
    player.push_back(giveCard());
    //check if player blackjack
    int playerValue = getValue(player);
    if (playerValue == 21)
    {
        playerBlackjack = true;
    }
    //handle blackjack cases
    if (dealerBlackjack)
    {
        showHands();
        cout << "Dealer has Blackjack!" << endl;
        money = round(money - (bet * 1.5));
        cout << "You lost $" << round(bet * 1.5) << "." << endl;
        return money;
    }
    else if (playerBlackjack)
    {
        showHands();
        cout << "You have Blackjack!" << endl;
        money = round(money + (bet * 1.5));
        cout << "You won $" << round(bet * 1.5) << "." << endl;
        return money;
    }
    else if (playerBlackjack && dealerBlackjack)
    {
        showHands();
        cout << "Both parties have Blackjack!" << endl;
        return money;
    }
    //main game-loop
    while (gameRunning)
    {
        //show dealer's hand
        cout << "Dealer's hand:" << endl
        << dealer[0].suit << ", " << dealer[0].value << endl
        << "Number of dealer's cards: " << dealer.size() << endl;
        cout << "What will you do? Stand (s), hit (h) or fold (f)?" << endl;
        //show player's hand
        cout << "Your hand:" << endl;
        for (int i = 0; i < player.size(); i++)
        {
            cout << player[i].suit << ", " << player[i].value << endl;
        }
        char choice;
        //do-while loop for input validation for player decision
        bool input_ok = false;
        do
        {
            playerValue = getValue(player);
            cin >> choice;
            if (cin.fail())
            {
                cout << "Invalid input entered. Please type s, h or f." << endl;
            }
            else if (choice == 's' || choice == 'h' || choice == 'f')
            {
                //breaking do-while loop
                input_ok = true;
                //don't you dare try to get all 52 cards drawn, or the persson tries to be a unethical player
                if (drawnCards.size() == 52 || playerValue > 21)
                {
                    choice = 's';
                    break;
                }
            }
            //who knows what the player typed
            else
            {
                cout << "Invalid input entered. Please type s, h or f." << endl;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        while (!input_ok);
        //when player stands, end game and determine winner/loser
        if (choice == 's')
        {
            gameRunning = false;
            //if both bust
            if (playerValue > 21 && dealerValue > 21)
            {
                showHands();
                cout << "Both bust!" << endl;
            }
            //if dealer bust
            else if (dealerValue > 21)
            {
                showHands();
                cout << "Dealer Bust!" << endl;
                money = money + bet;
                cout << "You won $" << (bet) << "." << endl;
            }
            //if player bust
            else if (playerValue > 21)
            {
                showHands();
                cout << "You bust!" << endl;
                money = money - bet;
                cout << "You lost $" << (bet) << "." << endl;
            }
            //if player has higher value
            else if (playerValue > dealerValue && playerValue <= 21)
            {
                showHands();
                cout << "You Win!" << endl;
                money = money + bet;
                cout << "You won $" << (bet) << "." << endl;
            }
            //if dealer has higher value
            else if (playerValue < dealerValue && dealerValue <= 21)
            {
                showHands();
                cout << "You lose!" << endl;
                money = money - bet;
                cout << "You lost $" << (bet) << "." << endl;
            }
            //if both parties have same value
            else if (playerValue == dealerValue)
            {
                showHands();
                cout << "Tie!" << endl;
            }
        }
        //when player hits, draw card
        else if (choice =='h')
        {
            player.push_back(giveCard());
        }
        //when player folds, take half of betted amount and end game
        else if (choice == 'f')
        {
            gameRunning = false;
            cout << "You folded!" << endl;
            money = round(money - (bet * 0.5));
            cout << "You lost $" << round(bet * 0.5) << "." << endl;
        }
    }
    return money;
}

int getUserInput(int money)
{
    int bet;
    bool input_ok = false;
    // do-while loop for user validation
    do
    {
        cin >> bet;
        if (cin.fail())
        {
            cout << "Invalid input entered. Please type an integer." << endl;
        }
        else if (bet <= 0)
        {
            cout << "Please bet at least $1." << endl;
        }
        else if (bet > money)
        {
            cout << "Do not bet more than you have." << endl;
        }
        else
        {
            // breaking do-while loop
            input_ok = true;
        }
        // clearing cin failbit to allow future cin executions
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while (!input_ok);
    return bet;
}

cardType giveCard(void)
{
    cardType card;
    string suit;
    string value;
    //generate seed for actual random numbers
    srand((unsigned) time(NULL));
    //generate card suit and value while making sure it was not already drawn
    do
    {
        suit = suits[(rand() % 4)];
        value = cardValues[(rand() % 14)];
    }
    while (duplicateCheck(suit, value));
    card.suit = suit;
    card.value = value;
    //store selected card into drawnCards
    drawnCards.push_back(card);
    return card;
}

//iterate through drawnCards to check for cards already drawn
bool duplicateCheck(string suit, string value)
{
    for (int i = 0; i < drawnCards.size(); i++)
    {
        if (drawnCards[i].suit == suit && drawnCards[i].value == value)
        {
            return true;
        }
    }
    return false;
}

int getValue(vector<cardType> vect)
{
    int value = 0;
    for (int i = 0; i < vect.size(); i++)
    {
        //assign 10 to King, Queen and Jack
        if (vect[i].value == "King" || vect[i].value == "Queen" || vect[i].value == "Jack")
        {
            value += 10;
        }
        //assign either 1 or 11 to A, depending on value of other card(s)
        else if (vect[i].value == "A")
        {
            //if A can cause one to bust, assign 1
            if ((value + 11) > 21)
            {
                value += 1;
            }
            //otherwise, assign 11
            else
            {
                value += 11;
            }
        }
        //convert int in str to int
        else
        {
            value += stoi(vect[i].value);
        }
    }
    return value;
}

void showHands(void)
{
    //show both parties' hands
    cout << "Dealer's hand:" << endl;
    for (int i = 0; i < dealer.size(); i++)
    {
        cout << dealer[i].suit << ", " << dealer[i].value << endl;
    }
    cout << "Your hand:" << endl;
    for (int j = 0; j < player.size(); j++)
    {
        cout << player[j].suit << ", " << player[j].value << endl;
    }
}