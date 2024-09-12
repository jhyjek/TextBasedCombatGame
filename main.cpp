#include <iostream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class Room {
public:
    string name;
    string description;
    bool hasTreasure;
    Room* connectedRooms[5];

    Room(string n, string desc, bool treasure) {
        name = n;
        description = desc;
        hasTreasure = treasure;
        for (int i = 0; i < 5; ++i) {
            connectedRooms[i] = nullptr;
        }
    }
};

struct Character {
    string name;
    int health;
};

struct Monster {
    string name;
    int health;
    int currentRoomIndex;
    bool isActive;
    Monster(string n, int h) : name(n), health(h), isActive(true) {}
};

//A class for the game
class Game {
private:
    //Creates the rooms
    Room outdoor;
    Room livingRoom;
    Room poolRoom;
    Room bathroom;
    Room staircase;
    Room masterBedroom;
    Room upstairsHallway;
    Room masterBathroom;

    Room* currentRoom;

    //Creates the treasures
    bool treasure1;
    bool treasure2;
    bool treasure3;
    bool allTreasuresCollected;

    //Creates the player and monster
    Character player;

    Monster rook;
    Monster building;
    Monster baldEagle;

    void setPlayerName() {
        cout << "Enter your name: ";
        cin >> player.name;
        cout << "Welcome, " << player.name << "!\n";
    }

public:
    //creats the fuctions used in the game
    Game();
    void startGame();
    void exploreRoom();
    void performCombat(Character& player, Monster& enemy);
    void encounterMonster(Monster& enemy);
    void endGame(bool win);
    int rollDie();
    void StairCaseMap();
    void FrontOfTheHouseMap();
    void LivingRoomMap();
    void PoolRoomMap();
    void MasterBathroomMap();
    void BathroomMap();
    void MasterBedroomMap();
    void UpstairsHallwayMap();
};

Game::Game() :
            //Room name and descrption
            outdoor("Outdoor", "There are 2 different doors that are barricaded, but you can rip them down to go through.", false),
            livingRoom("LivingRoom", "A large room with a TV and a staircase that leads to the upstairs hallway.", true),
            poolRoom("PoolRoom", "A large, open room with a high ceiling with pool tables and a lot of empty beer cans.", true),
            bathroom("Bathroom", "There is a sink and toilet, and there are also pliers.", true),
            staircase("Staircase", "A staircase that only leads up, but there is also a bottle of beer that is full (Hostage may need it).", true),
            masterBedroom("MasterBedroom", "There is an awesome Californian king heated Tempurpedic bed at the top center of the room with end tables on either side, and there is a hostage at the center of the room.", false),
            upstairsHallway("UpstairsHallway", "A hallway that leads to the master bedroom, down to the living room, and the staircase", false),
            masterBathroom("MasterBathroom", "A big bathroom with a drippy toilet, shower, and sink, but there are also scissors on the sink", true),
            currentRoom(&outdoor),
            treasure1(true),
            treasure2(true),
            treasure3(true),
            allTreasuresCollected(false),
            player{"PlayerName", 10},
            rook("Rook", 3),
            building("Building", 3),
            baldEagle("BaldEagle", 3) {
            srand(time(0));

    // Connect the rooms according to the specified connections
    outdoor.connectedRooms[0] = &livingRoom;
    outdoor.connectedRooms[1] = &poolRoom;

    livingRoom.connectedRooms[0] = &outdoor;
    livingRoom.connectedRooms[1] = &poolRoom;
    livingRoom.connectedRooms[2] = &upstairsHallway;
    livingRoom.connectedRooms[3] = &bathroom;

    poolRoom.connectedRooms[0] = &livingRoom;
    poolRoom.connectedRooms[1] = &outdoor;
    poolRoom.connectedRooms[2] = &staircase;

    bathroom.connectedRooms[0] = &livingRoom;

    staircase.connectedRooms[0] = &poolRoom;
    staircase.connectedRooms[1] = &upstairsHallway;

    masterBedroom.connectedRooms[0] = &upstairsHallway;
    masterBedroom.connectedRooms[1] = &masterBathroom;

    upstairsHallway.connectedRooms[0] = &masterBedroom;
    upstairsHallway.connectedRooms[1] = &livingRoom;
    upstairsHallway.connectedRooms[2] = &staircase;

    masterBathroom.connectedRooms[0] = &masterBedroom;

    rook.currentRoomIndex = rand() % 7;
    building.currentRoomIndex = rand() % 7;
    baldEagle.currentRoomIndex = rand() % 7;
}

