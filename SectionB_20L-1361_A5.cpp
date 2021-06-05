#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include<windows.h>
using namespace std;

//Search Functionality
void searchGrid();
void createGrid();
char* GetStringFromBuffer(char*);
void deAllocateGridMemory(char**& , const int &);
bool seachGridForWords(char*, char**, int, int, int&, int&, int&, int&);
bool searchHorizontally(char*, char**, int, int, int&, int&, int&, int&);
bool searchVertically(char*, char**, int, int, int&, int&, int&, int&);
bool searchDiagonally(char*, char**, int, int, int&, int&, int&, int&);
bool searchReverseDiagonally(char*, char**, int, int, int&, int&, int&, int&);
void outputResults(ofstream&, const int &, const int &, const int &, const int &, bool);

//Create Functionality
int highestLength(char**);
char** allocateGridMemory(int, int);
void outputGrid(char**&, const int &, string);
void fillRemainingGrid(char**& , const int);
void diagonal_topLeftToBottomRight(char**, char* );
void fillGridWithWords(char**, char**, const int, int);
void leftToRight(char**, char*, int);
void rightToLeft(char**, char*, const int &, int);
void topToBottom(char**, char*, int);
void bottomToTop(char**, char*, int, int);
bool inputFileForGridCreation(char**&, int&, int&, int&, string&, int&);
void diagonal_bottomRightToTopLeft(char**, char*, int, int);
void diagonal_topRightToBottomLeft(char**, char*, const int &);
void diagonal_bottomLeftToTopRight(char**, char*, int, int);



void main()
{
	char choice;
	do
	{
		cout << "\n\n1. Create Grid\t\t\t[Press C]\n2. Search Words from Grid\t[Press S]\n3. Quit\t\t\t\t[Press Q]\n";
		choice = getch();

		if(choice == 'C' || choice == 'c')
		{
			cout << "\n\n------------------------------------- Creating Grid ------------------------------------------------\n\n";
			createGrid();
		}
		else if(choice == 'S' || choice == 's')
		{
			cout << "\n\n------------------------------------- Searching Grid ------------------------------------------------\n\n";
			searchGrid();
		}
		else if(choice != 'C' && choice != 'c' && choice != 'S' && choice != 's' && choice != 'Q' && choice != 'q')
		{
			cout << "\n\nGoing to Main Menu in 3 seconds......\n";
			Sleep(3000);
		}
	}
	while(choice != 'Q' && choice != 'q');

	cout << "\n\nQuitting program in 3 seconds.......\n";
	Sleep(3000);

	return;
}



//------------------------------------------------------------------------------Search Functionality--------------------------------------------------------

//Simple function that receives double pointers to Grid and WordList as references (we can only return one thing so we use references to avoid returning anything at all).
//Checks to make sure that file names end with ".txt", otherwise appends this to the end of the names (to prevent file handling errors.
//Loads rows, cols, number of test cases directly into the variables. Loads words into tempbuffer, uses GetStringFromBuffer function to allocate exact amount of memory on heap.
//Stores the pointer returned by GetStringFromBuffer into array of double pointers. After words have been loaded, uses a simple nested loop to populate the 2D grid from text file.
void inputFileForGridSearch(int& gridRows, int& gridCols, int& testCases, char**& testWords, char**& characterGrid, string& nameOfOutputFile)
{
	char tempBuffer[30];
	int gridFileLength;
	string nameOfInputFile, nameOfGridFile;


	cout << "Enter the name of text file that contains the grid: ";
	cin >> nameOfGridFile;

	cout << "Enter the name of text file to load the words: ";
	cin >> nameOfInputFile;

	cout << "Enter the name of Output text file where you wnat to place the results: ";
	cin >> nameOfOutputFile;



	if((nameOfInputFile.substr(nameOfInputFile.size() - 4, 4) != ".txt"))
	{
		nameOfInputFile += ".txt";
	}

	if((nameOfGridFile.substr(nameOfGridFile.size() - 4, 4) != ".txt"))
	{
		nameOfGridFile += ".txt";
	}

	if((nameOfOutputFile.substr(nameOfOutputFile.size() - 4, 4) != ".txt"))
	{
		nameOfOutputFile += ".txt";
	}



	ifstream inputFile(nameOfInputFile.c_str());
	if(!inputFile)
	{
		cout << "Text file could not be loaded. Please verify its name and location.\n";
		return;
	}

	ifstream inputGrid(nameOfGridFile.c_str());
	if(!inputGrid)
	{
		cout << "Text file could not be loaded. Please verify its name and location.\n";
		return;
	}



	inputFile >> gridRows >> gridCols >> testCases;
	testWords = new char*[testCases];
	characterGrid = new char*[gridRows];

	for(int i = 0; i < testCases; i++)
	{
		inputFile >> tempBuffer;
		testWords[i] = GetStringFromBuffer(tempBuffer);
	}


	for(int i = 0; i < gridRows; i++)
	{
		characterGrid[i] = new char[gridCols];
	}

	for(int i = 0; i < gridRows; i++)
	{
		for(int j = 0; j < gridCols; j++)
		{
			inputGrid >> characterGrid[i][j];
		}
	}


	inputFile.close();
	inputGrid.close();
	return;
}


