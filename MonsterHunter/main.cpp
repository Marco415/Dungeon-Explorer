#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;
int updateHighScores(int score);
void getMinMax(int arrNumbers[], int sizeArray, int column, int* minNum, int* maxNum);

///player structure
struct Player
{
    string name;
    int health;
    int maxHealth;
    int lightAttack;
    int heavyAttack;
    int energy;
    int maxEnergy;
    int level;
    int gamesPlayed;
    int numNewHighScores;
}playerInfo;

///game class
class Game
{
public:
    int monsterLevel;
    int monsterHealth;
    int arrMonsterAttack[2][5];
    int roundReached;
    int monstersDefeated;

    void createMonsterStats(int gameRound);
    int playGame();
};

///function in game class to create monster stats
void Game::createMonsterStats(int gameRound)
{
    srand(time(0));

    //monster level
    int levelDeterminer = rand() % 100 + 1;
    if(levelDeterminer > 0 && levelDeterminer <= 50)
    {
        monsterLevel = 1;
    }
    else if(levelDeterminer > 50 && levelDeterminer <= 80)
    {
        monsterLevel = 2;
    }
    else
    {
        monsterLevel = 3;
    }

    //monster health
    monsterHealth = ceil(rand() % 200 + (500 * (monsterLevel + 1)/2 * (gameRound + 1)/(gameRound + 2)));

    //monster attack
    for(int i = 0; i < 5; i++)
    {
        arrMonsterAttack[0][i] = ceil(rand() % 50 + (50 * (monsterLevel + 1)/2 * (gameRound + 1)/(gameRound + 2)));
        arrMonsterAttack[1][i] = ceil(rand() % 50 + (125 * (monsterLevel + 1)/2 * (gameRound + 1)/(gameRound + 2)));
    }
}

