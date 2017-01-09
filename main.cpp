#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <conio.h>

#define PRINTTEXTS 1

void readInt(int *i, char *text)
{
	printf("%s: ", text);
	scanf_s("%d", i);
}

void readChoice(int *i, char *text)
{
	while (*i < 1 || *i > 2)
	{
		readInt(i, text);
	}
}

int main(void)
{
	srand(time(NULL));
	TCHAR szNewTitle[MAX_PATH];

	int ready = 0;
	int startingBet = 0;
	int startingAssets = 0;
	int bestAssets = 0, losingStreak = 0, longestLosingStreak = 0, winningStreak = 0, longestWinningStreak = 0;
	int choice = 0;
	int chosenColor = 0;
	bool keepColor = true;
	double chanceOfWinning = 18.0 / 37.0;

	while (1)
	{
		ready = 0;
		choice = 0;
		chosenColor = 0;
		keepColor = true;
		readInt(&startingBet, "Starting bet");
		int assets = startingBet;

		readChoice(&chosenColor, "Black (1) or Red (2)?");
		readChoice(&choice, "Assets (1) or calculate assets with max losing streak (2)?");

		if (choice == 1)
			readInt(&assets, "Assets?");
		else
		{
			int maxWrong = 0;
			readInt(&maxWrong, "How long losing streak is allowed?");
			for (int i = 0; i < maxWrong - 1; i++)
				assets *= 2;
		}

		choice = 0;
		readChoice(&choice, "Keep the color (1) or change after every loss (2)?");

		if (choice == 1)
			keepColor = true;
		else
			keepColor = false;

		startingAssets = assets;
		bestAssets = assets;
		printf("Assets: %d.\n", startingAssets);
		printf("Probability of winning per round: %f.\n", chanceOfWinning);
		printf("You win if you get the color ");

		if (chosenColor == 1)
			printf("Black.\n");
		else
			printf("Red.\n");

		readInt(&ready, "Ready? (give any number)");
		int winningAmount = 0, bet = startingBet, rounds = 0, number = 0;
		int numberColor = 0;
		double random = 0;

		while (assets > 0)
		{
			if (assets < bet)
			{
				printf("Not enough assets for a whole bet! Bet: %d. Assets: %d.\n", bet, assets);
				bet = assets;
				assets = 0;
			}
			else
				assets -= bet;

			if (assets == 0)
				printf("All In!\n");

			random = ((double)rand() / (RAND_MAX));
			number = (int)(random * 37);

			switch (number)
			{
			case 0:
				numberColor = 0; // Green
				break;
			case 2:
			case 4:
			case 6:
			case 8:
			case 10:
			case 11:
			case 13:
			case 15:
			case 17:
			case 20:
			case 22:
			case 24:
			case 26:
			case 28:
			case 29:
			case 31:
			case 33:
			case 35:
				numberColor = 1; // Black
				break;
			default:
				numberColor = 2; // Red
			}

#if PRINTTEXTS
			if (numberColor == 1)
				printf("Red ");
			else if (numberColor == 2)
				printf("Black ");
			else
				printf("Green ");
#endif

			if (numberColor == chosenColor)
			{
				winningAmount = bet * 2;
				assets += winningAmount;
				bet = startingBet;
				losingStreak = 0;
				winningStreak++;

				if (winningStreak > longestWinningStreak)
					longestWinningStreak = winningStreak;

				if (assets > bestAssets)
					bestAssets = assets;
#if PRINTTEXTS
				printf("%d! You won %d! Assets now: %d.\n", number, winningAmount, assets);
#endif
			}
			else
			{
#if PRINTTEXTS
				printf("%d! You lost %d! Assets left: %d.\n", number, bet, assets);
#endif
				winningStreak = 0;
				losingStreak++;
				if (losingStreak > longestLosingStreak)
					longestLosingStreak = losingStreak;
				bet *= 2;

				if (!keepColor && numberColor != -1) {
					if (chosenColor == 1)
						chosenColor = 2;
					else
						chosenColor = 1;
				}
			}

			rounds++;
			StringCchPrintf(szNewTitle, MAX_PATH, TEXT("Rounds: %d Starting Assets: %d Best Assets: %d (%.03f) Now: %d (%.03f)"), rounds, startingAssets, bestAssets, (float)bestAssets / (float)startingAssets, assets, (float)assets / (float)startingAssets);
			SetConsoleTitle(szNewTitle);
		}

		printf("Rounds: %d.\nStarting Assets: %d.\nBest Assets: %d.\nLongest Winning Streak: %d.\nLongest Losing Streak: %d.\n", rounds, startingAssets, bestAssets, longestWinningStreak, longestLosingStreak);
	}
	return 0;
}