//"Main" create function. Acts as the bridge between multiple functions to create a grid. Functionalitites include, taking grid input, taking words input, searching grid, printing results,
//and de-allocating dynamic memory
void searchGrid()
{
	string nameOfOutputFile;
	bool resultOfSearch;
	int gridRows, gridCols, testCases, startRow, endRow, startCol, endCol;
	char** testWords = NULL;
	char** characterGrid = NULL;
	inputFileForGridSearch(gridRows, gridCols, testCases, testWords, characterGrid, nameOfOutputFile);

	ofstream outputObj(nameOfOutputFile.c_str());
	if (!outputObj)
	{
		cout << "Text File to store Results couldn't be created, please try again\n";
		return;
	}

	for(int i = 0; i < testCases; i++)
	{
		resultOfSearch = seachGridForWords(testWords[i], characterGrid, gridRows, gridCols, startRow, endRow, startCol, endCol);
		outputResults(outputObj, startRow, endRow, startCol, endCol, resultOfSearch);
	}
	for(int i = 0; i < testCases; i++)
	{
		delete[] testWords[i];
	}
	delete[] testWords;

	deAllocateGridMemory(characterGrid, gridRows);
	return;
}

//Simple driver function that takes the grid and word to search as paramemters and passes it to multiple specific functions.
//Then whatever output it receives from those functions, it passes along to the main function.
bool seachGridForWords(char* testCase, char** charGrid, int gridRows, int gridCols, int& startRow, int& endRow, int& startCol, int& endCol)
{

	if(searchHorizontally(testCase, charGrid, gridRows, gridCols, startRow, endRow, startCol, endCol) == true)
	{
		return true;
	}
	else if(searchVertically(testCase, charGrid, gridRows, gridCols, startRow, endRow, startCol, endCol) == true)
	{
		return true;
	}
	else if(searchDiagonally(testCase, charGrid, gridRows, gridCols, startRow, endRow, startCol, endCol) == true)
	{
		return true;
	}
	else if(searchReverseDiagonally(testCase, charGrid, gridRows, gridCols, startRow, endRow, startCol, endCol) == true)
	{
		return true;
	}

	return false;
}

/*                                        DESCRIPTION OF THE 4 SEARCH FUNCTIONS BELOW

Takes the grid and the word to search as paramemters, along with index/coordinate variables so we can use this information inside the output function.
It traverses the 2D grid and searches for the first occurance of initial character of search word matching. Then it starts a loop till the length of the word to check if the 
rest of the alphabets are found in the grid as well. If the loop doesn't break, it means that the whole word has been found. Thus, returning true.
If the loop breaks, this means that the whole word couldn't be located. So, it contunues searching till the grid ends. Then it returns false.									*/