void Game::exploreRoom() {
    cout << "You are in " << currentRoom->name << endl;
    cout << currentRoom->description << endl;

    // Check if the room has a treasure and if the player wants to pick it up
    if (currentRoom->hasTreasure) {
        cout << "There is a treasure in this room. Do you want to pick it up? (y/n)" << endl;
        char answer;
        cin >> answer;
        if (answer == 'y') {
            currentRoom->hasTreasure = false;
            // These are the 3 main treasure
            cout << "You picked up the treasure!" << endl;
            if (currentRoom == &bathroom) {
                treasure1 = false;
            } else if (currentRoom == &staircase) {
                treasure2 = false;
            } else if (currentRoom == &masterBathroom) {
                treasure3 = false;
            }
        }
    }

    // Checks to see if the player is in the master bedroom and that they have collected the 3 treasures to end the game
    if (currentRoom == &masterBedroom) {
        if (allTreasuresCollected) {
            cout << "There is a hostage in the MasterBedroom. Do you want to rescue the hostage? (y/n)" << endl;
            char answer;
            cin >> answer;
            if (answer == 'y') {
                cout << "You rescued the hostage! Congratulations, you won the game!" << endl;

                // Ask if the player wants to play again
                cout << "Do you want to play again? (y/n)" << endl;
                char playAgain;
                cin >> playAgain;

                if (playAgain != 'y') {
                    endGame(true);
                    cout << "Thanks for playing!" << endl;
                    exit(EXIT_SUCCESS);
                } else {
                    // Reset the values to play again
                    player.health = 10;
                    rook.isActive = true;
                    building.isActive = true;
                    baldEagle.isActive = true;
                    rook.currentRoomIndex = rand() % 7;
                    building.currentRoomIndex = rand() % 7;
                    baldEagle.currentRoomIndex = rand() % 7;
                    treasure1 = true;
                    treasure2 = true;
                    treasure3 = true;
                    allTreasuresCollected = false;
                    currentRoom = &outdoor;
                    startGame();
                }
            }
        } else {
            cout << "You are in the MasterBedroom, but you need to collect the three specific treasures to save the hostage." << endl;
        }
    }

    // Ask the player which room they want to go to next
    cout << "Which room do you want to go to next? ";
    cout << "(Available rooms: ";
    int connectedRoomsCount = 0;
    for (int i = 0; i < 5; ++i) {
        if (currentRoom->connectedRooms[i] != nullptr) {
            cout << currentRoom->connectedRooms[i]->name << " ";
            connectedRoomsCount++;
        }
    }
    cout << ")" << endl;

    string roomChoice;
    cin >> roomChoice;

    // Find the index of the current room in the array of connected rooms
    int index = -1;  // Initialize to an invalid value
    for (int i = 0; i < connectedRoomsCount; ++i) {
        if (roomChoice == currentRoom->connectedRooms[i]->name) {
            index = i;
            break;
        }
    }

    // Check if the player encounters a monster
    if (rook.currentRoomIndex == index && rook.isActive) {
        encounterMonster(rook);
    }

    if (building.currentRoomIndex == index && building.isActive) {
        encounterMonster(building);
    }

    if (baldEagle.currentRoomIndex == index && baldEagle.isActive) {
        encounterMonster(baldEagle);
    }

    // Update the current room index for the defeated monsters to respawn in a new room
    rook.currentRoomIndex = rand() % 7;
    building.currentRoomIndex = rand() % 7;
    baldEagle.currentRoomIndex = rand() % 7;

    if (player.health <= 0) {
        endGame(false);
    }

    if (treasure1 == false && treasure2 == false && treasure3 == false) {
        allTreasuresCollected = true;
    }


    bool isValidChoice = false;
    for (int i = 0; i < connectedRoomsCount; ++i) {
        if (roomChoice == currentRoom->connectedRooms[i]->name) {
            currentRoom = currentRoom->connectedRooms[i];
            isValidChoice = true;
            break;
        }
    }

    if (!isValidChoice) {
        cout << "Invalid room choice. Please choose a valid room." << endl;
    }

    exploreRoom(); //Function to exploreroom
}
// Dice Combat code
void Game::performCombat(Character& player, Monster& enemy) {
    while (player.health > 0 && enemy.health > 0)
    {
        // Ask the player if they are ready to roll the dice
        char ready;
        do {
            cout << "Are you ready to roll the dice? (y/n): ";
            cin >> ready;
        } while (ready != 'y' && ready != 'n');

        if (ready == 'n') {
            continue;
        }

        int playerRoll = rollDie();
        int enemyRoll = rollDie();

        cout << "Player " << player.name << " rolls: " << playerRoll << endl;
        cout << "Monster " << enemy.name << " rolls: " << enemyRoll << endl;

        if (playerRoll > enemyRoll) {
            cout << "Player hits the monster! Monster loses 1 life.\n";
            enemy.health -= 1;
        } else if (enemyRoll > playerRoll) {
            cout << "Monster hits the player! Player loses 1 life.\n";
            player.health -= 1;
        } else {
            cout << "It's a tie! No one loses life.\n";
        }

        cout << "Player " << player.name << " health: " << player.health << endl;
        cout << "Monster " << enemy.name << " health: " << enemy.health << endl;
    }

    if (enemy.health <= 0) {
        cout << "Player defeats the monster! You win!\n";
    } else {
        cout << "Player's health reached zero. You lose!\n";
    }
}

