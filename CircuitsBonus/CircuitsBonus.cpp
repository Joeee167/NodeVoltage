#include<iostream>
#include<Eigen/Dense>
#include<iomanip>

using namespace std;
using namespace Eigen;

enum enSteps { NumberOfNodes = 1, NumberOfR = 2, ReadValues = 3, CreateMatrix = 4, ReadCurrent = 5, ReadAnswers = 6, DisplayResults = 7, End = 8 };

struct stQuestionInfo
{
	int NumberOfNodes;
	int NumberOfR[100];
	int ValuesOfR[100][100] = { 0 };
	float MatrixOfG[100][100] = { 0 };
	float CurrentSources[100];
	float Results[100];
	float Answers[100];


};

stQuestionInfo QuestionInfo;

void ChooseOption();

void MainScreen()
{
	cout << "-----------------------------------------\n";
	cout << "\tGeneralized Node Solution\n";
	cout << "-----------------------------------------\n";

	cout << "\nFollow these steps in order : ";
	cout << "\n[1] : Enter the number of non-reference nodes.";
	cout << "\n[2] : Enter the number of resistances connected to each node.";
	cout << "\n[3] : Enter the value of resistances connected to each node.";
	cout << "\n[4] : Enter the value of common resistances between the nodes.";
	cout << "\n[5] : Enter the value of current sources connected to the nodes.";
	cout << "\n[6] : Enter your answers.";
	cout << "\n[7] : Display correct answers.";
	cout << "\n[8] : End Screen.";



	ChooseOption();


}

void GoBackToMainMenu()
{
	cout << "\n\nPress any key to go back to steps menu...\n";
	system("pause>0");

	system("cls");
	MainScreen();
}

void ReadNumberOfNodesMenu(stQuestionInfo& QuestionInfo)
{
	cout << "-----------------------------------------\n";
	cout << "\tRead Number of Nodes Menu\n";
	cout << "-----------------------------------------\n";

	cout << "\nEnter the number of nodes : ";
	cin >> QuestionInfo.NumberOfNodes;

}

void ReadNumberOfConnectedResistances(stQuestionInfo& QuestionInfo)
{
	cout << "--------------------------------------------------------------\n";
	cout << "\tEnter number of connected resistances\n";
	cout << "--------------------------------------------------------------\n";

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		cout << "\nNode[" << i + 1 << "] : ";
		cin >> QuestionInfo.NumberOfR[i];
	}
}

void ReadValuesOfR(stQuestionInfo& QuestionInfo)
{
	cout << "--------------------------------------------------------------\n";
	cout << "\tEnter values of connected resistances\n";
	cout << "--------------------------------------------------------------\n";

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		cout << "\nNode[" << i + 1 << "] : \n";

		for (int j = 0; j < QuestionInfo.NumberOfR[i]; j++)
		{
			cout << "\nR[" << j + 1 << "] : ";
			cin >> QuestionInfo.ValuesOfR[i][j];
		}

		cout << "\n--------------------------------------------------------------\n";

	}
}

void CreateTheMatrix(stQuestionInfo& QuestionInfo)
{
	cout << "--------------------------------------------------------------\n";
	cout << "\tCreating the matrix of G's\n";
	cout << "--------------------------------------------------------------\n";

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		bool flag = 1;

		for (int j = 0; j < QuestionInfo.NumberOfNodes; j++)
		{
			if (i == j)
			{
				for (int k = 0; k < QuestionInfo.NumberOfR[i]; k++)
				{
					QuestionInfo.MatrixOfG[i][j] += (1.0 / QuestionInfo.ValuesOfR[i][k]);
				}
			}

			else
			{
				cout << "\nCommon Resistances between node [" << i + 1 << "] and node [" << j + 1 << "] : \n";
				float R;
				char ans;

				while (flag)
				{
					cout << "\nEnter the value of R : ";
					cin >> R;

					if (R == 0)
					{
						QuestionInfo.MatrixOfG[i][j] += 0;

					}
					else
					{
						QuestionInfo.MatrixOfG[i][j] -= (1 / R);

					}


					cout << "\nDo you want to add other resistances?Y/N";
					cin >> ans;

					if (ans == 'N')
					{
						flag = 0;
					}



				}

				flag = 1;
			}
		}
	}

	cout << "\nMatrix of G : \n";
	cout << "---------------------------------------------------------------------------\n";

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		for (int j = 0; j < QuestionInfo.NumberOfNodes; j++)
		{
			if (i != j)
			{
				cout << -1 * QuestionInfo.MatrixOfG[i][j] << " ";

			}

			else
			{
				cout << QuestionInfo.MatrixOfG[i][j] << " ";

			}
		}

		cout << endl;
	}
}