bool searchHorizontally(char* testCase, char** charGrid, int gridRows, int gridCols, int& startRow, int& endRow, int& startCol, int& endCol)
{
	int lengthOfTestWord = strlen(testCase);
	bool foundWord;

	for(int i = 0; i < gridRows; i++)
	{
		for(int j = 0; j < gridCols; j++)
		{
			foundWord = false;

			if(charGrid[i][j] == testCase[0])
			{
				startRow = i, startCol = j;
				foundWord = true;

				//------------------------------------------------------------Left To Right---------------------------------------------------------------------
				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((j + k) >= 0) && ((j + k) < gridCols))
					{
						if(testCase[k] != charGrid[i][j + k])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}
				}

				if(foundWord == true)
				{
					endRow = startRow;
					endCol = j + (lengthOfTestWord - 1);
					return true;
				}


				//------------------------------------------------------------Right to Left---------------------------------------------------------------------
				foundWord = true;
				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((j - k) >= 0) && ((j - k) < gridCols))
					{
						if(testCase[k] != charGrid[i][j - k])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}
				}

				if(foundWord == true)
				{
					endRow = startRow;
					endCol = j - (lengthOfTestWord - 1);
					return true;
				}
			}
		}
	}
	return false;
}

bool searchVertically(char* testCase, char** charGrid, int gridRows, int gridCols, int& startRow, int& endRow, int& startCol, int& endCol)
{
	int lengthOfTestWord = strlen(testCase);
	bool foundWord;


	//------------------------------------------------------------Top To Bottom---------------------------------------------------------------------
	for(int i = 0; i < gridRows; i++)
	{
		for(int j = 0; j < gridCols; j++)
		{
			foundWord = false;

			if(charGrid[i][j] == testCase[0])
			{
				startRow = i, startCol = endCol = j;
				foundWord = true;

				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((i + k) >= 0) && ((i + k) < gridRows))
					{
						if(testCase[k] != charGrid[i + k][j])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}
				}

				if(foundWord == true)
				{
					endRow = startRow + (lengthOfTestWord - 1);
					return true;
				}
			}
		}
	}


	//------------------------------------------------------------Bottom To Top---------------------------------------------------------------------
	for(int i =  gridRows - 1; i >= 0; i--)
	{
		for(int j = 0; j < gridCols; j++)
		{
			foundWord = false;

			if(charGrid[i][j] == testCase[0])
			{
				startRow = i, startCol = endCol = j;
				foundWord = true;

				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((i - k) >= 0) && ((i - k) < gridRows))
					{
						if(testCase[k] != charGrid[i - k][j])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}

				}

				if(foundWord == true)
				{
					endRow = startRow - (lengthOfTestWord - 1);
					return true;
				}
			}
		}
	}

	return false;
}

bool searchDiagonally(char* testCase, char** charGrid, int gridRows, int gridCols, int& startRow, int& endRow, int& startCol, int& endCol)
{
	int lengthOfTestWord = strlen(testCase);
	bool foundWord;

	//------------------------------------------------------------Top-Left to Bottom-Right---------------------------------------------------------------------
	for(int i = 0; i < gridRows; i++)
	{
		for(int j = 0; j < gridCols; j++)
		{
			foundWord = false;

			if(charGrid[i][j] == testCase[0])
			{
				startRow = i, startCol = j;
				foundWord = true;

				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((i + k) >= 0) && ((i + k) < gridRows) && ((j + k) < gridCols)  && ((j + k) >= 0))
					{
						if(testCase[k] != charGrid[i + k][j + k])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}
				}

				if(foundWord == true)
				{
					endRow = startRow + (lengthOfTestWord - 1);
					endCol = startCol + (lengthOfTestWord - 1);
					return true;
				}
			}
		}
	}



	//------------------------------------------------------------Bottom-Right to Top-Left---------------------------------------------------------------------
	for(int i = gridRows - 1; i >= 0; i--)
	{
		for(int j = gridCols - 1; j >= 0; j--)
		{
			foundWord = false;

			if(charGrid[i][j] == testCase[0])
			{
				startRow = i, startCol = j;
				foundWord = true;

				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((i - k) >= 0) && ((i - k) < gridRows) && ((j - k) < gridCols)  && ((j - k) >= 0))
					{
						if(testCase[k] != charGrid[i - k][j - k])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}
				}

				if(foundWord == true)
				{
					endRow = startRow - (lengthOfTestWord - 1);
					endCol = startCol - (lengthOfTestWord - 1);
					return true;
				}
			}
		}
	}

	return false;
}