void Game::encounterMonster(Monster& enemy) {
    cout << "A wild " << enemy.name << " appears! Prepare for combat." << endl;
    performCombat(player, enemy);

    // Check if the monster is defeated
    if (enemy.health <= 0) {
        enemy.isActive = false;
        cout << enemy.name << " has been defeated and won't bother you anymore." << endl;
    }
}

void Game::endGame(bool win) {
    if (win) {
        cout << "You won the game!" << endl;
    } else {
        cout << "Your health reached zero. Game over!" << endl;
    }
}
// Rolling Dice code
int Game::rollDie() {
    return rand() % 6 + 1; // Roll a six-sided die
}

void Game::StairCaseMap() {
    cout << endl;
    cout << endl;
    cout << "Staircase" << endl;
    cout << endl;
    cout << endl;

    for(int a=1;a<=30;a++)
    {
        cout<<"*";
    }
    cout<<endl;
    for(int x=1;x<=3;x++) {
        cout << "*";
        for (int a = 1; a <= 28; a++) {
            cout << " ";
        }
        cout << "*";
        cout << endl;
    }
    for(int b=1;b<=10;b++)
    {
        for(int c=1;c<=1;c++)
        {
            cout<<"*";
        }
        for(int d=1;d<=13;d++)
        {
            cout<<" ";
        }
        cout<<"][";
        for(int e=16;e<=28;e++)
        {
            cout<<" ";
        }
        cout<<"*"<<endl;
    }
    for(int f=1;f<=2;f++)
    {
        cout<<"*";
    }
    cout<<"|floor1|";
    for(int g=1;g<=4;g++)
    {
        cout<<"*";
    }
    cout<<"][";
    cout<<"****";
    cout<<"|floor2|";
    cout<<"**"<<endl;
    cout << endl;
    cout << endl;
}