///function in game class to run game
int Game::playGame()
{
    cout << "\nThe game is now starting" << endl;

    //set values to variables in Player struct and Game class
    playerInfo.health = 1000;
    playerInfo.maxHealth = 1000;
    playerInfo.lightAttack = 100;
    playerInfo.heavyAttack = 150;
    playerInfo.maxEnergy = 15;
    playerInfo.level = 1;
    monstersDefeated = 0;

    //declare variables
    int gameRound = 0, score = 0, requiredExperience = 1000;
    bool continueGame = true;

    //do-while loop to continue running the game
    do
    {
        ++gameRound;
        cout << "\nRound " << gameRound << endl;

        //declare variables
        int battleExerience;
        bool endRound = false, monsterBlock = false;
        char nextRound, upgradeChoice;
        string tempNextRound, tempUpgradeChoice;
        playerInfo.energy = playerInfo.maxEnergy;

        //display player stats
        cout << "\nPlayer info:" << endl;
        cout << left << setw(20) << "Name:" << playerInfo.name << endl;
        cout << left << setw(20) << "Level:" << playerInfo.level << endl;
        cout << left << setw(20) << "Health:" << playerInfo.health << endl;
        cout << left << setw(20) << "Light attack:" << playerInfo.lightAttack << endl;
        cout << left << setw(20) << "Heavy attack:" << playerInfo.heavyAttack << endl;
        cout << left << setw(20) << "Energy:" << playerInfo.energy << endl;

        //create and display monster stats
        createMonsterStats(gameRound);
        cout << "\nMonster info:" << endl;
        cout << left << setw(20) << "Level:" << monsterLevel << endl;
        cout << left << setw(20) << "Health:" << monsterHealth << endl;
        int maxValue = arrMonsterAttack[0][0], minValue = arrMonsterAttack[0][0];
        int *pMax = &maxValue, *pMin = &minValue;
        getMinMax(arrMonsterAttack[0], 5, 0, pMin, pMax);
        cout << left << setw(20) << "Light attack:" << "From " << *pMin << " to " << *pMax << endl;
        maxValue = arrMonsterAttack[1][0];
        minValue = arrMonsterAttack[1][0];
        pMax = &maxValue;
        pMin = &minValue;
        getMinMax(arrMonsterAttack[1], 5, 1, pMin, pMax);
        cout << left << setw(20) << "Light attack:" << "From " << *pMin << " to " << *pMax << endl;

        battleExerience = ceil(monsterHealth * 3/4);

        //while loop to continue game round
        while(endRound == false)
        {
            srand(time(0));

            //declare variables
            int monsterLight, monsterHeavy, attackNumber, monsterAttackType;
            char playerAttackChoice;
            string tempAttackChoice;
            bool validPlayerAction = false, playerBlock = false;

            //get monster attack values
            attackNumber = rand() % 3 + 1;
            monsterLight = arrMonsterAttack[0][attackNumber];
            attackNumber = rand() % 3 + 1;
            monsterHeavy = arrMonsterAttack[1][attackNumber];
            monsterAttackType = rand() % 10 + 1;

            //determine monster block
            if(monsterAttackType == 10 || monsterAttackType == 8)
            {
                monsterBlock = false;
            }
            else if(monsterAttackType == 7 || monsterAttackType == 5)
            {
                monsterBlock = true;
            }
            else
            {
                monsterBlock = false;
            }

            //get player attack choice
            cout << "\nYou have " << playerInfo.energy << " energy." << endl;
            cout << "Enter your choice of attack (L-Light, H-Heavy, B-Block): ";
            cin >> tempAttackChoice;
            playerAttackChoice = toupper(tempAttackChoice[0]);
            while(tempAttackChoice.length() > 1)
            {
                cout << "Attack choice not in the correct format - it must only be a single character." << endl;
                cout << "Please re-enter: ";
                cin >> tempAttackChoice;
                playerAttackChoice = toupper(tempAttackChoice[0]);
            }
            while(playerAttackChoice != 'L' && playerAttackChoice != 'H' && playerAttackChoice != 'B')
            {
                cout << "Invalid attack type chosen - please re-enter: ";
                cin >> tempAttackChoice;
                playerAttackChoice = toupper(tempAttackChoice[0]);
                while(tempAttackChoice.length() > 1)
                {
                    cout << "Attack choice not in the correct format - it must only be a single character." << endl;
                    cout << "Please re-enter: ";
                    cin >> tempAttackChoice;
                    playerAttackChoice = toupper(tempAttackChoice[0]);
                }
            }

            //validate player attack choice
            while(validPlayerAction == false)
            {
                if(playerAttackChoice == 'H' && playerInfo.energy < 5)
                {
                    cout << "\nInsufficient energy for a heavy attack" << endl;
                    cout << "Please choose a different attack type (L-Light, B-Block): ";
                    cin >> tempAttackChoice;
                    playerAttackChoice = toupper(tempAttackChoice[0]);
                    while(tempAttackChoice.length() > 1)
                    {
                        cout << "Attack choice not in the correct format - it must only be a single character." << endl;
                        cout << "Please re-enter: ";
                        cin >> tempAttackChoice;
                        playerAttackChoice = toupper(tempAttackChoice[0]);
                    }
                    while(playerAttackChoice != 'L' && playerAttackChoice != 'H' && playerAttackChoice != 'B')
                    {
                        cout << "Invalid attack type chosen - please re-enter: ";
                        cin >> tempAttackChoice;
                        playerAttackChoice = toupper(tempAttackChoice[0]);
                        while(tempAttackChoice.length() > 1)
                        {
                            cout << "Attack choice not in the correct format - it must only be a single character." << endl;
                            cout << "Please re-enter: ";
                            cin >> tempAttackChoice;
                            playerAttackChoice = toupper(tempAttackChoice[0]);
                        }
                    }
                }
                else if(playerAttackChoice == 'L' && playerInfo.energy < 2)
                {
                    cout << "\nInsufficient energy for a light attack" << endl;
                    cout << "Please choose a different attack type (B-Block): ";
                    cin >> tempAttackChoice;
                    playerAttackChoice = toupper(tempAttackChoice[0]);
                    while(tempAttackChoice.length() > 1)
                    {
                        cout << "Attack choice not in the correct format - it must only be a single character." << endl;
                        cout << "Please re-enter: ";
                        cin >> tempAttackChoice;
                        playerAttackChoice = toupper(tempAttackChoice[0]);
                    }
                    while(playerAttackChoice != 'L' && playerAttackChoice != 'H' && playerAttackChoice != 'B')
                    {
                        cout << "Invalid attack type chosen - please re-enter: ";
                        cin >> tempAttackChoice;
                        playerAttackChoice = toupper(tempAttackChoice[0]);
                        while(tempAttackChoice.length() > 1)
                        {
                            cout << "Attack choice not in the correct format - it must only be a single character." << endl;
                            cout << "Please re-enter: ";
                            cin >> tempAttackChoice;
                            playerAttackChoice = toupper(tempAttackChoice[0]);
                        }
                    }
                }
                else
                {
                    validPlayerAction = true;
                }
            }

            //execute player attack
            switch(playerAttackChoice)
            {
            //player light attack
            case 'L':
                {
                    playerInfo.energy -= 2;
                    //monster blocks
                    if(monsterBlock == true)
                    {
                        if(monsterHealth >= ceil(playerInfo.lightAttack / 10))
                        {
                            monsterHealth -= ceil(playerInfo.lightAttack / 10);
                        }
                        else
                        {
                            monsterHealth = 0;
                        }
                        cout << "You used a light attack, but the monster blocked. The monster's health is now " << monsterHealth << endl;
                    }
                    //monster does not block
                    else
                    {
                        if(monsterHealth >= playerInfo.lightAttack)
                        {
                            monsterHealth -= playerInfo.lightAttack;
                        }
                        else
                        {
                            monsterHealth = 0;
                        }
                        cout << "You used a light attack. The monster's health is now " << monsterHealth << endl;
                    }
                    break;
                }

            //player heavy attack
            case 'H':
                {
                    playerInfo.energy -= 5;
                    //monster blocks
                    if(monsterBlock == true)
                    {
                        if(monsterHealth >= ceil(playerInfo.heavyAttack / 10))
                        {
                            monsterHealth -= ceil(playerInfo.heavyAttack / 10);
                        }
                        else
                        {
                            monsterHealth = 0;
                        }
                        cout << "You used a heavy attack, but the monster blocked. The monster's health is now " << monsterHealth << endl;
                    }
                    //monster does not block
                    else
                    {
                        if(monsterHealth >= playerInfo.heavyAttack)
                        {
                            monsterHealth -= playerInfo.heavyAttack;
                        }
                        else
                        {
                            monsterHealth = 0;
                        }
                        cout << "You used a heavy attack. The monster's health is now " << monsterHealth << endl;
                    }
                    break;
                }

            //player block
            case 'B':
                {
                    playerBlock = true;
                    cout << "You have chosen to block the monster's attack" << endl;
                }
                break;
            }

            //execute monster attack
            if(monsterHealth > 0)
            {
                //player blocks
                if(playerBlock == true)
                {
                    //monster heavy attack
                    if(monsterAttackType == 10 || monsterAttackType == 8)
                    {
                        if(playerInfo.health >= ceil(monsterHeavy / 10))
                        {
                            playerInfo.health -= ceil(monsterHeavy / 10);
                        }
                        else
                        {
                            playerInfo.health = 0;
                        }
                        cout << "The monster used a heavy attack, but you blocked. Your health is now " << playerInfo.health << endl;
                    }
                    //monster blocked
                    else if(monsterAttackType == 7 || monsterAttackType == 5)
                    {
                        cout << "The monster chose to block your attack" << endl;
                    }
                    //monster light attack
                    else
                    {
                        if(playerInfo.health >= ceil(monsterLight / 10))
                        {
                            playerInfo.health -= ceil(monsterLight / 10);
                        }
                        else
                        {
                            playerInfo.health = 0;
                        }
                        cout << "The monster used a light attack, but you blocked. Your health is now " << playerInfo.health << endl;
                    }
                }
                //player does not block
                else
                {
                    //monster heavy attack
                    if(monsterAttackType == 10 || monsterAttackType == 8)
                    {
                        if(playerInfo.health >= monsterHeavy)
                        {
                            playerInfo.health -= monsterHeavy;
                        }
                        else
                        {
                            playerInfo.health = 0;
                        }
                        cout << "The monster used a heavy attack. Your health is now " << playerInfo.health << endl;
                    }
                    //monster blocked
                    else if(monsterAttackType == 7 || monsterAttackType == 5)
                    {
                        cout << "The monster chose to block your attack" << endl;
                    }
                    //monster light attack
                    else
                    {
                        if(playerInfo.health >= monsterLight)
                        {
                           playerInfo.health -= monsterLight;
                        }
                        else
                        {
                            playerInfo.health = 0;
                        }
                        cout << "The monster used a light attack. Your health is now " << playerInfo.health << endl;
                    }
                }
            }

            //give the player 1 energy
            if(playerInfo.energy < playerInfo.maxEnergy)
            {
                ++playerInfo.energy;
            }

            //determine whether or not to end the round
            if(playerInfo.health <= 0 || monsterHealth <= 0)
            {
                endRound = true;
            }
        }

        cout << "\nThe round has ended" << endl;

        //determine whether or not to end the game
        if(playerInfo.health <= 0)
        {
            continueGame = false;
        }
        else
        {
            //calculate preliminary score
            ++monstersDefeated;
            if(monsterLevel == 1)
            {
                score += 100;
            }
            else if(monsterLevel == 2)
            {
                score += 200;
            }
            else
            {
                score += 400;
            }

            //ask if the player wants to continue
            cout << "\nDo you want to continue to the next round? (Y-Yes, N-No): ";
            cin >> tempNextRound;
            nextRound = toupper(tempNextRound[0]);
            while(tempNextRound.length() > 1)
            {
                cout << "Round end choice not in the correct format - it must only be a single character." << endl;
                cout << "Please re-enter: ";
                cin >> tempNextRound;
                nextRound = toupper(tempNextRound[0]);
            }
            while(nextRound != 'Y' && nextRound != 'N')
            {
                cout << "Invalid input given - please re-enter: ";
                cin >> tempNextRound;
                nextRound = toupper(tempNextRound[0]);
                while(tempNextRound.length() > 1)
                {
                    cout << "Round end choice not in the correct format - it must only be a single character." << endl;
                    cout << "Please re-enter: ";
                    cin >> tempNextRound;
                    nextRound = toupper(tempNextRound[0]);
                }
            }

            //determine whether or not to end the game
            if(nextRound == 'N')
            {
                continueGame = false;
            }
            else
            {
                //give the player an upgrade for completing a round
                cout << "\nPlease choose an upgrade:" << "\n1. Heal 150 health" << "\n2. Increase light attack by 25 and heavy attack by 30" <<
                    "\n3. Increase max health by 200" << "\n4. Increase max energy by 3" << endl;
                cout << "Enter your choice: ";
                cin >> tempUpgradeChoice;
                upgradeChoice = tempUpgradeChoice[0];
                while(tempUpgradeChoice.length() > 1)
                {
                    cout << "Upgrade choice not in the correct format - it must only be a single character." << endl;
                    cout << "Please re-enter: ";
                    cin >> tempUpgradeChoice;
                    upgradeChoice = tempUpgradeChoice[0];
                }
                while(upgradeChoice != '1' && upgradeChoice != '2' && upgradeChoice != '3' && upgradeChoice != '4')
                {
                    cout << "Invalid option chosen - please re-enter: ";
                    cin >> tempUpgradeChoice;
                    upgradeChoice = tempUpgradeChoice[0];
                    while(tempUpgradeChoice.length() > 1)
                    {
                        cout << "Upgrade choice not in the correct format - it must only be a single character." << endl;
                        cout << "Please re-enter: ";
                        cin >> tempUpgradeChoice;
                        upgradeChoice = tempUpgradeChoice[0];
                    }
                }

                switch(upgradeChoice)
                {
                //heal
                case '1':
                    {
                        playerInfo.health += 150;
                        if(playerInfo.health > playerInfo.maxHealth)
                        {
                            playerInfo.health = playerInfo.maxHealth;
                        }
                        break;
                    }

                //increase attack
                case '2':
                    {
                        playerInfo.lightAttack += 25;
                        playerInfo.heavyAttack += 30;
                        break;
                    }

                //increase health
                case '3':
                    {
                        playerInfo.maxHealth += 200;
                        playerInfo.health = ceil(playerInfo.health * playerInfo.maxHealth/(playerInfo.health + 200));
                        break;
                    }

                //increase energy
                case '4':
                    {
                        playerInfo.maxEnergy += 3;
                        break;
                    }
                }

                //calculate player experience
                if(requiredExperience > battleExerience)
                {
                    requiredExperience -= battleExerience;
                }
                else
                {
                    battleExerience -= requiredExperience;
                    ++playerInfo.level;
                    requiredExperience = ceil(1000 * (playerInfo.level + 1)/2);
                    requiredExperience -= battleExerience;

                    //give the player an upgrade for reaching the next level
                    cout << "\nYou have reached level " << playerInfo.level << endl;
                    cout << "Please choose an upgrade:" << "\n1. Heal 150 health" << "\n2. Increase light attack by 25 and heavy attack by 30" <<
                        "\n3. Increase max health by 200" << "\n4. Increase max energy by 3" << endl;
                    cout << "Enter your choice: ";
                    cin >> tempUpgradeChoice;
                    upgradeChoice = tempUpgradeChoice[0];
                    while(tempUpgradeChoice.length() > 1)
                    {
                        cout << "Upgrade choice not in the correct format - it must only be a single character." << endl;
                        cout << "Please re-enter: ";
                        cin >> tempUpgradeChoice;
                        upgradeChoice = tempUpgradeChoice[0];
                    }
                    while(upgradeChoice != '1' && upgradeChoice != '2' && upgradeChoice != '3' && upgradeChoice != '4')
                    {
                        cout << "Invalid option chosen - please re-enter: ";
                        cin >> tempUpgradeChoice;
                        upgradeChoice = tempUpgradeChoice[0];
                        while(tempUpgradeChoice.length() > 1)
                        {
                            cout << "Upgrade choice not in the correct format - it must only be a single character." << endl;
                            cout << "Please re-enter: ";
                            cin >> tempUpgradeChoice;
                            upgradeChoice = tempUpgradeChoice[0];
                        }
                    }

                    switch(upgradeChoice)
                    {
                    //heal
                    case '1':
                        {
                            playerInfo.health += 150;
                            if(playerInfo.health > playerInfo.maxHealth)
                            {
                                playerInfo.health = playerInfo.maxHealth;
                            }
                            break;
                        }

                    //increase attack
                    case '2':
                        {
                            playerInfo.lightAttack += 25;
                            playerInfo.heavyAttack += 30;
                            break;
                        }

                    //increase health
                    case '3':
                        {
                            playerInfo.maxHealth += 200;
                            playerInfo.health = ceil(playerInfo.health * playerInfo.maxHealth/(playerInfo.health + 200));
                            break;
                        }

                    //increase energy
                    case '4':
                        {
                            playerInfo.maxEnergy += 3;
                            break;
                        }
                    }
                }
            }
        }
    }while(continueGame == true);

    //calculate round reached and final score
    roundReached = gameRound;
    score = ceil(score * (playerInfo.level + 1)/2);
    return score;
}