bool searchReverseDiagonally(char* testCase, char** charGrid, int gridRows, int gridCols, int& startRow, int& endRow, int& startCol, int& endCol)
{
	int lengthOfTestWord = strlen(testCase);
	bool foundWord;

	for(int i = gridRows - 1; i >= 0; i--)
	{
		for(int j = 0; j < gridCols; j++)
		{
			foundWord = false;

			if(charGrid[i][j] == testCase[0])
			{
				startRow = i, startCol = j;

				//----------------------------------------------------------------------Bottom-Left to top-Right---------------------------------------------

				foundWord = true;

				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((i - k) >= 0) && ((i - k) < gridRows) && ((j + k) < gridCols)  && ((j + k) >= 0))
					{
						if(testCase[k] != charGrid[i - k][j + k])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}
				}

				if(foundWord == true)
				{
					endRow = startRow - (lengthOfTestWord - 1);
					endCol = startCol + (lengthOfTestWord - 1);
					return true;
				}

				//----------------------------------------------------------------------Top-Right to Bottom-Left-----------------------------------------------

				foundWord = true;

				for(int k = 1; (k < lengthOfTestWord) && (foundWord == true); k++)
				{
					if(((i + k) >= 0) && ((i + k) < gridRows) && ((j - k) < gridCols)  && ((j - k) >= 0))
					{
						if(testCase[k] != charGrid[i + k][j - k])
						{
							foundWord = false;
						}
					}
					else
					{
						foundWord = false;
					}
				}

				if(foundWord == true)
				{
					endRow = startRow + (lengthOfTestWord - 1);
					endCol = startCol - (lengthOfTestWord - 1);
					return true;
				}

			}
		}

	}


	return false;
}

//If the main function says that the word has been found, it prints the result in correct format on console, and also outputs the information in the text file.
//Otherwise, displays the error message.
void outputResults(ofstream& outputObj, const int &startRow, const int &endRow, const int &startCol, const int &endCol, bool searchResult)
{
	if(searchResult == true)
	{
		cout << "{" << startRow << "," << startCol << "}{" << endRow << "," << endCol << "}" << endl;
		outputObj << "{" << startRow << "," << startCol << "}{" << endRow << "," << endCol << "}" << endl;
	}
	else
	{
		cout << "Not Found\n";
		outputObj << "Not Found\n";
	}
	return;
}

//Takes a cstring. Creates a new char pointer of bufferSize + 1 (for null character). Deep Copies all elements of Buffer to the newly created pointer. Returns the Pointer.
char* GetStringFromBuffer(char* buffer)
{
	int strSize = strlen(buffer);
	char* dataDestination = 0;

	if (strSize > 0)
	{
		dataDestination = new char[strSize+1];


		for(int i = 0; buffer[i] != '\0'; i++)
		{
			dataDestination[i] = buffer[i];
		}

		dataDestination[strSize] = '\0';
	}
	return dataDestination;
}

//
void deAllocateGridMemory(char**& charGrid, const int &gridSize)
{
	for(int i = 0; i < gridSize; i++)
	{
		delete[] charGrid[i];
	}

	delete[] charGrid;

	return;
}



//------------------------------------------------------------------------------Create Functionality--------------------------------------------------------

//"Main" create function. Acts as the bridge between multiple functions to create a grid. Functionalitites include, taking text input, creating grid, printing grid,
//and de-allocating dynamic memory
void createGrid()
{
	char** listOfWords = NULL;
	char** characterGrid = NULL;
	string outputFileName;
	int highestLength, numberOfWords, gridRows, gridCols;
	bool result = inputFileForGridCreation(listOfWords, gridRows, gridCols, numberOfWords, outputFileName, highestLength);

	if(result == false)
	{
		return;
	}

	for(int i = 0; i < numberOfWords; i++)
	{
		cout << listOfWords[i] << endl;
	}

	ofstream outputObj(outputFileName.c_str());
	if (!outputObj)
	{
		cout << "Text File to store Results couldn't be created, please try again\n";
		return;
	}

	characterGrid = allocateGridMemory(gridRows, gridCols);
	fillGridWithWords(characterGrid, listOfWords, 15, 10);

	for(int i = 0; i < numberOfWords; i++)
	{
		delete[] listOfWords[i];
	}
	delete[] listOfWords;

	outputGrid(characterGrid, 15, outputFileName);
	deAllocateGridMemory(characterGrid, gridRows);

	return;
}