void Game::FrontOfTheHouseMap() {
    cout << "Front Of the House\n";
    cout << endl;

    for(int a=1;a<=77;a++)
    {
        cout<<"*";
    }
    cout<<endl;


    for(int b=1;b<=8;b++)
    {


            for(int c=1;c<=1;c++)
            {
                cout<<"*";
            }

        for(int d=1;d<=75;d++)
        {
            cout<<" ";


        }
            cout<<"*"<<endl;
    }

    for(int e=1;e<=65;e++)
    {
        cout<<"*";
        if(e == 12)
        {
        cout <<"| x |";
        }
        if(e == 50)
        {
        cout <<"|     |";
        }
    }


    cout << endl;
    cout << "You are at the front of the hosue\n";
    cout << "Their is 2 entry ways which are baricaded off and you can rip down the barricade\n";
    cout << endl;
    cout << endl;
    cout << endl;
}

void Game::LivingRoomMap() {
    cout << "Living Room\n";
    cout << endl;
    cout << endl;

    for(int a=1;a<=30;a++)
    {
        cout<<"*";
        if(a == 15)
        {
        cout <<"|     |";
        }
    }
    cout<<endl;


    for(int b=1;b<=8;b++)
    {
        if (b == 5 || b == 4)
        {
            cout << " ";
        }
        else if(b == 3 || b == 6)
        {
            cout << "-";
        }

        else
        {
            for(int c=1;c<=1;c++)
            {
                cout<<"*";
            }
        }

        for(int d=1;d<=35;d++)
        {
            cout<<" ";


        }
        if(b == 2)
        {
            cout <<" Stairs " << endl;
        }
        if(b == 1 || b ==3)
        {
            cout << "---" << endl;
        }
        else
        {
            cout<<"*"<<endl;
        }
    }

    for(int e=1;e<=30;e++)
    {
        cout<<"*";
        if(e == 23)
        {
        cout <<"|     |";
        }
    }
    cout << endl;
    cout << endl;
    cout << endl;
}

void Game::PoolRoomMap() {
    cout << "Pool room\n";
    cout << endl;
    cout << endl;




    for(int a=1;a<=35;a++)
    {
        cout<<"*";
        if(a == 12)
        {
        cout <<"| x |";
        }
    }
    cout<<endl;


    for(int b=1;b<=11;b++)
    {
        for(int c=1;c<=1;c++)
        {
            cout<<"*";
        }


        for(int d=1;d<=38;d++)
        {
            cout<<" ";


        }
        if(b == 5 || b == 4)
        {
            cout <<" " << endl;
        }
        if(b == 3 || b == 6)
        {
            cout << "---" << endl;
        }
        else
        {
            cout<<"*"<<endl;
        }
    }


    for(int e=1;e<=33;e++)
    {
        cout<<"*";
        if(e == 23)
        {
        cout <<"|     |";
        }
    }
    cout << endl;
    cout << endl;
    cout << endl;
}

void Game::MasterBathroomMap() {
    cout << endl;
cout << endl;
cout << "Master Bathroom\n";
cout << endl;
cout << endl;

for(int a=1;a<= 22;a++)

    {
        cout<<"*";
    }
    cout<<endl;

    for(int b=1;b<=6;b++)
    {
        for(int c=1;c<=1;c++)
        {
             cout<<"*";
        }
        for(int d=1;d<=20;d++)
        {
            cout<<" ";
        }
        if(b == 3)
        {
            cout <<" " << endl;
        }
        if(b == 2 || b == 4)
        {
            cout << "---" << endl;
        }
        else
        {
            cout<<"*"<<endl;
        }
    }


    for(int e=1;e<=22;e++)
    {
        cout<<"*";
    }
    cout<<endl;
    cout << endl;
    cout << endl;
}