///main function
int main()
{
    //declare variables
    string playerName, tempGameOption;
    bool validName = false;
    char gameOption = '0';
    Game objGame;

    //get player name
    cout << "Input your name, player: ";
    getline(cin, playerName);

    //validate player name
    while(validName == false)
    {
        if(playerName == "")
        {
            cout << "Your name cannot be completely empty, please re-enter: ";
            getline(cin, playerName);
        }
        else
        {
            if(isalpha(playerName[0]) == false)
            {
                cout << "Your name must begin with a letter, please re-enter: ";
                getline(cin, playerName);
            }
            else
            {
                validName = true;
            }
        }
    }

    //set values in playerInfo
    playerInfo.name = playerName;
    playerInfo.gamesPlayed = 0;
    playerInfo.numNewHighScores = 0;

    //display welcome message
    cout << "\nWelcome to Monster Hunter, " << playerName << endl;

    //while loop to keep game running
    while(gameOption != '5')
    {
        //show menu
        cout << "\nGame options:" << "\n1. Game rules" << "\n2. Play game" << "\n3. High scores" << "\n4. Change name" << "\n5. Exit" << endl;
        //get player option chosen
        cout << "Enter your choice: ";
        cin >> tempGameOption;
        gameOption = tempGameOption[0];
        while(tempGameOption.length() > 1)
        {
            cout << "Game option not in the correct format - it must only be a single character" << endl;
            cout << "Please re-enter: ";
            cin >> tempGameOption;
            gameOption = tempGameOption[0];
        }

        switch(gameOption)
        {
        // option 1 - game rules
        case('1'):
            {
                //display game rule from file
                ifstream gameRules;
                gameRules.open("Game_Rules.txt", ios::in);
                //check if the file exists
                if(gameRules.is_open())
                {
                    string line;
                    cout << endl;

                    while(getline(gameRules, line))
                    {
                        cout << line << endl;
                    }

                    gameRules.close();
                }
                //file doesn't exist
                else
                {
                    cout << "\nFile not found" << endl;
                }

                break;
            }

        //option 2 - play game
        case('2'):
            {
                //get and display game stats
                int score = objGame.playGame();
                int highScore = updateHighScores(score);
                ++playerInfo.gamesPlayed;
                cout << "\nGame results:" << endl;
                cout << left << setw(30) << "Score:" << score << endl;
                cout << left << setw(30) << "Level:" << playerInfo.level << endl;
                cout << left << setw(30) << "Round reached:" << objGame.roundReached << endl;
                cout << left << setw(30) << "Monsters defeated:" << objGame.monstersDefeated << endl;

                //determine if new high score was achieved
                if(highScore != 0)
                {
                    cout << left << setw(30) << "New high score achieved:" << score << endl;
                    ++playerInfo.numNewHighScores;
                }
                break;
            }

        //option 3 - high scores
        case('3'):
            {
                int scoreRank = 0;
                ifstream highScores;
                highScores.open("High_Scores.txt", ios::in);
                //check if the file exists
                if(highScores.is_open())
                {
                    cout << left << setw(4) << "\n#" << left << setw(20) << "Name" << "Score" << endl;
                    string line;

                    //loop until end of file
                    while(getline(highScores, line))
                    {
                        int found = line.find("±");
                        string readName = "", readScore = "";
                        ++scoreRank;

                        //for loop to read name from file
                        for(int i = 0; i < found; i++)
                        {
                            readName += line[i];
                        }

                        //for loop to read score from file
                        for(int i = found + 1; i < line.length(); i++)
                        {
                            readScore += line[i];
                        }

                        //display high score
                        cout << left << setw(4) << scoreRank << left << setw(20) << readName << readScore << endl;
                    }

                    highScores.close();
                }
                //file doesn't
                else
                {
                    cout << "\nNo high scores have been recorded so far" << endl;
                }

                break;
            }

        //option 4 - change name
        case('4'):
            {
                //get player name
                cout << "\nInput your name, player: ";
                cin.ignore();
                getline(cin, playerName);
                validName = false;
                bool nameChanged = false;

                //validate player name
                while(validName == false)
                {
                    if(playerName == playerInfo.name)
                    {
                        validName = true;
                        cout << "Name was not changed." << endl;
                    }
                    else if(playerName == "")
                    {
                        cout << "Your name cannot be completely empty, please re-enter: ";
                        getline(cin, playerName);
                    }
                    else
                    {
                        if(isalpha(playerName[0]) == false)
                        {
                            cout << "Your name must begin with a letter, please re-enter: ";
                            getline(cin, playerName);
                        }
                        else
                        {
                            //display old player stats
                            cout << "\nFinal player results:" << endl;
                            cout << left << setw(40) << "Player:" << playerInfo.name << endl;
                            cout << left << setw(40) << "Games played:" << playerInfo.gamesPlayed << endl;
                            cout << left << setw(40) << "Number of new high scores achieved:" << playerInfo.numNewHighScores << endl;
                            cout << "\nThank you for playing Monster Hunter, " << playerInfo.name << ". Goodbye." << endl;
                            validName = true;
                            nameChanged = true;
                        }
                    }
                }

                if(nameChanged == true)
                {
                    //set new values in playerInfo
                    cout << "\nWelcome to Monster Hunter, " << playerName << endl;
                    playerInfo.name = playerName;
                    playerInfo.gamesPlayed = 0;
                    playerInfo.numNewHighScores = 0;
                }

                break;
            }

        //option 5 - exit
        case('5'):
            {
                //display player stats and exit message
                cout << "\nFinal player results:" << endl;
                cout << left << setw(40) << "Player:" << playerInfo.name << endl;
                cout << left << setw(40) << "Games played:" << playerInfo.gamesPlayed << endl;
                cout << left << setw(40) << "Number of new high scores achieved:" << playerInfo.numNewHighScores << endl;
                cout << "\nThank you for playing Monster Hunter, " << playerInfo.name << ". Goodbye." << endl;
                cout << "\nNow exiting the game..." << endl;
                break;
            }

        //invalid option
        default:
            {
                cout << "\nInvalid game option chosen, please re-enter." << endl;
                break;
            }
        }
    }

    return 0;
}