void ReadCurrentSources(stQuestionInfo& QuestionInfo)
{
	cout << "--------------------------------------------------------------\n";
	cout << "\tReading the values of current sources\n";
	cout << "--------------------------------------------------------------\n";

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		cout << "\nEnter value of current at node[" << i + 1 << "] : ";
		cin >> QuestionInfo.CurrentSources[i];
	}
}

void ReadTheAnswers(stQuestionInfo& QuestionInfo)
{
	cout << "\n----------------------------------------------------\n";
	cout << "\tEnter your answers";
	cout << "\n----------------------------------------------------\n";

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		cout << "v[" << i + 1 << "] : ";
		cin >> QuestionInfo.Answers[i];
	}
}

void DisplayTheAnswers(stQuestionInfo& QuestionInfo)
{
	MatrixXd A(QuestionInfo.NumberOfNodes, QuestionInfo.NumberOfNodes);
	VectorXd b(QuestionInfo.NumberOfNodes);
	VectorXd x(QuestionInfo.NumberOfNodes);

	cout << "\n----------------------------------------------------\n";
	cout << "\tNode Solution";
	cout << "\n----------------------------------------------------\n";


	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		for (int j = 0; j < QuestionInfo.NumberOfNodes; j++)
		{
			A(i, j) = QuestionInfo.MatrixOfG[i][j];
		}
	}

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		b(i) = QuestionInfo.CurrentSources[i];
	}

	x = A.colPivHouseholderQr().solve(b);

	cout << "\nSolution:" << endl;
	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		QuestionInfo.Results[i] = x(i);
		cout << "v(" << i + 1 << "): " << x(i) << endl;

	}
}



void EndScreen(stQuestionInfo& QuestionInfo)
{
	cout << "\n----------------------------------------------------\n";
	cout << "\tEnough Circuits";
	cout << "\n----------------------------------------------------\n";

	cout << "-------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(10) << " Question Number ";
	cout << "| " << left << setw(20) << " Your Answer ";
	cout << "| " << left << setw(10) << " Correct Answer ";
	cout << "\n-------------------------------------------------------------------------------\n";

	for (int i = 0; i < QuestionInfo.NumberOfNodes; i++)
	{
		cout << "| " << left << setw(10) << i + 1;
		cout << "| " << left << setw(20) << QuestionInfo.Answers[i];
		cout << "| " << left << setw(10) << QuestionInfo.Results[i] << endl;

	}



}


void ChooseOption()
{
	cout << "\n\nEnter the step number : ";
	int option;
	cin >> option;



	enSteps stepNumber = (enSteps)option;

	switch (stepNumber)
	{
	case enSteps::NumberOfNodes:
		system("cls");
		ReadNumberOfNodesMenu(QuestionInfo);
		GoBackToMainMenu();
		break;

	case enSteps::NumberOfR:
		system("cls");
		ReadNumberOfConnectedResistances(QuestionInfo);
		GoBackToMainMenu();
		break;

	case enSteps::ReadValues:
		system("cls");
		ReadValuesOfR(QuestionInfo);
		GoBackToMainMenu();
		break;


	case enSteps::CreateMatrix:
		system("cls");
		CreateTheMatrix(QuestionInfo);
		GoBackToMainMenu();
		break;

	case enSteps::ReadCurrent:
		system("cls");
		ReadCurrentSources(QuestionInfo);
		GoBackToMainMenu();
		break;

	case enSteps::ReadAnswers:
		system("cls");
		ReadTheAnswers(QuestionInfo);
		GoBackToMainMenu();
		break;

	case enSteps::DisplayResults:
		system("cls");
		DisplayTheAnswers(QuestionInfo);
		GoBackToMainMenu();
		break;

	case enSteps::End:
		system("cls");
		EndScreen(QuestionInfo);
		break;



	}


}

void StartProgram()
{
	MainScreen();
}

int main()
{

	StartProgram();

	return 0;

}