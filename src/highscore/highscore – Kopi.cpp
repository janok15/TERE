#include "highscore.h"

HighScore::HighScore()
{
    if(fexists("highscores.dat")) // Does the save file exist?
    {
        loadHighScores(); // It does, load it into memory
    }
    else { // The file does not exist, make a new one
        createSaveFile(true); // Clear the save file
    }

    //createSaveFile(true);

    validHighScore(0, 2500);
    validHighScore(2, 2500);

    std::cout << "'" << key[1039] << "'\n";


    //replaceHighScore(2, "fsdfs", 225000, 22);
}


HighScore::~HighScore() {}


bool HighScore::fexists(const std::string& filename)
{
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}

/*
enkel xor kryptering, for high score:
char string[11]="A nice cat";
    char key[11]="ABCDEFGHIJ";

    for(int x=0; x<10; x++)
    {
        string[x]=string[x]^key[x];
        std::cout <<  string[x];
    }

    std::cout << "\n";

    for(int x=0; x<10; x++)
    {
        string[x]=string[x]^key[x];
        std::cout <<  string[x];
    }

    std::cout << "\n"; */



void HighScore::createSaveFile(bool clear) // Create a new save file, or update the current one
{
    saveToFile.open("highscores.dat");

    std::string temp[16];

    //char temp;
    int keyIndex = 0;

    for(int mode = 0; mode < 3; mode++ )
    {
        for(int entry = 0; entry < 8; entry++)
        {
            if(clear) // Reset the high score list
            {

/*

                // Add length xor in front ... in hex

                for(int x = 0; x < scoreName[mode][entry].length(); x++)
                {
                    //string[x]=string[x]^key[x];
                    //std::cout <<  string[x];

                    temp[x] = scoreName[mode][entry][x]^key[keyIndex];
                    //temp = scoreName[mode][entry][x]^key[keyIndex];

                    keyIndex++;


                    //int a = temp;
//(int)sRandomBag.at(i) - 48
                    //std::cout << (int)temp << "-"; //std::stoi(temp[x]);

                    saveToFile << temp[x];
                }


                saveToFile << std::endl;
*/
                saveToFile << scoreName[mode][entry] << std::endl;
                saveToFile << score[mode][entry] << std::endl;
                saveToFile << scoreLevel[mode][entry] << std::endl;
            }
            else // Update with new data
            {
                saveToFile << tempScoreName[mode][entry] << std::endl;
                saveToFile << tempScore[mode][entry] << std::endl;
                saveToFile << tempScoreLevel[mode][entry] << std::endl;
            }
        }
    }

std::cout << "ki: " << keyIndex << "\n";

    saveToFile.close();
}


inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;

    char * p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}



void HighScore::loadHighScores()
{
    bool problem = false;
    std::string tempString, tmpString;

    int keyIndex = 0;

    loadFromFile.open("highscores.dat");

    for(int mode = 0; mode < 3; mode++)
    {
        for(int entry = 0; entry < 8; entry++)
        {



            //loadFromFile >> tempScoreName[mode][entry];
            loadFromFile >> tmpString;

            for(int x = 0; x < tmpString.length(); x++)
            {
                tempScoreName[mode][entry][x] = tmpString[x]^key[keyIndex];
                keyIndex++;

                std::cout << "" << tempScoreName[mode][entry][x] << "";

                //string[x]=string[x]^key[x];
                //std::cout <<  string[x];
            }

            std::cout << "\n";


            loadFromFile >> tempString;

            if(!isInteger(tempString)) // This is not the expected integer
                problem = true;
            else {
                tempScore[mode][entry] = std::stoi(tempString);
            }

            loadFromFile >> tempString;

            if(!isInteger(tempString)) // This is not the expected integer
                problem = true;
            else {
                tempScoreLevel[mode][entry] = std::stoi(tempString);
            }

            if (loadFromFile.eof()) // reached end-of-file prematurely (file is shorter than it should be)
                problem = true;

            if(problem)
                break;
        }

        if(problem)
            break;
    }

    if(problem) // Problem encountered, recreate the high score list
    {
        loadFromFile.close();
        createSaveFile(true);
        loadHighScores();
    }

    loadFromFile.close();

    // The entries are now in tempScoreName (strings) and tempScore (int)
}


bool HighScore::validHighScore(int mode, int value)
{
    // Mode 0 - Marathon
    // Mode 1 - Line Clear
    // Mode 2 - Sprint (Sprint Mode is mode 3 in-game, less is better)

    if(mode < 2)
    {
        if(value >= tempScore[mode][7])
        {
            std::cout << "The value: " << value << " is >= " << tempScore[mode][7] << "\n";
            return true;
        }
        else {
            std::cout << "The value: " << value << " is not better than " << tempScore[mode][7] << "\n";
            return false;
        }


    }
    else // Sprint mode
    {
        if(value <= tempScore[mode][7])
        {
            std::cout << "The value: " << value << " is <= " << tempScore[mode][7] << "\n";
            return true;
        }
        else {
            std::cout << "The value: " << value << " is not better than " << tempScore[mode][7] << "\n";
            return false;
        }
    }
}


void HighScore::shuffleList(int mode, int pos, std::string name, int value, int level)
{
    for(int index = 7; index > pos; index--)
    {
        tempScoreName[mode][index] = tempScoreName[mode][index - 1];
        tempScore[mode][index] = tempScore[mode][index - 1];
        tempScoreLevel[mode][index] = tempScoreLevel[mode][index - 1];
    }

    tempScoreName[mode][pos] = name;
    tempScore[mode][pos] = value;
    tempScoreLevel[mode][pos] = level;
}


void HighScore::replaceHighScore(int mode, std::string name, int value, int level)
{
    int pos = 7;

    if(mode < 2)
    {
        for(int i = 7; i > -1; i--)
        {
            if(tempScore[mode][i] <= value)
                pos = i;
        }
        shuffleList(mode, pos, name, value, level);
    }
    else
    {
        for(int i = 7; i > -1; i--)
        {
            if(tempScore[mode][i] >= value)
                pos = i;
        }
        shuffleList(mode, pos, name, value, level);
    }

    for(int i = 0; i < 8; i++)
    {
        std::cout << "name: " << tempScoreName[mode][i] << "  score: " << tempScore[mode][i] << "  level: " << tempScoreLevel[mode][i] << "\n";
    }

    createSaveFile(false); // Update the save file with new data
}