//Takes the size of the grid, dynamiccaly allocated required memory on heap an dreturn the pointer of the allocated memory.
char** allocateGridMemory(int rows, int columns)
{
	char** charGrid = new char*[rows];

	for(int i = 0; i < rows; i++)
	{
		charGrid[i] = new char[columns];
	}

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			charGrid[i][j] = '*';
		}
	}

	return charGrid;
}

//Simple function that loads each Word to be placed inside the puzzle and stores the word in a double pointer. Uses GetSTringFromBuffer to ensure that not a single byte is wasted.
bool inputFileForGridCreation(char**& listOfWords, int& gridRows, int& gridCols, int& numberOfWords, string& nameOfOutputFile, int& longestWord)
{
	string nameOfInputFile;

	cout << "Enter the name of text file to load the words: ";
	cin >> nameOfInputFile;
	if((nameOfInputFile.substr(nameOfInputFile.size() - 4, 4) != ".txt"))
	{
		nameOfInputFile += ".txt";
	}

	ifstream inputObject(nameOfInputFile.c_str());
	if(!inputObject)
	{
		cout << "Text file could not be loaded. Please verify its name and location.\n";
		return false;
	}

	cout<< "Enter number of Words in the file: ";
	cin >> numberOfWords;
	if(numberOfWords <= 0)
	{
		cout << "Number of words must be greater than 0\n\n";
		return false;
	}

	cout << "Enter number of Rows you want in the grid: ";
	cin >> gridRows;
	if(gridRows <= 0)
	{
		cout << "Grid Rows must be greater than 0\n\n";
		return false;
	}


	cout << "Enter number of Columns you want in the grid: ";
	cin >> gridCols;
	if(gridCols <= 0)
	{
		cout << "Grid Columns must be greater than 0\n\n";
		return false;
	}

	cout << "Enter the name of Output text file where you want to place the grid: ";
	cin >> nameOfOutputFile;




	if((nameOfOutputFile.substr(nameOfOutputFile.size() - 4, 4) != ".txt"))
	{
		nameOfOutputFile += ".txt";
	}


	char tempBuffer[40];

	listOfWords = new char*[numberOfWords];
	for(int i = 0; i < numberOfWords; i++)
	{
		inputObject  >> tempBuffer;
		listOfWords[i] = new char;
		listOfWords[i] = GetStringFromBuffer(tempBuffer);
	}

	longestWord = highestLength(listOfWords);

	if(longestWord >= gridRows || longestWord >= gridCols)
	{
		cout << "Grid cannot be formed, press any key to return to the home screen: \n\n";
		getch();
		return false;
	}

	inputObject.close();

	return true;
}

//Simple function that prints the grid in correct format on console, and also outputs the information in the text file.
//Displays the error message if some error occurs.
void outputGrid(char**& grid, const int &gridSize, string outputFileName)
{
	ofstream outputObj(outputFileName.c_str());

	if(!outputObj.is_open())
	{
		cout << "Output file could NOT be created for some reason!\n";
	}
	else
	{
		for(int i = 0; i < gridSize; i++)
		{
			for(int j = 0; j < gridSize; j++)
			{
				outputObj << grid[i][j] << " ";
				cout << grid[i][j] << " ";
			}
			outputObj << endl;
			cout << endl;

		}
		outputObj.close();
	}

	return;
}