///function to update high scores
int updateHighScores(int score)
{
    //create arrays and variables
    string line, arrNames[11];
    int arrScores[11], element = -1, highScore = 0;

    //create and open text file
    ifstream readScores;
    readScores.open("High_Scores.txt", ios::in);

    //check if the file exists
    if(readScores.is_open())
    {
        //read scores into the arrays
        while(getline(readScores, line))
        {
            string playerName = "", playerScore = "";
            int found = line.find("±");
            ++element;

            //for loop to get name from file line
            for(int i = 0; i < found; i++)
            {
                playerName += line[i];
            }
            //for loop to get score from file line
            for(int i = found + 1; i <= line.length(); i++)
            {
                playerScore += line[i];
            }

            //store name and score in arrays
            arrNames[element] = playerName;
            arrScores[element] = stoi(playerScore);
        }
        readScores.close();

        //insert elements into array
        int pos = 10;
        arrNames[element + 1] = playerInfo.name;
        arrScores[element + 1] = score;

        //sort arrays
        for(int i = element; i >= 0; i--)
        {
            if(arrScores[i + 1] > arrScores[i])
            {
                int tempScore = arrScores[i];
                arrScores[i] = arrScores[i + 1];
                arrScores[i + 1] = tempScore;
                string tempName = arrNames[i];
                arrNames[i] = arrNames[i + 1];
                arrNames[i + 1] = tempName;
                pos = i;
            }
        }

        //determine if a high score was made
        if(pos != 10 && element < 10)
        {
            ++highScore;
        }

        //get array size
        ++element;
        if(element > 9)
        {
            element = 9;
        }
    }
    //file doesn't exist
    else
    {
        //store name and score in arrays
        arrNames[0] = playerInfo.name;
        arrScores[0] = score;
        element = 0;
        ++highScore;
    }

    //write new high scores into file
    ofstream writeScores;
    writeScores.open("High_Scores.txt", ios::out);
    for(int i = 0; i <= element; i++)
    {
        line = arrNames[i] + "±" + to_string(arrScores[i]);
        writeScores << line << endl;
    }
    writeScores.close();

    return highScore;
}

///function to determine max and min values in an array
void getMinMax(int arrNumbers[], int sizeArray, int column, int* minNum, int* maxNum)
{
    for(int i = 0; i < sizeArray; i++)
    {
        //set min value
        if(arrNumbers[i] < *minNum)
        {
            *minNum = arrNumbers[i];
        }
        //set max value
        if(arrNumbers[i] > *maxNum)
        {
            *maxNum = arrNumbers[i];
        }
    }
}