void Game::BathroomMap() {
    cout << endl;
    cout << "Bathroom";
    cout << endl;
    cout << endl;
  for(int a=1;a<=8;a++)
    {
        cout<<"*";
        if (a==4)
        {
            cout<<("|     |");
        }
    }
    cout<<endl;


    for(int b=1;b<=7;b++)
    {

        for(int c=1;c<=1;c++)
        {
            cout<<"*";
        }


        for(int d=1;d<=13;d++)
        {
            cout<<" ";
        }

        cout<<"*"<<endl;
    }


    for(int e=1;e<=15;e++)
    {
        cout<<"*";
    }
    cout << endl;
    cout << endl;
    cout << endl;
}

void Game::MasterBedroomMap() {
    cout << endl;
    cout << "MasterBedroom";
    cout << endl;
    cout << endl;
    for(int a=1;a<=30;a++)
    {
        cout<<"*";
    }
    cout<<endl;


    for(int b=1;b<=10;b++)
    {
        if (b == 8)
        {
            cout << " ";
        }
        else if(b == 7 || b == 9)
        {
            cout << "-";
        }

        else
        {
            for(int c=1;c<=1;c++)
            {
                cout<<"*";
            }
        }

        for(int d=1;d<=28;d++)
        {
            cout<<" ";
        }

        cout<<"*"<<endl;
    }


    for(int e=1;e<=24;e++)
    {
        cout<<"*";
        if(e==12)
        {
          cout<< "|    |";
        }

    }
    cout << endl;
    cout << endl;
    cout << endl;
}

void Game::UpstairsHallwayMap() {
    cout << endl;
    cout << "Upstairs Hallway";
    cout << endl;
    cout << endl;
  for(int a=1;a<=8;a++)
    {
        cout<<"*";
        if (a==4)
        {
            cout<<("|     |");
        }
    }
    cout<<endl;


    for(int b=1;b<=15;b++)
    {
        if (b == 8 || b == 7)
        {
            cout << " ";
        }
        else if(b == 6 || b == 9)
        {
            cout << "-";
        }
        else
        {
            for(int c=1;c<=1;c++)
            {
            cout<<"*";
            }
        }


        for(int d=1;d<=13;d++)
        {
            cout<<" ";
        }

        cout<<"*"<<endl;
    }


    for(int e=1;e<=8;e++)
    {
        cout<<"*";
        if (e==4)
        {
            cout<<("|     |");
        }
    }
    cout << endl;
    cout << endl;
    cout << endl;
}

void Game::startGame() {
    int menuSelect;

    do {
        cout << "1 - Start game\n";
        cout << "2 - Game Rules\n";
        cout << "3 - Game Map\n";
        cout << "4 - Exit\n";
        cout << "Select: ";
        cin >> menuSelect;

        switch (menuSelect) {
            case 1:
                // Ask for the player's name
                setPlayerName();
                cout << "Starting Game... \n";
                // Run the game
                exploreRoom();
                break;
            case 2:
                // Prints the Rules
                cout << "Loading Game Rules... \n";
                cout << "You are attacking the building to save a hostage. Their are 3 people defending the hostage and you need to either avoid them and escape with the hostage or kill them.\n To save the hostage you need to find pliers, scissor, and beer and when you do go to the master bedroom and save the hostage.\n";
                break;
            case 3:
                // Prints the map out
                cout << "Loading Game Map... \n";
                cout << endl;
                FrontOfTheHouseMap();
                LivingRoomMap();
                PoolRoomMap();
                StairCaseMap();
                MasterBedroomMap();
                MasterBathroomMap();
                BathroomMap();
                UpstairsHallwayMap();
                break;
            case 4:
                // Exit the code
                cout << "Exiting the game... \n";
                break;
            default:
                cout << "Invalid choice. Please choose a valid option.\n";
        }

        // Check if the player wants to play again
        if (menuSelect != 4) {
            cout << "Do you want to play again? (y/n): ";
            char playAgain;
            cin >> playAgain;

            if (playAgain != 'y') {
                cout << "Thanks for playing!" << endl;
                break;  // Exit the loop if the player chooses not to play again
            }
        }

    } while (menuSelect != 4);
}

int main() {
    Game game;
    game.startGame();

    return 0;
}