//Main Driver function that passes individual words to different functions so they can add words in specific arrangements inside the Grid.
//Fills the remaining spaces with Random Characters.
void fillGridWithWords(char** charGrid, char** listOfWords, const int gridSize, int numberOfWords)
{
	for(int i = 0; i < numberOfWords; i++)
	{
		switch (i)
		{
			case 0:
				leftToRight(charGrid, listOfWords[0], 14);
				break;
			case 1:
				diagonal_bottomLeftToTopRight(charGrid, listOfWords[1], 14, 4);
				break;
			case 2:
				rightToLeft(charGrid,listOfWords[2], gridSize, 12);
				break;
			case 3:
				rightToLeft(charGrid,listOfWords[3], gridSize, 11);
				break;
			case 4:
				bottomToTop(charGrid, listOfWords[4], 12, 0);
				break;
			case 5:
				bottomToTop(charGrid, listOfWords[5], 12, 1);
				break;
			case 6:
				topToBottom(charGrid, listOfWords[6], 14);
				break;
			case 7:
				topToBottom(charGrid, listOfWords[7], 13);
				break;
			case 8:
				diagonal_bottomRightToTopLeft(charGrid, listOfWords[8], 9, 12);
				break;
			case 9:
				diagonal_topLeftToBottomRight(charGrid, listOfWords[9]);
				break;
			default:
				break;
		}

	}


	fillRemainingGrid(charGrid, gridSize);

	return;
}


/*                                                                DESCRIPTION FOR THE 8 FUNCTIONS BELOW
These functions take a single word and starting position for the word, they then populate the word's characters inside the grid in specific patterns			*/

void leftToRight(char** charGrid, char* wordToPlace, int rowCounter)
{
	int wordSize = strlen(wordToPlace);

	for(int i = 0; i < wordSize; i++)
	{
		charGrid[rowCounter][i] = wordToPlace[i];
	}

	return;
}

void rightToLeft(char** charGrid, char* wordToPlace, const int &gridSize, int rowCounter)
{
	int wordSize = strlen(wordToPlace);
	for(int i = 0; i < wordSize; i++)
	{
		charGrid[rowCounter][(gridSize - i) - 1] = wordToPlace[i];
	}

	return;
}

void topToBottom(char** charGrid, char* wordToPlace, int colCounter)
{
	int wordSize = strlen(wordToPlace);
	for(int i = 0; i < wordSize; i++)
	{
		charGrid[i][colCounter] = wordToPlace[i];
	}

}

void bottomToTop(char** charGrid, char* wordToPlace, int startRow, int colCounter)
{
	int wordSize = strlen(wordToPlace);
	for(int i = startRow, counter = 0; counter < wordSize; i--, counter++)
	{
		charGrid[i][colCounter] = wordToPlace[counter];
	}
	return;
}

void diagonal_topLeftToBottomRight(char** charGrid, char* wordToPlace)
{
	int wordSize = strlen(wordToPlace);
	for(int i = 0; i < wordSize; i++)
	{
		charGrid[i][i] = wordToPlace[i];
	}

	return;
}

void diagonal_bottomRightToTopLeft(char** charGrid, char* wordToPlace, int startRow, int startCol)
{
	int wordSize = strlen(wordToPlace);
	for(int counter = 0; counter < wordSize;  counter++)
	{
		charGrid[startRow - counter][startCol - counter] = wordToPlace[counter];
	}

	return;
}

void diagonal_topRightToBottomLeft(char** charGrid, char* wordToPlace, const int &gridSize)
{
	int wordSize = strlen(wordToPlace);

	for(int i = 0; i < wordSize; i++)
	{

	}

}

void diagonal_bottomLeftToTopRight(char** charGrid, char* wordToPlace, int startRow, int startCol)
{
	int wordSize = strlen(wordToPlace);
	for(int counter = 0; counter < wordSize; counter++)
	{
		charGrid[startRow - counter][startCol + counter] = wordToPlace[counter];
	}

	return;
}



//Takes the grid as reference. Wherever it encounters an asterisk (*), it generates a random Capital alphabet andd places it there. 
void fillRemainingGrid(char**& charGrid, const int sizeOfGrid)
{
	for(int i = 0; i < sizeOfGrid; i++)
	{
		for(int j = 0; j < sizeOfGrid; j++)
		{
			if(charGrid[i][j] == '*')
			{
				charGrid[i][j] = char(rand()% 26 + 65);
			}
		}

	}
}


//Traverses the list of words and finds the length of the longest word. Returns that length. 
int highestLength(char** wordList)
{
	int maxLength = 0;
	for(int i = 0; i < 10; i++)
	{
		if(strlen(wordList[i]) > maxLength)
		{
			maxLength = strlen(wordList[i]);
		}

	}
	return maxLength;
}