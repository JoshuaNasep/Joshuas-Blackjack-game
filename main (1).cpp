#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

enum card_suit
{
  A, H, S, P
};

int MIN_BET = 25;

struct username
{
  string username;
  int money;
};

struct card
{
  int value;
  card_suit suit;
  bool up;
};

struct player
{
  username info;
  vector<card> hand;
  int bet;
  int insurance_bet;
};

card deal();
card hitMe();
int score(vector<card>);
bool hasAce(vector<card>);
void printCards(vector<card>);
char printSuit(card);
void doubleDown(player, player&);
int winner(player, player&);
void payout(player, player&);
void split(player&, player&);
void bet(player&);
void insurance(vector<player>&);
username load();
username create();
void play(vector<player>&);
void clear(vector<card>&);
void dealer_play(player&);
void displayMenu();

int main(){
  unsigned int time_seed = time(0);
  srand(time_seed);
  player user;
  player dealer;
  vector<player> players;
  char input;

  dealer.info.username = "Dealer";
  players.push_back(dealer);

  int menu;

  do
  {
    displayMenu();
    cin >> menu;

    if(menu == 1)
    {
      cout << "How many players(1 or 2 or 3 or 4 or 5)" << endl;
      int numPlayers;
      cin >> numPlayers;
      for(int i = 0;i< numPlayers;i++)
      {
        user.info = create();
        players.push_back(user);
      }
    }
    else
    {
      cout << "Please enter 1 or 2 or 3 or 4 or 5" << endl;
      menu = 0;
      cin.ignore();
    }
  }while(menu != 1);
  bool cond = false;
  int size = players.size();
  do{
    for(int i =1;i<players.size();i++)
    {
      if(players[i].info.money < 25)
      { 
      cout << players[i].info.username << " is currently out of money. Would you like to add some, yes(1) or no(2)?" << endl;
      cin >> input;
      if(input == '1')
      {
      cout << "How much would you like to add?" << endl; //how much
      cin >> players[i].info.money;
        }
        else
        {
          cout << players[i].info.username << " you are leaving with $" << players[i].info.money << endl;
          cout << "Thank ypu for playing." << endl;
          players.erase(players.begin() + (i));
          i--;
        }
      }

      if(cond && players.size() <=5)
      {
        cout << players[i].info.username << " Would you like to keep playing press (1) or press (2) to leave game" << endl;
        cin >> input;
        if(input == '2')
        {
          cout << players[i].info.username << " you are leaving with $" << players[i].info.money << endl;
          cout << "Thank ypu for playing." << endl;
          players.erase(players.begin() + (i));
          i--;
        }
      }
    }
    play(players);
    cond = true;
  }
  while(players.size() <=5);
  return 0;
}

void displayMenu(){
  cout << "Welcome to Joshuas' game of Blackjack." << endl;
  cout << "Please enter 1 to start playing Blackjack." << endl; 
  cout << "1) play Blackjack" << endl;
}

void dealer_play(player &dealer)
{
  if((score(dealer.hand) < 0.17) || (score(dealer.hand) == 0.17 && hasAce(dealer.hand)))
  {
    dealer.hand.push_back(hitMe());
  } 
}

void play(vector<player> &players)
{
  char input;

  for(int i =1;i<players.size();i++)
  {
    bet(players[i]);
    cout << "Money: " << players[i].info.money << "Bet: " << players[1].bet << endl;
  }
  
  for(int i = 0;i< (players.size()*2);i++)
  { 
    players[(i%players.size())].hand.push_back(deal());
    if((i%players.size()) == 0 && (i%2) == 0)
    {
      players[(i%players.size())].hand[(i%2)].up = false; 
    }
  }
  
  for(int i=1;i<players.size();i++)
  {
    cout << players[i].info.username << " has: " << score(players[i].hand) << endl;
  }

  for(int i =0;i<players.size();i++)
  {
    cout << players[i].info.username << " Cards:" << endl;
    printCards(players[i].hand);
  }
  
  bool cont = true;
  for(int i = 1;i<players.size();i++)
  {
    do{
      if(players[0].hand[1].value == 1 && cont)
      {
        insurance(players);
        if(score(players[0].hand) == 21)
        {
          players[0].hand[0].up = true;
      
          printCards(players[0].hand);
          
          for(int i =1;i<players.size();i++)
          {
            payout(players[0],players[i]);
          }
          input = 's';
        }
        cont = false;
      }
      if(players[0].hand[1].value >= 10 && cont)
      {
        if(score(players[0].hand) == 21)
        {
          players[0].hand[0].up = true;

          printCards(players[0].hand);
          
          for(int i =1;i<players.size();i++)
          {
            payout(players[0],players[i]);
          }
          input = 's';
        }     
        cont = false;
      }
      
      if(score(players[0].hand) <= 21)
      {
        if(((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || players[i].hand[0].value == players[i].hand[1].value) && players[i].hand.size() == 2  && score(players[i].hand) == 10)
        { 
          cout << players[i].info.username << " score: " << score(players[i].hand) << endl;
          cout << "Would you like to Doublestand(Ds), split(sp),take a hit(H), or stand(S)" << endl;
        }
          else if(((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || (players[i].hand[0].value == players[i].hand[1].value)) && players[i].hand.size() == 2)
          {
            cout << players[i].info.username << " score: " << score(players[i].hand) << endl;
            cout << "Would you like to split(sp) your cards, take a hit(H), or stand(S)" << endl;
          }
          else if(players[i].hand.size() == 2 && score(players[i].hand) >= 9 && score(players[i].hand) <= 11 && !(hasAce(players[i].hand)))
          {
            cout << players[i].info.username << " score: " << score(players[i].hand) << endl;
            cout << "Would you like to Doublestand(Ds), take a hit(H), or stand(S)" << endl;
          }
          else
          {
            cout << players[i].info.username << " score: " << score(players[i].hand) << endl;
            cout << "Hit(H) or stand(S)";
          }
          cin >> input;
          switch(input)
          {
          case 'S':
            split(players[0], players[i]);
            printCards(players[i].hand);
            break; 
          case 'H':
            doublestand(players[0], players[i]);
            input = 's';
            break;
          case 'P':
            players[i].hand.push_back(hitMe());
            printCards(players[i].hand);
            cout << players[i].info.username << " score is now " << score(players[i].hand) << endl;
            break;
          default:
            input = 's';
          }
          if(score(players[i].hand) > 21)
          {
            input = 's'; 
          }
      }
    }while(!(input == 'S' || input == 's'));
  }

  dealer_play(players[0]);

  players[0].hand[0].up = true;
  
  
  for(int i =0;i<players.size();i++)
  {
    cout << players[i].info.username << " score: " << score(players[i].hand) << " Cards: ";
    printCards(players[i].hand);
  }

 
  for(int i =1;i<players.size();i++)
  {
    if(score(players[i].hand) > 21)
    {
      cout << "You busted! ";
    }
    int win = winner(players[0], players[i]);
    payout(players[0],players[i]);
    clear(players[i].hand);
  } 

  clear(players[0].hand);
}

void clear(vector<card> &hand)
{
  hand.clear();
}

username create()
{
  username user;
  cout << "What is the username you'd like to use?" << endl;
  fstream input;
  string filename;

  do
  {
    cin >> user.username;
    filename = user.username + ".dat";
    input.open(filename);
    if(!input.fail())
    {
      cout << "This username is already taken. Please choose another one." << endl;
    }
  }while(!input.fail());

  cout << "Please enter how much money you'd like to deposit" << endl; 
  cin >> user.money;

  input.close();
  return user;
}

 username load()
 {
  string filename;
  struct username user;
  ifstream input;
  getline(input, user.username);
  input >> user.money;
  input.close();
  return user; 
}

void insurance(vector<player> &players)
{
  int size = players.size();
  
  for(int i=1;i<size;i++)
  {
    int bet; 
    players[i].insurance_bet = 0;
    if((players[i].info.money-(players[i].bet/2)) >=0)
    { 
      do
      {
        cin.ignore();
        cout << "How much would you like to bet for insurance, up to " << (players[i].bet/2) << players[i].info.username << "?(0 if you don't want to take out insurance)" << endl;
        cin >> bet;
      }while(!(bet <= (players[i].bet/2) && bet >=0)); 
      if(bet != 0)
      {
        players[i].insurance_bet = bet;
        players[i].info.money-=bet;
      }
    }
  }

  if(score(players[0].hand) == 21)
  {
    for(int i = 1; i<size;i++)
    {
      if(players[i].insurance_bet != 0)
      {
        players[i].info.money+=(players[i].insurance_bet*2);
        players[i].insurance_bet = 0;
      }
    }
  }
  else
  {
    for(int i = 1; i<size;i++)
    {
      if(players[i].insurance_bet != 0)
      {
        players[i].insurance_bet = 0;
      }
    }
  } 
}

void bet(player &user)
{
  int bet;
  do
  {
    cout << user.info.username << endl;
    cout << "How much would you like to bet? (Must be greater than the " << MIN_BET <<  " and less than " << user.info.money << ")" << endl;
    cin >> bet;
  }while(!(bet >= MIN_BET && bet <= user.info.money));
  user.info.money -= bet;
  user.bet = bet;
}

void split(player &dealer, player &user)
{
  player split;
  vector<player> players;
  players.push_back(dealer);
  split.bet = user.bet;
  user.info.money-= user.bet;
  split.hand.push_back(user.hand[0]);
  split.hand.push_back(deal());
  user.hand[0] = deal();
  split.info.username = user.info.username;
  players.push_back(split);
  printCards(players[1].hand);
  char input;
  do
  {
    for(int i =1;i<players.size();i++)
    {
      if(score(players[i].hand) > 21)
      {
        input = 's';
      }
      else
      {
        cout << "Hit(H) or Stand(S):";
        cin >> input;
      }
      
      if(input == 'H' || input == 'h')
      {
        players[i].hand.push_back(hitMe());
        printCards(players[i].hand);
        cout << players[i].info.username << " score is now " << score(players[i].hand) << endl;
      }
    }
  }while(!(input == 'S' || input == 's'));
  dealer_play(players[0]);
  for(int i =1;i<players.size();i++)
  {
    cout << players[i].info.username << " score: " << score(players[i].hand) << " Cards:" << endl;
    printCards(players[i].hand);
  }

  for(int i =1;i<players.size();i++)
  {
    if(score(players[i].hand) > 21)
    {
      cout << "You busted!";
    }
    payout(players[0],players[i]);
  } 
}

void payout(player dealer, player &user)
{
  if(winner(dealer, user) == 1)
  {
    if(score(user.hand) == 21 && hasAce(user.hand) && user.hand.size() == 2)
    {
      user.info.money += ((user.bet*3)/2);
      user.bet = 0;
      cout << user.info.username << " won!" << endl;
    }
    else
    {
      user.info.money+= (user.bet*2);
      user.bet = 0;
      cout << user.info.username << " won!" << endl;
    }
  }
  else if (winner(dealer, user) == 0)
  {
    user.info.money+= user.bet;
    user.bet = 0;
    cout << user.info.username << " tied!" << endl;
  }
  else
  {
    user.bet = 0;
    cout <<user.info.username << " lost!" << endl;
  }
}

int winner(player dealer, player &user)
{
  if(score(dealer.hand) == score(user.hand))
  {
    return 0;
  }
  else if(((score(dealer.hand) < score(user.hand)) && (score(user.hand) <= 21)) || (score(dealer.hand) > 21 && score(user.hand) <= 21))
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

void doubleDown(player dealer, player &user)
{
  int bet;
  do
  {
    cout << "How much would you like to bet? (Must be greater than the " << MIN_BET << " and less than "<< user.info.money <<")" << endl;
    cin >> bet;
  }while(!(bet > MIN_BET && bet <= user.bet && bet <= user.info.money));
  user.bet+=bet;
  user.info.money-=bet;
  user.hand.push_back(deal());
  payout(dealer, user);
}

char printSuit(card new_card)
{
  switch(new_card.suit)
  { 
    case 0:
      return 'A';
    case 1:
      return 'H';
    case 2:
      return 'S';
    case 3:
      return 'P';
  }
}

void printCards(vector<card> hand)
{
  const string CARD_VALUES[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "X"};
  for(int i=0;i<hand.size();i++)
  {
    if(hand[i].up)
    {
      cout << CARD_VALUES[(hand[i].value-1)] << printSuit(hand[i]) << " "; 
    }
    else
    {
      cout << CARD_VALUES[13] << CARD_VALUES[13] << " ";
    }
  }
  cout << endl;
}

bool hasAce(vector<card> hand)
{
  bool has_ace = false;
  char input;
  for(int i =0;i<hand.size();i++)
  {
    if (hand[i].value == 1)
    { 
      if(input == '1')
      has_ace = true;
    }
    else if(hand[i].value == 11)
    {
      if(input == '2')
      has_ace = true;
    }
  }
  return has_ace;
}

int score(vector<card> hand)
{
  int total = 0;
  for(int i = 0; i<hand.size();i++)
  {
    if(hand[i].value >= 10)
    {
      total+=10;
    }
    else
    {
      total += hand[i].value;
    }
  }

  if(hasAce(hand) && total <= 11)
  {
    total+=10;
  }
  return total;
}

card hitMe(){
  return deal();
}

card deal()
{
  card new_card;

  new_card.value = 1 + rand() % 13;
  int suit = rand() % 4;
  switch(suit)
  {
    case 0:
      new_card.suit = A;
      break;
    case 1:
      new_card.suit = H;
      break;
    case 2:
      new_card.suit = S;
      break;
    case 3:
      new_card.suit = P;
      break;
  }
  new_card.up = true;

  return new_card;
}
